import QtQuick 2.0
import QtMultimedia 5.0

Rectangle {
    id:root
    objectName: qsTr("root")
    color:"black"
    opacity: 0

    MediaPlayer {
        id:mediaPlayer
        objectName: qsTr("mediaPlayer")
        autoLoad: false

        onError: {
            if (MediaPlayer.NoError != error) {
                console.log("[qmlvideo] VideoItem.onError error " + error + " errorString " + errorString)
                root.fatalError()
            }
        }
    }

    VideoOutput {
        id: videoContent
        objectName: qsTr("videoContent")
        anchors.fill: parent
        anchors.topMargin: 70
        source: mediaPlayer

        function setFullScreen(isFullScreen)
        {
            if(isFullScreen){
                videoContent.anchors.topMargin = 0;
            }else{
                videoContent.anchors.topMargin = 70;
            }
        }
    }
}

