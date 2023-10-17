// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
//
#ifndef KTAILCTL_LOCATION_H
#define KTAILCTL_LOCATION_H

#include <QJsonObject>
#include <QObject>

class Location : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString country READ country NOTIFY countryChanged)
    Q_PROPERTY(QString countryCode READ countryCode NOTIFY countryCodeChanged)
    Q_PROPERTY(QString city READ city NOTIFY cityChanged)
    Q_PROPERTY(QString cityCode READ cityCode NOTIFY cityCodeChanged)
    Q_PROPERTY(int priority READ priority NOTIFY priorityChanged)
private:
    QString mCountry;
    QString mCountryCode;
    QString mCity;
    QString mCityCode;
    int mPriority;

protected:
    bool setCountry(const QString &country);
    bool setCountryCode(const QString &countryCode);
    bool setCity(const QString &city);
    bool setCityCode(const QString &cityCode);
    bool setPriority(int priority);

    void setCountryFromJSON(const QJsonObject &json);
    void setCountryCodeFromJSON(const QJsonObject &json);
    void setCityFromJSON(const QJsonObject &json);
    void setCityCodeFromJSON(const QJsonObject &json);
    void setPriorityFromJSON(const QJsonObject &json);

signals:
    void countryChanged(const QString &value);
    void countryCodeChanged(const QString &value);
    void cityChanged(const QString &value);
    void cityCodeChanged(const QString &value);
    void priorityChanged(int);

public:
    explicit Location(QObject *parent = nullptr);
    virtual ~Location() = default;

    static Location *fromJSON(const QJsonObject &json, QObject *parent = nullptr);
    bool setTo(const Location *other);

    const QString &country() const;
    const QString &countryCode() const;
    const QString &city() const;
    const QString &cityCode() const;
    int priority() const;

    Location &operator=(const Location &other);
    bool operator==(const Location &other) const;
    bool operator!=(const Location &other) const;
};

#endif /* KTAILCTL_LOCATION_H */
