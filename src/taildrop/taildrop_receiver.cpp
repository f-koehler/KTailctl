// SPDX-License-Identifier: GPL-3.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>
#include "taildrop_receiver.h"
#include "util.h"
#include <libtailwrap.h>

#include <QDebug>
#include <QDir>

Q_LOGGING_CATEGORY(logcat_taildrop_receiver, "org.fkoehler.KTailctl.TaildropReceiver")

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
    , mEnabled(KTailctlConfig::taildropEnabled())
    , mDirectory(KTailctlConfig::taildropDirectory())
    , mStrategy(strategyToString(KTailctlConfig::taildropStrategy()))
{
    if (mEnabled) {
        start();
    }

    // QObject::connect(mConfig, &KTailctlConfig::taildropEnabledChanged, [this]() {
    //     setEnabled(mConfig->taildropEnabled());
    // });
    // QObject::connect(mConfig, &KTailctlConfig::taildropDirectoryChanged, [this]() {
    //     setDirectory(mConfig->taildropDirectory());
    // });
    // QObject::connect(mConfig, &KTailctlConfig::taildropStrategyChanged, [this]() {
    //     setStrategy(strategyToString(mConfig->taildropStrategy()));
    // });
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
    QByteArray const strategyBytes = mStrategy.toUtf8();
    QByteArray const directoryBytes = mDirectory.toUtf8();
    GoString const strat{strategyBytes.constData(), strategyBytes.length()};
    GoString const dir{directoryBytes.constData(), directoryBytes.length()};
    tailscale_receive_files(strat, dir);
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
            terminate();
            wait();
            start();
        } else {
            terminate();
            wait();
        }
    }
}
void TaildropReceiver::setDirectory(const QString &directory)
{
    if (mDirectory != directory) {
        qCDebug(logcat_taildrop_receiver) << "set directory" << directory;
        QDir().mkpath(directory);
        mDirectory = directory;
        terminate();
        wait();
        start();
        emit directoryChanged(directory);
    }
}
void TaildropReceiver::setStrategy(const QString &strategy)
{
    if (mStrategy != strategy) {
        qCDebug(logcat_taildrop_receiver) << "set strategy" << strategy;
        mStrategy = strategy;
        terminate();
        wait();
        start();
        emit strategyChanged(strategy);
    }
}
