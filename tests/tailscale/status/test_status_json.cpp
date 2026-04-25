#include <QJsonArray>
#include <QJsonObject>
#include <QSignalSpy>
#include <QTest>

#include "tailscale/status/status.hpp"

// Status::Status() calls refresh() → tailscale_status() in its constructor.
// tailscale_wrapper_stub.cpp provides a stub that returns "{}", so the
// constructor completes with all fields at defaults. Tests then call
// updateFromJson() directly with crafted JSON to exercise the parsing logic.

namespace
{

class TestStatusJson : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    static void parseBasicFields()
    {
        Status status;
        QJsonObject json{
            {QStringLiteral("Version"), QStringLiteral("1.80.0")},
            {QStringLiteral("TUN"), true},
            {QStringLiteral("HaveNodeKey"), true},
            {QStringLiteral("AuthUrl"), QStringLiteral("https://login.tailscale.com/a/abc")},
            {QStringLiteral("TailscaleIps"), QJsonArray{QStringLiteral("100.64.0.1")}},
            {QStringLiteral("BackendState"), QStringLiteral("Running")},
        };

        status.updateFromJson(json);

        QCOMPARE(status.version(), QStringLiteral("1.80.0"));
        QCOMPARE(status.isTun(), true);
        QCOMPARE(status.haveNodeKey(), true);
        QCOMPARE(status.authUrl(), QStringLiteral("https://login.tailscale.com/a/abc"));
        QCOMPARE(status.tailscaleIps().size(), 1);
        QCOMPARE(status.backendState(), Status::BackendState::Running);
    }

    static void parseAllBackendStates()
    {
        const QList<QPair<QString, Status::BackendState>> cases = {
            {QStringLiteral("NoState"), Status::BackendState::NoState},
            {QStringLiteral("NeedsLogin"), Status::BackendState::NeedsLogin},
            {QStringLiteral("NeedsMachineAuth"), Status::BackendState::NeedsMachineAuth},
            {QStringLiteral("Stopped"), Status::BackendState::Stopped},
            {QStringLiteral("Starting"), Status::BackendState::Starting},
            {QStringLiteral("Running"), Status::BackendState::Running},
        };

        Status status;
        for (const auto &[stateStr, expectedState] : cases) {
            QJsonObject json{{QStringLiteral("BackendState"), stateStr}};
            status.updateFromJson(json);
            QCOMPARE(status.backendState(), expectedState);
        }
    }

    static void unknownBackendStateFallsBackToNoState()
    {
        Status status;
        QJsonObject json{{QStringLiteral("BackendState"), QStringLiteral("Bogus")}};
        status.updateFromJson(json);
        QCOMPARE(status.backendState(), Status::BackendState::NoState);
    }

    static void backendStateChangedSignalEmitted()
    {
        Status status;

        // Prime to a known state
        QJsonObject prime{{QStringLiteral("BackendState"), QStringLiteral("Stopped")}};
        status.updateFromJson(prime);

        const QSignalSpy spy(&status, &Status::backendStateChanged);
        QJsonObject json{{QStringLiteral("BackendState"), QStringLiteral("Running")}};
        status.updateFromJson(json);

        QCOMPARE(spy.count(), 1);
    }

    static void backendStateChangedNotEmittedWhenSame()
    {
        // updateFromJson uses json.take() which removes keys from the object, so
        // a fresh QJsonObject must be created for every call.
        auto makeRunning = []() -> QJsonObject {
            return {{QStringLiteral("BackendState"), QStringLiteral("Running")}};
        };

        Status status;
        auto prime = makeRunning();
        status.updateFromJson(prime); // NoState → Running (before spy)

        const QSignalSpy spy(&status, &Status::backendStateChanged);
        auto same = makeRunning();
        status.updateFromJson(same); // Running → Running: no signal expected

        QCOMPARE(spy.count(), 0);
    }

    static void parseSelfPeer()
    {
        Status status;
        QJsonObject json{
            {QStringLiteral("Self"),
             QJsonObject{
                 {QStringLiteral("ID"), QStringLiteral("n:self1")},
                 {QStringLiteral("HostName"), QStringLiteral("my-machine")},
                 {QStringLiteral("Online"), true},
             }},
        };

        status.updateFromJson(json);

        QVERIFY(status.self() != nullptr);
        QCOMPARE(status.self()->id(), QStringLiteral("n:self1"));
        QCOMPARE(status.self()->hostName(), QStringLiteral("my-machine"));
        QCOMPARE(status.self()->online(), true);
    }

