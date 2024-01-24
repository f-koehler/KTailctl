export CGO_CXXFLAGS=$(pkg-config --cflags Qt5Core)
export CGO_LDFLAGS=$(pkg-config --libs Qt5Core)
