#include <KAboutData>
#include <KConfigGroup>
#include <KDBusService>
#include <KIconTheme>
#include <KLocalizedQmlContext>
#include <KLocalizedString>
#include <KSharedConfig>
#include <KWindowSystem>
#include <QApplication>
#include <QByteArray>
#include <QCoreApplication>
#include <QFileInfo>
#include <QGuiApplication>
#include <QIcon>
#include <QJSEngine>
#include <QJSValue>
#include <QList>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQuickStyle>
#include <QQuickWindow>
#include <QString>
#include <QStringLiteral>
#include <QStyleHints>
#include <QSystemTrayIcon>
#include <Qt>
#include <memory>
#include <optional>
#include <qlogging.h>
#include <qloggingcategory.h>
// Q_OS_MACOS is defined transitively by the Qt headers above; BreezeIcons must come after them.
#ifdef Q_OS_MACOS
#include <BreezeIcons>
#include <KColorSchemeManager>
#endif

#include "config/config_auto_save.hpp"
#include "ktailctl_config.h"
#include "logging_tailscale.hpp"
#include "tailscale/tailscale.hpp"
#include "tray_icon/tray_icon.hpp"
#include "tray_icon_themes.hpp"
#include "util.hpp"
#include "version-ktailctl.h"

namespace
{
// Debug-level messages are suppressed by default (see DEFAULT_SEVERITY in the
// ecm_qt_declare_logging_category() calls). This turns them on for our own
// categories when the user enables verbose logging in the settings, without
// clobbering any rules the user already set via QT_LOGGING_RULES.
void applyLoggingRules()
{
    QString rules = qEnvironmentVariable("QT_LOGGING_RULES");
    if (Config::verboseLogging()) {
        if (!rules.isEmpty()) {
            rules += QStringLiteral("\n");
        }
        rules += QStringLiteral("org.fkoehler.KTailctl*.debug=true");
    }
    QLoggingCategory::setFilterRules(rules);
}
}

