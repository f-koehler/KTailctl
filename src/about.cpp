// SPDX-License-Identifier: GPL-3.0-or-later
// PDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "about.h"
#include <version.h>
KAboutData AboutType::aboutData() const
{
    auto aboutData = KAboutData::applicationData();
    aboutData.setVersion(QByteArray::fromStdString(getVersion()));
    return aboutData;
}
