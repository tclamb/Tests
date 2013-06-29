SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .cpp

UNAME_S = $(shell uname -s)

TESTS = Random
SOURCES = $(TESTS:=.cpp)

ifeq ($(UNAME_S),Darwin)
	CFLAGS = -std=c++11 -stdlib=libc++ -O3
	FRAMEWORKS = 
	LDFLAGS = $(FRAMEWORKS) -lc++

	CC = clang++
endif

ifeq ($(UNAME_S),Linux)
	CFLAGS = --std=c++11 -g -O3 -flto
	LIBS =
	LDFLAGS = $(LIBS) -flto

	CC = g++
endif

all: $(TESTS)

.cpp:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf $(TESTS)

wc:
	wc -l *.h *.cpp
