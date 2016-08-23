

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

LIBTAG_SRC=$(SRC_DIR)libtagpp.cpp
LIBTAG_OBJ=$(OBJ_DIR)libtagpp.o

EXE=rtags
LIB=libtagspp.a

all: $(LIB)

$(LIB): $(OBJS) $(LIBTAG_OBJ)
	ar rcs $@ $^

$(OBJ_DIR)%.o: $(OBJ_SRC)%.cpp
	$(CC) -c $< -o $@ $(FLAGS)


$(EXE): $(OBJ_MAIN) $(OBJS) $(LIBTAG_OBJ)
	$(CC) $^ -o $@ $(FLAGS)

$(OBJ_MAIN): $(CODE_MAIN)
	$(CC) -c $< -o $@ -I $(SRC_DIR) $(FLAGS)


libtagpp.o: $(LIBTAG_OBJ)

$(LIBTAG_OBJ): $(LIBTAG_SRC) $(SRC_DIR)libtagpp.hpp
	$(CC) -c $< -o $@ -I $(SRC_DIR) $(FLAGS)

clean:
	rm -rf $(OBJ_DIR)*.o

clear:
	rm -rf $(OBJ_DIR)*.o $(EXE) $(OBJ_MAIN) $(LIB)
