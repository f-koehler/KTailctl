#include <KAboutData>
#include <KDBusService>
#include <KLocalizedContext>
#include <KLocalizedString>
#include <QApplication>
#include <QByteArray>
#include <QCoreApplication>
#include <QIcon>
#include <QJSEngine>
#include <QJSValue>
#include <QList>
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQuickWindow>
#include <QString>
#include <QStringLiteral>
#include <QSystemTrayIcon>
#include <Qt>
#include <memory>

#include "config/config_auto_save.hpp"
#include "ktailctl_config.h"
#include "tailscale/tailscale.hpp"
#include "tray_icon/tray_icon.hpp"
#include "tray_icon_themes.hpp"
#include "util.hpp"
#include "version-ktailctl.h"

auto main(int argc, char *argv[]) -> int
{
    const QApplication app(argc, argv);

    QApplication::setWindowIcon(QIcon(QStringLiteral(":/icons/logo.svg")));

    KLocalizedString::setApplicationDomain(QByteArrayLiteral("org.fkoehler.KTailctl"));
    QCoreApplication::setOrganizationDomain(QStringLiteral("fkoehler.org"));
    QCoreApplication::setApplicationName(QStringLiteral("KTailctl"));
    QCoreApplication::setOrganizationName(QStringLiteral("fkoehler.org"));

    KAboutData aboutData(QStringLiteral("ktailctl"),
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
    const KDBusService service(KDBusService::Unique);

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
    auto *config = Config::self();
    qmlRegisterSingletonInstance("org.fkoehler.KTailctl", qmlMajorVersion, 0, "Config", config);
    new ConfigAutoSave(config, config);
    qmlRegisterSingletonType("org.fkoehler.KTailctl", qmlMajorVersion, 0, "About", [](QQmlEngine *engine, QJSEngine *) -> QJSValue {
        return engine->toScriptValue(KAboutData::applicationData());
    });

    engine->rootContext()->setContextObject(new KLocalizedContext(engine.get()));
    engine->loadFromModule("org.fkoehler.KTailctl", "Main");
    if (engine->rootObjects().isEmpty()) {
        return -1;
    }

    auto *window = dynamic_cast<QQuickWindow *>(engine->rootObjects().constFirst());

    // clicking tray icon should toggle window
    auto *tray_icon = new TrayIcon(tailscale);
    tray_icon->show();
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

    QObject::connect(&app, &QCoreApplication::aboutToQuit, &app, [&engine]() {
        engine.reset();
    });

    return QApplication::exec();
}
