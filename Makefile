include param.mk

SRCDIR=src/
CDCDIR=cdc/

all :
	cd $(SRCDIR) && make

clean :
	cd $(SRCDIR) && make clean

rec :
	cd $(SRCDIR) && make rec

cdc :
	cd $(CDCDIR) && make view > /dev/null 2>&1 &

.PHONY:all clean rec cdc

