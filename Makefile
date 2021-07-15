
CC = gcc

DEL = rm -rf

CFLGAS += -std=c99
CFLAGS += -D TEST
CFLAGS += -g

OBJ_PATH = .
BIN_PATH = .
SRC_PATH = .

IDIR = .
INC += -I$(IDIR)

TARGET = $(BIN_PATH)/test 

C_SRCS += examples/test_linux.c
C_SRCS += multi_timer.c
OBJ := $(patsubst %.c,%.o,$(filter %.c,$(addprefix $(SRC_PATH)/,$(C_SRCS))))
TARGET_OBJ := $(addprefix $(OBJ_PATH)/,$(OBJ))

$(warning "TARGET_OBJ=$(TARGET_OBJ)")

.PHONY : clean all

all: $(TARGET)

$(TARGET) : $(TARGET_OBJ)
	$(CC) $(INC) $(CFLAGS)  $^  -o $@

$(TARGET_OBJ): %.o : %.c
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	-$(DEL) $(OBJ_PATH)/$(OBJ)
	-$(DEL) $(TARGET)
