#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>

namespace FFChan {
class Translator : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString locale READ locale WRITE setLocale NOTIFY localeChanged)
public:
    explicit Translator(QObject *parent = nullptr);

    QString locale() const { return m_locale; }
    void setLocale(const QString &loc);

    Q_INVOKABLE QString tr(const QString &key) const;
    Q_INVOKABLE bool loadLocaleFile(const QString &locale);

signals:
    void localeChanged();
    void translationsUpdated();

private:
    bool loadFromDevice(const QString &path);

    QString m_locale;
    QJsonObject m_translations;
};
};