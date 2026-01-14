#include "SettingsBridge.h"
#include "Settings.h"
#include <QDebug>

SettingsBridge::SettingsBridge(QObject *parent)
    : QObject(parent)
{
}

QString SettingsBridge::get(const QString &key, const QString &defaultValue)
{
    return FFChan::getSetting(key, defaultValue);
}

void SettingsBridge::set(const QString &key, const QVariant &value)
{
    FFChan::setSetting(key, value);
    emit settingChanged(key, value);
}
