#ifndef KTAILCTL_CONFIG_AUTO_SAVE_HPP
#define KTAILCTL_CONFIG_AUTO_SAVE_HPP

#include "ktailctl_config.h"
#include <QObject>
#include <qtmetamacros.h>

class ConfigAutoSave : public QObject
{
    Q_OBJECT

public:
    explicit ConfigAutoSave(Config *config, QObject *parent = nullptr);
    Q_SLOT static void save();
};

#endif // KTAILCTL_CONFIG_AUTO_SAVE_HPP
