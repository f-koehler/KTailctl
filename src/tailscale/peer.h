#ifndef KTAILCTL_PEER_H
#define KTAILCTL_PEER_H

#include <QDateTime>
#include <optional>

#include "location.h"
#include "peer_data.h"

class Peer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString tailscaleID READ id NOTIFY idChanged)
    Q_PROPERTY(QString publicKey READ publicKey NOTIFY publicKeyChanged)
    Q_PROPERTY(QString hostName READ hostName NOTIFY hostNameChanged)
    Q_PROPERTY(QString dnsName READ dnsName NOTIFY dnsNameChanged)
    Q_PROPERTY(QString os READ os NOTIFY osChanged)
    Q_PROPERTY(QStringList tailscaleIps READ tailscaleIps NOTIFY tailscaleIpsChanged)
    Q_PROPERTY(QString relay READ relay NOTIFY relayChanged)
    Q_PROPERTY(long rxBytes READ rxBytes NOTIFY rxBytesChanged)
    Q_PROPERTY(long txBytes READ txBytes NOTIFY txBytesChanged)
    Q_PROPERTY(QDateTime created READ created NOTIFY createdChanged)
    Q_PROPERTY(QDateTime lastSeen READ lastSeen NOTIFY lastSeenChanged)
    Q_PROPERTY(bool isOnline READ isOnline NOTIFY isOnlineChanged)
    Q_PROPERTY(bool isActive READ isActive NOTIFY isActiveChanged)
    Q_PROPERTY(bool isCurrentExitNode READ isCurrentExitNode NOTIFY isCurrentExitNodeChanged)
    Q_PROPERTY(bool isExitNode READ isExitNode NOTIFY isExitNodeChanged)
    Q_PROPERTY(QStringList sshHostKeys READ sshHostKeys NOTIFY sshHostKeysChanged)
    Q_PROPERTY(bool isRunningSSH READ isRunningSSH NOTIFY isRunningSSHChanged)
    Q_PROPERTY(QString sshCommand READ sshCommand NOTIFY sshCommandChanged)
    Q_PROPERTY(QStringList tags READ tags NOTIFY tagsChanged)
    Q_PROPERTY(bool isMullvad READ isMullvad NOTIFY isMullvadChanged)
    Q_PROPERTY(Location *location READ location NOTIFY locationChanged)

private:
    PeerData mData;
    Location *mLocation = nullptr;
    QDateTime mCreated;
    QDateTime mLastSeen;

public:
    explicit Peer(QObject *parent = nullptr);
    virtual ~Peer() = default;

    bool update(PeerData &newData);

    const QString &id() const;
    const QString &publicKey() const;
    const QString &hostName() const;
    const QString &dnsName() const;
    const QString &os() const;
    const QStringList &tailscaleIps() const;
    const QString &relay() const;
    long rxBytes() const;
    long txBytes() const;
    const QDateTime &created() const;
    const QDateTime &lastSeen() const;
    bool isOnline() const;
    bool isActive() const;
    bool isCurrentExitNode() const;
    bool isExitNode() const;
    const QStringList &sshHostKeys() const;
    bool isRunningSSH() const;
    QString sshCommand() const;
    const QStringList &tags() const;
    bool isMullvad() const;
    Location *location() const;
    const PeerData &peerData() const;

signals:
    void idChanged(const QString &id);
    void publicKeyChanged(const QString &publicKey);
    void hostNameChanged(const QString &hostName);
    void dnsNameChanged(const QString &dnsName);
    void osChanged(const QString &os);
    void tailscaleIpsChanged(const QStringList &tailscaleIps);
    void relayChanged(const QString &relay);
    void rxBytesChanged(long rxBytes);
    void txBytesChanged(long txBytes);
    void createdChanged(const QDateTime &created);
    void lastSeenChanged(const QDateTime &lastSeen);
    void isOnlineChanged(bool isOnline);
    void isActiveChanged(bool isActive);
    void isCurrentExitNodeChanged(bool isCurrentExitNode);
    void isExitNodeChanged(bool isExitNode);
    void sshHostKeysChanged(const QStringList &sshHostKeys);
    void sshCommandChanged(const QString &sshCommand);
    void isRunningSSHChanged(bool isRunningSSH);
    void tagsChanged(const QStringList &tags);
    void isMullvadChanged(bool isMullvad);
    void locationChanged(Location *location);
};

#endif /* KTAILCTL_PEER_H */
