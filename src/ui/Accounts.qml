import QtQuick
import org.kde.kirigami as Kirigami

Kirigami.ScrollablePage {
    id: accounts

    Layout.fillWidth: true

    Kirigami.CardsListView {
        id: listAccounts

        delegate: Kirigami.AbstractCard {
            contentItem: Flow {
            }
        }
    }
}
