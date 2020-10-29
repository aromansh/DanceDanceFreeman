FILES=ddf.c input.c keystroke.c
BIN_NAME=ddf
LIB_EVDEV_FLAGS=-I/usr/include/libevdev-1.0/ -levdev
LIB_XDO_FLAGS=-lxdo

build:
	gcc ${FILES} -o ${BIN_NAME} ${LIB_EVDEV_FLAGS} ${LIB_XDO_FLAGS}
