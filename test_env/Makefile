main: hakka2.c api.h
	gcc hakka2.c -o main -O3 -std=c11 -w

validator/validator: validator/validator.cpp
	g++ validator/validator.cpp -o validator/validator -O3

.PHONY=run
run: main validator/validator
	./main < testdata/test.in | validator/validator
