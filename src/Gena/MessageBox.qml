import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root

    property alias text: label.text
    property int type: MessageBox.Info

    enum Type {
        Info,
        Error
    }

    modal: true
    bottomPadding: 0
    anchors.centerIn: parent

    RowLayout {
        anchors.fill: parent

        Image {
            sourceSize: Qt.size(64, 64)
            source: {
                switch (root.type) {
                    case MessageBox.Info:  return "qrc:/icons/geninfo.svg"
                    case MessageBox.Error: return "qrc:/icons/generror.svg"
                }
            }
        }

        Label {
            id: label
            Layout.fillWidth: true
            wrapMode: Text.Wrap
        }
    }
}
