#include "config_auto_save.hpp"
#include "ktailctl_config.h"
#include <QByteArray>
#include <QMetaMethod>
#include <QMetaObject>
#include <QObject>

ConfigAutoSave::ConfigAutoSave(Config *config, QObject *parent)
    : QObject(parent)
{
    const QMetaObject *metaObj = config->metaObject();
    const QMetaMethod saveSlot = ConfigAutoSave::staticMetaObject.method(ConfigAutoSave::staticMetaObject.indexOfSlot("save()"));
    for (int i = metaObj->methodOffset(); i < metaObj->methodCount(); ++i) {
        const QMetaMethod signal = metaObj->method(i);
        if (signal.methodType() == QMetaMethod::Signal && signal.name().endsWith("Changed")) {
            QObject::connect(config, signal, this, saveSlot);
        }
    }
}

void ConfigAutoSave::save()
{
    Config::self()->save();
}
