all: heap usb-helper

usb-helper: usb-helper.c heap.h
	gcc -g usb-helper.c -o usb-helper

heap: heap.h
	gcc heap.h -o heap
