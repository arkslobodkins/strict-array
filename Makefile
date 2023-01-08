all: example

CXX = g++-12.2
CXXFLAGS = -O3 -std=gnu++20
LFLAGS = -lquadmath -lm

example: example.cpp
	$(CXX) $(CXXFLAGS) example.cpp -o example.x $(LFLAGS)

clean:
	rm -rf example.x

