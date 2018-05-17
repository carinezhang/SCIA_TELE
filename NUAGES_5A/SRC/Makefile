
#---------------------------------------------------------
#makefile permettant la compilation du programme principal
#---------------------------------------------------------
.KEEP_STATE:

#definition des variables
CC=gcc
OBJ=../OBJ
EXE=../EXE
CCFLAGS=-std=c99 -Wall -g `pkg-config --cflags gtk+-2.0`
LDFLAGS=`pkg-config --libs gtk+-2.0`

#compilation
OBJ_FILES=compute.o main.o 
OBJ_AUTO_FILES = auto_compute.o 

all: imaProjet.exe imaProject_auto.exe

.c.o:
	${CC} ${CCFLAGS} -o $@ -c $<

imaProjet.exe:${OBJ_FILES}
	${CC} -o ${EXE}/$@ ${OBJ_FILES} ${LDFLAGS}

imaProject_auto.exe:${OBJ_AUTO_FILES}
	${CC} -o ${EXE}/$@ ${OBJ_AUTO_FILES} ${LDFLAGS}
	mv *.o ${OBJ}

clean:
	\rm -f ${OBJ}/*.o ../*/core ${EXE}/*.exe

install:
	cp *.h ${INC}

.SUFFIXES: .exe .o .c .h
