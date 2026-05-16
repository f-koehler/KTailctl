#ifndef KTAILCTL_APP_CONNECTOR_HPP
#define KTAILCTL_APP_CONNECTOR_HPP

#include <QBindable>
#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>

class AppConnectorPreferences : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool advertise READ advertise BINDABLE bindableAdvertise NOTIFY advertiseChanged)

public:
    explicit AppConnectorPreferences(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    void updateFromJson(QJsonObject &json);

    [[nodiscard]] auto advertise() const noexcept -> bool
    {
        return mAdvertise;
    }

    [[nodiscard]] auto bindableAdvertise() -> QBindable<bool>
    {
        return {&mAdvertise};
    }

Q_SIGNALS:
    void advertiseChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(AppConnectorPreferences, bool, mAdvertise, &AppConnectorPreferences::advertiseChanged)
};

#endif // KTAILCTL_APP_CONNECTOR_HPP
