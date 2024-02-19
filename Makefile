all: heap usb-helper

usb-helper: usb-helper.c heap.c
	gcc usb-helper.c -o usb-helper

heap: heap.c
	gcc heap.c -o heap
