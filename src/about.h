// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_ABOUT_H
#define TAILCTL_ABOUT_H

#include <KAboutData>
#include <QObject>

class AboutType : public QObject {
  Q_OBJECT
  Q_PROPERTY(KAboutData aboutData READ aboutData CONSTANT)
public:
  [[nodiscard]] KAboutData aboutData() const;
};

#endif /* TAILCTL_ABOUT_H */
