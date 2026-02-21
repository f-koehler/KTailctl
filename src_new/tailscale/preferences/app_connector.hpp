#ifndef KTAILCTL_APP_CONNECTOR_HPP
#define KTAILCTL_APP_CONNECTOR_HPP

#include <QBindable>
#include <QJsonObject>
#include <QObject>

class AppConnectorPreferences : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool advertise READ advertise BINDABLE bindableAdvertise)

private:
    QProperty<bool> mAdvertise;

public:
    explicit AppConnectorPreferences(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    void updateFromJson(QJsonObject &json)
    {
        mAdvertise = json.take(QStringLiteral("advertise")).toBool();
    }

    [[nodiscard]] bool advertise() const noexcept
    {
        return mAdvertise;
    }

    [[nodiscard]] QBindable<bool> bindableAdvertise()
    {
        return {&mAdvertise};
    }
};

#endif // KTAILCTL_APP_CONNECTOR_HPP
