all:
	mkdir -p build
	g++ -std=c++11 -o build/main src/main.cpp


clean:
	rm -rf build