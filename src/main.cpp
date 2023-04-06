/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
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
#include "status.h"
#include "tailscale.h"
#include "util.h"
#include "version-tailctl.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName(QStringLiteral("KDE"));
    QCoreApplication::setApplicationName(QStringLiteral("Tailctl"));

    KAboutData aboutData(
        // The program name used internally.
        QStringLiteral("Tailctl"),
        // A displayable program name string.
        i18nc("@title", "Tailctl"),
        // The program version string.
        QStringLiteral(TAILCTL_VERSION_STRING),
        // Short description of what the app does.
        i18n("GUI for tailscale on the KDE Plasma desktop"),
        // The license this code is released under.
        KAboutLicense::GPL,
        // Copyright Statement.
        i18n("(c) 2023"));
    aboutData.addAuthor(i18nc("@info:credit", "Fabian Köhler"),
                        i18nc("@info:credit", "Project Maintainer"),
                        QStringLiteral("me@fkoehler.org"),
                        QStringLiteral("https://fkoehler.org"));
    KAboutData::setApplicationData(aboutData);

    QQmlApplicationEngine engine;

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.Tailctl", 1, 0, "AboutType", &about);

    Tailscale tailscale;
    qmlRegisterSingletonInstance("org.kde.Tailctl", 1, 0, "Tailscale", &tailscale);

    App application(&tailscale);
    qmlRegisterSingletonInstance("org.kde.Tailctl", 1, 0, "App", &application);

    qmlRegisterType<Peer>("org.kde.Tailctl", 1, 0, "Peer");
    qmlRegisterType<Status>("org.kde.Tailctl", 1, 0, "Status");
    qmlRegisterType<TailctlConfig>("org.kde.Tailctl", 1, 0, "TailctlConfig");

    Util util;
    qmlRegisterSingletonInstance("org.kde.Tailctl", 1, 0, "Util", &util);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    const auto *config = TailctlConfig::self();

    auto *window = (QQuickWindow *)engine.rootObjects().first();
    application.trayIcon()->setWindow(window);
    if (config->startMinimized()) {
        window->hide();
    } else {
        window->show();
    }

    return app.exec();
}
