CC        = gcc
MKDIR     = mkdir
CP        = cp
RM        = rm
PRINTF    = printf
CFLAGS    = -O3 -std=c11 -c -Wall -Wconversion -fPIC
SRC_DIR   = src
BIN_DIR   = bin
TEST_DIR  = test
BUILD_DIR = build

SOURCES = $(SRC_DIR)/util/regex.c \
          $(SRC_DIR)/array_list.c \
          $(SRC_DIR)/linked_list.c \
          $(SRC_DIR)/array_map.c  \
          $(SRC_DIR)/string.c
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.o, $(SOURCES))

TEST_SOURCES = $(TEST_DIR)/tests/util/regex.c \
               $(TEST_DIR)/tests/array_list.c \
               $(TEST_DIR)/tests/linked_list.c \
               $(TEST_DIR)/tests/array_map.c \
               $(TEST_DIR)/tests/string.c
TEST_OBJECTS = $(patsubst $(TEST_DIR)/%.c, $(TEST_DIR)/%.o, $(TEST_SOURCES))
TEST_EXE     = $(TEST_DIR)/test.sh
TEST_CFLAGS  = -O3 -std=c11 -Wall -L$(BUILD_DIR) -Wl,-rpath,$(abspath $(BUILD_DIR)) -lccomponents

LIB_CFLAGS = -Wall -shared -o
LIB_NAME   = libccomponents
ifeq ($(OS), Windows_NT)
    LIB = $(LIB_NAME).dll
	PRE_TESTING = $(CP) $(BUILD_DIR)/$(LIB) .
	POST_TESTING = $(RM) -f $(LIB)
else
    LIB = $(LIB_NAME).so
endif

# List of terminals that supports escape sequences
ESC_TERMS = xterm xterm-256color
ifeq ($(filter $(TERM), $(ESC_TERMS)),)
    MSG_BEG = @$(PRINTF) "[CComponents]
	MSG_END = "
	TEST_PARAMS = --noesc
else
    E_PUR   = \033[0;35m
    E_BLU   = \033[0;34m
    E_RED   = \033[0;31m
    E_B     = \033[1m
    E_0     = \033[0m
    E_0BLU  = $(E_0)$(E_BLU)
    MSG_BEG = @$(PRINTF) "$(E_PUR)$(E_B)[CComponents]$(E_0)
    MSG_END = "
endif

all: notest test

notest: headers $(LIB)

headers: $(BUILD_DIR)
	$(MSG_BEG)$(E_BLU)Copying CComponents $(E_B)header files$(E_0BLU):$(E_0)\n$(MSG_END)
	$(CP) $(SRC_DIR)/ccomponents.h $(BUILD_DIR)/
	$(CP) $(SRC_DIR)/foreach.h $(BUILD_DIR)/

$(BIN_DIR):
	$(MSG_BEG)$(E_BLU)Creating a $(E_B)bin$(E_0BLU) directory:$(E_0)\n$(MSG_END)
	$(MKDIR) $(BIN_DIR)
	$(MKDIR) $(BIN_DIR)/util

$(BUILD_DIR):
	$(MSG_BEG)$(E_BLU)Creating a $(E_B)build$(E_0BLU) directory:$(E_0)\n$(MSG_END)
	$(MKDIR) $(BUILD_DIR)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c $(BIN_DIR)
	$(MSG_BEG)$(E_BLU)Compiling a $(E_B)$<$(E_0BLU) source file:$(E_0)\n$(MSG_END)
	$(CC) $< $(CFLAGS) -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.c
	$(MSG_BEG)$(E_BLU)Building test $(E_B)$^$(E_0BLU):$(E_0)\n$(MSG_END)
	$(CC) $< $(TEST_CFLAGS) -o $@

test: $(TEST_OBJECTS) $(LIB)
	$(PRE_TESTING)
	$(MSG_BEG)$(E_RED)YOU CAN BUILD THIS LIBRARY WITHOUT TESTS BY RUNNING make WITH notest GOAL$(E_0)\n$(MSG_END)
	$(MSG_BEG)$(E_BLU)Running $(E_B)tests$(E_0BLU):$(E_0)\n$(MSG_END)
	./$(TEST_EXE) $(TEST_DIR)/tests $(TEST_PARAMS)
	$(POST_TESTING)

$(LIB): $(OBJECTS) $(BUILD_DIR)
	$(MSG_BEG)$(E_BLU)Building a $(E_B)library$(E_0BLU):$(E_0)\n$(MSG_END)
	$(CC) $(LIB_CFLAGS) $(BUILD_DIR)/$(LIB) $(OBJECTS)

clean:
	$(MSG_BEG)$(E_BLU)Recursive removing $(E_B)bin$(E_0BLU) directory:$(E_0)\n$(MSG_END)
	$(RM) -rf bin
	$(MSG_BEG)$(E_BLU)Removing $(E_B)tests$(E_0BLU) symlink:$(E_0)\n$(MSG_END)
	$(RM) -f $(OBJECTS_TST) $(TEST_OBJ)
	$(MSG_BEG)$(E_BLU)Recursive removing $(E_B)build$(E_0BLU) directory:$(E_0)\n$(MSG_END)
	$(RM) -rf build
	$(MSG_BEG)$(E_BLU)Removing $(E_B)tests$(E_0BLU):$(E_0)\n$(MSG_END)
	$(RM) -f $(TEST_OBJECTS)