// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "util.h"

#include "libtailwrap.h"
#include <QClipboard>
#include <QGuiApplication>
#include <QMimeData>
#include <QUrl>
#include <ksystemclipboard.h>

void setClipboardText(const QString &text)
{
    auto *data = new QMimeData();
    data->setData(QStringLiteral("text/plain"), text.toUtf8());
    KSystemClipboard::instance()->setMimeData(data, QClipboard::Clipboard);
}

QIcon loadOsIcon(QString osname)
{
    osname = osname.toLower();
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
    return QIcon::fromTheme("network-vpn");
}

QString formatCapacityHumanReadable(long bytes)
{
    static constexpr std::array<const char *, 5> units = {"TiB", "GiB", "MiB", "KiB", "B"};
    static constexpr std::array<qint64, 5> limits = {1ll << 40ll, 1ll << 30ll, 1ll << 20ll, 1ll << 10ll, 1ll};
    for (std::size_t i = 0; i < units.size(); ++i) {
        if (bytes >= limits.at(i)) {
            return QString("%1 %2").arg(static_cast<double>(bytes) / static_cast<double>(limits.at(i)), 0, 'f', 3).arg(units.at(i));
        }
    }
    return "0 B";
}

QString formatSpeedHumanReadable(double bytes_per_second)
{
    static constexpr std::array<const char *, 5> units = {"TiB/s", "GiB/s", "MiB/s", "KiB/s", "B/s"};
    static constexpr std::array<double, 5> limits = {static_cast<double>(1ll << 40ll),
                                                     static_cast<double>(1ll << 30ll),
                                                     static_cast<double>(1ll << 20ll),
                                                     static_cast<double>(1ll << 10ll),
                                                     1.0};
    for (std::size_t i = 0; i < units.size(); ++i) {
        if (bytes_per_second >= limits.at(i)) {
            return QString("%1 %2").arg(bytes_per_second / static_cast<double>(limits.at(i)), 0, 'f', 2).arg(units.at(i));
        }
    }
    return "0 B/s";
}

QString formatDurationHumanReadable(const QDateTime &from, const QDateTime &to)
{
    static constexpr std::array<qint64, 6> conversions = {365ll * 30ll * 24ll * 60ll * 60ll * 1000ll,
                                                          34 * 30ll * 60ll * 60ll * 1000ll,
                                                          24ll * 60ll * 60ll * 1000ll,
                                                          60ll * 60ll * 1000ll,
                                                          60ll * 1000ll,
                                                          1000ll};
    static constexpr std::array<const char *, 6> units = {"year", "month", "day", "hour", "minute", "second"};

    QString result = "";
    if (from > to) {
        return result;
    }
    qint64 msecs = from.msecsTo(to);
    if (msecs < 1000) {
        return result;
    }
    for (std::size_t i = 0; i < conversions.size(); ++i) {
        if (msecs >= conversions.at(i)) {
            if (!result.isEmpty()) {
                result += ", ";
            }
            qint64 const count = msecs / conversions.at(i);
            result += QString("%1 %2%3 ").arg(count).arg(units.at(i)).arg(count > 1 ? "s" : "");
            msecs %= conversions.at(i);
        }
    }

    return result;
}

QString fileUrlToString(const QUrl &url)
{
    return url.toString(QUrl::PreferLocalFile);
}

QStringList fileUrlsToStrings(const QList<QUrl> &urls)
{
    QStringList result;
    std::transform(urls.begin(), urls.end(), std::back_inserter(result), fileUrlToString);
    return result;
}

qint64 toMSecsSinceEpoch(const QDateTime &dateTime)
{
    return dateTime.toMSecsSinceEpoch();
}

void Util::setClipboardText(const QString &text)
{
    ::setClipboardText(text);
}
QString Util::formatCapacityHumanReadable(long bytes)
{
    return ::formatCapacityHumanReadable(bytes);
}
QString Util::formatSpeedHumanReadable(double bytes_per_second)
{
    return ::formatSpeedHumanReadable(bytes_per_second);
}
QString Util::formatDurationHumanReadable(const QDateTime &from, const QDateTime &to)
{
    return ::formatDurationHumanReadable(from, to);
}
QString Util::fileUrlToString(const QUrl &url)
{
    return ::fileUrlToString(url);
}
QStringList Util::fileUrlsToStrings(const QList<QUrl> &urls)
{
    return ::fileUrlsToStrings(urls);
}
qint64 Util::toMSecsSinceEpoch(const QDateTime &dateTime)
{
    return ::toMSecsSinceEpoch(dateTime);
}
QIcon Util::loadOsIcon(const QString &os)
{
    return ::loadOsIcon(os);
}
void Util::setExitNode(const QString &node)
{
    QByteArray bytes = node.toUtf8();
    GoString tmp{bytes.data(), bytes.size()};
    tailscale_set_exit_node(&tmp);
}
void Util::unsetExitNode()
{
    GoString tmp{nullptr, 0};
    tailscale_set_exit_node(&tmp);
}
