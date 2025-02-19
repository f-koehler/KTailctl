// SPDX-License-Identifier: GPL-3.0-or-later
// PDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "about.hpp"
#include "version-ktailctl.h"
KAboutData AboutType::aboutData()
{
    auto aboutData = KAboutData::applicationData();
    aboutData.setVersion(KTAILCTL_VERSION_STRING);
    return aboutData;
}
