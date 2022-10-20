.PHONY: clean
CC = g++
RM = rm
EXE = main
OBJS = \
	main.o \
	windowing.o\
	base.o\

# teaport.o 
#   main.o
#   test.o

run: main
	./main nilu.obj

$(EXE): $(OBJS)
	$(CC) -o $@ $^ -lGL -lGLU -lglut
%.o: %.c
	$(CC) -o $@ -c $^
%.o: %.cpp
	$(CC) -o $@ -c $^
clean:
	$(RM) $(EXE) $(OBJS)