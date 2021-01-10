
CC = gcc

CFLGAS += -std=c99
CFLAGS += -D TEST
CFLAGS += -g

OBJ_PATH = .
BIN_PATH = .
SRC_PATH = .

IDIR = .
INC += -I$(IDIR)

TARGET = $(BIN_PATH)/test 

C_SRCS += ./examples/test_linux.c
C_SRCS += ./multi_timer.c
OBJ := $(patsubst %.c,%.o,$(filter %.c,$(C_SRCS)))


$(TARGET) : $(OBJ_PATH)/$(OBJ)
	$(CC) $(INC) $(CFLAGS)  $(OBJ)  -o $(TARGET)

$(OBJ): %.o : %.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

.PHONY : clean
clean:
	@-rm *.exe
	@-rm *.o
