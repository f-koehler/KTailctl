#include "freedesktop_settings_portal.hpp"

FreedesktopSettingsPortal::FreedesktopSettingsPortal(QObject *parent)
    : QObject(parent)
    , mConnection(QDBusConnection::sessionBus())
{
    mSettingsInterface = new org::freedesktop::portal::Settings(QStringLiteral("org.freedesktop.portal.Settings"), "/", mConnection, this);

    // register for color scheme changes
    connect(mSettingsInterface,
            &org::freedesktop::portal::Settings::SettingChanged,
            this,
            [this](const QString &ns, const QString &key, const QDBusVariant &value) {
                if (ns == QStringLiteral("org.freedesktop.appearance") && key == QStringLiteral("color-scheme")) {
                    updateColorScheme(value);
                }
            });

    // get initial value for color scheme setting
    auto request = mSettingsInterface->ReadOne("org.freedesktop.appearance", "color-scheme");
    request.waitForFinished();
    updateColorScheme(request.value());
    emit colorSchemeChanged(mColorScheme);
}

void FreedesktopSettingsPortal::updateColorScheme(const QDBusVariant &value)
{
    ColorScheme newColorScheme;
    const int colorSchemeValue = value.variant().toInt();
    switch (colorSchemeValue) {
    case 0:
        newColorScheme = ColorScheme::NoPreference;
        break;
    case 1:
        newColorScheme = ColorScheme::Dark;
        break;
    case 2:
        newColorScheme = ColorScheme::Light;
        break;
    default:
        newColorScheme = ColorScheme::NoPreference;
        qCritical() << "Unknown color scheme:" << colorSchemeValue;
    }
    if (newColorScheme != mColorScheme) {
        mColorScheme = newColorScheme;
        emit colorSchemeChanged(mColorScheme);
    }
}

FreedesktopSettingsPortal *FreedesktopSettingsPortal::instance()
{
    static FreedesktopSettingsPortal instance;
    return &instance;
}

FreedesktopSettingsPortal::ColorScheme FreedesktopSettingsPortal::colorScheme() const
{
    return mColorScheme;
}
