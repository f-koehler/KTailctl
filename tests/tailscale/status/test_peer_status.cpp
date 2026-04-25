#include <QJsonArray>
#include <QJsonObject>
#include <QTest>

#include "tailscale/status/peer_status.hpp"

namespace
{

class TestPeerStatus : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    // NOLINTNEXTLINE(readability-function-cognitive-complexity)
    static void parseBasicFields()
    {
        static constexpr qint64 user_id = 99;
        QJsonObject json{
            {QStringLiteral("ID"), QStringLiteral("n:abc123")},
            {QStringLiteral("PublicKey"), QStringLiteral("nodekey:aabbcc")},
            {QStringLiteral("HostName"), QStringLiteral("my-desktop")},
            {QStringLiteral("DNSName"), QStringLiteral("my-desktop.example.ts.net.")},
            {QStringLiteral("OS"), QStringLiteral("linux")},
            {QStringLiteral("UserId"), user_id},
            {QStringLiteral("AltSharerUserId"), user_id},
            {QStringLiteral("Relay"), QStringLiteral("fra")},
            {QStringLiteral("Online"), true},
            {QStringLiteral("ExitNode"), false},
            {QStringLiteral("ExitNodeOption"), true},
            {QStringLiteral("Active"), true},
        };

        PeerStatus peer;
        peer.updateFromJson(json);

        QCOMPARE(peer.id(), QStringLiteral("n:abc123"));
        QCOMPARE(peer.publicKey(), QStringLiteral("nodekey:aabbcc"));
        QCOMPARE(peer.hostName(), QStringLiteral("my-desktop"));
        QCOMPARE(peer.dnsName(), QStringLiteral("my-desktop.example.ts.net."));
        QCOMPARE(peer.os(), QStringLiteral("linux"));
        QCOMPARE(peer.userId(), user_id);
        QCOMPARE(peer.relay(), QStringLiteral("fra"));
        QCOMPARE(peer.online(), true);
        QCOMPARE(peer.exitNode(), false);
        QCOMPARE(peer.exitNodeOption(), true);
        QCOMPARE(peer.active(), true);
    }

    static void parseIpAndRouteLists()
    {
        QJsonObject json{
            {QStringLiteral("TailscaleIPs"), QJsonArray{QStringLiteral("100.64.0.5"), QStringLiteral("fd7a::5")}},
            {QStringLiteral("AllowedIPs"), QJsonArray{QStringLiteral("100.64.0.5/32")}},
            {QStringLiteral("PrimaryRoutes"), QJsonArray{QStringLiteral("10.0.0.0/8")}},
            {QStringLiteral("Addresses"), QJsonArray{QStringLiteral("100.64.0.5/32")}},
        };

        PeerStatus peer;
        peer.updateFromJson(json);

        QCOMPARE(peer.tailscaleIps().size(), 2);
        QCOMPARE(peer.tailscaleIps().at(0), QStringLiteral("100.64.0.5"));
        QCOMPARE(peer.allowedIps().size(), 1);
        QCOMPARE(peer.primaryRoutes().size(), 1);
        QCOMPARE(peer.primaryRoutes().at(0), QStringLiteral("10.0.0.0/8"));
    }

    static void parseByteCounts()
    {
        static constexpr qint64 received_bytes = 1024L * 1024L;
        static constexpr qint64 transmitted_bytes = 2048L * 1024L;

        QJsonObject json{
            {QStringLiteral("ReceivedBytes"), received_bytes},
            {QStringLiteral("TransmittedBytes"), transmitted_bytes},
        };

        PeerStatus peer;
        peer.updateFromJson(json);

        QCOMPARE(peer.receivedBytes(), received_bytes);
        QCOMPARE(peer.transmittedBytes(), transmitted_bytes);
    }

    static void parseDateFields()
    {
        QJsonObject json{
            {QStringLiteral("Created"), QStringLiteral("2024-01-15T10:30:00.000Z")},
            {QStringLiteral("LastWrite"), QStringLiteral("2025-03-01T12:00:00.000Z")},
            {QStringLiteral("LastSeen"), QStringLiteral("2025-04-01T08:00:00.000Z")},
            {QStringLiteral("LastHandshake"), QStringLiteral("2025-04-24T23:59:00.000Z")},
        };

        PeerStatus peer;
        peer.updateFromJson(json);

        QVERIFY(peer.created().isValid());
        QCOMPARE(peer.created().date().year(), 2024);
        QCOMPARE(peer.created().date().month(), 1);
        QCOMPARE(peer.created().date().day(), 15);

        QVERIFY(peer.lastSeen().isValid());
        QCOMPARE(peer.lastSeen().date().year(), 2025);
    }

