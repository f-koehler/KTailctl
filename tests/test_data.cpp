#include "test_data.h"
#include "json.h"
#include "location.h"
#include "peer.h"
#include "status.h"

#include <QString>
#include <QStringLiteral>
#include <QTest>

void TestJSON::toQString()
{
    const auto j_str = json::parse(R"(
        "foo"
    )");
    QCOMPARE(j_str.get<QString>(), QStringLiteral("foo"));

    const auto j_object = json::parse(R"(
        {"foo": "bar"}
    )");
    QCOMPARE(j_object["foo"].get<QString>(), QStringLiteral("bar"));
}

void TestJSON::toQStringList()
{
    const auto j_array = json::parse(R"(
        ["foo", "bar"]
    )");
    const auto j_array_str = j_array.get<QStringList>();
    QCOMPARE(j_array_str.size(), 2);
    QCOMPARE(j_array_str[0], QStringLiteral("foo"));
    QCOMPARE(j_array_str[1], QStringLiteral("bar"));

    const auto j_object = json::parse(R"(
        {"foo": ["bar", "baz"]}
    )");
    const auto j_array_str2 = j_object["foo"].get<QStringList>();
    QCOMPARE(j_array_str2.size(), 2);
    QCOMPARE(j_array_str2[0], QStringLiteral("bar"));
    QCOMPARE(j_array_str2[1], QStringLiteral("baz"));
}

void TestJSON::toLocationData()
{
    const auto j = json::parse(R"(
        {
            "Country": "USA",
            "CountryCode": "US",
            "City": "San Jose, CA",
            "CityCode": "SJC",
            "Priority": 1
        }
    )");
    const auto location = j.get<LocationData>();
    QCOMPARE(location.country, QStringLiteral("USA"));
    QCOMPARE(location.countryCode, QStringLiteral("US"));
    QCOMPARE(location.city, QStringLiteral("San Jose, CA"));
    QCOMPARE(location.cityCode, QStringLiteral("SJC"));
    QCOMPARE(location.priority, 1);

    QVERIFY(location == location);
    QVERIFY(!(location != location));

    auto location2 = j.get<LocationData>();
    QVERIFY(location == location2);
    QVERIFY(!(location != location2));
    location2.city = QStringLiteral("San Francisco, CA");
    QVERIFY(!(location == location2));
    QVERIFY(location != location2);
}

void TestJSON::toPeerData()
{
    const auto j = json::parse(R"(
        {
            "ID": "n9ERQQ5CNTRL",
            "PublicKey": "nodekey:00d69102d8f1aa93e0a7bafd5634c37e704c8acf8ccd28025dced364c6b4567a",
            "HostName": "gb-mnc-wg-007",
            "DNSName": "gb-mnc-wg-007.mullvad.ts.net.",
            "OS": "",
            "UserID": 38520981566058213,
            "TailscaleIPs": [
                "100.116.86.124",
                "fd7a:115c:a1e0:ab12:4843:cd96:6274:567c"
            ],
            "Tags": [
                "tag:mullvad-exit-node"
            ],
            "Addrs": null,
            "CurAddr": "",
            "Relay": "",
            "RxBytes": 0,
            "TxBytes": 0,
            "Created": "2023-09-22T12:32:45.752541995Z",
            "LastWrite": "0001-01-01T00:00:00Z",
            "LastSeen": "0001-01-01T00:00:00Z",
            "LastHandshake": "0001-01-01T00:00:00Z",
            "Online": true,
            "ExitNode": false,
            "ExitNodeOption": true,
            "Active": false,
            "PeerAPIURL": null,
            "InNetworkMap": true,
            "InMagicSock": true,
            "InEngine": false,
            "Location": {
                "Country": "UK",
                "CountryCode": "GB",
                "City": "Manchester",
                "CityCode": "MNC",
                "Priority": 100
            }
        }
    )");
    const auto peer = j.get<PeerData>();
    QCOMPARE(peer.id, QStringLiteral("n9ERQQ5CNTRL"));
    QCOMPARE(peer.publicKey, QStringLiteral("nodekey:00d69102d8f1aa93e0a7bafd5634c37e704c8acf8ccd28025dced364c6b4567a"));
    QCOMPARE(peer.hostName, QStringLiteral("gb-mnc-wg-007"));
    QCOMPARE(peer.dnsName, QStringLiteral("gb-mnc-wg-007.mullvad.ts.net."));
    QCOMPARE(peer.os, QStringLiteral(""));
    QCOMPARE(peer.tailscaleIps.size(), 2);
    QCOMPARE(peer.tailscaleIps[0], QStringLiteral("100.116.86.124"));
    QCOMPARE(peer.tailscaleIps[1], QStringLiteral("fd7a:115c:a1e0:ab12:4843:cd96:6274:567c"));
    QCOMPARE(peer.tags.size(), 1);
    QCOMPARE(peer.tags[0], QStringLiteral("tag:mullvad-exit-node"));
    QCOMPARE(peer.rxBytes, 0);
    QCOMPARE(peer.txBytes, 0);
    QCOMPARE(peer.created, QStringLiteral("2023-09-22T12:32:45.752541995Z"));
    QCOMPARE(peer.lastSeen, QStringLiteral("0001-01-01T00:00:00Z"));
    QCOMPARE(peer.isOnline, true);
    QCOMPARE(peer.isCurrentExitNode, false);
    QCOMPARE(peer.isExitNode, true);
    QCOMPARE(peer.isActive, false);
    QVERIFY(peer.sshHostKeys.isEmpty());
    QCOMPARE(peer.tags.size(), 1);
    QCOMPARE(peer.tags[0], QStringLiteral("tag:mullvad-exit-node"));
    QVERIFY(peer.isMullvad);
    QVERIFY(peer.location.has_value());
    QCOMPARE(peer.location.value().country, QStringLiteral("UK"));
    QCOMPARE(peer.location.value().countryCode, QStringLiteral("GB"));
    QCOMPARE(peer.location.value().city, QStringLiteral("Manchester"));
    QCOMPARE(peer.location.value().cityCode, QStringLiteral("MNC"));
    QCOMPARE(peer.location.value().priority, 100);

    QVERIFY(peer == peer);
    QVERIFY(!(peer != peer));

    auto peer2 = j.get<PeerData>();
    QVERIFY(peer == peer2);
    QVERIFY(!(peer != peer2));
    peer2.hostName = QStringLiteral("gb-mnc-wg-008");
    QVERIFY(!(peer == peer2));
    QVERIFY(peer != peer2);
}

QTEST_MAIN(TestJSON)
#include "test_data.moc"
