# originally compiled with gcc version 6.2.0 20161005 

Flags = -W -Wreturn-type -pedantic -pedantic-errors -Wundef -std=c99 -Wall -Wextra -O -D_DEFAULT_SOURCE #-D_BSD_SOURCE
CPP = gcc


all : Makefile   main.o icmp_send.o icmp_receive_one.o utils.o
	$(CPP) $(Flags) -o mytr   main.o icmp_send.o icmp_receive_one.o utils.o

main.o : Makefile   main.c  icmp_send.h  icmp_receive_one.h utils.h
	$(CPP) -c $(Flags) main.c -o  main.o

icmp_send.o : Makefile   icmp_send.c icmp_send.h
	$(CPP) -c $(Flags) icmp_send.c -o icmp_send.o

icmp_receive_one.o : Makefile   icmp_receive_one.c icmp_receive_one.h
	$(CPP) -c $(Flags) icmp_receive_one.c -o icmp_receive_one.o

utils.o : Makefile    utils.c utils.h
	$(CPP) -c $(Flags) utils.c -o utils.o

clean :
	rm -f *.o

distclean : 
	rm -f *.o mytr
