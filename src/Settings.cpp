#include "Settings.h"
#include <QFile>
#include <QTextStream>
#include <yaml-cpp/yaml.h>

namespace FFChan {

bool SettingsManager::loadYaml(const QString &path, QSettings &qsettings) {
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

} // namespace FFChan
