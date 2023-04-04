/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickWindow>
#include <QUrl>
#include <QtQml>

#include "about.h"
#include "app.h"
#include "peer.h"
#include "peer_model.h"
#include "status.h"
#include "version-tailctl.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

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
        i18n("Application Description"),
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

    App application;
    qmlRegisterSingletonInstance("org.kde.Tailctl", 1, 0, "App", &application);

    qmlRegisterType<Peer>("org.kde.Tailctl", 1, 0, "Peer");
    qmlRegisterType<Status>("org.kde.Tailctl", 1, 0, "Status");
    qmlRegisterType<TailctlConfig>("org.kde.Tailctl", 1, 0, "TailctlConfig");

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    application.setWindow((QQuickWindow *)engine.rootObjects().first());

    return app.exec();
}
