#pragma once

#include <QObject>
#include <QColor>
#include <QString>

namespace FFChan {
class ThemeManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged)
public:
    explicit ThemeManager(QObject *parent = nullptr) : QObject(parent), m_mode("auto") {}

    QColor color() const { return m_color; }
    void setColor(const QColor &c) {
        if (c == m_color) return;
        m_color = c;
        emit colorChanged(m_color);
        applyTheme();
    }

    QString mode() const { return m_mode; }
    Q_INVOKABLE void setMode(const QString &m);

    // Apply the current mode to Qt (light/dark/auto)
    void applyTheme();

signals:
    void colorChanged(const QColor &color);
    void modeChanged(const QString &mode);

private:
    QColor m_color;
    QString m_mode;
    bool m_listeningToSystemScheme = false;
    bool m_applying = false;
};
} // namespace FFChan
