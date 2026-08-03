import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs
import QtQuick.Layouts

import Gena

ApplicationWindow
{
    id: root
    visible: true
    minimumWidth: 340
    minimumHeight: 530

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
                { text: "Console Application",    value: Options.ProjectType.Executable },
                { text: "Qt Widgets Application", value: Options.ProjectType.QMainWindow }
            ]
            currentIndex: 0
        }

        LabeledComboBox {
            id: comboBoxTestFramework
            title: "test framework"
            model: [
                { text: "QTest",      value: Options.Dependency.QTest },
                { text: "Catch2",     value: Options.Dependency.Catch2 },
                { text: "GoogleTest", value: Options.Dependency.GoogleTest }
            ]
            currentIndex: 2
        }

        GroupBox {
            title: "dependencies"
            Layout.fillWidth: true
            ColumnLayout {
                spacing: -10
                Layout.fillWidth: true
                CheckBox {
                    id: checkBoxCLI11
                    text: "CLI11"
                }
                CheckBox {
                    id: checkBoxSpdlog
                    text: "spdlog"
                }
                CheckBox {
                    id: checkBoxJson
                    text: "nlohmann/json"
                }
            }
        }

        CheckBox {
            id: checkBoxSetupGit
            text: "setup git repository"
            checked: true
        }

        Item { Layout.fillHeight: true }

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
            generator.composeDependenciesAndGenerate()
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

        function composeDependenciesAndGenerate() {
            let deps = comboBoxTestFramework.currentValue
                     | (checkBoxCLI11.checked  ? Options.CLI11  : 0)
                     | (checkBoxSpdlog.checked ? Options.Spdlog : 0)
                     | (checkBoxJson.checked   ? Options.Json   : 0)

            generator.generateAsync({
                name:         textFieldName.text,
                type:         comboBoxProjectType.currentValue,
                standard:     comboBoxCppStandard.currentValue,
                dependencies: deps,
                namespace:    textFieldNamespace.text,
                location:     folderDialog.selectedFolder,
                setupGit:     checkBoxSetupGit.checked
            })
        }
    }
}
