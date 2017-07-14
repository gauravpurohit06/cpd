import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.4

Rectangle {
    visible: true

    signal printButtonClicked(string printerName)
    signal cancelButtonClicked()

    /* This Rectangle is for the sidebar */

    Rectangle {
        height: parent.height
        width: 150
        color: "#EEEEEE"

        ListModel {
            id: sidebar_model

            ListElement {
                name: "General"
                value: "general.qml"    // value points to the page it loads when clicked
            }
            ListElement {
                name: "Page Setup"
                value: "page_setup.qml"
            }
            ListElement {
                name: "Options"
                value: "options.qml"
            }
            ListElement {
                name: "Jobs"
                value: "jobs.qml"
            }
            ListElement {
                name: "Advanced"
                value: "advanced.qml"
            }
            /*ListElement {
                name: "Preview"
                value: "preview.qml"
            }*/
        }

        ListView {
            width: parent.width
            height: parent.height
            model: sidebar_model
            delegate: Rectangle { // Every button in the sidebar is made using a Rectangle with a MouseArea
                width: parent.width
                height: 40
                color: "#EEEEEE"

                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: name
                    x: 10
                }

                MouseArea{
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: { page_loader.source = value }
                    onEntered: { parent.color = "#BDBDBD" }
                    onExited:  { parent.color = "#EEEEEE" }
                }
            }
        }

        Rectangle {
            x: 10
            y: parent.height - 120
            width: parent.width - 20
            height: 40
            color: "#E91E63"

            Text {
                anchors.centerIn: parent
                text: "Print"
                color: "white"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: { parent.color = "#D81B60" }
                onExited:  { parent.color = "#E91E63" }
                onClicked: printButtonClicked("Xerox")
            }
        }

        Rectangle {
            x: 10
            y: parent.height - 60
            width: parent.width - 20
            height: 40
            color: "#CFD8DC"

            Text {
                anchors.centerIn: parent
                text: "Cancel"
                color: "black"
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                onEntered: { parent.color = "#B0BEC5" }
                onExited:  { parent.color = "#CFD8DC" }
                onClicked: { cancelButtonClicked() }
            }
        }
    }

    /* This Rectangle is for the main view */

    Rectangle{
        id:main_view
        x: 150; width: parent.width - 150
        height: parent.height

        Loader{
            id: page_loader
            source: "general.qml"
            width: parent.width
            height: parent.height
        }
    }
}
