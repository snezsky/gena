import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root
    
    required property int index
    required property string url
    required property bool deletable

    width: root.ListView.view.width

    RowLayout {
        Layout.fillWidth: true

        TextField {
            Layout.fillWidth: true
            text: root.url
            placeholderText: "url"
            onEditingFinished: root.ListView.view.model.setProperty(root.index, "url", text) 
        }

        ToolButton {
            id: buttonDelete
            enabled: deletable
            icon.color: enabled ? "tomato" : "gray"
            icon.source: "qrc:/icons/delete.svg"

            onClicked: root.ListView.view.model.remove(root.index)
        }
    }
}
