#pragma once

#include <QObject>
#include <QColor>

namespace FFChan {
class ThemeManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    explicit ThemeManager(QObject *parent = nullptr) : QObject(parent) {}

    QColor color() const { return m_color; }
    void setColor(const QColor &c) {
        if (c == m_color) return;
        m_color = c;
        emit colorChanged(m_color);
    }

signals:
    void colorChanged(const QColor &color);

private:
    QColor m_color;
};
} // namespace FFChan
