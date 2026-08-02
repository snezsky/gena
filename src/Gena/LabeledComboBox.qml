import QtQuick.Controls
import QtQuick.Layouts

RowLayout {
    property alias title: label.text
    property alias model: comboBox.model
    property alias currentIndex: comboBox.currentIndex
    property alias currentValue: comboBox.currentValue
    
    Layout.fillWidth: true

    Label {
        id: label
        Layout.preferredWidth: 100
    }
    ComboBox {
        id: comboBox
        textRole: "text"
        valueRole: "value"
        Layout.fillWidth: true
    }
}
