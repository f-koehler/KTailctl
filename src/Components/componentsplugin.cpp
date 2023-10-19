#include <QClipboard>
#include <QMimeData>
#include <QQmlEngine>
#include <QQmlExtensionPlugin>
#include <ksystemclipboard.h>

class ComponentsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    ComponentsPlugin() = default;
    ~ComponentsPlugin() = default;
    void registerTypes(const char *uri) override
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("org.fkoehler.KTailctl.Components"));
        qmlRegisterModule(uri, 1, 0);
    }
};

#include "componentsplugin.moc"