    static void parseZeroLastSeenBecomesCurrentTime()
    {
        // The Go daemon emits "0001-01-01T00:00:00Z" for unknown/never-seen peers.
        // The code maps this to QDateTime::currentDateTimeUtc() so QML can display "just now".
        QJsonObject json{
            {QStringLiteral("LastSeen"), QStringLiteral("0001-01-01T00:00:00Z")},
        };

        const QDateTime before = QDateTime::currentDateTimeUtc();
        PeerStatus peer;
        peer.updateFromJson(json);
        const QDateTime after = QDateTime::currentDateTimeUtc();

        // lastSeen must be between the two timestamps captured around the call
        QVERIFY(peer.lastSeen() >= before);
        QVERIFY(peer.lastSeen() <= after);
    }

    static void mullvadNodeDetectedFromTags()
    {
        QJsonObject jsonMullvad{
            {QStringLiteral("Tags"),
             QJsonArray{
                 QStringLiteral("tag:mullvad-exit-node"),
                 QStringLiteral("tag:other"),
             }},
        };
        QJsonObject jsonNonMullvad{
            {QStringLiteral("Tags"), QJsonArray{QStringLiteral("tag:exit-node")}},
        };
        QJsonObject jsonNoTags;

        PeerStatus peer;

        peer.updateFromJson(jsonMullvad);
        QCOMPARE(peer.mullvadNode(), true);

        peer.updateFromJson(jsonNonMullvad);
        QCOMPARE(peer.mullvadNode(), false);

        peer.updateFromJson(jsonNoTags);
        QCOMPARE(peer.mullvadNode(), false);
    }

    static void parseWithLocation()
    {
        static constexpr double longitude = 18.07;
        static constexpr double latitude = 59.33;
        static constexpr int priority = 10;

        QJsonObject locationJson{
            {QStringLiteral("Country"), QStringLiteral("Sweden")},
            {QStringLiteral("CountryCode"), QStringLiteral("SE")},
            {QStringLiteral("City"), QStringLiteral("Stockholm")},
            {QStringLiteral("CityCode"), QStringLiteral("sto")},
            {QStringLiteral("Latitude"), latitude},
            {QStringLiteral("Longitude"), longitude},
            {QStringLiteral("Priority"), priority},
        };
        QJsonObject json{{QStringLiteral("Location"), locationJson}};

        PeerStatus peer;
        peer.updateFromJson(json);

        QVERIFY(peer.location() != nullptr);
        QCOMPARE(peer.location()->country(), QStringLiteral("Sweden"));
        QCOMPARE(peer.location()->countryCode(), QStringLiteral("SE"));
        QCOMPARE(peer.location()->city(), QStringLiteral("Stockholm"));
        QCOMPARE(peer.location()->latitude(), latitude);
        QCOMPARE(peer.location()->longitude(), longitude);
        QCOMPARE(peer.location()->priority(), priority);
    }

    static void parseWithoutLocationYieldsNullptr()
    {
        QJsonObject json{{QStringLiteral("HostName"), QStringLiteral("no-location")}};

        PeerStatus peer;
        peer.updateFromJson(json);

        QCOMPARE(peer.location(), nullptr);
    }

    static void locationRemovedOnSubsequentUpdate()
    {
        QJsonObject withLocation{
            {QStringLiteral("Location"),
             QJsonObject{
                 {QStringLiteral("Country"), QStringLiteral("Germany")},
                 {QStringLiteral("CountryCode"), QStringLiteral("DE")},
             }},
        };
        QJsonObject withoutLocation{{QStringLiteral("HostName"), QStringLiteral("host")}};

        PeerStatus peer;
        peer.updateFromJson(withLocation);
        QVERIFY(peer.location() != nullptr);

        peer.updateFromJson(withoutLocation);
        // The old Location object is scheduled for deleteLater; the property is set to nullptr.
        QCOMPARE(peer.location(), nullptr);
    }

    static void parseSshHostKeys()
    {
        QJsonObject json{
            {QStringLiteral("sshHostKeys"),
             QJsonArray{
                 QStringLiteral("ecdsa-sha2-nistp256 AAAA..."),
                 QStringLiteral("ssh-ed25519 AAAA..."),
             }},
        };

        PeerStatus peer;
        peer.updateFromJson(json);

        QCOMPARE(peer.sshHostKeys().size(), 2);
        QVERIFY(peer.sshHostKeys().at(0).startsWith(QStringLiteral("ecdsa")));
    }

    static void parseEmptyJson()
    {
        QJsonObject json;
        PeerStatus peer;
        peer.updateFromJson(json);

        QCOMPARE(peer.id(), QString{});
        QCOMPARE(peer.online(), false);
        QCOMPARE(peer.exitNode(), false);
        QCOMPARE(peer.mullvadNode(), false);
        QCOMPARE(peer.receivedBytes(), static_cast<qint64>(0));
        QCOMPARE(peer.location(), nullptr);
    }
};

}

QTEST_GUILESS_MAIN(TestPeerStatus)
#include "test_peer_status.moc"
