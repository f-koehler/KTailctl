import QtQuick
import org.kde.kirigami as Kirigami

// A compact live line plot of recent ping latencies with a labeled y-axis.
// Samples are positioned by their exact wall-clock age and the plot is
// repainted every frame, so points glide smoothly to the left — also while
// pinging is paused, in which case a growing gap appears on the right.
// Consecutive samples are connected by a line; failed/timed-out samples
// (value < 0) break the line and render as a faint marker band.
Canvas {
    id: graph

    // A KTailctl.Pinger instance (obtained via Tailscale.pinger(address)).
    property var pinger: null

    // Width of the visible time window, in ping intervals (matches Pinger::MaxHistory).
    readonly property int columns: 60
    readonly property int intervalMs: pinger ? pinger.intervalMs : 1000
    readonly property real windowMs: columns * intervalMs
    // Two successful samples further apart than this are not connected, so a
    // paused stretch shows up as a gap instead of a long straight line.
    readonly property real maxConnectGapMs: 3 * intervalMs

    readonly property color lineColor: Kirigami.Theme.highlightColor
    readonly property color failColor: Kirigami.Theme.negativeTextColor
    readonly property color axisColor: Kirigami.Theme.textColor

    // Target y-axis maximum, rounded to a "nice" value for the labels, and an
    // animated copy used for the geometry so rescaling does not jump.
    readonly property real targetMaxMs: niceCeil(pinger ? pinger.maxLatencyMs : 0)
    property real scaleMaxMs: targetMaxMs
    Behavior on scaleMaxMs {
        NumberAnimation {
            duration: Kirigami.Units.longDuration
            easing.type: Easing.OutCubic
        }
    }

    // True once nothing visible can move anymore (paused and every sample has
    // scrolled out of the window); stops the per-frame repaint.
    property bool idle: false

    implicitHeight: Kirigami.Units.gridUnit * 6

    onPingerChanged: {
        idle = false;
        requestPaint();
    }
    onWidthChanged: requestPaint()
    onHeightChanged: requestPaint()
    onScaleMaxMsChanged: requestPaint()

    Connections {
        target: graph.pinger
        function onHistoryChanged(): void {
            graph.idle = false;
            graph.requestPaint();
        }
        function onRunningChanged(): void {
            graph.idle = false;
        }
    }

    // Repaint on every rendered frame for smooth scrolling. This only ticks
    // while the window is actually being rendered (not when hidden to tray).
    FrameAnimation {
        running: graph.visible && graph.pinger !== null && !graph.idle
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

    // Pick a "nice" x-axis tick step (in ms) giving roughly four intervals
    // across the visible window.
    function niceTimeStep(spanMs: real): real {
        const candidates = [1000, 2000, 5000, 10000, 15000, 30000, 60000, 120000, 300000, 600000];
        for (const step of candidates) {
            if (spanMs / step <= 5) {
                return step;
            }
        }
        return candidates[candidates.length - 1];
    }

    function formatAge(ms: real): string {
        if (ms === 0) {
            return "now";
        }
        return ms % 60000 === 0 ? "-" + (ms / 60000) + " min" : "-" + (ms / 1000) + " s";
    }

    onPaint: {
        const ctx = getContext("2d");
        ctx.reset();
        ctx.clearRect(0, 0, width, height);

        const fontPx = Kirigami.Theme.smallFont.pixelSize;
        ctx.font = fontPx + "px " + Kirigami.Theme.smallFont.family;
        ctx.textBaseline = "middle";

        const labelMax = targetMaxMs;
        const maxMs = Math.max(scaleMaxMs, 1e-6);
        const decimals = labelMax < 10 ? 1 : 0;

        // Reserve a left gutter for the y-axis labels, a bottom strip for the
        // x-axis labels, and top padding so the top y label is not clipped.
        const padV = fontPx;
        const gutter = ctx.measureText(labelMax.toFixed(decimals) + " ms").width + Kirigami.Units.smallSpacing * 2;
        const xAxisHeight = fontPx + Kirigami.Units.smallSpacing;
        const plotLeft = gutter;
        const plotRight = width;
        const plotTop = padV / 2;
        const plotBottom = height - xAxisHeight - padV / 2;
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
            ctx.fillText((level * labelMax).toFixed(decimals) + " ms", gutter - Kirigami.Units.smallSpacing, y);
        }

        // Vertical gridlines and x-axis labels at fixed ages ("now" on the
        // right). The ticks stay put while the samples scroll past them. The
        // outermost labels are aligned inwards so they are never clipped, and
        // labels that would collide with the previously drawn one are skipped.
        const timeStep = niceTimeStep(windowMs);
        const labelY = height - xAxisHeight / 2;
        let lastLabelLeft = Infinity;
        for (let age = 0; age <= windowMs + 0.5; age += timeStep) {
            const x = plotRight - (age / windowMs) * plotWidth;
            ctx.strokeStyle = Qt.rgba(axisColor.r, axisColor.g, axisColor.b, 0.1);
            ctx.beginPath();
            ctx.moveTo(Math.round(x) - 0.5, plotTop);
            ctx.lineTo(Math.round(x) - 0.5, plotBottom);
            ctx.stroke();

            const label = formatAge(age);
            const labelWidth = ctx.measureText(label).width;
            let left = x - labelWidth / 2;
            left = Math.min(left, plotRight - labelWidth);
            left = Math.max(left, plotLeft);
            if (left + labelWidth + Kirigami.Units.smallSpacing > lastLabelLeft) {
                continue;
            }
            ctx.textAlign = "left";
            ctx.fillStyle = Qt.rgba(axisColor.r, axisColor.g, axisColor.b, 0.7);
            ctx.fillText(label, left, labelY);
            lastLabelLeft = left;
        }

        if (!pinger) {
            return;
        }
        const latencies = pinger.history;
        const times = pinger.historyTimes;
        const n = Math.min(latencies.length, times.length);
        const now = Date.now();

        if (!pinger.running && (n === 0 || now - times[n - 1] > windowMs)) {
            idle = true;
        }
        if (n === 0) {
            return;
        }

        const xOf = t => plotRight - ((now - t) / windowMs) * plotWidth;
        const yOf = v => plotBottom - Math.min(1, v / maxMs) * plotHeight;
        const sampleWidth = plotWidth / columns;

        // Clip to the plot area so points entering/leaving the window are cut
        // off cleanly at the axis instead of overlapping the labels.
        ctx.save();
        ctx.beginPath();
        ctx.rect(plotLeft, 0, plotWidth, plotBottom + padV / 2);
        ctx.clip();

        // Failed samples: faint full-height band.
        ctx.fillStyle = Qt.rgba(failColor.r, failColor.g, failColor.b, 0.3);
        for (let i = 0; i < n; ++i) {
            if (latencies[i] < 0) {
                ctx.fillRect(xOf(times[i]) - sampleWidth / 2, plotTop, sampleWidth, plotHeight);
            }
        }

        // Line segments connecting consecutive successful samples, with a soft
        // fill underneath.
        const segments = [];
        let current = [];
        for (let i = 0; i < n; ++i) {
            const value = latencies[i];
            const t = times[i];
            if (now - t > windowMs + maxConnectGapMs) {
                continue; // far outside the window, also for connecting lines
            }
            const prevT = i > 0 ? times[i - 1] : t;
            if (value < 0 || (current.length > 0 && t - prevT > maxConnectGapMs)) {
                if (current.length > 0) {
                    segments.push(current);
                }
                current = [];
            }
            if (value >= 0) {
                current.push({
                    x: xOf(t),
                    y: yOf(value)
                });
            }
        }
        if (current.length > 0) {
            segments.push(current);
        }

        ctx.lineJoin = "round";
        ctx.lineCap = "round";
        for (const seg of segments) {
            if (seg.length > 1) {
                ctx.beginPath();
                ctx.moveTo(seg[0].x, plotBottom);
                for (const p of seg) {
                    ctx.lineTo(p.x, p.y);
                }
                ctx.lineTo(seg[seg.length - 1].x, plotBottom);
                ctx.closePath();
                ctx.fillStyle = Qt.rgba(lineColor.r, lineColor.g, lineColor.b, 0.15);
                ctx.fill();

                ctx.beginPath();
                ctx.moveTo(seg[0].x, seg[0].y);
                for (let j = 1; j < seg.length; ++j) {
                    ctx.lineTo(seg[j].x, seg[j].y);
                }
                ctx.strokeStyle = lineColor;
                ctx.lineWidth = 1.5;
                ctx.stroke();
            }
        }

        // Sample points on top of the lines.
        const radius = Math.max(1.5, Math.min(3, sampleWidth / 3));
        ctx.fillStyle = lineColor;
        for (const seg of segments) {
            for (const p of seg) {
                ctx.beginPath();
                ctx.arc(p.x, p.y, radius, 0, 2 * Math.PI);
                ctx.fill();
            }
        }

        ctx.restore();
    }
}
