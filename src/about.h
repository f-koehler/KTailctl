// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef KTAILCTL_ABOUT_H
#define KTAILCTL_ABOUT_H

#include <KAboutData>
#include <QObject>

class AboutType : public QObject
{
    Q_OBJECT
    Q_PROPERTY(KAboutData aboutData READ aboutData CONSTANT)
public:
    [[nodiscard]] static KAboutData aboutData();
};

#endif /* KTAILCTL_ABOUT_H */
