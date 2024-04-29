import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
  id: root
  width: 960
  height: 480
  visible: true
  title: qsTr("Queue Processing")

  MyModel {
    id: myModel
  }

  ProcessingQueueModel {
    id: queueModel
    dataSource: myModel.outData
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: 12
    spacing: 12

    RowLayout {
      Layout.fillHeight: true
      Layout.fillWidth: true
      clip: true

      ColumnLayout {
        id: firstCol
        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.preferredWidth: 33

        ListView {
          id: lv1
          spacing: 4
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
              text: `Content Items`
            }
          }
          delegate: Rectangle {
            width: lv1.width
            height: 30
            color: "lightblue"

            RowLayout {
              anchors.fill: parent
              spacing: 6

              CheckBox {
                Layout.fillHeight: true
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
          Layout.fillHeight: true
          Layout.fillWidth: true
          model: queueModel
          header: Rectangle {
            width: lv3.width
            height: 30
            color: "gray"

            Label{
              anchors.fill: parent
              elide: Text.ElideRight
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: Text.AlignHCenter
              text: `Processing Queue`
            }
          }

          delegate: Rectangle {
            width: lv2.width
            height: 30
            color: "lightblue"

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
                value: 50
              }
              Button {
                Layout.preferredWidth: implicitHeight
                Layout.fillHeight: true
                padding: 0
                leftPadding: 0
                rightPadding: 0
                checkable: true
                text: model.paused ? "II" : "X"

                onCheckedChanged: model.paused = checked
              }
            }
          }
        }

        Button {
          Layout.fillWidth: true
          text: `Clear`
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
          Layout.fillHeight: true
          Layout.fillWidth: true
          model: 20
          header: Rectangle {
            width: lv3.width
            height: 30
            color: "gray"

            Label{
              anchors.fill: parent
              elide: Text.ElideRight
              verticalAlignment: Text.AlignVCenter
              horizontalAlignment: Text.AlignHCenter
              text: `Processed itens`
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
                text: `Title asadasd asd asd asd${index}`
              }
            }
          }
        }
      }
    }

    Button {
      Layout.fillWidth: true
      enabled: myModel.toProcessing
      text: "Processing"

      onClicked: myModel.processing()
    }
  }
}
