#include "util.hpp"

#include "tailscale/preferences/preferences.hpp"
#include "tailscale/status/client_version.hpp"
#include "tailscale/status/exit_node_status.hpp"
#include "tailscale/status/peer_status.hpp"
#include "tailscale/status/status.hpp"
#include "tailscale/status/tailnet_status.hpp"
#include "tailscale/tailscale.hpp"
#include "tray_icon/tray_icon.hpp"
#include <KAboutData>
#include <KDBusService>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QIcon>
#include <QQuickWindow>
#include <QtQml>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/logo.svg")));

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("org.fkoehler.KTailctl"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("fkoehler.org"));
    QCoreApplication::setApplicationName(QStringLiteral("KTailctl"));
    QCoreApplication::setOrganizationName(QStringLiteral("fkoehler.org"));

    KAboutData aboutData(QStringLiteral("ktailctl"),
                         i18nc("@title", "KTailctl"),
                         // TODO: version string via CMake
                         QStringLiteral("1.0"),
                         i18n("GUI for tailscale on the Linux desktop"),
                         KAboutLicense::GPL,
                         i18n("(c) Fabian Koehler 2023"));

    aboutData.addAuthor(i18nc("@info:credit", "Fabian Koehler"),
                        i18nc("@info:credit", "Project Maintainer"),
                        QStringLiteral("fabian@fkoehler.me"),
                        QStringLiteral("https://fkoehler.org"));

    aboutData.setBugAddress("https://github.com/f-koehler/KTailctl/issues");
    aboutData.setDesktopFileName(QStringLiteral("org.fkoehler.KTailctl"));
    aboutData.setHomepage(QStringLiteral("https://github.com/f-koehler/KTailctl"));
    aboutData.setOrganizationDomain("fkoehler.org");
    aboutData.addAuthor(i18nc("@info:credit", "Fabian Köhler"),
                        i18nc("@info:credit", "Project Maintainer"),
                        QStringLiteral("me@fkoehler.org"),
                        QStringLiteral("https://fkoehler.org"));
    // TODO(fk): about icon
    aboutData.setProgramLogo(QIcon(QStringLiteral(":/icons/logo.svg")));
    KAboutData::setApplicationData(aboutData);
    const KDBusService service(KDBusService::Unique);

    QQmlApplicationEngine engine;
    qmlRegisterType<ClientVersion>("org.fkoehler.KTailctl", 1, 0, "ClientVersion");
    qmlRegisterType<ExitNodeStatus>("org.fkoehler.KTailctl", 1, 0, "ExitNodeStatus");
    qmlRegisterType<Location>("org.fkoehler.KTailctl", 1, 0, "Location");
    qmlRegisterType<LoginProfile>("org.fkoehler.KTailctl", 1, 0, "LoginProfile");
    qmlRegisterType<NetworkProfile>("org.fkoehler.KTailctl", 1, 0, "NetworkProfile");
    qmlRegisterType<PeerStatus>("org.fkoehler.KTailctl", 1, 0, "PeerStatus");
    qmlRegisterType<TailscaleNew::LoginProfileModel>("org.fkoehler.KTailctl", 1, 0, "LoginProfileModel");
    qmlRegisterType<Status::PeerModel>("org.fkoehler.KTailctl", 1, 0, "PeerModel");
    qmlRegisterType<Status>("org.fkoehler.KTailctl", 1, 0, "Status");
    qmlRegisterType<Preferences>("org.fkoehler.KTailctl", 1, 0, "Preferences");
    qmlRegisterType<TailnetStatus>("org.fkoehler.KTailctl", 1, 0, "TailnetStatus");
    qmlRegisterType<UserProfile>("org.fkoehler.KTailctl", 1, 0, "UserProfile");

    TailscaleNew *tailscale = new TailscaleNew();
    Util *util = new Util();
    qmlRegisterSingletonType("org.fkoehler.KTailctl", 1, 0, "About", [](QQmlEngine *engine, QJSEngine *) -> QJSValue {
        return engine->toScriptValue(KAboutData::applicationData());
    });
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "Tailscale", tailscale);
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "Util", util);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QStringLiteral("qrc:/Main.qml"));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    auto *window = dynamic_cast<QQuickWindow *>(engine.rootObjects().first());

    // clicking tray icon should toggle window
    auto *tray_icon = new TrayIcon(tailscale);
    tray_icon->show();
    QObject::connect(tray_icon, &QSystemTrayIcon::activated, [window](const QSystemTrayIcon::ActivationReason &reason) {
        switch (reason) {
        case QSystemTrayIcon::ActivationReason::Trigger:
        case QSystemTrayIcon::ActivationReason::DoubleClick:
            if (window == nullptr) {
                return;
            }
            if (window->isVisible()) {
                window->hide();
            } else {
                window->show();
            }
            break;
        case QSystemTrayIcon::ActivationReason::MiddleClick:
            // TODO(fk): toggle tailscale
        default:
            break;
        };
    });
    QObject::connect(tray_icon, &TrayIcon::showWindow, window, [window]() {
        window->show();
    });
    QObject::connect(tray_icon, &TrayIcon::quitRequested, &app, &QCoreApplication::quit, Qt::QueuedConnection);
    QObject::connect(tray_icon, &TrayIcon::toggleTailscale, tailscale, &TailscaleNew::toggleTailscale);

    return app.exec();
}
