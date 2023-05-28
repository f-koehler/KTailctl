# Mac Build

Launch from project root:

```bash
DYLD_LIBRARY_PATH=$PWD/build/tailctlpp:$PWD/lib/prefix/lib/qml/org/kde/kirigami.2/:$DYLD_LIBRARY_PATH QML2_IMPORT_PATH=lib/prefix/lib/qml/ build/bin/ktailctl.app/Contents/MacOS/ktailctl
```
