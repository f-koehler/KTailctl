#ifndef KTAILCTL_NODES_NODEPROPERTIES_HPP
#define KTAILCTL_NODES_NODEPROPERTIES_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtQmlIntegration/qqmlintegration.h>

class NodeProperties : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ getId CONSTANT)
    Q_PROPERTY(QString name READ getName NOTIFY nameChanged)
    Q_PROPERTY(QString publicKey READ getPublicKey NOTIFY publicKeyChanged)
    Q_PROPERTY(QStringList ipAddresses READ getIpAddresses NOTIFY ipAddressesChanged)
    QML_ELEMENT

private:
    QString mId;
    QString mName;
    QString mPublicKey;
    QStringList mIpAddresses;

public:
    explicit NodeProperties(const QString &id, QObject *parent = nullptr);
    explicit NodeProperties(QString &&id, QObject *parent = nullptr);

    const QString &getId() const;
    const QString &getName() const;
    const QString &getPublicKey() const;
    const QStringList &getIpAddresses() const;

signals:
    void nameChanged(const QString &) const;
    void publicKeyChanged(const QString &) const;
    void ipAddressesChanged(const QStringList &) const;
};

#endif /* KTAILCTL_KTAILCTL_NODES_NODEPROPERTIES_HPP */
