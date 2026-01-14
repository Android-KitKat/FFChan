#include "Translator.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

namespace FFChan {
Translator::Translator(QObject *parent)
    : QObject(parent)
{
}

void Translator::setLocale(const QString &loc)
{
    if (m_locale == loc) return;
    if (loadLocaleFile(loc)) {
        m_locale = loc;
        emit localeChanged();
        emit translationsUpdated();
    }
}

bool Translator::loadLocaleFile(const QString &locale)
{
    // Try resource path first
    QString resPath = QStringLiteral(":/translations/%1.json").arg(locale);
    if (loadFromDevice(resPath)) return true;

    // Fallback to disk relative translations/ folder
    QString diskPath = QStringLiteral("translations/%1.json").arg(locale);
    return loadFromDevice(diskPath);
}

bool Translator::loadFromDevice(const QString &path)
{
    QFile f(path);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    const QByteArray data = f.readAll();
    f.close();

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);
    if (err.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "Failed to parse translations:" << path << err.errorString();
        return false;
    }

    m_translations = doc.object();
    return true;
}

QString Translator::tr(const QString &key) const
{
    if (m_translations.contains(key)) {
        QJsonValue v = m_translations.value(key);
        if (v.isString()) return v.toString();
    }
    return key; // fallback to key itself
}
};