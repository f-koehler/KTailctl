#ifndef KTAILCTL_SRC_SETTINGS_PORTAL_HPP
#define KTAILCTL_SRC_SETTINGS_PORTAL_HPP

#include "settingsinterface.h"
#include <QDBusConnection>
#include <QObject>

class FreedesktopSettingsPortal : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ColorScheme colorScheme READ colorScheme NOTIFY colorSchemeChanged)

public:
    enum class ColorScheme : int {
        NoPreference = 0,
        Dark = 1,
        Light = 2,
    };
    Q_ENUM(ColorScheme)

private:
    QDBusConnection mConnection;
    org::freedesktop::portal::Settings *mSettingsInterface;
    ColorScheme mColorScheme = ColorScheme::NoPreference;

    explicit FreedesktopSettingsPortal(QObject *parent = nullptr);

    void updateColorScheme(const QDBusVariant &value);

public:
    static FreedesktopSettingsPortal *instance();

    ColorScheme colorScheme() const;

signals:
    void colorSchemeChanged(ColorScheme value);
};

#endif /* KTAILCTL_SRC_SETTINGS_PORTAL_HPP */
