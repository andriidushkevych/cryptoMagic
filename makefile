# =======================================================
#            cryptoMagic Andrii Dushkevych
# =======================================================
# FINAL BINARY Target
./bin/myApp : ./obj/cryptoMagic.o ./obj/cryptology.o
	cc ./obj/cryptoMagic.o ./obj/cryptology.o -o ./bin/cryptoMagic
#
# =======================================================
#                     Dependencies
# =======================================================                     
./obj/cryptoMagic.o : ./src/cryptoMagic.c ./inc/cryptology.h
	cc -c ./src/cryptoMagic.c -o ./obj/cryptoMagic.o

./obj/cryptology.o : ./src/cryptology.c ./inc/cryptology.h
	cc -c ./src/cryptology.c -o ./obj/cryptology.o


#
# =======================================================
# Other targets
# =======================================================                     
all : ./bin/cryptoMagic

clean:
	rm -f ./bin/*
	rm -f ./obj/*.o
	rm -f ./inc/*.h~
	rm -f ./src/*.c~
