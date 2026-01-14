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

} // namespace FFChan
