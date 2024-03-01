export CGO_CXXFLAGS=$(pkg-config --cflags Qt6Core)
export CGO_LDFLAGS=$(pkg-config --libs Qt6Core)
