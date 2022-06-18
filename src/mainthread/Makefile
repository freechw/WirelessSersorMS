CC=gcc
CFLAGS1=-Wall -c -g 
CFLAGS2= -lpthread
OBJS=main.o data_global.o pthread_zigbee_rcv.o  link_list.o pthread_main.o pthread_camera.o  pthread_refresh.o  pthread_uart_send.o uart_cache.o 

zigbee :$(OBJS)
	$(CC) -o $@ $^ $(CFLAGS2)
$(OBJS):%.o:%.c
	$(CC) $(CFLAGS1) $< -o $@

.PHONY:clean
clean:
	rm *.o zigbee

