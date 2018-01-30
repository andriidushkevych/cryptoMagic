/*
*FILE		:	cryptology.h
*PROJECT	:	PROGR1970 - SP::A-01 - The cryptoMagic Utility
*PROGRAMMER	:	Andrii Dushkevych
*FIRST VERSION	:	2018-01-28
*DESCRIPTION	:	
*This file contains methods prototypes and defined constants for cryptoMagic utility
*/

int parseInput(int argc, char* argv[], char* basefileName, char* fullFileName);
void encryption(char* basefileName, char* fullFileName);
void decryption(char* basefileName, char* fullFileName);
void printError(int errorCode, char* fullFileName);
int transformChar(char ch);
int isInAtoF (char ch);

//modes

#define INVALID			-1
#define ENCR 			0
#define DECR 			1

//command line params number

#define PARAMS_DEFAULT_ENCR	2
#define PARAMS_WITH_KEY		3

//error codes

#define INPUT_ERROR		1
#define READ_FILE_ERROR		2

//other
#define TEXT_LINE_SIZE		121
#define CRYPT_LINE_SIZE		256
