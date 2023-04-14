import QtQuick 2.15
import QtQuick.Controls 2.15 as Controls
import QtQuick.Layouts 1.15
import QtCharts 2.5
import org.kde.kirigami 2.19 as Kirigami
import org.fkoehler.KTailctl 1.0

Kirigami.ScrollablePage {
    id: statistics

    Layout.fillWidth: true

    title: i18n("Statistics")


    ChartView {
        id: chart
        anchors.fill: parent
        antialiasing: true

        Component.onCompleted: {
            //var series = createSeries(ChartView.SeriesTypeLine);
            //series = Tailscale.statistics.speedUpTotal.series;
            print(Tailscale.statistics.speedUpTotal.series.count());
        }
    }
}
