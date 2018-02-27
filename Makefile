# define libraries
LIB = -lGL -lGLU -lglut
#LIB2 = libim/libim.a jpeg/libjpeg.a

# define makefile targets
CC = g++ -Wall
BIN = AngryBricks

#all: $(LIB2) $(BIN)
all: $(BIN)

clean:
	/bin/rm -f $(BIN)

#jpeg/libjpeg.a:
#	cd jpeg;make;cd ..
#libim/libim.a:
#	cd libim;make;cd ..

AngryBricks: angryBricks.cpp timer.cpp
	$(CC) -o angryBricks angryBricks.cpp timer.cpp $(LIB)