auto main(int argc, char *argv[]) -> int
{
#ifdef Q_OS_MACOS
    // Without a desktop shell to supply one, Qt Quick Controls falls back to its native "macOS"
    // style, which doesn't support the pixel-level customization Kirigami/FormCard controls rely
    // on (see the "current style does not support customization" QML warnings) and reports very
    // different implicit control sizes than the desktop style KTailctl is designed around (e.g.
    // an oversized collapsed sidebar). org.kde.desktop (qqc2-desktop-style) fixes both. Must be
    // set before QGuiApplication/QApplication is constructed.
    QQuickStyle::setStyle(QStringLiteral("org.kde.desktop"));
#endif
    const QApplication app(argc, argv);

    QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/logo.svg")));

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("org.fkoehler.KTailctl"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("fkoehler.org"));
    QCoreApplication::setApplicationName(QStringLiteral("KTailctl"));
    QCoreApplication::setOrganizationName(QStringLiteral("fkoehler.org"));

    auto *config = Config::self();
    applyLoggingRules();
    QObject::connect(config, &Config::verboseLoggingChanged, &app, &applyLoggingRules);

#ifdef Q_OS_MACOS
    // macOS has no desktop icon theme mechanism at all (Craft's breeze-icons build for macOS
    // skips installing a real theme directory, matching how apps are meant to consume it there:
    // QIcon::fromTheme() would otherwise only resolve the handful of names Qt's Cocoa platform
    // theme happens to map to native symbols). BreezeIcons::initIcons() unpacks the icons the
    // library carries into the :/icons resource root and registers that as QIcon's fallback
    // theme. Also making it the *current* theme name (rather than leaving it fallback-only) is
    // what gets KIconLoader-based lookups (used by qqc2-desktop-style controls, not just plain
    // QIcon::fromTheme()) to resolve against it too.
    BreezeIcons::initIcons();
    QIcon::setThemeName(QIcon::fallbackThemeName());

    // Without a real kdeglobals set up by System Settings, KColorScheme has no color scheme to
    // read and falls back to hardcoded (light) defaults regardless of the actual system
    // appearance -- which is how widgets like search/text fields end up with a dark background
    // (correctly picked up from the native macOS palette) but near-black default text (from
    // KColorScheme's light-scheme fallback). Instantiating KColorSchemeManager makes it follow
    // and apply the system light/dark scheme instead.
    KColorSchemeManager::instance();
#endif

    // Custom/system icon themes aren't discoverable in the Flatpak sandbox either; force breeze/
    // breeze-dark by name there so KIconThemes falls back to the bundled KF6::BreezeIcons library.
    const bool forceIconTheme = QFileInfo::exists(QStringLiteral("/.flatpak-info"));
    if (forceIconTheme) {
        const bool darkColorScheme = QGuiApplication::styleHints()->colorScheme() == Qt::ColorScheme::Dark;
        const QString theme = darkColorScheme ? QStringLiteral("breeze-dark") : QStringLiteral("breeze");
        qCInfo(Logging::TailscaleMain) << "Forcing icon theme:" << theme;
        KConfigGroup(KSharedConfig::openConfig(QString(), KConfig::NoGlobals), QStringLiteral("Icons")).writeEntry("Theme", theme);
        KIconTheme::reconfigure();
        QIcon::setThemeName(theme);
    }

    KAboutData aboutData(QStringLiteral("KTailctl"),
                         i18nc("@title", "KTailctl"),
                         QStringLiteral(KTAILCTL_VERSION_STRING),
                         i18n("GUI for tailscale on the Linux desktop"),
                         KAboutLicense::GPL,
                         i18n("(c) Fabian Koehler 2023"));

    aboutData.setBugAddress("https://github.com/f-koehler/KTailctl/issues");
    aboutData.setDesktopFileName(QStringLiteral("org.fkoehler.KTailctl"));
    aboutData.setHomepage(QStringLiteral("https://github.com/f-koehler/KTailctl"));
    aboutData.setOrganizationDomain("fkoehler.org");
    aboutData.addAuthor(i18nc("@info:credit", "Fabian Koehler"),
                        i18nc("@info:credit", "Project Maintainer"),
                        QStringLiteral("fabian@fkoehler.me"),
                        QStringLiteral("https://fkoehler.org"));
    aboutData.setProgramLogo(QIcon(QStringLiteral(":/icons/logo.svg")));
    KAboutData::setApplicationData(aboutData);
    // KDBusService needs a running D-Bus session bus. Linux desktops always have one, but macOS
    // does not ship one by default, and QDBusConnection::sessionBus() blocks indefinitely trying
    // to reach it, so single-instance activation is skipped there for now (see KTAILCTL-macOS).
    std::optional<KDBusService> service;
#ifndef Q_OS_MACOS
    service.emplace(KDBusService::Unique);
    qCInfo(Logging::TailscaleMain) << "KDBusService name:" << service->serviceName();
#endif

    auto *tailscale = new Tailscale();
    Tailscale::setQmlInstance(tailscale);
    auto *util = new Util();
    Util::setQmlInstance(util);
    auto *tray_icon_themes = new TrayIconThemes();
    TrayIconThemes::setQmlInstance(tray_icon_themes);

    // Forward-declared here; defined in the generated ktailctl_qmltyperegistrations.cpp.
    // Called explicitly because QQmlModuleRegistration's lazy-callback is not reliably
    // triggered when the module is loaded from a file-system qmldir without a plugin line.
    void qml_register_types_org_fkoehler_KTailctl();
    qml_register_types_org_fkoehler_KTailctl();

    auto engine = std::make_unique<QQmlApplicationEngine>();
    static constexpr int qmlMajorVersion = 254;
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", qmlMajorVersion, 0, "Config", config);
    new ConfigAutoSave(config, config); // cleaned up throug QObject ownership model
    qmlRegisterSingletonType("org.fkoehler.KTailctl", qmlMajorVersion, 0, "About", [](QQmlEngine *engine, QJSEngine *) -> QJSValue {
        return engine->toScriptValue(KAboutData::applicationData());
    });

    KLocalization::setupLocalizedContext(engine.get());
    engine->loadFromModule("org.fkoehler.KTailctl", "Main");
    if (engine->rootObjects().isEmpty()) {
        return -1;
    }

    auto *window = dynamic_cast<QQuickWindow *>(engine->rootObjects().constFirst());

    // honor the "Start minimized" setting on startup
    if (window != nullptr) {
        if (Config::startMinimized()) {
            window->hide();
        } else {
            window->show();
        }
    }

    // clicking tray icon should toggle window
    auto *tray_icon = new TrayIcon(tailscale);
    tray_icon->setVisible(Config::enableTrayIcon());
    QObject::connect(tray_icon, &QSystemTrayIcon::activated, tray_icon, [window](const QSystemTrayIcon::ActivationReason &reason) {
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
    QObject::connect(tray_icon, &TrayIcon::showWindow, window, [window] {
        window->show();
    });
    QObject::connect(tray_icon, &TrayIcon::quitRequested, &app, &QCoreApplication::quit, Qt::QueuedConnection);

    // re-launching the unique instance should reveal and focus the window
    if (service) {
        QObject::connect(&service.value(), &KDBusService::activateRequested, window, [window](const QStringList &, const QString &) {
            if (window == nullptr) {
                return;
            }
            window->show();
            KWindowSystem::updateStartupId(window);
            window->raise();
            KWindowSystem::activateWindow(window);
        });
    }

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &app, [&engine]() {
        engine.reset();
    });

    return QApplication::exec();
}
