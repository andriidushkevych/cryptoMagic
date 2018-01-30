/*
*FILE		:	cryptoMagic.c
*PROJECT	:	PROGR1970 - SP::A-01 - The cryptoMagic Utility
*PROGRAMMER	:	Andrii Dushkevych
*FIRST VERSION	:	2018-01-28
*DESCRIPTION	:	
*This file contains main method for cryptomagic utility.
*/
#include <stdio.h>
#include <stdlib.h>

#include "../inc/cryptology.h"

void main(int argc, char* argv[])
{
	char basefileName[500] = ""; //assume filename + path
	char fullFileName[510] = "";
	int mode = parseInput(argc, argv, basefileName, fullFileName);

	if (mode == INVALID)
	{
		printError(INPUT_ERROR, NULL);
	}
	else
	{
		if (mode == ENCR)
		{
			encryption(basefileName, fullFileName);
		}

		if (mode == DECR)
		{
			decryption(basefileName, fullFileName);
		}
	}
}
