#ifndef KTAILCTL_UTIL_HPP
#define KTAILCTL_UTIL_HPP

#include <QClipboard>
#include <QObject>
#include <QQmlEngine>
#include <QtQmlIntegration/qqmlintegration.h>

class Util : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    inline static Util *s_instance = nullptr;
    static auto create(QQmlEngine * /*engine*/, QJSEngine * /*jsEngine*/) -> Util *
    {
        return s_instance;
    }
    static void setQmlInstance(Util *instance)
    {
        s_instance = instance;
    }

    explicit Util(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    Q_INVOKABLE static QString formatDurationHumanReadable(const QDateTime &dateTime); // NOLINT(modernize-use-trailing-return-type)
    Q_INVOKABLE static void setClipboardText(const QString &text);
    Q_INVOKABLE static QString systemUser(); // NOLINT(modernize-use-trailing-return-type)
};

#endif // KTAILCTL_UTIL_HPP
