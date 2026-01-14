#include "Loader.h"
#include "Settings.h"
#include <QDir>
#include <QFile>
#include <QUrl>

namespace FFChan {

bool loadConfigIfExists(const QString &cfgPath, QSettings &settings) {
  if (!QFile::exists(cfgPath)) {
    return false;
  }

  SettingsManager manager;
  return manager.loadYaml(cfgPath, settings);
}

bool prepareEngine(QQmlApplicationEngine &engine) {
  QString cfgPath = QDir::current().filePath("config.yaml");
  QSettings settings;
  bool ok = loadConfigIfExists(cfgPath, settings);

  if (ok) {
    engine.load(QUrl(QStringLiteral("qrc:/qml/MainWindow.qml")));
  } else {
    engine.load(QUrl(QStringLiteral("qrc:/qml/Wizard.qml")));
  }

  return !engine.rootObjects().isEmpty();
}

} // namespace FFChan