    static void selfClearedWhenMissingFromJson()
    {
        Status status;
        QJsonObject withSelf{
            {QStringLiteral("Self"),
             QJsonObject{
                 {QStringLiteral("ID"), QStringLiteral("n:self1")},
             }},
        };
        QJsonObject withoutSelf;

        status.updateFromJson(withSelf);
        QVERIFY(status.self() != nullptr);

        status.updateFromJson(withoutSelf);
        QCOMPARE(status.self(), nullptr);
    }

    static void parsePeers()
    {
        Status status;
        QJsonObject json{
            {QStringLiteral("Peer"),
             QJsonObject{
                 {QStringLiteral("nodekey:aaa"),
                  QJsonObject{
                      {QStringLiteral("ID"), QStringLiteral("n:aaa")},
                      {QStringLiteral("HostName"), QStringLiteral("peer-a")},
                  }},
                 {QStringLiteral("nodekey:bbb"),
                  QJsonObject{
                      {QStringLiteral("ID"), QStringLiteral("n:bbb")},
                      {QStringLiteral("HostName"), QStringLiteral("peer-b")},
                  }},
             }},
        };

        status.updateFromJson(json);

        QCOMPARE(status.peerModel()->rowCount({}), 2);
    }

    static void peerWithIdLookup()
    {
        Status status;
        QJsonObject json{
            {QStringLiteral("Peer"),
             QJsonObject{
                 {QStringLiteral("nodekey:aaa"),
                  QJsonObject{
                      {QStringLiteral("ID"), QStringLiteral("n:aaa")},
                      {QStringLiteral("HostName"), QStringLiteral("peer-a")},
                  }},
             }},
        };

        status.updateFromJson(json);

        const PeerStatus *found = status.peerWithId(QStringLiteral("n:aaa"));
        QVERIFY(found != nullptr);
        QCOMPARE(found->hostName(), QStringLiteral("peer-a"));

        QCOMPARE(status.peerWithId(QStringLiteral("n:missing")), nullptr);
    }

    static void peerRemovedOnSubsequentUpdate()
    {
        Status status;
        QJsonObject json1{
            {QStringLiteral("Peer"),
             QJsonObject{
                 {QStringLiteral("nodekey:aaa"),
                  QJsonObject{
                      {QStringLiteral("ID"), QStringLiteral("n:aaa")},
                      {QStringLiteral("HostName"), QStringLiteral("peer-a")},
                  }},
                 {QStringLiteral("nodekey:bbb"),
                  QJsonObject{
                      {QStringLiteral("ID"), QStringLiteral("n:bbb")},
                      {QStringLiteral("HostName"), QStringLiteral("peer-b")},
                  }},
             }},
        };
        QJsonObject json2{
            {QStringLiteral("Peer"),
             QJsonObject{
                 {QStringLiteral("nodekey:aaa"),
                  QJsonObject{
                      {QStringLiteral("ID"), QStringLiteral("n:aaa")},
                      {QStringLiteral("HostName"), QStringLiteral("peer-a")},
                  }},
             }},
        };

        status.updateFromJson(json1);
        QCOMPARE(status.peerModel()->rowCount({}), 2);

        status.updateFromJson(json2);
        QCOMPARE(status.peerModel()->rowCount({}), 1);
        QVERIFY(status.peerWithId(QStringLiteral("n:bbb")) == nullptr);
    }

    static void parseCurrentTailnet()
    {
        Status status;
        QJsonObject json{
            {QStringLiteral("CurrentTailnet"),
             QJsonObject{
                 {QStringLiteral("Name"), QStringLiteral("example.ts.net")},
                 {QStringLiteral("MagicDnsEnabled"), true},
             }},
        };

        status.updateFromJson(json);

        QVERIFY(status.currentTailnet() != nullptr);
        QCOMPARE(status.currentTailnet()->name(), QStringLiteral("example.ts.net"));
        QCOMPARE(status.currentTailnet()->magicDnsEnabled(), true);
    }

    static void parseHealthMessages()
    {
        Status status;
        QJsonObject json{
            {QStringLiteral("Health"),
             QJsonArray{
                 QStringLiteral("warning: subnet route needs approval"),
             }},
        };

        status.updateFromJson(json);

        QCOMPARE(status.health().size(), 1);
        QVERIFY(status.health().at(0).contains(QStringLiteral("subnet route")));
    }

