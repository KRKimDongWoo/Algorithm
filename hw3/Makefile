OBJ = main.c hw3_c.h
file: $(OBJ)
	gcc -DFILE_INPUT_ENABLED -DINPUT_FILE='"./inputs/in1.txt"' -DOUTPUT_FILE='"./inputs/out1.txt"' -o run $(OBJ)
	./run
	gcc -DFILE_INPUT_ENABLED -DINPUT_FILE='"./inputs/in2.txt"' -DOUTPUT_FILE='"./inputs/out2.txt"' -o run $(OBJ)
	./run
	gcc -DFILE_INPUT_ENABLED -DINPUT_FILE='"./inputs/in3.txt"' -DOUTPUT_FILE='"./inputs/out3.txt"' -o run $(OBJ)
	./run
	gcc -DFILE_INPUT_ENABLED -DINPUT_FILE='"./inputs/in4.txt"' -DOUTPUT_FILE='"./inputs/out4.txt"' -o run $(OBJ)
	./run
	gcc -DFILE_INPUT_ENABLED -DINPUT_FILE='"./inputs/max_1000.txt"' -DOUTPUT_FILE='"./inputs/max_1000_out.txt"' -o run $(OBJ)
	./run
	gcc -DFILE_INPUT_ENABLED -DINPUT_FILE='"./inputs/min_1000.txt"' -DOUTPUT_FILE='"./inputs/min_1000_out.txt"' -o run $(OBJ)
	./run
	gcc -DFILE_INPUT_ENABLED -DINPUT_FILE='"./inputs/cycle_1000.txt"' -DOUTPUT_FILE='"./inputs/cycle_1000_out.txt"' -o run $(OBJ)
	./run	
	rm ./run -rf

std: $(OBJ)
	gcc -o run $(OBJ)
	./run
	rm ./run -rf
