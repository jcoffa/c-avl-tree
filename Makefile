# all, clean, move, and echo are not filenames
.PHONY: all clean move echo

# Directories
SRC = src
HED = include
BIN = bin
VPATH := $(SRC):$(HED):$(BIN)

# Files
LIB = libavltree.so
SRCS := $(wildcard $(SRC)/*.c) Queue/src/Queue.c
HEDS := $(wildcard $(HED)/*.h) Queue/include/Queue.h
OBJS := $(addprefix $(BIN)/,$(notdir $(SRCS:%.c=%.o)))

# Compilation options
CFLAGS := -std=c99 -Wall -Wpedantic $(addprefix -I,$(SRC) $(HED) $(BIN) Queue/include) -O2


##############
# Make Rules #
##############

all: $(LIB) move

$(LIB): $(OBJS)
	gcc -g -shared $(OBJS) -o $(BIN)/$(LIB)

Queue.o: Queue/src/Queue.c Queue/include/Queue.h
	gcc $(CLFAGS) -c -fpic $< -o $(BIN)/$@

$(BIN)/%.o: src/%.c include/%.h
	gcc -g $(CFLAGS) -c -fpic $< -o $@


#############
# Utilities #
#############

clean:
	rm -f ../$(LIB) $(BIN)/*.o $(BIN)/$(LIB)

move:
	mv $(BIN)/$(LIB) ../

echo:
	@echo SRC = $(SRC)
	@echo HED = $(HED)
	@echo BIN = $(BIN)
	@echo
	@echo SRCS = $(SRCS)
	@echo HEDS = $(HEDS)
	@echo OBJS = $(OBJS)
	@echo
	@echo LIB = $(LIB)
	@echo
	@echo CLFAGS = $(CFLAGS)

