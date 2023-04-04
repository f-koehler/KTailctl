// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

#ifndef TAILCTL_APP_H
#define TAILCTL_APP_H

#include "peer_model.h"
#include "status.h"
#include "tailctlconfig.h"
#include "taildrop_process.h"

#include <QObject>
#include <QProcess>
#include <QQuickWindow>
#include <QSystemTrayIcon>

class QQuickWindow;

class App : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TailctlConfig *config READ config NOTIFY configChanged)
    Q_PROPERTY(Status *status READ status NOTIFY statusChanged)
    Q_PROPERTY(Peer *peerDetails READ peerDetails NOTIFY peerDetailsChanged)
    Q_PROPERTY(PeerModel *peerModel READ peerModel NOTIFY peerModelChanged)

private:
    TailctlConfig *m_config;
    TaildropProcess m_taildrop_process;
    Status m_status;
    Peer m_peer_details;
    PeerModel m_peer_model;

    QSystemTrayIcon m_tray_icon;
    QQuickWindow *m_window;

signals:
    void configChanged();
    void statusChanged();
    void peerDetailsChanged();
    void peerModelChanged();

public slots:
    Q_INVOKABLE void updateTrayMenu();

public:
    App(QObject *parent = nullptr);

    TailctlConfig *config();
    Status *status();
    Peer *peerDetails();
    PeerModel *peerModel();

    void setWindow(QQuickWindow *window);

    // Restore current window geometry
    Q_INVOKABLE void restoreWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;
    // Save current window geometry
    Q_INVOKABLE void saveWindowGeometry(QQuickWindow *window, const QString &group = QStringLiteral("main")) const;

    Q_INVOKABLE void setPeerDetails(const QString &id);

    Q_INVOKABLE void setClipboardText(const QString &text);

    Q_INVOKABLE QString formatCapacityHumanReadable(long bytes) const;
    Q_INVOKABLE QString formatSpeedHumanReadable(double bytes_per_second) const;
    Q_INVOKABLE QString formatDurationHumanReadable(const QDateTime &from, const QDateTime &to = QDateTime::currentDateTime()) const;
    Q_INVOKABLE QString fileUrlToString(const QUrl &url) const;
    Q_INVOKABLE qint64 toMSecsSinceEpoch(const QDateTime &dateTime) const;
};

#endif /* TAILCTL_APP_H */
