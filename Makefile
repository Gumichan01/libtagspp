

CC=g++
FLAGS=-Wall -g
HEADER_DIR=src/
SRC_DIR=src/
OBJ_DIR=src/
OBJS=$(OBJ_DIR)8859.o $(OBJ_DIR)flac.o $(OBJ_DIR)id3genres.o $(OBJ_DIR)id3v1.o \
$(OBJ_DIR)id3v2.o $(OBJ_DIR)m4a.o $(OBJ_DIR)tags.o $(OBJ_DIR)utf16.o \
$(OBJ_DIR)vorbis.o

MAIN_DIR=examples/
CODE_MAIN=$(MAIN_DIR)readtags.cpp
OBJ_MAIN=$(MAIN_DIR)readtags.o

EXE=rtags
LIB=libtags.a

all: $(LIB)

$(LIB): $(OBJS)
	ar rcs $@ $^

$(OBJ_DIR)%.o: $(OBJ_SRC)%.cpp
	$(CC) -c $< -o $@


$(EXE): $(OBJ_MAIN) $(OBJS)
	$(CC) $^ -o $@

$(OBJ_MAIN): $(CODE_MAIN)
	$(CC) -c $< -o $@ -I $(SRC_DIR)

clean:
	rm -rf $(OBJ_DIR)*.o

clear:
	rm -rf $(OBJ_DIR)*.o $(EXE) $(OBJ_MAIN) $(LIB)
