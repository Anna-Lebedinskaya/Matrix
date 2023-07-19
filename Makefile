.PHONY : all s21_matrix_oop.a test gcov_report leaks cpplint cppcheck clean 

CPP = g++ -std=c++17 -lstdc++
FLAGS = -pedantic-errors -Wall -Werror -Wextra
COV_FALGS = -fprofile-arcs -ftest-coverage

OS= $(shell uname)

ifeq ($(OS), Linux)
	OPEN = xdg-open
	LIBS = -lgtest -pthread --coverage
	LEAKS = valgrind --track-origins=yes --leak-check=full  ./test
endif

ifeq ($(OS), Darwin)
	OPEN = open
	LIBS = -lgtest -L /usr/local/lib --coverage
	LEAKS = leaks --atExit -- ./test
endif


all: gcov_report

s21_matrix_oop.a: clean s21_matrix_oop.cpp
	$(CPP) -c s21_matrix_oop.cpp
	ar -rcs s21_matrix_oop.a s21_matrix_oop.o

test: clean
	$(CPP)  s21_matrix_oop_test.cpp s21_matrix_oop.cpp $(LIBS) $(FLAGS) $(COV_FALGS) -o test
	./test

gcov_report: clean test 
	lcov --no-external -t "test_matrix" -o test_matrix.info -c -d .
	genhtml -o report test_matrix.info
	@$(OPEN) ./report/index.html

leaks:
	$(LEAKS)

cpplint:
	@cp ../materials/linters/.clang-format .
	clang-format -i *.cpp *.h
	clang-format -n *.cpp *.h --verbose
	@rm .clang-format

cppcheck:
	@cppcheck --language=c++ --enable=all --suppress=missingIncludeSystem s21_matrix_oop.cpp s21_matrix_oop_test.cpp s21_matrix_oop.h

clean:
	-@rm -f *.out *.a test *.o *.gcno *.gcda *.info
	-@rm -rf ./report
