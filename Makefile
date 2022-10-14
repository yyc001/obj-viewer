.PHONY: clean
CC = g++
RM = rm
EXE = main
OBJS = \
  main.o \
  test.o
$(EXE): $(OBJS)
	$(CC) -o $@ $^ -lGL -lGLU -lglut
%.o: %.c
	$(CC) -o $@ -c $^
%.o: %.cpp
	$(CC) -o $@ -c $^
clean:
	$(RM) $(EXE) $(OBJS)