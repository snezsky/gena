import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ColumnLayout {
    id: root
    readonly property ListModel model: view.model

    Label { text: "Submodules" }

    Frame {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ColumnLayout {
            anchors.fill: parent

            RowLayout {
                Layout.fillWidth: true
                Label { text: "Name"; Layout.preferredWidth: 185; font.bold: true; }
                Label { text: "Url";  Layout.fillWidth: true; font.bold: true; }
            }

            ListView {
                id: view
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                spacing: 10

                ScrollBar.vertical: ScrollBar {
                    width: 8
                    background.visible: false
                    Component.onCompleted: contentItem.radius = width / 2
                }

                model: ListModel {}
                delegate: SubmodulesDelegate{ spacing: view.spacing }
            }

            Button {
                text: "Add Submodule"

                icon.color: "cornflowerblue"
                icon.source: "qrc:/icons/add.svg"

                onClicked: { view.model.append({ "name": "", "url": "" }) }
            }
        }
    }
}
