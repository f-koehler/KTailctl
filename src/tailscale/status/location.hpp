#ifndef KTAILCTL_LOCATION_HPP
#define KTAILCTL_LOCATION_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

class Location : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString country READ country BINDABLE bindableCountry NOTIFY countryChanged)
    Q_PROPERTY(QString countryCode READ countryCode BINDABLE bindableCountryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(QString city READ city BINDABLE bindableCity NOTIFY cityChanged)
    Q_PROPERTY(QString cityCode READ cityCode BINDABLE bindableCityCode NOTIFY cityCodeChanged)
    Q_PROPERTY(double latitude READ latitude BINDABLE bindableLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(double longitude READ longitude BINDABLE bindableLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(int priority READ priority BINDABLE bindablePriority NOTIFY priorityChanged)

public:
    explicit Location(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    explicit Location(QJsonObject &json, QObject *parent = nullptr)
        : QObject(parent)
    {
        updateFromJson(json);
    }

    void updateFromJson(QJsonObject &json);

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

Q_SIGNALS:
    void countryChanged();
    void countryCodeChanged();
    void cityChanged();
    void cityCodeChanged();
    void latitudeChanged();
    void longitudeChanged();
    void priorityChanged();

private:
    Q_OBJECT_BINDABLE_PROPERTY(Location, QString, mCountry, &Location::countryChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Location, QString, mCountryCode, &Location::countryCodeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Location, QString, mCity, &Location::cityChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Location, QString, mCityCode, &Location::cityCodeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Location, double, mLatitude, &Location::latitudeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Location, double, mLongitude, &Location::longitudeChanged)
    Q_OBJECT_BINDABLE_PROPERTY(Location, int, mPriority, &Location::priorityChanged)
};

#endif // KTAILCTL_LOCATION_HPP
