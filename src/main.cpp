// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QUrl>
#include <QtGlobal>
#include <QtQml>

#include <KAboutData>
#include <KDBusService>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <KWindowSystem>

#include "about.hpp"
#include "app.hpp"
#include "logging.hpp"
#include "peer_model.hpp"
#include "preferences.hpp"
#include "speed_statistics.hpp"
#include "statistics.hpp"
#include "taildrop_sender.hpp"
#include "tailscale.hpp"
#include "util.hpp"
#include "version-ktailctl.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    qInstallMessageHandler(handleLogMessage);

    QIcon::setFallbackSearchPaths(QIcon::fallbackSearchPaths() << QStringLiteral(":/country-flags"));

#ifdef __APPLE__
    QQuickStyle::setStyle(QStringLiteral("macOS"));
#endif
    QApplication app(argc, argv); // NOLINT(misc-const-correctness)

    QApplication::setWindowIcon(QIcon::fromTheme(QStringLiteral("ktailctl")));

    QCoreApplication::setOrganizationDomain(QStringLiteral("fkoehler.org"));
    QCoreApplication::setApplicationName(QStringLiteral("KTailctl"));
    QCoreApplication::setOrganizationName(QStringLiteral("fkoehler.org"));
    KLocalizedString::setApplicationDomain(QByteArrayLiteral("org.fkoehler.KTailctl"));
    KAboutData aboutData( // NOLINT(misc-const-correctness)
                          // The program name used internally.
        QStringLiteral("KTailctl"),
        // A displayable program name string.
        i18nc("@title", "KTailctl"),
        // The program version string.
        QStringLiteral(KTAILCTL_VERSION_STRING),
        // Short description of what the app does.
        i18n("GUI for tailscale on the KDE Plasma desktop"),
        // The license this code is released under.
        KAboutLicense::GPL,
        // Copyright Statement.
        i18n("(c) Fabian Koehler 2023"));

    aboutData.setBugAddress("https://github.com/f-koehler/KTailctl/issues");
    aboutData.setDesktopFileName(QStringLiteral("org.fkoehler.KTailctl"));
    aboutData.setHomepage(QStringLiteral("https://github.com/f-koehler/KTailctl"));
    aboutData.setOrganizationDomain("fkoehler.org");
    aboutData.addAuthor(i18nc("@info:credit", "Fabian Köhler"),
                        i18nc("@info:credit", "Project Maintainer"),
                        QStringLiteral("me@fkoehler.org"),
                        QStringLiteral("https://fkoehler.org"));
    aboutData.setProgramLogo(QIcon(QStringLiteral(":/icons/logo.svg")));
    KAboutData::setApplicationData(aboutData);
    KDBusService service(KDBusService::Unique);
    qInfo() << "KDBusService name:" << service.serviceName();

    auto *about = new AboutType();
    auto *application = new App();
    auto *util = new Util();

    if (qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE")) {
        QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
    }
#ifdef KTAILCTL_FLATPAK_BUILD
    QIcon::setThemeName(QStringLiteral("breeze"));
#endif

    QQmlApplicationEngine engine; // NOLINT(misc-const-correctness)

    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "AboutType", about);
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "Tailscale", Tailscale::instance());
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "Preferences", Preferences::instance());
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "App", application);
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "Util", util);
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", 1, 0, "TaildropSendJobFactory", TaildropSendJobFactory::instance());

    qmlRegisterType<SpeedStatistics>("org.fkoehler.KTailctl", 1, 0, "SpeedStatistics");
    qmlRegisterType<Statistics>("org.fkoehler.KTailctl", 1, 0, "Statistics");
    qmlRegisterType<KTailctlConfig>("org.fkoehler.KTailctl", 1, 0, "KTailctlConfig"); // TODO(fk): remove, now handled via CMake

    engine.rootContext()->setContextObject(new KLocalizedContext(&engine));
    engine.rootContext()->setContextProperty(QStringLiteral("styleName"), QQuickStyle::name());
    engine.loadFromModule("org.fkoehler.KTailctl", "Main");
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

    // for screenshots for flatpak
    // window->resize(QSize(1598, 869));

    QObject::connect(&service, &KDBusService::activateRequested, &engine, [&engine, window](const QStringList &, const QString &) {
        if (window) {
            window->show();
            KWindowSystem::updateStartupId(window);
            window->raise();
            KWindowSystem::activateWindow(window);
        }
    });

    return QApplication::exec();
}
