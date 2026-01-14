#pragma once

#include <QObject>
#include <QString>
#include <QVariant>

class SettingsBridge : public QObject {
    Q_OBJECT
public:
    explicit SettingsBridge(QObject *parent = nullptr);

    Q_INVOKABLE QString get(const QString &key, const QString &defaultValue = QString());
    Q_INVOKABLE void set(const QString &key, const QVariant &value);

signals:
    void settingChanged(const QString &key, const QVariant &value);
};
