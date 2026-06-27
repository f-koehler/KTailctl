import QtQuick
import org.kde.kirigami as Kirigami

// A compact live bar graph of recent ping latencies with a labeled y-axis.
// Samples are positioned by their wall-clock age (one column per ping
// interval), so the bars scroll left even while pinging is paused — a paused
// stretch shows up as a growing gap on the right. Failed/timed-out samples
// (value < 0) render as a faint full-height marker.
Canvas {
    id: graph

    // A KTailctl.Pinger instance (obtained via Tailscale.pinger(address)).
    property var pinger: null

    // Width of the visible time window, in columns (matches Pinger::MaxHistory).
    readonly property int columns: 60
    readonly property int intervalMs: pinger ? pinger.intervalMs : 1000

    readonly property color barColor: Kirigami.Theme.highlightColor
    readonly property color failColor: Kirigami.Theme.negativeTextColor
    readonly property color axisColor: Kirigami.Theme.textColor

    implicitHeight: Kirigami.Units.gridUnit * 5

    onPingerChanged: requestPaint()
    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()

    Connections {
        target: graph.pinger
        function onHistoryChanged(): void {
            graph.requestPaint();
        }
    }

    // Advance the time axis once per ping interval, even while paused, so a gap
    // grows on the right whenever pinging is stopped.
    Timer {
        interval: graph.intervalMs
        running: graph.visible && graph.pinger !== null
        repeat: true
        onTriggered: graph.requestPaint()
    }

    // Round up to a "nice" axis maximum (1/2/5 × 10ⁿ) for readable labels.
    function niceCeil(value: real): real {
        if (value <= 0) {
            return 1;
        }
        const exponent = Math.floor(Math.log10(value));
        const base = Math.pow(10, exponent);
        const fraction = value / base;
        const niceFraction = fraction <= 1 ? 1 : (fraction <= 2 ? 2 : (fraction <= 5 ? 5 : 10));
        return niceFraction * base;
    }

    onPaint: {
        const ctx = getContext("2d");
        ctx.reset();
        ctx.clearRect(0, 0, width, height);

        const fontPx = Kirigami.Theme.smallFont.pixelSize;
        ctx.font = fontPx + "px " + Kirigami.Theme.smallFont.family;
        ctx.textBaseline = "middle";

        const maxMs = niceCeil(pinger ? pinger.maxLatencyMs : 0);
        const decimals = maxMs < 10 ? 1 : 0;

        // Reserve a left gutter for the y-axis labels and vertical padding so
        // the top/bottom labels are not clipped.
        const padV = fontPx;
        const gutter = ctx.measureText(maxMs.toFixed(decimals) + " ms").width + Kirigami.Units.smallSpacing * 2;
        const plotLeft = gutter;
        const plotRight = width;
        const plotTop = padV / 2;
        const plotBottom = height - padV / 2;
        const plotHeight = Math.max(1, plotBottom - plotTop);
        const plotWidth = Math.max(1, plotRight - plotLeft);

        // Gridlines and y-axis labels at 0, 50% and 100% of the scale.
        ctx.lineWidth = 1;
        ctx.textAlign = "right";
        for (const level of [0, 0.5, 1]) {
            const y = plotBottom - level * plotHeight;
            ctx.strokeStyle = Qt.rgba(axisColor.r, axisColor.g, axisColor.b, 0.15);
            ctx.beginPath();
            ctx.moveTo(plotLeft, y + 0.5);
            ctx.lineTo(plotRight, y + 0.5);
            ctx.stroke();
            ctx.fillStyle = Qt.rgba(axisColor.r, axisColor.g, axisColor.b, 0.7);
            ctx.fillText((level * maxMs).toFixed(decimals) + " ms", gutter - Kirigami.Units.smallSpacing, y);
        }

        if (!pinger) {
            return;
        }
        const latencies = pinger.history;
        const times = pinger.historyTimes;
        const n = Math.min(latencies.length, times.length);
        if (n === 0) {
            return;
        }

        const now = Date.now();
        const columnWidth = plotWidth / graph.columns;
        const barWidth = Math.max(1, columnWidth - 1);

        for (let i = 0; i < n; ++i) {
            // Column 0 is the rightmost (newest); older samples shift left.
            const column = Math.floor((now - times[i]) / graph.intervalMs);
            if (column < 0 || column >= graph.columns) {
                continue;
            }
            const x = plotRight - (column + 1) * columnWidth;
            const value = latencies[i];
            if (value < 0) {
                ctx.fillStyle = Qt.rgba(failColor.r, failColor.g, failColor.b, 0.35);
                ctx.fillRect(x, plotTop, barWidth, plotHeight);
            } else {
                const barHeight = Math.max(1, (value / maxMs) * plotHeight);
                ctx.fillStyle = barColor;
                ctx.fillRect(x, plotBottom - barHeight, barWidth, barHeight);
            }
        }
    }
}