    static void peerWithIdReturnsSelfWhenIdMatches()
    {
        Status status;
        QJsonObject json{
            {QStringLiteral("Self"),
             QJsonObject{
                 {QStringLiteral("ID"), QStringLiteral("n:self")},
                 {QStringLiteral("HostName"), QStringLiteral("selfhost")},
             }},
        };

        status.updateFromJson(json);

        const PeerStatus *found = status.peerWithId(QStringLiteral("n:self"));
        QVERIFY(found != nullptr);
        QCOMPARE(found->hostName(), QStringLiteral("selfhost"));
    }

    // -----------------------------------------------------------------------
    // Tests below use realistic (anonymized) JSON that mirrors real-world
    // tailscale status output.  Mullvad node data is public and kept as-is.
    // -----------------------------------------------------------------------

    static void parseRealisticStatus()
    {
        // Full top-level fields + Self + CurrentTailnet, no peers.
        // Mirrors the structure of a real "tailscale status --json" snapshot
        // with all personal details anonymized.
        static const QString version = QStringLiteral("1.96.4-t8cf541dfd-g62bc84ce7");
        static const QString selfId = QStringLiteral("nAAAAAAAAAAAAAAAA");
        static const QString hostName = QStringLiteral("laptop");
        static const QString dnsName = QStringLiteral("laptop.example-tailnet.ts.net.");
        static const QString operatingSystem = QStringLiteral("linux");
        static const QString relay = QStringLiteral("sin");
        static const QString tailnetName = QStringLiteral("example-user.github");
        static const QString magicDnsSuffix = QStringLiteral("example-tailnet.ts.net");
        static constexpr qint64 userId = 12345678901234567UL;

        Status status;
        QJsonObject json{
            {QStringLiteral("Version"), version},
            {QStringLiteral("TUN"), true},
            {QStringLiteral("BackendState"), QStringLiteral("Running")},
            {QStringLiteral("HaveNodeKey"), true},
            {QStringLiteral("AuthUrl"), QStringLiteral("")},
            {QStringLiteral("TailscaleIps"),
             QJsonArray{
                 QStringLiteral("100.64.0.1"),
                 QStringLiteral("fd7a:115c:a1e0::1"),
             }},
            {QStringLiteral("Self"),
             QJsonObject{
                 {QStringLiteral("ID"), selfId},
                 {QStringLiteral("PublicKey"), QStringLiteral("nodekey:aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa")},
                 {QStringLiteral("HostName"), hostName},
                 {QStringLiteral("DNSName"), dnsName},
                 {QStringLiteral("OS"), operatingSystem},
                 {QStringLiteral("UserId"), userId},
                 {QStringLiteral("TailscaleIPs"),
                  QJsonArray{
                      QStringLiteral("100.64.0.1"),
                      QStringLiteral("fd7a:115c:a1e0::1"),
                  }},
                 {QStringLiteral("AllowedIPs"),
                  QJsonArray{
                      QStringLiteral("100.64.0.1/32"),
                      QStringLiteral("fd7a:115c:a1e0::1/128"),
                  }},
                 {QStringLiteral("Relay"), relay},
                 {QStringLiteral("PeerRelay"), QString{}},
                 {QStringLiteral("ReceivedBytes"), 0},
                 {QStringLiteral("TransmittedBytes"), 0},
                 {QStringLiteral("Created"), QStringLiteral("2026-01-10T02:59:47.302705195Z")},
                 {QStringLiteral("Online"), true},
                 {QStringLiteral("ExitNode"), false},
                 {QStringLiteral("ExitNodeOption"), false},
                 {QStringLiteral("Active"), false},
                 {QStringLiteral("InNetworkMap"), true},
                 {QStringLiteral("InMagicSock"), false},
                 {QStringLiteral("InEngine"), false},
             }},
            {QStringLiteral("Health"), QJsonArray{}},
            {QStringLiteral("CurrentTailnet"),
             QJsonObject{
                 {QStringLiteral("Name"), tailnetName},
                 {QStringLiteral("MagicDnsSuffix"), magicDnsSuffix},
                 {QStringLiteral("MagicDnsEnabled"), true},
             }},
        };

        status.updateFromJson(json);

        QCOMPARE(status.version(), version);
        QCOMPARE(status.isTun(), true);
        QCOMPARE(status.haveNodeKey(), true);
        QCOMPARE(status.backendState(), Status::BackendState::Running);
        QCOMPARE(status.tailscaleIps().size(), 2);
        QCOMPARE(status.health().size(), 0);
        QCOMPARE(status.peerModel()->rowCount({}), 0);

        QVERIFY(status.self() != nullptr);
        QCOMPARE(status.self()->id(), selfId);
        QCOMPARE(status.self()->hostName(), hostName);
        QCOMPARE(status.self()->dnsName(), dnsName);
        QCOMPARE(status.self()->os(), operatingSystem);
        QCOMPARE(status.self()->online(), true);
        QCOMPARE(status.self()->relay(), relay);
        QCOMPARE(status.self()->tailscaleIps().size(), 2);

        QVERIFY(status.currentTailnet() != nullptr);
        QCOMPARE(status.currentTailnet()->name(), tailnetName);
        QCOMPARE(status.currentTailnet()->magicDnsSuffix(), magicDnsSuffix);
        QCOMPARE(status.currentTailnet()->magicDnsEnabled(), true);
    }

