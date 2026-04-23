#ifndef KTAILCTL_UTIL_HPP
#define KTAILCTL_UTIL_HPP

#include <KSystemClipboard>
#include <QClipboard>
#include <QDateTime>
#include <QMimeData>
#include <QObject>

class Util : public QObject
{
    Q_OBJECT

public:
    explicit Util(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    Q_INVOKABLE static QString formatDurationHumanReadable(const QDateTime &dateTime);
    Q_INVOKABLE static void setClipboardText(const QString &text);
};

#endif // KTAILCTL_UTIL_HPP
