import QtQuick
import org.kde.kirigami as Kirigami

// A compact live bar graph of recent ping latencies. Each bar is one sample,
// newest on the right; failed/timed-out samples (value < 0) render as a faint
// full-height marker so gaps are visible. Repaints whenever the bound Pinger
// reports a new sample.
Canvas {
    id: graph

    // A KTailctl.Pinger instance (obtained via Tailscale.pinger(address)).
    property var pinger: null

    // Number of sample slots; matches Pinger::MaxHistory so bars keep a stable
    // width and the graph "scrolls" as history fills up.
    readonly property int slots: 60

    readonly property color barColor: Kirigami.Theme.highlightColor
    readonly property color failColor: Kirigami.Theme.negativeTextColor
    readonly property color gridColor: Kirigami.Theme.textColor

    implicitHeight: Kirigami.Units.gridUnit * 3

    onPingerChanged: requestPaint()
    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()

    Connections {
        target: graph.pinger
        function onHistoryChanged(): void {
            graph.requestPaint();
        }
    }

    onPaint: {
        const ctx = getContext("2d");
        ctx.reset();
        ctx.clearRect(0, 0, width, height);

        // Baseline
        ctx.strokeStyle = Qt.rgba(gridColor.r, gridColor.g, gridColor.b, 0.2);
        ctx.lineWidth = 1;
        ctx.beginPath();
        ctx.moveTo(0, height - 0.5);
        ctx.lineTo(width, height - 0.5);
        ctx.stroke();

        if (!pinger) {
            return;
        }
        const hist = pinger.history;
        const n = hist.length;
        if (n === 0) {
            return;
        }

        const maxMs = Math.max(pinger.maxLatencyMs, 1);
        const slotWidth = width / graph.slots;
        const barWidth = Math.max(1, slotWidth - 1);

        for (let i = 0; i < n; ++i) {
            // Right-align so the newest sample sits at the right edge.
            const x = width - (n - i) * slotWidth;
            const value = hist[i];
            if (value < 0) {
                // Failure / timeout: faint full-height marker.
                ctx.fillStyle = Qt.rgba(failColor.r, failColor.g, failColor.b, 0.35);
                ctx.fillRect(x, 0, barWidth, height);
            } else {
                const barHeight = Math.max(1, (value / maxMs) * height);
                ctx.fillStyle = barColor;
                ctx.fillRect(x, height - barHeight, barWidth, barHeight);
            }
        }
    }
}
