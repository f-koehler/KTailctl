// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#include "app.h"
#include <KSharedConfig>
#include <KWindowConfig>
#include <QClipboard>
#include <QGuiApplication>
#include <QMenu>
#include <QQuickWindow>

#include <functional>

App::App(QObject *parent) : QObject(parent), m_tray_icon(this) {
  m_tray_icon.setContextMenu(new QMenu());

  QObject::connect(&m_status, &Status::peersChanged, &m_peer_model,
                   &PeerModel::updatePeers);
  QObject::connect(&m_status, &Status::refreshed, &m_peer_details,
                   &Peer::updateFromStatus);
  QObject::connect(m_tray_icon.contextMenu(), &QMenu::aboutToShow, this,
                   &App::updateTrayMenu);

  m_tray_icon.setIcon(QIcon::fromTheme(QStringLiteral("online")));
  m_tray_icon.show();
}

Status *App::status() { return &m_status; }
Peer *App::peerDetails() { return &m_peer_details; }
PeerModel *App::peerModel() { return &m_peer_model; }

void App::restoreWindowGeometry(QQuickWindow *window,
                                const QString &group) const {
  KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig,
                       QStandardPaths::AppDataLocation);
  KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
  KWindowConfig::restoreWindowSize(window, windowGroup);
  KWindowConfig::restoreWindowPosition(window, windowGroup);
}

void App::saveWindowGeometry(QQuickWindow *window, const QString &group) const {
  KConfig dataResource(QStringLiteral("data"), KConfig::SimpleConfig,
                       QStandardPaths::AppDataLocation);
  KConfigGroup windowGroup(&dataResource, QStringLiteral("Window-") + group);
  KWindowConfig::saveWindowPosition(window, windowGroup);
  KWindowConfig::saveWindowSize(window, windowGroup);
  dataResource.sync();
}

void App::setPeerDetails(const QString &id) {
  if (m_status.self()->id() == id) {
    m_peer_details = *m_status.self();
    emit peerDetailsChanged();
  } else {
    auto pos = std::find_if(m_status.peers().begin(), m_status.peers().end(),
                            [&id](Peer *peer) { return peer->id() == id; });
    if (pos == m_status.peers().end()) {
      qWarning() << "Peer" << id << "not found";
      return;
    }
    if (m_peer_details.setTo(**pos)) {
      qDebug() << "Peer details updated";
      emit peerDetailsChanged();
    }
  }
}

void App::setClipboardText(const QString &text) {
  QClipboard *clipboard = QGuiApplication::clipboard();
  clipboard->setText(text);
}

QIcon loadOSIcon(const QString &os) {
  QIcon icon = QIcon(QString(":/icon/%1.svg").arg(os.toLower()));
  icon.setIsMask(true);
  return icon;
}

void App::updateTrayMenu() {
  QClipboard *clipboard = QGuiApplication::clipboard();
  auto create_action = [clipboard](QMenu *menu, const QString &text) {
    auto *action = menu->addAction(text);
    connect(action, &QAction::triggered,
            [clipboard, &text]() { clipboard->setText(text); });
    return action;
  };
  QMenu *menu = new QMenu();
  for (auto peer : m_status.peers()) {
    auto *submenu = menu->addMenu(loadOSIcon(peer->os()), peer->hostName());
    create_action(submenu, peer->dnsName());
    for (auto address : peer->tailscaleIPs()) {
      create_action(submenu, address);
    }
  }
  m_tray_icon.setContextMenu(menu);
}