// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include <QApplication>
#include <QMessageBox>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QUrl>
#include <QtGlobal>
#include <QtQml>

#include <KAboutData>
#include <KLocalizedContext>
#include <KLocalizedString>

#include "about.h"
#include "app.h"
#include "location.h"
#include "logging.h"
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
    qInstallMessageHandler(handleLogMessage);

#ifdef __APPLE__
    QQuickStyle::setStyle(QStringLiteral("macOS"));
#endif
    QApplication app(argc, argv); // NOLINT(misc-const-correctness)
    KLocalizedString::setApplicationDomain("org.fkoehler.KTailctl");
    QCoreApplication::setOrganizationName(QStringLiteral("fkoehler.org"));
    QCoreApplication::setApplicationName(QStringLiteral("KTailctl"));

    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("ktailctl")));

    KAboutData aboutData( // NOLINT(misc-const-correctness)

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
    aboutData.setHomepage("https://github.com/f-koehler/KTailctl");
    aboutData.setBugAddress("https://github.com/f-koehler/KTailctl/issues");
    aboutData.addAuthor(i18nc("@info:credit", "Fabian Köhler"),
                        i18nc("@info:credit", "Project Maintainer"),
                        QStringLiteral("me@fkoehler.org"),
                        QStringLiteral("https://fkoehler.org"));
    KAboutData::setApplicationData(aboutData);

    auto *about = new AboutType();
    auto *tailscale = new Tailscale();
    auto *application = new App(tailscale);
    auto *util = new Util();
    auto *taildropSender = new TaildropSender();

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
#ifdef KTAILCTL_FLATPAK_BUILD
    QIcon::setThemeName(QStringLiteral("breeze"));
#endif

    QQmlApplicationEngine engine; // NOLINT(misc-const-correctness)

    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "AboutType", about);
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "Tailscale", tailscale);
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "App", application);
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "Util", util);
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "TaildropSender", taildropSender);

    qmlRegisterType<Peer>("org.fkoehler.KTailctl", 1, 0, "Peer");
    qmlRegisterType<Status>("org.fkoehler.KTailctl", 1, 0, "Status");
    qmlRegisterType<SpeedStatistics>("org.fkoehler.KTailctl", 1, 0, "SpeedStatistics");
    qmlRegisterType<Statistics>("org.fkoehler.KTailctl", 1, 0, "Statistics");
    qmlRegisterType<KTailctlConfig>("org.fkoehler.KTailctl", 1, 0, "KTailctlConfig");
    qmlRegisterType<Location>("org.fkoehler.KTailctl", 1, 0, "Location");

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.load(QUrl(QStringLiteral("qrc:///main.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    auto *window = dynamic_cast<QQuickWindow *>(engine.rootObjects().first());
    application->trayIcon()->setWindow(window);
    if (KTailctlConfig::startMinimized()) {
        window->hide();
    } else {
        window->show();
    }

#ifdef KTAILCTL_APPIMAGE_BUILD
    KTailctlConfig *config = KTailctlConfig::self();
    if (config->askUserAboutUpdateCheck()) {
        QMessageBox msgUpdateCheck;
        msgUpdateCheck.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        msgUpdateCheck.setDefaultButton(QMessageBox::Yes);
        msgUpdateCheck.setText(i18n("Do you want to enable automatic update checks?"));
        msgUpdateCheck.setIcon(QMessageBox::Question);
        const auto choice = msgUpdateCheck.exec();
        switch (choice) {
        case QMessageBox::Yes:
            config->setEnableUpdateCheck(true);
            config->setAskUserAboutUpdateCheck(false);
            break;
        case QMessageBox::No:
            config->setEnableUpdateCheck(false);
            config->setAskUserAboutUpdateCheck(false);
            break;
        default:
            break;
        }
        config->save();
    }
#endif

    // for screenshots for flatpak
    // window->resize(QSize(1598, 869));

    return QApplication::exec();
}
