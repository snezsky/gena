import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    property alias title: label.text
    property alias text: textField.text
    property alias validator: textField.validator
    property bool empty: !textField.text
    property string tooltip

    Layout.fillWidth: true

    Label {
        id: label
        Layout.preferredWidth: 100
    }
    RowLayout {
        Layout.fillWidth: true

        TextField {
            id: textField
            Layout.fillWidth: true
        }

        ToolButton {
            padding: 0
            Layout.preferredWidth: textField.height
            Layout.preferredHeight: textField.height

            icon.width: textField.height
            icon.height: textField.height
            icon.source: "qrc:/icons/tooltip.svg"
            icon.color: hovered ? Material.primaryTextColor : Material.hintTextColor

            ToolTip.delay: 500
            ToolTip.text: tooltip
            ToolTip.visible: hovered

            focusPolicy: Qt.NoFocus
        }
    }
}
