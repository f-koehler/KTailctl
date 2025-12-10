#ifndef KTAILCTL_LOCATION_HPP
#define KTAILCTL_LOCATION_HPP

#include <QObject>
#include <QProperty>
#include <QString>

class Location : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString country READ country BINDABLE bindableCountry)
    Q_PROPERTY(QString countryCode READ countryCode BINDABLE bindableCountryCode)
    Q_PROPERTY(QString city READ city BINDABLE bindableCity)
    Q_PROPERTY(QString cityCode READ cityCode BINDABLE bindableCityCode)
    Q_PROPERTY(double latitude READ latitude BINDABLE bindableLatitude)
    Q_PROPERTY(double longitude READ longitude BINDABLE bindableLongitude)
    Q_PROPERTY(int priority READ priority BINDABLE bindablePriority)

private:
    QProperty<QString> mCountry;
    QProperty<QString> mCountryCode;
    QProperty<QString> mCity;
    QProperty<QString> mCityCode;
    QProperty<double> mLatitude;
    QProperty<double> mLongitude;
    QProperty<int> mPriority;

public:
    explicit Location(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    // Getters
    [[nodiscard]] const QString &country() const noexcept
    {
        return mCountry;
    }
    [[nodiscard]] const QString &countryCode() const noexcept
    {
        return mCountryCode;
    }
    [[nodiscard]] const QString &city() const noexcept
    {
        return mCity;
    }
    [[nodiscard]] const QString &cityCode() const noexcept
    {
        return mCityCode;
    }
    [[nodiscard]] double latitude() const noexcept
    {
        return mLatitude;
    }
    [[nodiscard]] double longitude() const noexcept
    {
        return mLongitude;
    }
    [[nodiscard]] int priority() const noexcept
    {
        return mPriority;
    }

    // Bindables
    [[nodiscard]] QBindable<QString> bindableCountry()
    {
        return {&mCountry};
    }
    [[nodiscard]] QBindable<QString> bindableCountryCode()
    {
        return {&mCountryCode};
    }
    [[nodiscard]] QBindable<QString> bindableCity()
    {
        return {&mCity};
    }
    [[nodiscard]] QBindable<QString> bindableCityCode()
    {
        return {&mCityCode};
    }
    [[nodiscard]] QBindable<double> bindableLatitude()
    {
        return {&mLatitude};
    }
    [[nodiscard]] QBindable<double> bindableLongitude()
    {
        return {&mLongitude};
    }
    [[nodiscard]] QBindable<int> bindablePriority()
    {
        return {&mPriority};
    }
};

#endif // KTAILCTL_LOCATION_HPP
