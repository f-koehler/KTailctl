#ifndef KTAILCTL_UTIL_HPP
#define KTAILCTL_UTIL_HPP

#include <KSystemClipboard>
#include <QClipboard>
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

    Q_INVOKABLE static void setClipboardText(const QString &text)
    {
        auto *data = new QMimeData();
        data->setData(QStringLiteral("text/plain"), text.toUtf8());
        KSystemClipboard::instance()->setMimeData(data, QClipboard::Clipboard);
    }
};

#endif // KTAILCTL_UTIL_HPP
