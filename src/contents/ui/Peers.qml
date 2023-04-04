// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2023 Fabian Köhler <me@fkoehler.org>

import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import org.kde.kirigami 2.19 as Kirigami
import org.kde.Tailctl 1.0

Kirigami.ScrollablePage {
    id: peers

    Layout.fillWidth: true

    title: i18n("Peers")

    actions.main: Kirigami.Action {
        text: App.status.backendState == "Running" ? "Online" : "Offline"
    }

    Kirigami.CardsListView {
        id: listPeers
        model: App.peerModel
        delegate: Kirigami.AbstractCard {
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout

                    anchors {
                        left: parent.left
                        right: parent.right
                        top: parent.top
                    }

                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.smallSpacing
                    columns: 5

                    Kirigami.Icon {
                        source: Util.loadOsIcon(os)
                    }

                    Controls.Label {
                        text: hostName
                    }

                    Kirigami.Icon {
                        source: isOnline ? "online" : "offline"
                    }

                    Controls.Button {
                        text: "Details"
                        onClicked: {
                            App.setPeerDetails(tailscaleID);
                            pageStack.layers.push('qrc:Peer.qml');
                        }
                    }
                }
            }
        }
    }
}
