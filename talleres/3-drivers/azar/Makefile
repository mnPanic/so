obj-m := azar.o
KVERSION := $(shell uname -r)

all:
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) modules
	insmod azar.ko

clean:
	rmmod azar.ko
	make -C /lib/modules/$(KVERSION)/build SUBDIRS=$(shell pwd) clean
