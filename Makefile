objser = main_ser.o main_ser_fun.o

objcli = main_cli.o main_cli_fun.o

objlib = -lsqlite3 -lpthread

test: server client clean

server: $(objser)
	gcc -o ser $(objser) $(objlib)

client: $(objcli)
	gcc -o cli $(objcli) 

vpath %.c client server
vpath %.h client server

$(objser) : ser.h
$(objcli) : cli.h

.PATH : clean

clean :
	rm $(objser) $(objcli)
