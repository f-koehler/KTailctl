#include "util.hpp"

QString Util::formatDurationHumanReadable(const QDateTime &dateTime)
{
    if (!dateTime.isValid()) {
        return QString{};
    }
    const qint64 secs = dateTime.secsTo(QDateTime::currentDateTimeUtc());
    if (secs < 60) {
        return QStringLiteral("just now");
    }
    if (secs < 3600) {
        return QString::number(secs / 60) + QStringLiteral(" minutes ago");
    }
    if (secs < 86400) {
        return QString::number(secs / 3600) + QStringLiteral(" hours ago");
    }
    return QString::number(secs / 86400) + QStringLiteral(" days ago");
}

void Util::setClipboardText(const QString &text)
{
    auto *data = new QMimeData();
    data->setData(QStringLiteral("text/plain"), text.toUtf8());
    KSystemClipboard::instance()->setMimeData(data, QClipboard::Clipboard);
}
