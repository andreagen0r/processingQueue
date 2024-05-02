import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Qt.labs.qmlmodels

ApplicationWindow {
  id: root
  width: 960
  height: 480
  visible: true
  title: qsTr("Queue Processing")

  MyModel {
    id: myModel
  }

  ProcessingModel {
    id: processingModel
    source: myModel
  }

  HistoryModel {
    id: histModel
    source: myModel
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: 12
    spacing: 12

    RowLayout {
      Layout.fillHeight: true
      Layout.fillWidth: true

      ColumnLayout {
        id: firstCol
        spacing: 12
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.preferredWidth: 33

        ListView {
          id: lv1
          spacing: 4
          clip: true
          Layout.fillHeight: true
          Layout.fillWidth: true
          model: myModel
          header: Rectangle {
            width: lv3.width
            height: 30
            color: "gray"

            Label{
              anchors.fill: parent
              elide: Text.ElideRight
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: Text.AlignHCenter
              text: qsTr("Gallery")
            }
          }
          delegate: Rectangle {
            width: lv1.width
            height: 30
            color: "lightblue"

            RowLayout {
              anchors.fill: parent
              anchors.leftMargin: 12
              spacing: 6

              CheckBox {
                Layout.fillHeight: true
                horizontalPadding: 0
                leftInset: 0
                rightInset: 0
                leftPadding: 0
                rightPadding: 0
                visible: btnSelectMode.checked
                // Layout.maximumWidth: implicitHeight
                onCheckedChanged: {
                  model.selected = checked
                }
              }

              Label{
                Layout.fillHeight: true
                Layout.fillWidth: true
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                text: model.title
              }

            }
          }
        }

        RowLayout {
          Layout.fillWidth: true

          Button {
            id: btnSelectMode
            checkable: true
            text: qsTr("Select Mode")
            onCheckedChanged: {
              if (!checked) {
                myModel.unselectAll()
              }
            }
          }
        }


      }

      ColumnLayout {
        id: secondCol
        spacing: 12
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.preferredWidth: 33

        ListView {
          id: lv2
          spacing: 4
          clip: true
          Layout.fillHeight: true
          Layout.fillWidth: true
          model: processingModel
          header: Rectangle {
            width: lv3.width
            height: 30
            color: "gray"

            Label{
              anchors.fill: parent
              elide: Text.ElideRight
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: Text.AlignHCenter
              text: qsTr("Processing Queue")
            }
          }

          delegate: Rectangle {
            width: lv2.width
            height: 30
            color: "lightblue"
            visible: !model.finished

            RowLayout {
              anchors.fill: parent

              Label{
                Layout.fillHeight: true
                Layout.leftMargin: 12
                Layout.minimumWidth: 80
                Layout.maximumWidth: 120
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                text: model.title
              }
              ProgressBar {
                Layout.fillWidth: true
                from: 0
                to: 100
                value: model.progress
                // onValueChanged: console.log(value)
              }
              Button {
                id: btnPause
                Layout.preferredWidth: implicitHeight
                Layout.fillHeight: true
                padding: 0
                leftPadding: 0
                rightPadding: 0
                checkable: true
                icon.height: 16
                icon.width: 16
                icon.source: "qrc:/qt/qml/QueueProcessing/pause.svg"
              }

              Button {
                id: btnCancel
                Layout.preferredWidth: implicitHeight
                Layout.fillHeight: true
                padding: 0
                leftPadding: 0
                rightPadding: 0
                icon.height: 16
                icon.width: 16
                icon.source: "qrc:/qt/qml/QueueProcessing/delete.svg"
                ToolTip.visible: hovered
                ToolTip.delay: Qt.styleHints.mousePressAndHoldInterval
                ToolTip.text: qsTr("Cancel the job")
              }
            }
          }
        }

        Button {
          Layout.fillWidth: true
          text: qsTr("Clear all")
          onClicked: processingModel.clear()
        }
      }

      ColumnLayout {
        id: thirddCol
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.preferredWidth: 33

        ListView {
          id: lv3
          spacing: 4
          clip: true
          Layout.fillHeight: true
          Layout.fillWidth: true
          model: histModel
          header: Rectangle {
            width: lv3.width
            height: 30
            color: "gray"

            Label{
              anchors.fill: parent
              elide: Text.ElideRight
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: Text.AlignHCenter
              text: qsTr("History")
            }
          }

          delegate: Rectangle {
            width: lv3.width
            height: 30
            color: "lightblue"

            RowLayout {
              anchors.fill: parent

              Label{
                Layout.fillHeight: true
                Layout.leftMargin: 12
                elide: Text.ElideRight
                verticalAlignment: Text.AlignVCenter
                text: model.title
              }
            }
          }
        }

        Button {
          Layout.fillWidth: true
          text: qsTr("Clear History")
          onClicked: myModel.clearHistory()
        }
      }
    }

    RowLayout {
      Layout.fillWidth: true

      ProgressBar {
        Layout.fillWidth: true
        from: 0
        to: myModel.selectedCount
        value: myModel.finishedCount

        Behavior on value { NumberAnimation {duration: 250} }

      }

      Button {
        // Layout.fillWidth: true
        enabled: myModel.toProcessing
        text: qsTr("Processing")

        onClicked: {
          myModel.processing()
        }
      }
    }


  }
}
