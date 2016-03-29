INC=./includes
SRC=./src
OUT=./bin


$(OUT)/lab: $(OUT)/main.o $(OUT)/mode_std.o $(OUT)/mode_child.o $(OUT)/mode_posix.o $(OUT)/mode_kill.o $(OUT)/mode_pipe.o
	gcc -o $(OUT)/lab $(OUT)/main.o $(OUT)/mode_std.o $(OUT)/mode_child.o $(OUT)/mode_posix.o $(OUT)/mode_kill.o $(OUT)/mode_pipe.o

$(OUT)/main.o: $(SRC)/main.c $(INC)/funct.h
	gcc -I$(INC) -o $(OUT)/main.o -c $(SRC)/main.c

$(OUT)/mode_std.o: $(SRC)/mode_std.c $(INC)/funct.h
	gcc -I$(INC) -o $(OUT)/mode_std.o -c $(SRC)/mode_std.c

$(OUT)/mode_child.o: $(SRC)/mode_child.c $(INC)/funct.h
	gcc -I$(INC) -o $(OUT)/mode_child.o -c $(SRC)/mode_child.c

$(OUT)/mode_posix.o: $(SRC)/mode_posix.c $(INC)/funct.h
	gcc -I$(INC) -o $(OUT)/mode_posix.o -c $(SRC)/mode_posix.c

$(OUT)/mode_kill.o: $(SRC)/mode_kill.c $(INC)/funct.h
	gcc -I$(INC) -o $(OUT)/mode_kill.o -c $(SRC)/mode_kill.c

$(OUT)/mode_pipe.o: $(SRC)/mode_pipe.c $(INC)/funct.h
	gcc -I$(INC) -o $(OUT)/mode_pipe.o -c $(SRC)/mode_pipe.c


clean:
	rm -f $(OUT)/*.o $(OUT)/lab
