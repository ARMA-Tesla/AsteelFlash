#include <rs232.h>
#include <cvintwrk.h>
#include <userint.h>
#include <tcpsupp.h>
#include "ARW.h"
#include <stdio.h>
#include <formatio.h>
#include <utility.h>

#pragma warning( push)
#pragma warning( disable: 4100 4127)

#define SONOMETER_TIMEOUT 	5000


#undef __FUNC__
#define __FUNC__	"{ARW}::Connect"
SElExceptionPtr Connect(SSonometerPtr me)
{

SElExceptionPtr	pexception = NULL;

	if (OpenComConfig (me->port, "", me->baudRate, 0, 8, 1, 512, 512) < 0) 
		EXCTHROW( SONOMETER_CONNECTION_FAILED, "Probléme Ouverture COM");

Error:
	EXCRETHROW( pexception);
}


#undef __FUNC__
#define __FUNC__	"{ARW}::Disconnect"
SElExceptionPtr Disconnect(SSonometerPtr me)
{

SElExceptionPtr	pexception = NULL;
CloseCom (me->port);

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{ARW}::Write"
SElExceptionPtr Write(SSonometerPtr me,const char* Query)
{
	SElExceptionPtr	pexception = NULL;
	unsigned char command[32];
	int size = 0; 
	int i;
	
	
memset(command, 0x00, 32);	
	
/* Start of Block Transfer */
command[0] = 0X02;

/* Device ID */
command[1] = 0X01;

/* Command Block */
command[2] = 0X43;

/* Command */
strcat(command, Query);

/* Query parameter: ? */
size = strlen(command);
command[size] = 0X3F;

/* Stop caracter */
size++;
command[size] = 0X03;

/* CheckSum */
size++;
command[size] = 0X00;

/* CR + LF */
size++;
command[size] = 0X0D;
size++;
command[size] = 0X0A;

	for(i=0;i<size+1;i++)
	{
		ComWrtByte (me->port, command[i]);
		Sleep(20);
	}

Sleep(700);
ReturnRS232Err ();

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{ARW}::Read"
SElExceptionPtr Read(SSonometerPtr me,const char* data)
{
	SElExceptionPtr	pexception = NULL;
	char read_buffer[1024];
	int  received;
	int i;
	
memset(read_buffer, 0x00, 1024);
received  = GetInQLen (me->port);
if (received)
	{
		for(i=0;i<received;i++)
			read_buffer[i] = ComRdByte (me->port);
		
		sprintf (data, "%s", read_buffer);
	}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{ARW}::SoundLevelMeter"
SElExceptionPtr SoundLevelMeter(SSonometerPtr me, int group, float *data, int *count_data, char *log)             
{
	SElExceptionPtr	pexception = NULL;
	char command[256];
	char read_buffer[1024];
	char query[32];
	char msg[1024+128];
	int received;
	int i,size;
		
	char trameUtile[512];
	char value[32];
	int start_index = 0;
	int end_index = 0;
	int last_index = 0;
	int index = 0;
	unsigned int count = 0;
	float fvalue;
	int longueur_trame_utile;
	
				
/* exemple 

Group 0: LAF, LAS, LAI, LBF, LBS, LBI, LCF, LCS, LCI, LZF, LZS, LZI
Group 1: LAFsd, LASsd, LAIsd, LBFsd, LBSsd, LBIsd, LCFsd, LCSsd, LCIsd, LZFsd, LZSsd, LZIsd
Group 2: LAsel, LBsel, LCsel, LZsel Group 3: LAe, LBe, LCe, LZe
Group4: LAFmax, LASmax, LAImax, LBFmax, LBSmax, LBImax, LCFmax, LCSmax, LCImax, LZFmax, LZSmax, LZImax
Group 5: LAFmin, LASmin, LAImin, LBFmin, LBSmin, LBImin, LCFmin, LCSmin, LCImin, LZFmin, LZSmin, LZImin
Group 6: LApeak, LBpeak, LCpeak, LZpeak
Group 7: LAeq, LBeq, LCeq, LZeq
Group 8: Percentage values and statistics of ten LN

----- REQUEST READ GROUP 7
02 01 43 44 53 4C 37 20 31 20 3F 03 21 0D 0A  

Start of Block Transfer : 02 
Device ID :  01 
Command Block : 43
Command : 44 53 4C 37 20 31 20     DSL[44 53 4C] + group[37 20] + Query parameter [31 20]
Query parameter: ? : 3F
Stop caracter : 03
CheckSum : 21
CR : 0D
LF : 0A

----- ANSWER READ GROUP 7
02 01 41 30 36 35 2E 30 2C 30 36 36 2E 32 2C 30 36 37 2E 30 2C 30 36 37 2E 32 03 6E 0D 0A  

Start of Block Transfer : 02 
Device ID :  01 
Command Block : 41
LAeq : 30 36 35 2E 30
;    : 2C
LBeq : 30 36 36 2E 32
;    : 2C
LCeq : 30 36 37 2E 30
;    : 2C
LZeq : 30 36 37 2E 32
Stop caracter : 03
CheckSum : 6E
CR : 0D
LF : 0A

*/

FlushInQ (me->port);

	
memset(command, 0x00, 32);	
	
/* Start of Block Transfer */
command[0] = 0X02;

/* Device ID */
command[1] = 0X01;

/* Command Block */
command[2] = 0X43;

/* Command */
sprintf(query, "DSL%d 1 ",group);
strcat(command, query);

/* Query parameter: ? */
size = strlen(command);
command[size] = 0X3F;

/* Stop caracter */
size++;
command[size] = 0X03;

/* CheckSum */
size++;
command[size] = 0X00;

/* CR + LF */
size++;
command[size] = 0X0D;
size++;
command[size] = 0X0A;

	for(i=0;i<size+1;i++)
	{
		ComWrtByte (me->port, command[i]);
		Sleep(20);
	}
ReturnRS232Err ();

Sleep(1200);

/* Read */
memset(read_buffer, 0x00, 1024);

received  = GetInQLen (me->port);

if (received)
	{
		for(i=0;i<received;i++)
			read_buffer[i] = ComRdByte (me->port);
		read_buffer[i] = 0;
		
		if (read_buffer [received-2] != 0X0D || read_buffer [received-1] != 0X0A)
		{
			sprintf(msg, "Read data failed \r\n bytes read : %s", read_buffer);
			EXCTHROW( SONOMETER_READ_FAILED, msg);		
		}
		else
		{
			for(i=0;i<received;i++)	
			{
				if (read_buffer [i] == 0X41)	
					start_index = i;

				if (read_buffer [i] == 0X03)	
					end_index = i;	
			}
						
			CopyString(trameUtile, 0, read_buffer, start_index+1, (end_index-(start_index+1)));	
			
			longueur_trame_utile = strlen (trameUtile);
			
			float * pArray = malloc ( 1 * sizeof ( float ));

			for(index=0;index<longueur_trame_utile+1;index++)
			{
				if (trameUtile [index] == 0X2C || index == longueur_trame_utile)
				{			
					if (count == 0)
					{
						last_index = index;
						CopyString(value, 0, trameUtile, 0, index);
						sscanf(value, "%f", &fvalue);
						pArray[count] = fvalue;
						count ++;
					}
					else
					{
						CopyString(value, 0, trameUtile, last_index+1, (index-(last_index+1)));
					    pArray = realloc(pArray, (count+1) * sizeof (*pArray));    
						sscanf(value, "%f", &fvalue);
						pArray[count] = fvalue;
						last_index = index;
						count ++;
					}
				}
			}
			*count_data = count;
			for(i=0;i<count;i++)
				data[i] = pArray[i];
		}
	}
else
	{		
		EXCTHROW( SONOMETER_READ_FAILED, "No data Read data received!");	
	}	
Error:
	EXCRETHROW( pexception);
}

#pragma warning( pop)
