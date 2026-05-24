#include "config_auto_save.hpp"

#include <QMetaMethod>

ConfigAutoSave::ConfigAutoSave(Config *config, QObject *parent)
    : QObject(parent)
{
    const QMetaObject *metaObj = config->metaObject();
    const QMetaMethod saveSlot = metaObject()->method(metaObject()->indexOfSlot("save()"));
    for (int i = metaObj->methodOffset(); i < metaObj->methodCount(); ++i) {
        const QMetaMethod method = metaObj->method(i);
        if (method.methodType() == QMetaMethod::Signal && method.name().endsWith("Changed")) {
            QObject::connect(config, method, this, saveSlot);
        }
    }
}

void ConfigAutoSave::save()
{
    Config::self()->save();
}
