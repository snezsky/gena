import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

RowLayout {
    id: root
    
    required property int index
    required property string url
    required property string name

    width: root.ListView.view.width

    TextField {
        Layout.preferredWidth: 180

        text: root.name
        validator: RegularExpressionValidator { regularExpression: /^[A-Za-z0-9][A-Za-z0-9._ -]*$/ }
    
        onEditingFinished: (text) => root.ListView.view.model.setProperty(root.index, "name", text) 
    }

    RowLayout {
        Layout.fillWidth: true

        TextField {
            Layout.fillWidth: true
            text: root.url
            onEditingFinished: root.ListView.view.model.setProperty(root.index, "url", text) 
        }

        ToolButton {
            id: buttonDelete

            icon.color: "tomato"
            icon.source: "qrc:/icons/delete.svg"

            onClicked: root.ListView.view.model.remove(root.index)
        }
    }
}
