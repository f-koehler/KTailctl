#ifndef KTAILCTL_AUTO_UPDATE_HPP
#define KTAILCTL_AUTO_UPDATE_HPP

#include <QObject>
#include <QProperty>

class AutoUpdatePreferences : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool check READ check BINDABLE bindableCheck)
    Q_PROPERTY(bool apply READ apply BINDABLE bindableApply)

private:
    QProperty<bool> mCheck;
    QProperty<bool> mApply;

public:
    explicit AutoUpdatePreferences(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    void updateFromJson(QJsonObject &json)
    {
        mCheck = json.take(QStringLiteral("Check")).toBool();
        mApply = json.take(QStringLiteral("Apply")).toBool();
    }

    [[nodiscard]] bool check() const noexcept
    {
        return mCheck;
    }
    [[nodiscard]] bool apply() const noexcept
    {
        return mApply;
    }

    [[nodiscard]] QBindable<bool> bindableCheck()
    {
        return {&mCheck};
    }
    [[nodiscard]] QBindable<bool> bindableApply()
    {
        return {&mApply};
    }
};

#endif // KTAILCTL_AUTO_UPDATE_HPP
