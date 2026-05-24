#pragma once

#include "ktailctl_config.h"
#include <QObject>

class ConfigAutoSave : public QObject
{
    Q_OBJECT

public:
    explicit ConfigAutoSave(Config *config, QObject *parent = nullptr);

public Q_SLOTS:
    static void save();
};
