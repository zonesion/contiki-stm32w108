CONTIKI_PROJECT=slip-radio
all: $(CONTIKI_PROJECT)

CONTIKI=../../..

WITH_UIP6=1
UIP_CONF_IPV6=1
UIP_CONF_RPL=1

#linker optimizations
SMALL=1

CFLAGS += -DPROJECT_CONF_H=\"project-conf.h\"
PROJECT_SOURCEFILES += slip-net.c slip-sicslowpan.c 

include $(CONTIKI)/Makefile.include
