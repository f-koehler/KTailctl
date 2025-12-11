#include <QApplication>
#include <QtQml>

#include "tailscale/status/client_version.hpp"
#include "tailscale/status/exit_node_status.hpp"
#include "tailscale/status/peer_status.hpp"
#include "tailscale/status/status.hpp"
#include "tailscale/status/tailnet_status.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationDomain(QStringLiteral("fkoehler.org"));
    QCoreApplication::setApplicationName(QStringLiteral("KTailctl"));
    QCoreApplication::setOrganizationName(QStringLiteral("fkoehler.org"));

    QQmlApplicationEngine engine;
    qmlRegisterType<ClientVersion>("org.fkoehler.KTailctl", 1, 0, "ClientVersion");
    qmlRegisterType<ExitNodeStatus>("org.fkoehler.KTailctl", 1, 0, "ExitNodeStatus");
    qmlRegisterType<Location>("org.fkoehler.KTailctl", 1, 0, "Location");
    qmlRegisterType<PeerStatus>("org.fkoehler.KTailctl", 1, 0, "PeerStatus");
    qmlRegisterType<Status>("org.fkoehler.KTailctl", 1, 0, "STatus");
    qmlRegisterType<TailnetStatus>("org.fkoehler.KTailctl", 1, 0, "TailnetStatus");
    qmlRegisterType<UserProfile>("org.fkoehler.KTailctl", 1, 0, "UserProfile");
    engine.loadFromModule("org.fkoehler.KTailctl", "Main");
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
