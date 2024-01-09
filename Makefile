APPS = ./apps
BIN = ./bin
INCLUDE = ./include
LIB = ./lib
OBJ = ./obj
SRC = ./src
FLAGS = -O3 -Wall
LIBS = -led -L $(LIB)

DIR = C:\Users\Admin\Desktop\Developer\ws-apc-remake\after

all: libed myapps

libed: $(OBJ)/worklib.o 
	ar -rcs $(LIB)/libed.a $(OBJ)/*.*

myapps: $(BIN)/app

run: 
	$(BIN)/app.exe

$(OBJ)/%.o: $(SRC)/%.c $(INCLUDE)/%.h
	gcc $(FLAGS) -c $< -I $(INCLUDE) -o $@

$(BIN)/%: $(APPS)/%.c
	gcc $(FLAGS) $< $(LIBS) -I $(INCLUDE) -o $@

clean: cleanapps
	del /q "$(DIR)\obj\*.*"
	del /q "$(DIR)\bin\*.*"
	del /q "$(DIR)\lib\*.*"

cleanapps:
	del /q "$(DIR)\bin\*.*"

