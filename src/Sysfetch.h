#pragma once

#include <QColor>
#include <QString>
#include <QStringList>

namespace FFChan {

QColor getSystemThemeColor();
QString getSystemLanguage();
QStringList getFFmpegPath();

// Returns "light", "dark" or "auto" (fallback if cannot detect)
QString getSystemThemeMode();

} // namespace FFChan
