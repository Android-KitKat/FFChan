#pragma once

#include <QQmlApplicationEngine>
#include <QSettings>
#include <QString>

namespace FFChan {

bool loadConfigIfExists(const QString &cfgPath, QSettings &settings);

// Prepare the QQmlApplicationEngine: detect config.yaml, populate QSettings
// and load either MainWindow.qml or Wizard.qml. Returns true on success.
bool prepareEngine(QQmlApplicationEngine &engine);

} // namespace FFChan