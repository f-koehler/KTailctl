#ifndef KTAILCTL_ICON_THEME_MODEL_H
#define KTAILCTL_ICON_THEME_MODEL_H

#include <QStringListModel>

class IconThemeModel : public QStringListModel
{
    Q_OBJECT

protected:
    using QStringListModel::setStringList;

public slots:
    void findIconThemes();
};

#endif /* KTAILCTL_ICON_THEME_MODEL_H */
