#ifndef KTAILCTL_NODES_TAILSCALENODEPROPERTIES_HPP
#define KTAILCTL_NODES_TAILSCALENODEPROPERTIES_HPP

#include <KTailctl/Nodes/NodeProperties.hpp>
#include <QDateTime>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QtQmlIntegration/qqmlintegration.h>

class TailscaleNodeProperties : public NodeProperties
{
    Q_OBJECT
    Q_PROPERTY(QStringList tags READ getTags NOTIFY tagsChanged)
    Q_PROPERTY(bool isExitNode READ isExitNode NOTIFY isExitNodeChanged)
    Q_PROPERTY(QString dnsName READ getDnsName NOTIFY dnsNameChanged)
    Q_PROPERTY(QDateTime lastSeenTime READ getLastSeenTime NOTIFY lastSeenTimeChanged)
    Q_PROPERTY(QDateTime creationTime READ getCreationTime NOTIFY creationTimeChanged)
    QML_ELEMENT

private:
    QStringList mTags;
    bool mIsExitNode;
    QString mDnsName;
    QString mOperatingSystem;
    QDateTime mLastSeenTime;
    QDateTime mCreationTime;

public:
    explicit TailscaleNodeProperties(const QString &id, QObject *parent = nullptr);
    explicit TailscaleNodeProperties(QString &&id, QObject *parent = nullptr);
    virtual ~TailscaleNodeProperties() = default;

    const QStringList &getTags() const;
    bool isExitNode() const;
    const QString &getDnsName() const;
    const QString &getOperatingSystem() const;
    const QDateTime &getLastSeenTime() const;
    const QDateTime &getCreationTime() const;

signals:
    void tagsChanged(const QStringList &);
    void isExitNodeChanged(bool);
    void dnsNameChanged(const QString &);
    void operatingSystemChanged(const QString &);
    void lastSeenTimeChanged(const QDateTime &);
    void creationTimeChanged(const QDateTime &);
};

#endif /* KTAILCTL_KTAILCTL_NODES_TAILSCALENODEPROPERTIES_HPP */
