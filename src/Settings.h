#pragma once

#include <QSettings>
#include <QString>

namespace FFChan {

class SettingsManager {
public:
  SettingsManager() = default;

  bool loadYaml(const QString &path, QSettings &qsettings);
  bool saveYaml(const QString &path, const QSettings &qsettings);
};

// Global settings helpers
void initializeSettings(const QString &configPath);
QSettings &globalSettings();
QString getSetting(const QString &key, const QString &defaultValue = QString());
void setSetting(const QString &key, const QVariant &value);

} // namespace FFChan
