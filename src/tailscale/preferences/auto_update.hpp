#ifndef KTAILCTL_AUTO_UPDATE_HPP
#define KTAILCTL_AUTO_UPDATE_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>

class AutoUpdatePreferences : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool check READ check BINDABLE bindableCheck NOTIFY checkChanged)
    Q_PROPERTY(bool apply READ apply BINDABLE bindableApply NOTIFY applyChanged)

public:
    explicit AutoUpdatePreferences(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    void updateFromJson(QJsonObject &json);

    [[nodiscard]] auto check() const noexcept -> bool
    {
        return mCheck;
    }
    [[nodiscard]] auto apply() const noexcept -> bool
    {
        return mApply;
    }

    [[nodiscard]] auto bindableCheck() -> QBindable<bool>
    {
        return {&mCheck};
    }
    [[nodiscard]] auto bindableApply() -> QBindable<bool>
    {
        return {&mApply};
    }

Q_SIGNALS:
    void checkChanged();
    void applyChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(AutoUpdatePreferences, bool, mCheck, &AutoUpdatePreferences::checkChanged)
    Q_OBJECT_BINDABLE_PROPERTY(AutoUpdatePreferences, bool, mApply, &AutoUpdatePreferences::applyChanged)
};

#endif // KTAILCTL_AUTO_UPDATE_HPP
