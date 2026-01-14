import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 480
    height: 320
    title: i18n.tr("wizard.welcome")

    Column {
        anchors.centerIn: parent
        spacing: 12
        Text { text: i18n.tr("wizard.welcome"); horizontalAlignment: Text.AlignHCenter }
        Button { text: i18n.tr("controls.clickMe"); onClicked: {
            // create a default config.yaml in current working directory
            var cfg = "ffmpeg-path: \"/usr/bin/ffmpeg\"\ntheme-color: \"#0078D4\"\n";
            var fh = Qt.openUrlExternally("data:text/plain," + encodeURIComponent(cfg));
        } }
    }
}