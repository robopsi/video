import QtQuick 2.0
import QtMultimedia 5.0

Rectangle {
    id:root
    color:"black"

    MediaPlayer {
        id:mediaPlayer
        objectName: qsTr("mediaPlayer")
        autoLoad: false
        //        loops: Audio.Infinite
        //        source: "file:///c:/Users/cjs/mnt/video/第2课：上午字符串.avi"
        //        autoPlay: true

        onError: {
            if (MediaPlayer.NoError != error) {
                console.log("[qmlvideo] VideoItem.onError error " + error + " errorString " + errorString)
                root.fatalError()
            }
        }
    }

    VideoOutput {
        id: video
        objectName: qsTr("videoContent")
        anchors.fill: parent
        source: mediaPlayer
    }
}

