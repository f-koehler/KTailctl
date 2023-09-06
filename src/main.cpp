// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QUrl>
#include <QtQml>

#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "about.h"
#include "app.h"
#include "peer.h"
#include "peer_model.h"
#include "speed_statistics.h"
#include "statistics.h"
#include "status.h"
#include "taildrop_sender.h"
#include "tailscale.h"
#include "util.h"
#include "version-ktailctl.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#ifdef __APPLE__
    QQuickStyle::setStyle(QStringLiteral("macOS"));
#endif
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setApplicationName(QStringLiteral("KTailctl"));

    app.setWindowIcon(QIcon::fromTheme(QStringLiteral("ktailctl")));

    KAboutData aboutData(
        // The program name used internally.
        QStringLiteral("KTailctl"),
        // A displayable program name string.
        i18nc("@title", "KTailctl"),
        // The program version string.
        QStringLiteral(TAILCTL_VERSION_STRING),
        // Short description of what the app does.
        i18n("GUI for tailscale on the KDE Plasma desktop"),
        // The license this code is released under.
        KAboutLicense::GPL,
        // Copyright Statement.
        i18n("(c) 2023"));
    aboutData.setHomepage("https://github.com/f-koehler/KTailctl/issues");
    aboutData.setBugAddress("https://github.com/f-koehler/KTailctl/issues");
    aboutData.addAuthor(i18nc("@info:credit", "Fabian Köhler"),
                        i18nc("@info:credit", "Project Maintainer"),
                        QStringLiteral("me@fkoehler.org"),
                        QStringLiteral("https://fkoehler.org"));
    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

    AboutType about;
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "AboutType", &about);

    Tailscale tailscale;
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "Tailscale", &tailscale);

    App application(&tailscale);
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "App", &application);

    qmlRegisterType<Peer>("org.fkoehler.KTailctl", 1, 0, "Peer");
    qmlRegisterType<Status>("org.fkoehler.KTailctl", 1, 0, "Status");
    qmlRegisterType<SpeedStatistics>("org.fkoehler.KTailctl", 1, 0, "SpeedStatistics");
    qmlRegisterType<Statistics>("org.fkoehler.KTailctl", 1, 0, "Statistics");
    qmlRegisterType<KTailctlConfig>("org.fkoehler.KTailctl", 1, 0, "KTailctlConfig");

    QmlTaildropSender taildropSender;
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "TaildropSender", &taildropSender);

    Util util;
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "Util", &util);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    const auto *config = KTailctlConfig::self();

    auto *window = (QQuickWindow *)engine.rootObjects().first();
    application.trayIcon()->setWindow(window);
    if (config->startMinimized()) {
        window->hide();
    } else {
        window->show();
    }

    // for screenshots for flatpak
    // window->resize(QSize(1598, 869));

    return app.exec();
}
