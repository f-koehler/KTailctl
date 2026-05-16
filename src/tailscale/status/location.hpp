#ifndef KTAILCTL_LOCATION_HPP
#define KTAILCTL_LOCATION_HPP

#include <QJsonObject>
#include <QObject>
#include <QObjectBindableProperty>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>
#include <qtmetamacros.h>

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
    [[nodiscard]] auto country() const noexcept -> const QString &
    {
        return mCountry;
    }

    [[nodiscard]] auto countryCode() const noexcept -> const QString &
    {
        return mCountryCode;
    }

    [[nodiscard]] auto city() const noexcept -> const QString &
    {
        return mCity;
    }

    [[nodiscard]] auto cityCode() const noexcept -> const QString &
    {
        return mCityCode;
    }

    [[nodiscard]] auto latitude() const noexcept -> double
    {
        return mLatitude;
    }

    [[nodiscard]] auto longitude() const noexcept -> double
    {
        return mLongitude;
    }

    [[nodiscard]] auto priority() const noexcept -> int
    {
        return mPriority;
    }

    // Bindables
    [[nodiscard]] auto bindableCountry() -> QBindable<QString>
    {
        return {&mCountry};
    }

    [[nodiscard]] auto bindableCountryCode() -> QBindable<QString>
    {
        return {&mCountryCode};
    }

    [[nodiscard]] auto bindableCity() -> QBindable<QString>
    {
        return {&mCity};
    }

    [[nodiscard]] auto bindableCityCode() -> QBindable<QString>
    {
        return {&mCityCode};
    }

    [[nodiscard]] auto bindableLatitude() -> QBindable<double>
    {
        return {&mLatitude};
    }

    [[nodiscard]] auto bindableLongitude() -> QBindable<double>
    {
        return {&mLongitude};
    }

    [[nodiscard]] auto bindablePriority() -> QBindable<int>
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
