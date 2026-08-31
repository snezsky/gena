import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import Gena

ApplicationWindow
{
    id: root
    visible: true
    width: 560
    height: 585

    ColumnLayout
    {
        anchors.fill: parent
        anchors.margins: 12

        LabeledTextField {
            id: textFieldName
            title: "name"
            tooltip: "Will be used as the name of the CMake project, targets, and files"
            validator: RegularExpressionValidator { regularExpression: /^[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*$/ }
        }

        LabeledTextField {
            id: textFieldNamespace
            title: "namespace"
            tooltip: "Will be used as a C++ namespace and as a prefix for CMake functions/variables"
            validator: RegularExpressionValidator { regularExpression: /^[A-Za-z][A-Za-z0-9]*(?:_[A-Za-z0-9]+)*$/ }
        }

        LabeledComboBox {
            id: comboBoxCppStandard
            title: "cpp standard"
            model: [
                { text: "C++17", value: Options.CppStandard.Cpp17 },
                { text: "C++20", value: Options.CppStandard.Cpp20 },
                { text: "C++23", value: Options.CppStandard.Cpp23 }
            ]
            currentIndex: 1
        }

        LabeledComboBox {
            id: comboBoxProjectType
            title: "project type"
            model: [
                { text: "Library",                value: Options.ProjectType.Library },
                { text: "Console Application",    value: Options.ProjectType.ConsoleApplication },
                { text: "Qt Quick Application",   value: Options.ProjectType.QtQuickApplication },
                { text: "Qt Widgets Application", value: Options.ProjectType.QtWidgetsApplication }
            ]
            currentIndex: 0
        }

        LabeledComboBox {
            id: comboBoxTestFramework
            title: "test framework"
            model: [
                { text: "QTest",      value: Options.TestFramework.QTest },
                { text: "Catch2",     value: Options.TestFramework.Catch2 },
                { text: "GoogleTest", value: Options.TestFramework.GoogleTest }
            ]
            currentIndex: 2
            onCurrentValueChanged: {
                viewSubmodules.update_test_framework_submodule(currentValue)
            }
        }

        SubmodulesView {
            id: viewSubmodules
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Button {
            id: buttonGenerate
            text: "Generate"
            Layout.fillWidth: true
            enabled: !textFieldName.empty && !textFieldNamespace.empty
            onClicked: folderDialog.open()
        }
    }

    Shortcut {
        sequence: "Return"
        onActivated: {
            if (buttonGenerate.enabled) {
                buttonGenerate.clicked()
            }
        }
    }

    Item {
        id: overlay
        visible: false
        anchors.fill: parent

        Rectangle {
            anchors.fill: parent
            color: Qt.rgba(0, 0, 0, 0.4)
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: overlay.visible
        }
    }

    MessageBox {
        id: successDialog
        type: MessageBox.Info
        title: "Generation successful"
        text: "Open the project directory?"
        standardButtons: Dialog.Open | Dialog.No
        onAccepted: Qt.openUrlExternally(generator.projectDirectory)
    }

    MessageBox {
        id: errorDialog
        type: MessageBox.Error
        title: "Generation failed"
        standardButtons: Dialog.Ok
    }

    FolderDialog {
        id: folderDialog
        title: "Select the project destination"
        onAccepted: {
            overlay.visible = true
            generator.startGeneration()
        }
    }

    Generator {
        id: generator

        onGenerationFinished: {
            overlay.visible = false
            successDialog.open()
        }

        onGenerationFailed: (message) => {
            overlay.visible = false
            errorDialog.text = message
            errorDialog.open()
        }

        function startGeneration() {
            const submodules = []
            for (let i = 0; i < viewSubmodules.model.count; ++i) {
                const submodule = viewSubmodules.model.get(i)
                if (submodule.url) {
                    submodules.push(submodule.url)
                }
            }

            generator.generateAsync({
                name:            textFieldName.text,
                type:            comboBoxProjectType.currentValue,
                standard:        comboBoxCppStandard.currentValue,
                testFramework:   comboBoxTestFramework.currentValue,
                namespace:       textFieldNamespace.text,
                outputDirectory: folderDialog.selectedFolder,
                submodules:      submodules
            })
        }
    }
}
