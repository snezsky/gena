import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import Gena

ColumnLayout {
    id: root
    property alias model: view.model

    Label { text: "Submodules" }

    Frame {
        Layout.fillWidth: true
        Layout.fillHeight: true

        ColumnLayout {
            anchors.fill: parent

            ListView {
                id: view
                Layout.fillWidth: true
                Layout.fillHeight: true
                clip: true
                spacing: 10
                topMargin: 10

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

                icon.color: enabled ? "cornflowerblue" : "gray"
                icon.source: "qrc:/icons/add.svg"

                onClicked: { view.model.append({ url: "", deletable: true }) }
            }
        }
    }

    function update_test_framework_submodule(testFramework) {

        function removeUrlsContaining(text) {
            const searchText = text.toLowerCase()
            for (let i = model.count - 1; i >= 0; --i) {
                const submodule = model.get(i)
                if (submodule.url.toLowerCase().includes(searchText)) {
                    model.remove(i)
                }
            }
        }

        removeUrlsContaining("/catch2")
        removeUrlsContaining("/googletest")

        if (testFramework === Options.TestFramework.Catch2) {
            model.insert(0, {url: "https://github.com/catchorg/Catch2.git", deletable: false })
        }
        else if (testFramework === Options.TestFramework.GoogleTest) {
            model.insert(0, {url: "https://github.com/google/googletest.git", deletable: false })
        }
    }
}
