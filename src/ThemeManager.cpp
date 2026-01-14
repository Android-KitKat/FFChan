#include "ThemeManager.h"
#include "Sysfetch.h"

#include <QGuiApplication>
#include <QPalette>
#include <QStyleHints>

namespace FFChan {

void ThemeManager::setMode(const QString &m)
{
    const QString normalized = (m == "light" || m == "dark" || m == "auto") ? m : QStringLiteral("auto");
    if (normalized == m_mode) return;
    m_mode = normalized;
    emit modeChanged(m_mode);
    applyTheme();
}

void ThemeManager::applyTheme()
{
    if (!QGuiApplication::instance()) return;
    if (m_applying) return;
    m_applying = true;

    QStyleHints *hints = QGuiApplication::styleHints();
    if (hints && !m_listeningToSystemScheme) {
        m_listeningToSystemScheme = true;
        QObject::connect(hints, &QStyleHints::colorSchemeChanged, this, [this](Qt::ColorScheme) {
            if (m_mode == "auto") applyTheme();
        });
    }

    // Light/Dark/Auto:
    // - auto: don't fight the platform theme; revert any previous override
    // - light/dark: request Qt to apply the scheme (styles may or may not honor this)
    if (hints) { //呜呜呜不行喵
        if (m_mode == "dark") {
            hints->setColorScheme(Qt::ColorScheme::Dark);
        } else if (m_mode == "light") {
            hints->setColorScheme(Qt::ColorScheme::Light);
        } else {
            hints->setColorScheme(Qt::ColorScheme::Unknown);
        }
    }

    // Accent (themeColor): keep native palette, only override highlight/link.
    if (m_color.isValid()) {
        QPalette pal = QGuiApplication::palette();
        pal.setColor(QPalette::Highlight, m_color);
        pal.setColor(QPalette::Link, m_color);
        QGuiApplication::setPalette(pal);
    }

    m_applying = false;
}

} // namespace FFChan
