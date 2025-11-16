import Qt.labs.platform

SystemTrayIcon {
    visible: true
    icon.name: "folder-open"

    onActivated: {
        window.show();
        window.raise();
        window.requestActivate();
    }

    menu: Menu {
        MenuItem {
            text: "Show"
            icon.name: "window"
            onTriggered: {
                window.show();
                window.raise();
                window.requestActivate();
            }
        }
        MenuSeparator {}
        Menu {
            title: "Accounts"
            icon.name: "user"

            MenuItem {
                text: "Add Account"
            }
        }
        MenuItem {
            text: "This Node"
            icon.name: "computer"
        }
        MenuItem {
            text: "Peers"
            icon.name: "applications-network"
        }
        MenuItem {
            text: "Exit Nodes"
            icon.name: "internet-services"
        }
        MenuSeparator {}
        MenuItem {
            text: "Quit"
            icon.name: "application-exit"
        }
    }
}
