all: usb-helper 

usb-helper: usb-helper.c 
	gcc -o usb-helper map.c requests.c mounts.c constants.c usb-helper.c 

map: map.h

requests: requests.h

mounts: mounts.h

constants: constants.h
