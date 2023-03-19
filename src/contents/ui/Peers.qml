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
        text: Status.backendState == "Running" ? "Online" : "Offline"
    }

    Kirigami.CardsListView {
        id: listPeers
        model: PeerModel
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
                    columns: 3

                    Kirigami.Icon {
                        source: getOSIcon(os)
                        //color: "#ffffff"
                        isMask: true
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
                            console.log(tailscaleID);
                            pageStack.layers.push('qrc:Peer.qml');
                        }
                    }
                }
            }
        }
    }
}
