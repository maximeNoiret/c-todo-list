COMPILER = gcc
CFLAGS = -Wall -Werror -g -fsanitize=address,undefined
COMPILE_OBJECT = $(COMPILER) $(CFLAGS) -c $< -o $@

main: main.o task.o terminalManagement.o
	$(COMPILER) $(CFLAGS) -o $@ $^  -lsqlite3

main.o: main.c task.h
	$(COMPILE_OBJECT)

task.o: task.c task.h terminalManagement.h
	$(COMPILE_OBJECT)

terminalManagement.o: terminalManagement.c terminalManagement.h
	$(COMPILE_OBJECT)

clean:
	rm *.o main