// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "util.h"

#include <QClipboard>
#include <QGuiApplication>
#include <QUrl>

void setClipboardText(const QString &text)
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text, QClipboard::Clipboard);
    if (clipboard->supportsSelection()) {
        clipboard->setText(text, QClipboard::Selection);
    }
}

QIcon loadOsIcon(const QString &os)
{
    const auto osname = os.toLower();
    if (osname == "linux") {
        return QIcon::fromTheme("computer");
    }
    if (osname == "windows") {
        return QIcon::fromTheme("computer");
    }
    if (osname == "macos") {
        return QIcon::fromTheme("computer");
    }
    if (osname == "android") {
        return QIcon::fromTheme("smartphone");
    }
    if (osname == "ios") {
        return QIcon::fromTheme("smartphone");
    }
    return QIcon::fromTheme("question");
}

QString formatCapacityHumanReadable(long bytes)
{
    static constexpr const char *units[] = {"TiB", "GiB", "MiB", "KiB", "B"};
    static constexpr long limits[] = {1L << 40, 1L << 30, 1L << 20, 1L << 10, 1L};
    for (int i = 0; i < 5; ++i) {
        if (bytes >= limits[i]) {
            return QString("%1 %2").arg(static_cast<double>(bytes) / static_cast<double>(limits[i]), 0, 'f', 3).arg(units[i]);
        }
    }
    return "0 B";
}

QString formatSpeedHumanReadable(double bytes_per_second)
{
    static constexpr const char *units[] = {"TiB/s", "GiB/s", "MiB/s", "KiB/s", "B/s"};
    static constexpr double limits[] = {static_cast<double>(1L << 40),
                                        static_cast<double>(1L << 30),
                                        static_cast<double>(1L << 20),
                                        static_cast<double>(1L << 10),
                                        1.0};
    for (int i = 0; i < 5; ++i) {
        if (bytes_per_second >= limits[i]) {
            return QString("%1 %2").arg(bytes_per_second / static_cast<double>(limits[i]), 0, 'f', 2).arg(units[i]);
        }
    }
    return "0 B/s";
}

QString formatDurationHumanReadable(const QDateTime &from, const QDateTime &to)
{
    static constexpr qint64 conversions[] =
        {365l * 30l * 24l * 60l * 60l * 1000l, 34 * 30l * 60l * 60l * 1000l, 24l * 60l * 60l * 1000l, 60l * 60l * 1000l, 60l * 1000l, 1000l};
    static constexpr const char *units[] = {"year", "month", "day", "hour", "minute", "second"};

    QString result = "";
    if (from > to) {
        return result;
    }
    qint64 msecs = from.msecsTo(to);
    if (msecs < 1000) {
        return result;
    }
    for (int i = 0; i < 6; ++i) {
        if (msecs >= conversions[i]) {
            if (!result.isEmpty()) {
                result += ", ";
            }
            qint64 count = msecs / conversions[i];
            result += QString("%1 %2%3 ").arg(count).arg(units[i]).arg(count > 1 ? "s" : "");
            msecs %= conversions[i];
        }
    }

    return result;
}

QString fileUrlToString(const QUrl &url)
{
    return url.toString(QUrl::PreferLocalFile);
}

qint64 toMSecsSinceEpoch(const QDateTime &dateTime)
{
    return dateTime.toMSecsSinceEpoch();
}

void Util::setClipboardText(const QString &text) const
{
    ::setClipboardText(text);
}
QString Util::formatCapacityHumanReadable(long bytes) const
{
    return ::formatCapacityHumanReadable(bytes);
}
QString Util::formatSpeedHumanReadable(double bytes_per_second) const
{
    return ::formatSpeedHumanReadable(bytes_per_second);
}
QString Util::formatDurationHumanReadable(const QDateTime &from, const QDateTime &to) const
{
    return ::formatDurationHumanReadable(from, to);
}
QString Util::fileUrlToString(const QUrl &url) const
{
    return ::fileUrlToString(url);
}
qint64 Util::toMSecsSinceEpoch(const QDateTime &dateTime) const
{
    return ::toMSecsSinceEpoch(dateTime);
}
QIcon Util::loadOsIcon(const QString &os) const
{
    return ::loadOsIcon(os);
}