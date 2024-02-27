#include "icon_theme_model.h"

#include <QDebug>
#include <QDir>
#include <QDirIterator>
#include <QIcon>
#include <QSet>

void IconThemeModel::findIconThemes()
{
    const QStringList searchPaths = QIcon::themeSearchPaths();
    QSet<QString> iconThemes;
    iconThemes.insert(QStringLiteral("Default"));

    for (const QString &searchPath : searchPaths) {
        QDirIterator iter = QDirIterator(searchPath, QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable, QDirIterator::NoIteratorFlags);
        while (iter.hasNext()) {
            iter.next();
            iconThemes.insert(iter.fileName());
        }
    }
    setStringList(iconThemes.values());
}
