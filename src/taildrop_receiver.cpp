// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "taildrop_receiver.h"
#include "util.h"
#include <libtailctlpp.h>

#include <QDebug>

QString strategyToString(const KTailctlConfig::EnumTaildropStrategy::type &strategy)
{
    switch (strategy) {
    case KTailctlConfig::EnumTaildropStrategy::Overwrite:
        return "overwrite";
    case KTailctlConfig::EnumTaildropStrategy::Skip:
        return "skip";
    default:
        return "rename";
    }
}

TaildropReceiver::TaildropReceiver()
    : QThread(nullptr)
    , mConfig(KTailctlConfig::self())
    , mEnabled(mConfig->taildropEnabled())
    , mDirectory(mConfig->taildropDirectory())
    , mStrategy(strategyToString(mConfig->taildropStrategy()))
{
    if (mEnabled) {
        start();
    }

    QObject::connect(mConfig, &KTailctlConfig::taildropEnabledChanged, [this]() {
        setEnabled(mConfig->taildropEnabled());
    });
    QObject::connect(mConfig, &KTailctlConfig::taildropDirectoryChanged, [this]() {
        setDirectory(mConfig->taildropDirectory());
    });
    QObject::connect(mConfig, &KTailctlConfig::taildropStrategyChanged, [this]() {
        setStrategy(strategyToString(mConfig->taildropStrategy()));
    });
}

TaildropReceiver *TaildropReceiver::self()
{
    static TaildropReceiver *instance = nullptr;
    if (instance == nullptr) {
        instance = new TaildropReceiver();
    }
    return instance;
}

void TaildropReceiver::run()
{
    GoString strategy{mStrategy.toUtf8().constData(), mStrategy.length()};
    GoString directory{mDirectory.toUtf8().constData(), mDirectory.length()};
    tailscale_receive_files(strategy, directory);
}

bool TaildropReceiver::enabled() const
{
    return mEnabled;
}
const QString &TaildropReceiver::directory() const
{
    return mDirectory;
}
const QString &TaildropReceiver::strategy() const
{
    return mStrategy;
}

void TaildropReceiver::setEnabled(bool enabled)
{
    if (mEnabled != enabled) {
        mEnabled = enabled;
        emit enabledChanged(mEnabled);
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
        emit directoryChanged(directory);
        mDirectory = directory;
        quit();
        start();
    }
}
void TaildropReceiver::setStrategy(const QString &strategy)
{
    if (mStrategy != strategy) {
        emit strategyChanged(strategy);
        mStrategy = strategy;
        quit();
        start();
    }
}
