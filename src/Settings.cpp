
// Settings.cpp — implementation
#include "Settings.h"
#include "Sysfetch.h"
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <yaml-cpp/yaml.h>

namespace FFChan {

bool SettingsManager::loadYaml(const QString &path, QSettings &qsettings) {
  // Set sensible defaults from the system if not already present
  if (!qsettings.contains("theme-color")) {
    QColor c = getSystemThemeColor();
    qsettings.setValue("theme-color", c.name());
  }
  if (!qsettings.contains("ffmpeg-path")) {
    qsettings.setValue("ffmpeg-path", QString());
  }
  if (!qsettings.contains("language")) {
    qsettings.setValue("language", getSystemLanguage());
  }

  try {
    YAML::Node config = YAML::LoadFile(path.toStdString());

    if (config["ffmpeg-path"]) {
      QString ffmpegPath =
          QString::fromStdString(config["ffmpeg-path"].as<std::string>());
      qsettings.setValue("ffmpeg-path", ffmpegPath);
    }

    if (config["theme-color"]) {
      QString themeColor =
          QString::fromStdString(config["theme-color"].as<std::string>());
      qsettings.setValue("theme-color", themeColor);
    }

    if (config["language"]) {
      QString lang =
          QString::fromStdString(config["language"].as<std::string>());
      qsettings.setValue("language", lang);
    }

    return true;
  } catch (const YAML::Exception &e) {
    return false;
  }
}

bool SettingsManager::saveYaml(const QString &path,
                               const QSettings &qsettings) {
  try {
    YAML::Emitter out;
    out << YAML::BeginMap;

    if (qsettings.contains("ffmpeg-path")) {
      out << YAML::Key << "ffmpeg-path";
      out << YAML::Value
          << qsettings.value("ffmpeg-path").toString().toStdString();
    }

    if (qsettings.contains("theme-color")) {
      out << YAML::Key << "theme-color";
      out << YAML::Value
          << qsettings.value("theme-color").toString().toStdString();
    }

    if (qsettings.contains("language")) {
      out << YAML::Key << "language";
      out << YAML::Value
          << qsettings.value("language").toString().toStdString();
    }

    out << YAML::EndMap;

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
      return false;
    }

    QTextStream stream(&file);
    stream << QString::fromStdString(out.c_str());
    file.close();

    return true;
  } catch (const YAML::Exception &e) {
    return false;
  }
}

// Global settings implementation
static QSettings *g_settings = nullptr;
static QString g_configPath;

void initializeSettings(const QString &configPath) {
  if (g_settings)
    return;
  g_settings = new QSettings();
  g_configPath = configPath;

  // Populate defaults from system
  if (!g_settings->contains("theme-color")) {
    QColor c = getSystemThemeColor();
    g_settings->setValue("theme-color", c.name());
  }
  if (!g_settings->contains("ffmpeg-path")) {
    g_settings->setValue("ffmpeg-path", QString());
  }
  if (!g_settings->contains("language")) {
    g_settings->setValue("language", getSystemLanguage());
  }

  SettingsManager mgr;
  if (QFile::exists(configPath)) {
    mgr.loadYaml(configPath, *g_settings);
  } else {
    // create file with defaults
    mgr.saveYaml(configPath, *g_settings);
  }
}

QSettings &globalSettings() {
  if (!g_settings) {
    initializeSettings(QDir::current().filePath("config.yaml"));
  }
  return *g_settings;
}

QString getSetting(const QString &key, const QString &defaultValue) {
  QSettings &s = globalSettings();
  return s.value(key, defaultValue).toString();
}

void setSetting(const QString &key, const QVariant &value) {
  QSettings &s = globalSettings();
  s.setValue(key, value);
  // persist immediately
  if (!g_configPath.isEmpty()) {
    SettingsManager mgr;
    mgr.saveYaml(g_configPath, s);
  }
}

} // namespace FFChan
