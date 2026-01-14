#include "Sysfetch.h"
#include <QGuiApplication>
#include <QPalette>
#include <QSettings>
#include <QProcessEnvironment>
#include <QProcess>
#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QLocale>
#include <QDirIterator>
#include <QDebug>

namespace FFChan {

QColor getSystemThemeColor()
{
#if defined(Q_OS_WIN)
    QSettings reg("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\DWM", QSettings::NativeFormat);
    QVariant v = reg.value("ColorizationColor");
    if (v.isValid()) {
        quint32 val = v.toUInt();
        int r = val & 0xFF;
        int g = (val >> 8) & 0xFF;
        int b = (val >> 16) & 0xFF;
        return QColor(r, g, b);
    }
    // fallback to palette
    if (QGuiApplication::instance()) {
        return QGuiApplication::palette().color(QPalette::Highlight);
    }
#elif defined(Q_OS_MAC)
    // macOS: try to read accent color via `defaults` and map common values; otherwise use palette
    QProcess p;
    p.start("defaults", QStringList() << "read" << "-g" << "AppleAccentColor");
    if (p.waitForFinished(200)) {
        QByteArray out = p.readAllStandardOutput().trimmed();
        bool ok = false;
        int code = out.toInt(&ok);
        if (ok) {
            // map a few common accent codes to colors (best-effort)
            switch (code) {
                case 0: return QColor("#0A84FF"); // blue
                case 1: return QColor("#FF453A"); // red
                case 2: return QColor("#32D74B"); // green
                case 3: return QColor("#FFD60A"); // yellow
                case 4: return QColor("#BF5AF2"); // purple
                case 6: return QColor("#FF9F0A"); // orange
                default: break;
            }
        }
    }
    if (QGuiApplication::instance()) return QGuiApplication::palette().color(QPalette::Highlight);
#else
    // Linux and others: prefer Qt palette (works for Qt theme) as a reasonable guess
    if (QGuiApplication::instance()) {
        return QGuiApplication::palette().color(QPalette::Highlight);
    }
#endif

    return QColor("#39c5bb"); // fallback
}

QString getSystemLanguage()
{
    QStringList available;
    // gather translations available in resource :/translations
    QDir qresDir(":/translations");
    for (const QString &fname : qresDir.entryList(QStringList() << "*.json", QDir::Files)) {
        QString base = QFileInfo(fname).baseName();
        available << base;
    }

#if defined(Q_OS_LINUX)
    QByteArray langEnv = qgetenv("LANG");
    QString loc = QString::fromUtf8(langEnv).split('.').first();
    if (loc.isEmpty()) loc = QLocale::system().name();
#else
    QString loc = QLocale::system().name();
#endif

    // Try exact match (e.g., zh_CN), then language-only (zh)
    if (available.contains(loc)) return loc;
    QString langOnly = loc.split('_').first();
    for (const QString &a : available) {
        if (a.startsWith(langOnly)) return a;
    }

    // fallback to en if present, otherwise first available or empty
    if (available.contains("en")) return QStringLiteral("en");
    if (!available.isEmpty()) return available.first();
    return QStringLiteral("en");
}

QStringList getFFmpegPath()
{
    QStringList results;

    // 1) Use which/where to find candidates
#if defined(Q_OS_WIN)
    QProcess p;
    p.start("where", QStringList() << "ffmpeg");
    if (p.waitForFinished(500)) {
        QByteArray out = p.readAllStandardOutput();
        for (const QByteArray &line : out.split('\n')) {
            QString path = QString::fromLocal8Bit(line).trimmed();
            if (!path.isEmpty() && QFileInfo(path).isExecutable()) results << path;
        }
    }
#else
    QProcess p;
    p.start("which", QStringList() << "-a" << "ffmpeg");
    if (p.waitForFinished(500)) {
        QByteArray out = p.readAllStandardOutput();
        for (const QByteArray &line : out.split('\n')) {
            QString path = QString::fromUtf8(line).trimmed();
            if (!path.isEmpty() && QFileInfo(path).isExecutable()) results << path;
        }
    }
#endif

    // 2) Scan PATH directories
    QByteArray pathEnv = qgetenv("PATH");
#if defined(Q_OS_WIN)
    char sep = ';';
#else
    char sep = ':';
#endif
    QStringList paths = QString::fromUtf8(pathEnv).split(sep, Qt::SkipEmptyParts);
    QString ffName;
#if defined(Q_OS_WIN)
    ffName = "ffmpeg.exe";
#else
    ffName = "ffmpeg";
#endif
    for (QString dir : paths) {
        QFileInfo fi(dir);
        if (!fi.isDir()) continue;
        QString ff = QDir(dir).filePath(ffName);
        if (QFileInfo(ff).isExecutable()) results << ff;
    }

    // 3) Common locations
#if defined(Q_OS_WIN)
    QStringList commonWin = {
        QDir::fromNativeSeparators("C:/Program Files/ffmpeg/bin/ffmpeg.exe"),
        QDir::fromNativeSeparators("C:/Program Files (x86)/ffmpeg/bin/ffmpeg.exe")
    };
    for (const QString &pth : commonWin) if (QFileInfo(pth).isExecutable()) results << pth;
#else
    QStringList common = {"/usr/bin/ffmpeg", "/usr/local/bin/ffmpeg", "/snap/bin/ffmpeg", "/opt/homebrew/bin/ffmpeg"};
    for (const QString &pth : common) if (QFileInfo(pth).isExecutable()) results << pth;
#endif

    // deduplicate
    QStringList unique;
    for (const QString &r : results) {
        QString abs = QFileInfo(r).absoluteFilePath();
        if (!unique.contains(abs)) unique << abs;
    }

    return unique;
}

} // namespace FFChan
