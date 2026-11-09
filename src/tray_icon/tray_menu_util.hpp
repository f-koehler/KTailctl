#ifndef KTAILCTL_TRAY_MENU_UTIL_HPP
#define KTAILCTL_TRAY_MENU_UTIL_HPP

#include <QAbstractItemModel>
#include <QMenu>
#include <QObject>
#include <QTimer>

namespace TrayMenuUtil
{
/**
 * Creates a coalescing, single-shot timer (owned by `menu`) that invokes `rebuild` on the menu.
 *
 * Tray menus must be populated *before* they are shown and must not be modified while they are open: DBusMenu hosts such as the GNOME AppIndicator
 * extension never open an empty submenu, and rebuilding items in response to `aboutToShow` makes them recreate the submenu with new ids, which closes it again.
 * Menus are therefore rebuilt whenever their underlying data changes instead.
 */
template<typename Fn>
auto makeRebuildTimer(QMenu *menu, Fn rebuild) -> QTimer *
{
    auto *timer = new QTimer(menu);
    timer->setSingleShot(true);
    timer->setInterval(0);
    QObject::connect(timer, &QTimer::timeout, menu, rebuild);
    return timer;
}

/// Schedules `timer` whenever the contents of `model` change.
inline void rebuildOnModelChange(QAbstractItemModel *model, QTimer *timer)
{
    const auto schedule = [timer] -> void {
        timer->start();
    };
    QObject::connect(model, &QAbstractItemModel::modelReset, timer, schedule);
    QObject::connect(model, &QAbstractItemModel::layoutChanged, timer, schedule);
    QObject::connect(model, &QAbstractItemModel::rowsInserted, timer, schedule);
    QObject::connect(model, &QAbstractItemModel::rowsRemoved, timer, schedule);
    QObject::connect(model, &QAbstractItemModel::rowsMoved, timer, schedule);
    QObject::connect(model, &QAbstractItemModel::dataChanged, timer, schedule);
}
} // namespace TrayMenuUtil

#endif // KTAILCTL_TRAY_MENU_UTIL_HPP