    static void parseRealisticRegularPeer()
    {
        // A regular user-device peer: no Tags, no Location, not an exit-node option.
        // Field names match what peer_status.cpp actually reads (camelCase variants).
        static const QString peerId = QStringLiteral("nBBBBBBBBBBBBBBBB");
        static const QString publicKey = QStringLiteral("nodekey:bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
        static const QString hostName = QStringLiteral("workstation");
        static const QString dnsName = QStringLiteral("workstation.example-tailnet.ts.net.");
        static const QString operatingSystem = QStringLiteral("linux");
        static const QString relay = QStringLiteral("sin");
        static constexpr qint64  userId = 1234567891234567UL;

        Status status;
        QJsonObject json{
            {QStringLiteral("Peer"),
             QJsonObject{
                 {publicKey,
                  QJsonObject{
                      {QStringLiteral("ID"), peerId},
                      {QStringLiteral("PublicKey"), publicKey},
                      {QStringLiteral("HostName"), hostName},
                      {QStringLiteral("DNSName"), dnsName},
                      {QStringLiteral("OS"), operatingSystem},
                      {QStringLiteral("UserId"), userId},
                      {QStringLiteral("TailscaleIPs"),
                       QJsonArray{
                           QStringLiteral("100.64.0.2"),
                           QStringLiteral("fd7a:115c:a1e0::2"),
                       }},
                      {QStringLiteral("AllowedIPs"),
                       QJsonArray{
                           QStringLiteral("100.64.0.2/32"),
                           QStringLiteral("fd7a:115c:a1e0::2/128"),
                       }},
                      {QStringLiteral("Relay"), relay},
                      {QStringLiteral("PeerRelay"), QStringLiteral("")},
                      {QStringLiteral("ReceivedBytes"), 0},
                      {QStringLiteral("TransmittedBytes"), 0},
                      {QStringLiteral("Created"), QStringLiteral("2025-12-08T02:35:16.715111557Z")},
                      {QStringLiteral("LastSeen"), QStringLiteral("2026-04-24T09:50:17.1Z")},
                      {QStringLiteral("Online"), false},
                      {QStringLiteral("ExitNode"), false},
                      {QStringLiteral("ExitNodeOption"), false},
                      {QStringLiteral("Active"), false},
                      {QStringLiteral("InNetworkMap"), true},
                      {QStringLiteral("InMagicSock"), true},
                      {QStringLiteral("InEngine"), false},
                  }},
             }},
        };

        status.updateFromJson(json);

        QCOMPARE(status.peerModel()->rowCount({}), 1);

        const PeerStatus *peer = status.peerWithId(peerId);
        QVERIFY(peer != nullptr);
        QCOMPARE(peer->hostName(), hostName);
        QCOMPARE(peer->dnsName(), dnsName);
        QCOMPARE(peer->os(), operatingSystem);
        QCOMPARE(peer->online(), false);
        QCOMPARE(peer->exitNode(), false);
        QCOMPARE(peer->exitNodeOption(), false);
        QCOMPARE(peer->mullvadNode(), false);
        QVERIFY(peer->location() == nullptr);
        QCOMPARE(peer->tags().size(), 0);
        QCOMPARE(peer->tailscaleIps().size(), 2);
        QCOMPARE(peer->relay(), relay);
        QCOMPARE(peer->inNetworkMap(), true);
        QCOMPARE(peer->inMagicSock(), true);
    }

    static void parseRealisticMullvadPeer()
    {
        // A real Mullvad exit node (public infrastructure, kept as-is):
        // se-got-wg-101 in Gothenburg, Sweden.
        static const QString peerId = QStringLiteral("nGrHAyXRy211CNTRL");
        static const QString publicKey = QStringLiteral("nodekey:07c51501e364016e0d8861ddd65a65f77dc3ae1e32ee932aa572691f44ab1a34");
        static const QString hostName = QStringLiteral("se-got-wg-101");
        static const QString dnsName = QStringLiteral("se-got-wg-101.mullvad.ts.net.");
        static const QString country = QStringLiteral("Sweden");
        static const QString countryCode = QStringLiteral("SE");
        static const QString city = QStringLiteral("Gothenburg");
        static const QString cityCode = QStringLiteral("GOT");
        static constexpr double latitude = 57.70887;
        static constexpr double longitude = 11.97456;
        static constexpr int priority = 100;
        static constexpr qint64 userId = 38520981566058213UL;

        Status status;
        QJsonObject json{
            {QStringLiteral("Peer"),
             QJsonObject{
                 {publicKey,
                  QJsonObject{
                      {QStringLiteral("ID"), peerId},
                      {QStringLiteral("PublicKey"), publicKey},
                      {QStringLiteral("HostName"), hostName},
                      {QStringLiteral("DNSName"), dnsName},
                      {QStringLiteral("OS"), QStringLiteral("")},
                      {QStringLiteral("UserId"), userId},
                      {QStringLiteral("TailscaleIPs"),
                       QJsonArray{
                           QStringLiteral("100.107.26.53"),
                           QStringLiteral("fd7a:115c:a1e0::4401:1a35"),
                       }},
                      {QStringLiteral("AllowedIPs"),
                       QJsonArray{
                           QStringLiteral("100.107.26.53/32"),
                           QStringLiteral("fd7a:115c:a1e0::4401:1a35/128"),
                           QStringLiteral("0.0.0.0/0"),
                           QStringLiteral("::/0"),
                       }},
                      {QStringLiteral("Tags"), QJsonArray{QStringLiteral("tag:mullvad-exit-node")}},
                      {QStringLiteral("Relay"), QString{}},
                      {QStringLiteral("PeerRelay"), QString{}},
                      {QStringLiteral("ReceivedBytes"), 0},
                      {QStringLiteral("TransmittedBytes"), 0},
                      {QStringLiteral("Created"), QStringLiteral("2025-02-19T13:29:47.031879235Z")},
                      {QStringLiteral("Online"), true},
                      {QStringLiteral("ExitNode"), false},
                      {QStringLiteral("ExitNodeOption"), true},
                      {QStringLiteral("Active"), false},
                      {QStringLiteral("InNetworkMap"), true},
                      {QStringLiteral("InMagicSock"), true},
                      {QStringLiteral("InEngine"), false},
                      {QStringLiteral("Location"),
                       QJsonObject{
                           {QStringLiteral("Country"), country},
                           {QStringLiteral("CountryCode"), countryCode},
                           {QStringLiteral("City"), city},
                           {QStringLiteral("CityCode"), cityCode},
                           {QStringLiteral("Latitude"), latitude},
                           {QStringLiteral("Longitude"), longitude},
                           {QStringLiteral("Priority"), priority},
                       }},
                  }},
             }},
        };

        status.updateFromJson(json);

        QCOMPARE(status.peerModel()->rowCount({}), 1);

        const PeerStatus *peer = status.peerWithId(peerId);
        QVERIFY(peer != nullptr);
        QCOMPARE(peer->hostName(), hostName);
        QCOMPARE(peer->dnsName(), dnsName);
        QCOMPARE(peer->online(), true);
        QCOMPARE(peer->exitNode(), false);
        QCOMPARE(peer->exitNodeOption(), true);
        QCOMPARE(peer->mullvadNode(), true);
        QCOMPARE(peer->tags(), QStringList{QStringLiteral("tag:mullvad-exit-node")});
        QCOMPARE(peer->allowedIps().size(), 4);

        QVERIFY(peer->location() != nullptr);
        QCOMPARE(peer->location()->country(), country);
        QCOMPARE(peer->location()->countryCode(), countryCode);
        QCOMPARE(peer->location()->city(), city);
        QCOMPARE(peer->location()->cityCode(), cityCode);
        QCOMPARE(peer->location()->latitude(), latitude);
        QCOMPARE(peer->location()->longitude(), longitude);
        QCOMPARE(peer->location()->priority(), priority);
    }

    static void parseRealisticMixedPeers()
    {
        // A realistic status with one regular user-device peer and one Mullvad
        // exit-node peer, combined in a single update.  Verifies that each type
        // is recognised and classified correctly by the parser.
        static const QString regularId = QStringLiteral("nBBBBBBBBBBBBBBBB");
        static const QString regularHostName = QStringLiteral("workstation");
        static const QString mullvadId = QStringLiteral("nGrHAyXRy211CNTRL");
        static const QString mullvadHostName = QStringLiteral("se-got-wg-101");
        static const QString mullvadCountryCode = QStringLiteral("SE");
        static constexpr double latitude = 57.70887;
        static constexpr double longitude = 11.97456;
        static constexpr int priority = 100;

        Status status;

        QJsonObject peerJson;
        peerJson.insert(QStringLiteral("nodekey:bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"),
                        QJsonObject{
                            {QStringLiteral("ID"), regularId},
                            {QStringLiteral("HostName"), regularHostName},
                            {QStringLiteral("DNSName"), QStringLiteral("workstation.example-tailnet.ts.net.")},
                            {QStringLiteral("Online"), false},
                            {QStringLiteral("ExitNode"), false},
                            {QStringLiteral("ExitNodeOption"), false},
                        });
        peerJson.insert(QStringLiteral("nodekey:07c51501e364016e0d8861ddd65a65f77dc3ae1e32ee932aa572691f44ab1a34"),
                        QJsonObject{
                            {QStringLiteral("ID"), mullvadId},
                            {QStringLiteral("HostName"), mullvadHostName},
                            {QStringLiteral("DNSName"), QStringLiteral("se-got-wg-101.mullvad.ts.net.")},
                            {QStringLiteral("Online"), true},
                            {QStringLiteral("ExitNode"), false},
                            {QStringLiteral("ExitNodeOption"), true},
                            {QStringLiteral("Tags"), QJsonArray{QStringLiteral("tag:mullvad-exit-node")}},
                            {QStringLiteral("Location"),
                             QJsonObject{
                                 {QStringLiteral("Country"), QStringLiteral("Sweden")},
                                 {QStringLiteral("CountryCode"), mullvadCountryCode},
                                 {QStringLiteral("City"), QStringLiteral("Gothenburg")},
                                 {QStringLiteral("CityCode"), QStringLiteral("GOT")},
                                 {QStringLiteral("Latitude"), latitude},
                                 {QStringLiteral("Longitude"), longitude},
                                 {QStringLiteral("Priority"), priority},
                             }},
                        });

        QJsonObject json{
            {QStringLiteral("BackendState"), QStringLiteral("Running")},
            {QStringLiteral("Self"),
             QJsonObject{
                 {QStringLiteral("ID"), QStringLiteral("nAAAAAAAAAAAAAAAA")},
                 {QStringLiteral("HostName"), QStringLiteral("laptop")},
                 {QStringLiteral("Online"), true},
             }},
            {QStringLiteral("Peer"), peerJson},
        };

        status.updateFromJson(json);

        QCOMPARE(status.peerModel()->rowCount({}), 2);

        const PeerStatus *regular = status.peerWithId(regularId);
        QVERIFY(regular != nullptr);
        QCOMPARE(regular->hostName(), regularHostName);
        QCOMPARE(regular->mullvadNode(), false);
        QVERIFY(regular->location() == nullptr);
        QCOMPARE(regular->tags().size(), 0);

        const PeerStatus *mullvad = status.peerWithId(mullvadId);
        QVERIFY(mullvad != nullptr);
        QCOMPARE(mullvad->hostName(), mullvadHostName);
        QCOMPARE(mullvad->mullvadNode(), true);
        QCOMPARE(mullvad->exitNodeOption(), true);
        QCOMPARE(mullvad->tags().size(), 1);
        QVERIFY(mullvad->location() != nullptr);
        QCOMPARE(mullvad->location()->countryCode(), mullvadCountryCode);
    }
};

}

QTEST_GUILESS_MAIN(TestStatusJson)
#include "test_status_json.moc"
