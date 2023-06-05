// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "taildrop_receiver.h"
#include "util.h"
#include <libtailctlpp.h>

#include <QDebug>

TaildropReceiver::TaildropReceiver(bool enabled, const QString &directory, const QString &strategy, QObject *parent)
    : QThread(parent)
    , mEnabled(enabled)
    , mDirectory(directory)
    , mStrategy(strategy)
{
    if (mEnabled) {
        start();
    }
}

void TaildropReceiver::run()
{
    GoString strategy{mStrategy.toUtf8().constData(), mStrategy.length()};
    GoString directory{mDirectory.toUtf8().constData(), mDirectory.length()};
    tailscale_receive_files(strategy, directory);
}

void TaildropReceiver::setEnabled(bool enabled)
{
    if (mEnabled != enabled) {
        mEnabled = enabled;
        if (mEnabled) {
            quit();
            start();
        } else {
            quit();
        }
    }
}
void TaildropReceiver::setDirectory(const QString &directory)
{
    if (mDirectory != directory) {
        mDirectory = directory;
        quit();
        start();
    }
}
void TaildropReceiver::setStrategy(const QString &strategy)
{
    if (mStrategy != strategy) {
        mStrategy = strategy;
        quit();
        start();
    }
}
