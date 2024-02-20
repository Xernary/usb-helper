all: usb-helper 

usb-helper: usb-helper.c 
	gcc -o usb-helper map.c requests.c constants.c usb-helper.c 

map: map.h

requests: requests.h

constants: constants.h
