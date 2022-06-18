CC=gcc
AFLAGS=-Wall -g
LDFLAGS= -lsqlite3
OBJS= capture1.cgi picture1.cgi picture2.cgi zigbee_beep.cgi zigbee_rely.cgi zigbee_led.cgi regist.cgi\
env1.cgi env2.cgi login.cgi delete.cgi

run:$(OBJS)

$(OBJS):%.cgi:%.c
	$(CC) $(AFLAGS) $< cgic.c -o $@ $(LDFLAGS)
	mv *.cgi ./out/

.PHONY:clean
clean:
	rm ./out/*.cgi

