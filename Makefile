SRC_DIR = src
BIN_DIR = bin

CC = clang # compiler
LIB_INCLUDE = -I/usr/local/include # path to headers
LIB_PATH = -L/usr/local/lib # path to libraries
CFLAGS =-g -Wall -Wextra $(LIB_INCLUDE) # compiler flags
TARGET = main # output file
SOURCE_FILE = $(SRC_DIR)/main.c # source file
SOURCES = $(SRC_DIR)/Error.c $(SRC_DIR)/Parser.c # sources
LIB_LEXICAL_ANALIZER = -llexicalAnalyzer
LIB_CJSON = -lcjson

LIBS_ALL = $(LIB_PATH) $(LIB_LEXICAL_ANALIZER) $(LIB_CJSON)

OUTPUT_FILE = $(BIN_DIR)/$(TARGET)

$(info Compile Project with: $(CC))
$(info OUTPUT_FILE: $(OUTPUT_FILE))

all: clean $(OUTPUT_FILE)

$(OUTPUT_FILE): $(SOURCE_FILE) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(OUTPUT_FILE) $(SOURCES) $(SOURCE_FILE) $(LIBS_ALL)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

build_ast:
	$(CC) $(CFLAGS) -o $(BIN_DIR)/ast $(SOURCES) $(SRC_DIR)/Ast.c $(LIBS_ALL)

clean:
	rm -rf $(BIN_DIR)/$(TARGET)

cleanCache:
	sudo ldconfig