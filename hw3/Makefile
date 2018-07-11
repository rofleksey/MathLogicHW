CXX = g++
CXXFLAGS+=-std=c++14 -O2
all: out out/project.o

run: out out/project.o
	./out/project.o

out/project.o: out/main.o out/parser.o out/deduction.o
	$(CXX) -Isrc out/main.o out/parser.o out/deduction.o $(CXXFLAGS) -o out/project.o
	
out/main.o: src/main.cpp src/parser.h src/deduction.h src/expression.h src/util.h
	$(CXX) src/main.cpp -c $(CXXFLAGS) -o out/main.o
	
out/parser.o: src/parser.cpp src/parser.h src/expression.h src/util.h
	$(CXX) src/parser.cpp -c $(CXXFLAGS) -o out/parser.o
	
out/deduction.o: src/deduction.cpp src/deduction.h src/parser.h src/util.h
	$(CXX) src/deduction.cpp -c $(CXXFLAGS) -o  out/deduction.o

pack:
	zip hw3.zip -r Makefile src proves

out:
	mkdir -p out

clean:
	rm -rf out hw3.zip output.txt
