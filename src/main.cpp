/*
    SPDX-License-Identifier: GPL-2.0-or-later
    SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
*/

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QUrl>
#include <QtQml>

#include "about.h"
#include "app.h"
#include "peer_model.h"
#include "version-tailctl.h"
#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "tailctlconfig.h"

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

    auto config = TailctlConfig::self();

    qmlRegisterSingletonInstance("org.kde.Tailctl", 1, 0, "Config", config);

    AboutType about;
    qmlRegisterSingletonInstance("org.kde.Tailctl", 1, 0, "AboutType", &about);

    App application;
    qmlRegisterSingletonInstance("org.kde.Tailctl", 1, 0, "App", &application);

    ModelPeers modelPeers;
    Status status;
    status.refresh();
    modelPeers.updatePeers(status.getPeers());
    qmlRegisterSingletonInstance("org.kde.Tailctl", 1, 0, "ModelPeers", &modelPeers);

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
