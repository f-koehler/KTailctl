# KTailctl — Review TODOs

Review of `src/` and the build system (starting at top-level `CMakeLists.txt`). Items marked **[fixed]** were applied as part of this review; everything else is still open.
## Code Quality

- `src/tailscale/status/peer_status.cpp:33,41` — `mTaildropTargetStatus` and `mKeyExpiry` are left unparsed with `// TODO` comments; they're declared as properties, so QML binding expressions will never see real values.
- `src/ui/Main.qml` — pages are referenced as `qrc:/ui/pages/*.qml` through `ui.qrc`. With `ecm_add_qml_module`/`ecm_target_qml_sources` already set up, they should be registered via the QML module and used by type name instead.

## Missing Features

- `src/ui/pages/Settings.qml` — several `FormSwitchDelegate`s have only `text:` and no `checked`/`onCheckedChanged`: "Accept routes", "Advertise app connector", "Stateful filtering", "Netfilter mode" (combo), "Report device posture", "Enable management web interface", "Automatic update check", "Automatic updates", the "Relay server port" spin box, and the relay server endpoint repeater + "Add new" field. These are visual placeholders only.
- `src/ui/pages/LoginProfileList.qml` — the "Switch to this account" `ToolButton` has no `onClicked` handler; the button is a no-op.
- `src/ui/pages/LoginProfileInfo.qml:42` — `textName.focus = false` does not actually un-focus the `FormTextFieldDelegate`. A focus-sink item receiving `forceActiveFocus()` is the working pattern.
- `src/main.cpp:104` — middle-click on the tray icon has `// TODO(fk): toggle tailscale`; should call `tailscale->toggle()`.
- `src/tailscale/wrapper` — `logging.go.in` / Go wrapper callbacks: confirm `callbacks.go` is actually used from C++ (referenced as a dependency but not checked in this review).
