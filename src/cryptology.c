/*
*FILE		:	cryptology.c
*PROJECT	:	PROGR1970 - SP::A-01 - The cryptoMagic Utility
*PROGRAMMER	:	Andrii Dushkevych
*FIRST VERSION	:	2018-01-28
*DESCRIPTION	:	
*This file contains implemented methods for cryptomagic utility.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../inc/cryptology.h"


/*
* Function: int parseInput(int argc, char* argv[], char* basefileName, char* fullFileName)
*
* Description: This function takes commant prompt parameters and parses those
*
* Parameter: int argc: number of command prompt params
* Parameter: char* argv[]: pointer at array of command prompt params
* Parameter: char* basefileName: base filename to return it in main function
* Parameter: char* fullFileName: full filename to return it in main function
* Return Value: int mode(encryption/decryption/error) value
*/

int parseInput(int argc, char* argv[], char* basefileName, char* fullFileName)
{
	int returnCode = INVALID;
	int fileNamePos = 0;

	if ( argc == PARAMS_DEFAULT_ENCR )
	{
		fileNamePos = 1; //assuming that we provide 2 arguments, executable name will 
				//always be first so filename would be second
				//if second is invalid file name or any other thing it will give 
				//file reading error
		returnCode = ENCR;
		strcpy(fullFileName, argv[fileNamePos]);

		if ( strrchr(argv[fileNamePos], '.') != NULL )
		{
			char* dotPos = strrchr(argv[fileNamePos], '.');
			strcpy(dotPos, "\0");
		}
		strcpy(basefileName, argv[fileNamePos]);
	}

	if ( argc == PARAMS_WITH_KEY )
	{
		for ( int i = 0; i < argc; i++)
		{
			if ( argv[i][0] == '-') 
			{
				if ( strcmp( argv[i], "-encrypt") == 0 )
				{
					returnCode = ENCR;
				}
				else if ( strcmp( argv[i], "-decrypt") == 0 )
				{
					returnCode = DECR;
				}
			}
			else
			{
				if ( fileNamePos == 0 )	
				{
					fileNamePos = i;
				}
			}
		}
		if ( returnCode != INVALID )
		{
			if ( strrchr(argv[fileNamePos], '.') != NULL )
			{
				strcpy(fullFileName, argv[fileNamePos]);
				char* dotPos = strrchr(argv[fileNamePos], '.');
				strcpy(dotPos, "\0");
			}
			strcpy(basefileName, argv[fileNamePos]);
		}
	}	
	return returnCode;
}

/*
* Function: void encryption(char* basefileName, char* fullFileName)
*
* Description: This function takes file name to encrypt its content and write it back to a
* basefilename.crp
*
* Parameter: char* basefileName: base filename to create .crp file
* Parameter: char* fullFileName: full filename write encrypted contents
* Return Value: nothing
*/

void encryption(char* basefileName, char* fullFileName)		
{
  	FILE* sourceFilePtr = NULL;
	FILE* destFilePtr = NULL;
	char lineFromSource[TEXT_LINE_SIZE];
	char* outputFile = strcat(basefileName, ".crp");

	sourceFilePtr = fopen(fullFileName, "r");
	
	if (sourceFilePtr)
	{
		destFilePtr = fopen(outputFile, "w");
		if (destFilePtr)
		{
			while (fgets(lineFromSource, TEXT_LINE_SIZE, sourceFilePtr) != NULL)
			{
				for (int i = 0; i < strlen(lineFromSource); i++)
				{
					if (lineFromSource[i] == '\n')
					{
						fprintf(destFilePtr, "\n");
					}
					else if (lineFromSource[i] == 9) // tab char
					{
						fprintf(destFilePtr, "TT");
					}
					else
					{
						int outChar = (lineFromSource[i] - 16);
						if (outChar < 32)
						{
							outChar = (outChar - 32) + 144;
						}
						fprintf(destFilePtr, "%02X", outChar);
					}
				}
			}
			fclose(destFilePtr);
		}
		fclose(sourceFilePtr);
	}
	else
	{
		printError(READ_FILE_ERROR, fullFileName);
	}
}

