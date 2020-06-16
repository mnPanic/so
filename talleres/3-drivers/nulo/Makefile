obj-m := nulo.o
KVERSION := $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) modules
	insmod nulo.ko

clean:
	rmmod nulo.ko
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) clean
