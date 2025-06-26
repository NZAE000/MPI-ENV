BIN 	:= exec
CC  	:= mpic++
CCGLAGS := -std=c++20 -g
INCLUDE := -I /opt/homebrew/Cellar/boost/1.88.0/include
LIBPATH := -L /opt/homebrew/Cellar/boost-mpi/1.88.0/lib -L /opt/homebrew/Cellar/boost/1.88.0/lib
LIBS 	:= -lboost_mpi -lboost_serialization
SRCPATH	:= bmpi-hello#default
SOURCE	:= main.cpp
BINPATH := bin

.PHONY : clean

$(BIN) :
	$(CC) $(CCGLAGS) $(SRCPATH)/$(SOURCE) $(INCLUDE) $(LIBPATH) $(LIBS) -o $(BINPATH)/$(BIN)

clean:
	rm $(BINPATH)/*