/*
* Function: void decryption(char* basefileName, char* fullFileName)
*
* Description: This function takes file name to decrypt its content and write it back to a
* basefilename.txt
*
* Parameter: char* basefileName: base filename to create .txt file
* Parameter: char* fullFileName: full filename write decrypted contents
* Return Value: nothing
*/

void decryption(char* basefileName, char* fullFileName)			
{
	FILE* sourceFilePtr = NULL;
	FILE* destFilePtr = NULL;
	char lineFromSource[CRYPT_LINE_SIZE];
	char* outputFile = strcat(basefileName, ".txt");

	sourceFilePtr = fopen(fullFileName, "r");
	
	if (sourceFilePtr)
	{
		destFilePtr = fopen(outputFile, "w");
		if (destFilePtr)
		{
			while (fgets(lineFromSource, CRYPT_LINE_SIZE, sourceFilePtr) != NULL)
			{
				for (int i = 0; i < strlen(lineFromSource); i=i+2)
				{
					if (lineFromSource[i] == '\n')
					{
						fputc('\n', destFilePtr);
						i--;
					}
					else if ((lineFromSource[i] == 'T') && (lineFromSource[i+1] == 'T')) // tab char
					{
						fputc('\t', destFilePtr);
					}
					else
					{
						int first = -1;
						int second = -1;
						int outChar = 0;

						if (isInAtoF(lineFromSource[i]) == 1)
						{
							first = transformChar(lineFromSource[i]);
						}
						else if (first == -1)
						{
							first = lineFromSource[i] - '0';
						}

						if (isInAtoF(lineFromSource[i+1]) == 1)
						{
							second = transformChar(lineFromSource[i+1]);
						}						
						else if (second == -1)
						{
							second = lineFromSource[i+1] - '0';
						}

						outChar = ((first * 16) + second + 16);
						if (outChar > 127)
						{
							outChar = (outChar - 144) + 32;
						}
						fputc(outChar, destFilePtr);
					}
				}
			}
			fclose(destFilePtr);
		}
		fclose(sourceFilePtr);
	}
	else
	{
		printError(READ_FILE_ERROR, fullFileName);
	}
}

/*
* Function: void printError(int errorCode, char* fullFileName)
*
* Description: This function takes file name and error code to write command window error
* message
*
* Parameter: int errorCode: error code int value
* Parameter: char* fullFileName: full filename to provide error info
* Return Value: nothing
*/

void printError(int errorCode, char* fullFileName)
{
	switch (errorCode)
	{
		case INPUT_ERROR :
			printf("INPUT ERROR! Expected input: ./cryptoMagic [-encrypt/-decrypt] filename\n");
			break;
		case READ_FILE_ERROR :
			printf("FILE READING ERROR! FILE WITH PROVIDED NAME (%s) NOT FOUND! \n", fullFileName);
			break;
		default :
			printf("ERROR! \n");
			break;
	}
}

/*
* Function: int transformChar(char ch)
*
* Description: This function takes char in A-F range and returns its numeric equivalent
*
* Parameter: char ch: char A-F
* Return Value: int value
*/

int transformChar(char ch)
{
	int result = 10;
	switch (ch)
	{
		case 'A' :
			result = 10;
			break;
		case 'B' :
			result = 11;
			break;
		case 'C' :
			result = 12;
			break;
		case 'D' :
			result = 13;
			break;
		case 'E' :
			result = 14;
			break;
		case 'F' :
			result = 15;
			break;
		default :
			break;
	}
	return result;
}

/*
* Function: int isInAtoF (char ch)
*
* Description: This function takes char and defines if it's value is in "ABCDEF" array
*
* Parameter: char ch: char
* Return Value: int 1 - true, 0 - false
*/

int isInAtoF (char ch)
{
	int i;
	char* arr = "ABCDEF";
	for (i = 0; i < 6; i++)
	{
		if (arr[i] == ch) 
		{
			return 1;
		}
	}
	return 0;
}
