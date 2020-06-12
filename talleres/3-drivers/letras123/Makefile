obj-m := letras123.o
KVERSION := $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) modules
	insmod letras123.ko

clean:
	rmmod letras123.ko
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) clean
