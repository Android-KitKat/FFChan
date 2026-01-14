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

  FFChan::SettingsManager manager;
  return manager.loadYaml(cfgPath, settings);
}

bool prepareSettings(const QString &cfgPath)
{
  bool existed = QFile::exists(cfgPath);
  initializeSettings(cfgPath);
  return existed;
}

bool prepareEngine(QQmlApplicationEngine &engine) {
  QString cfgPath = QDir::current().filePath("config.yaml");

  bool existed = prepareSettings(cfgPath);
  #ifndef QT_DEBUG
  if (existed) {
    engine.load(QUrl(QStringLiteral("qrc:/qml/MainWindow.qml")));
  } else {
  #endif
    engine.load(QUrl(QStringLiteral("qrc:/qml/Wizard.qml")));
  #ifndef QT_DEBUG
  }
  #endif

  return !engine.rootObjects().isEmpty();
}

} // namespace FFChan
