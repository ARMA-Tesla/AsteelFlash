#include <rs232.h>
#include <ansi_c.h>
#include <userint.h>
#include "IaiAxis.h"
#include <stdio.h>
#include <utility.h>
#include <formatio.h>

#pragma warning( push)
#pragma warning( disable: 4100 4127)

#define AXIS_TIMEOUT				500

#define AXIS_READ_MODE_NORMAL	0
#define AXIS_READ_MODE_STATUS	1

// Control codes for the Gateway axis

#define HEADER      0X3A       // Header function
#define LF      	0X0A      // Line feed
#define CR      	0X0D      // Carriage return
#define END      	0X00      // end string 


#define CR_CHAR			'\x0D'	// CR character
#define LF_CHAR			'\x0A'  // LF character

const unsigned char CRC7_POLY = 0x91;

//Chanement trame héxa en trame en float
int ChangeHexaToInt(unsigned char ucTab[9]) 
{   
	int i;
	int iNombre =0;
	unsigned char ucTabTemp[9];
	unsigned char ucTemp[2];
	
if (strlen(ucTab)<9)
{
sprintf(ucTabTemp,ucTab);
for (i=0; i<strlen(ucTab); i++)
{	
ucTabTemp[strlen(ucTab)-i-1]=ucTab[i];	
}
iNombre=0;
ucTemp[1]='\0';
for (i=0; i<strlen(ucTabTemp); i++)
{

	switch (ucTabTemp[i])
	{
		case 'A':
		iNombre=iNombre + 10 *  pow(16,i); 	
			break;
		case 'B':
		iNombre=iNombre + 11 *  pow(16,i); 		
			break;
		case 'C':
		iNombre=iNombre + 12 *  pow(16,i); 		
			break;
		case 'D':
		iNombre=iNombre + 13 *  pow(16,i); 	
			break;
		case 'E':
		iNombre=iNombre + 14 *  pow(16,i); 	
			break;
		case 'F':
		iNombre=iNombre + 15 *  pow(16,i); 	
		default:
		ucTemp[0]=ucTabTemp[i];
		
		
		iNombre=iNombre + atoi(ucTemp) * pow(16,i);
		
			break;
	}
}
}
	
return iNombre;	
	
}

void GetLRC(BYTE* pbData, int iCount, char *LRC)
{
int i;
char CharHex[2+1];
char String[12+1]; 
int Hex;
int Somme=0;
unsigned int iLRC;

for (i=0; i<iCount; i++)
	{
	sprintf (CharHex, "%c%c\0", pbData[i], pbData[i+1]);
	i++;
	sscanf(CharHex,"%X",&Hex);
	Somme = Somme + Hex;
	}
	iLRC = 0XFFFFFFFF - (Somme - 1);
	sprintf (String, "%X", iLRC);
	LRC[0] = String[6];
	LRC[1] = String[7];
}
#undef __FUNC__
#define __FUNC__	"{IaiAxis}::Initialize" 
SElExceptionPtr Initialize(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	char command[18];
	char Query[12];
	char slave_axis[5];
	char LRC[2];
	int i; 
	
// Ability Serial switch to Modbus 

/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '5';

/* Start address 4 bytes */
command[5] = '0';
command[6] = '4';
command[7] = '2';
command[8] = '7';

/* Changed data 4 bytes */
command[9] = 'F';
command[10] = 'F';
command[11] = '0';
command[12] = '0';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

ComWrt (me->port, command, StringLength (command));
Sleep(10);
ReturnRS232Err ();
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::PresentAlarm"  
SElExceptionPtr PresentAlarm(SaxisPtr me, int32_t axis, int32_t* status)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[12];
	char slave_axis[5];
	char LRC[2];
	int i;
	char Response[4+1];
	char read_buffer[512];
	int received;
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '3';

/* Start address 4 bytes */
command[5] = '9';
command[6] = '0';
command[7] = '0';
command[8] = '2';

/* Number of registers 4 bytes */
command[9] = '0';
command[10] = '0';
command[11] = '0';
command[12] = '1';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

FlushInQ (me->port);
FlushOutQ (me->port);

ComWrt (me->port, command, StringLength (command));
Sleep(20);
ReturnRS232Err ();

Sleep(20);

received  = GetInQLen (me->port);
ComRd (me->port, read_buffer, received);

	if (received >= 12)
	{
	Response[0] =  read_buffer[7];
	Response[1] =  read_buffer[8];
	Response[2] =  read_buffer[9];
	Response[3] =  read_buffer[10];
	Response[4] =  0;
	*status = ChangeHexaToInt(Response);
	}
	else 
	*status	= 0;

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::ControllerStatus"   
SElExceptionPtr ControllerStatus(SaxisPtr me, int32_t axis, int32_t* status)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[12];
	char slave_axis[5];
	char LRC[2];
	int i;
	char Response[2+1];
	char read_buffer[512];
	int received = 0;
	clock_t _start_test_clock;
	double   time;
	int file1;
	char sbuff[128];	
	
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '3';

/* Start address 4 bytes */
command[5] = '9';
command[6] = '0';
command[7] = '0';
command[8] = '5';

/* Number of registers 4 bytes */
command[9] = '0';
command[10] = '0';
command[11] = '0';
command[12] = '1';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

_start_test_clock =  clock();

FlushInQ (me->port);
FlushOutQ (me->port);

for(i=0;i<17;i++)
ComWrtByte (me->port, command[i]);

do
{
received  = GetInQLen (me->port);
time = (double)(clock()-_start_test_clock);
}while(received<15 && time < AXIS_TIMEOUT);

if (time>AXIS_TIMEOUT)
{
for(i=0;i<17;i++)
ComWrtByte (me->port, command[i]);	
}
for(i=0;i<received;i++)
	read_buffer[i] = ComRdByte (me->port);


if (read_buffer)
	{
	Response[0] =  read_buffer[9];
	Response[1] =  read_buffer[10];
	Response[2] =  0;
	}
*status = ChangeHexaToInt(Response);
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::CurrentPosition"   
SElExceptionPtr CurrentPosition(SaxisPtr me, int32_t axis, double* position)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[20];
	char slave_axis[5];
	char LRC[2+1];
	char LRC_To_Check[2+1];
	int i;
	char Response[4+1];
	int decimal;
	char read_buffer[512];
	int received = 0;
	clock_t _start_test_clock;
	double   time;
	int file1;
	char sbuff[128];
	
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '3';

/* Start address 4 bytes */
command[5] = '9';
command[6] = '0';
command[7] = '0';
command[8] = '0';

/* Number of registers 4 bytes */
command[9] = '0';
command[10] = '0';
command[11] = '0';
command[12] = '2';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

LRC[2] = 0X00;

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

FlushInQ (me->port);
FlushOutQ (me->port);

ComWrt (me->port, command, strlen(command));

Sleep(30);

received  = GetInQLen (me->port);
ComRd (me->port, read_buffer, received);

if(received == 19)
{
	if (read_buffer)
	{
	Response[0] =  read_buffer[11];
	Response[1] =  read_buffer[12];
	Response[2] =  read_buffer[13];
	Response[3] =  read_buffer[14];
	Response[4] =  0;
	decimal = ChangeHexaToInt(Response);
	*position = decimal * 0.01; //0.01mm precision
	}
}
else
{
	*position = 99.00;
}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::RequestPosition"   
SElExceptionPtr RequestPosition(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[20];
	char slave_axis[5];
	char LRC[2+1];
	char LRC_To_Check[2+1];
	int i;
	char Response[4+1];
	int decimal;
	char read_buffer[512];
	int received = 0;
	clock_t _start_test_clock;
	double   time;
	int file1;
	char sbuff[128];
	
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '3';

/* Start address 4 bytes */
command[5] = '9';
command[6] = '0';
command[7] = '0';
command[8] = '0';

/* Number of registers 4 bytes */
command[9] = '0';
command[10] = '0';
command[11] = '0';
command[12] = '2';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

LRC[2] = 0X00;

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

FlushInQ (me->port);
FlushOutQ (me->port);

ComWrt (me->port, command, strlen(command));

	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::ReadPosition"   
SElExceptionPtr ReadPosition(SaxisPtr me, int32_t axis, double* position)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[20];
	char slave_axis[5];
	char LRC[2+1];
	char LRC_To_Check[2+1];
	int i;
	char Response[4+1];
	int decimal;
	char read_buffer[512];
	int received = 0;
	clock_t _start_read;
	double   time;
	int file1;
	char sbuff[19];
	
	
/* wait for received bytes */
_start_read =  clock();
do
{
received  = GetInQLen (me->port);
time = (double)(clock()-_start_read);
}while(received<19 && time < 50);

ComRd (me->port, read_buffer, received);
	
if(received == 19)
{
		Response[0] =  read_buffer[11];
		Response[1] =  read_buffer[12];
		Response[2] =  read_buffer[13];
		Response[3] =  read_buffer[14];
		Response[4] =  0;
	decimal = ChangeHexaToInt(Response);
	*position = decimal * 0.01; //0.01mm precision
	
}
else
{
printf ("read_buffer = %s\r\n", read_buffer);
*position = 99.00;
}
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::EnableAxis" 
SElExceptionPtr EnableAxis(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[12];
	char slave_axis[5];
	char LRC[2];
	int i;
	char Response[4];
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '5';

/* Start address 4 bytes */
command[5] = '0';
command[6] = '4';
command[7] = '0';
command[8] = 'A';

/* Number of registers 4 bytes */
command[9] = '0';
command[10] = '0';
command[11] = '0';
command[12] = '0';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

ComWrt (me->port, command, StringLength (command));
Sleep(30);
ReturnRS232Err ();
	
Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::AlarmReset" 
SElExceptionPtr AlarmReset(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[12];
	char slave_axis[5];
	char LRC[2];
	int i;
	char Response[4];
	
/*  First time 01 05 04 07 FF 00 F0 --- Execute alarm reset */
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '5';

/* Start address 4 bytes */
command[5] = '0';
command[6] = '4';
command[7] = '0';
command[8] = '7';

/* Number of registers 4 bytes */
command[9] = 'F';
command[10] = 'F';
command[11] = '0';
command[12] = '0';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;
ComWrt (me->port, command, StringLength (command));
Sleep(10);
ReturnRS232Err ();

Sleep(10);

/*  Second time 01 05 04 07 00 00 EF --- Restore normal status */
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '5';

/* Start address 4 bytes */
command[5] = '0';
command[6] = '4';
command[7] = '0';
command[8] = '7';

/* Number of registers 4 bytes */
command[9] = '0';
command[10] = '0';
command[11] = '0';
command[12] = '0';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

ComWrt (me->port, command, StringLength (command));
Sleep(10);
ReturnRS232Err ();

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::HomeReturn"  
SElExceptionPtr HomeReturn(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[12];
	char slave_axis[5];
	char LRC[2];
	int i;
	char Response[4];
	
/*  First time: 01 05 04 0B 00 00 EB --- Set normal status */
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '5';

/* Start address 4 bytes */
command[5] = '0';
command[6] = '4';
command[7] = '0';
command[8] = 'B';

/* Number of registers 4 bytes */
command[9] = '0';
command[10] = '0';
command[11] = '0';
command[12] = '0';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

ComWrt (me->port, command, StringLength (command));
Sleep(30);
ReturnRS232Err ();

Sleep(20);

/*  Second time: 01 05 04 0B FF 00 EC --- Execute home return */
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '5';

/* Start address 4 bytes */
command[5] = '0';
command[6] = '4';
command[7] = '0';
command[8] = 'B';

/* Number of registers 4 bytes */
command[9] = 'F';
command[10] = 'F';
command[11] = '0';
command[12] = '0';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

ComWrt (me->port, command, StringLength (command));
Sleep(30);
ReturnRS232Err ();

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::StopAxis"  
SElExceptionPtr StopAxis(SaxisPtr me, int32_t axis)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[12];
	char slave_axis[5];
	char LRC[2];
	int i;
	char Response[4];
	
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '5';

/* Start address 4 bytes */
command[5] = '0';
command[6] = '4';
command[7] = '0';
command[8] = 'A';

/* Number of registers 4 bytes */
command[9] = 'F';
command[10] = 'F';
command[11] = '0';
command[12] = '0';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

ComWrt (me->port, command, StringLength (command));
Sleep(10);
ReturnRS232Err ();

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::MoveAxisPosition" 
SElExceptionPtr MoveAxisPosition(SaxisPtr me, int32_t axis, int32_t position)
{
	SElExceptionPtr	pexception = NULL;
	int32_t			wait = 0;
	char command[18];
	char Query[12];
	char slave_axis[5];
	char LRC[2];
	int i;
	char Response[4];
	char PositionHex[4+1];
	int file1;
	char sbuff[256];
	clock_t _start_read;
	clock_t _start_test_clock;
	double   time;
	int received;
	char read_buffer[512];
	char debug[512];


/* Writing to Registers POSR */
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '6';

/* Start address 4 bytes */ 
command[5] = '0';
command[6] = 'D';
command[7] = '0';
command[8] = '3';

/* Changed data 4 bytes */
sprintf(PositionHex, "%04X", position);
command[9] = PositionHex[0];
command[10] = PositionHex[1];
command[11] = PositionHex[2];
command[12] = PositionHex[3];

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

/*---------------------------------------------------------------*/
FlushInQ (me->port);
FlushOutQ (me->port);
memset(read_buffer, 0x00, 17);
/* wait for success command */
_start_test_clock =  clock();
do 
{
for(i=0;i<17;i++)
ComWrtByte (me->port, command[i]);

/* wait for received bytes */
_start_read =  clock();
do
{
received  = GetInQLen (me->port);
time = (double)(clock()-_start_read);
}while(received<17 && time < AXIS_TIMEOUT);

for(i=0;i<received;i++)
	read_buffer[i] = ComRdByte (me->port);
read_buffer[17] = 0;
time = (double)(clock()-_start_read);
} while( (strcmp(read_buffer, command)!=0) && (time < AXIS_TIMEOUT) );

if (time>=AXIS_TIMEOUT)
	EXCTHROW( AXIS_ERROR_TIMEOUT, "AXIS Timeouted");

/*---------------------------------------------------------------*/

/*  First time 01 05 04 0C FF 00 4D 09 --- Move to the specified position */
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '5';

/* Start address 4 bytes */
command[5] = '0';
command[6] = '4';
command[7] = '0';
command[8] = 'C';

/* Number of registers 4 bytes */
command[9] = 'F';
command[10] = 'F';
command[11] = '0';
command[12] = '0';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

/*---------------------------------------------------------------*/
FlushInQ (me->port);
FlushOutQ (me->port);
memset(read_buffer, 0x00, 17);
/* wait for success command */
_start_test_clock =  clock();
do 
{
for(i=0;i<17;i++)
ComWrtByte (me->port, command[i]);

/* wait for received bytes */
_start_read =  clock();
do
{
received  = GetInQLen (me->port);
time = (double)(clock()-_start_read);
}while(received<17 && time < AXIS_TIMEOUT);

for(i=0;i<received;i++)
	read_buffer[i] = ComRdByte (me->port);
read_buffer[17] = 0;
time = (double)(clock()-_start_read);
} while( (strcmp(read_buffer, command)!=0) && (time < AXIS_TIMEOUT) );

if (time>=AXIS_TIMEOUT)
	EXCTHROW( AXIS_ERROR_TIMEOUT, "AXIS Timeouted");

/*---------------------------------------------------------------*/


/* Second time 01 05 04 0C 00 00 0C F9 --- Restore to the normal status */
	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '0';
command[4] = '5';

/* Start address 4 bytes */
command[5] = '0';
command[6] = '4';
command[7] = '0';
command[8] = 'C';

/* Number of registers 4 bytes */
command[9] = '0';
command[10] = '0';
command[11] = '0';
command[12] = '0';

/* LRC Calculation */
for (i=0; i<12;i++)
	Query[i] = command[i+1];

GetLRC(Query,12,LRC);
command[13] = LRC[0];
command[14] = LRC[1];

/* End */
command[15] = CR;
command[16] = LF;
command[17] = END;

/*---------------------------------------------------------------*/
FlushInQ (me->port);
FlushOutQ (me->port);
memset(read_buffer, 0x00, 17);
/* wait for success command */
_start_test_clock =  clock();
do 
{
for(i=0;i<17;i++)
ComWrtByte (me->port, command[i]);

/* wait for received bytes */
_start_read =  clock();
do
{
received  = GetInQLen (me->port);
time = (double)(clock()-_start_read);
}while(received<17 && time < AXIS_TIMEOUT);

for(i=0;i<received;i++)
	read_buffer[i] = ComRdByte (me->port);
read_buffer[17] = 0;
time = (double)(clock()-_start_read);
} while( (strcmp(read_buffer, command)!=0) && (time < AXIS_TIMEOUT) );

if (time>=AXIS_TIMEOUT)
	EXCTHROW( AXIS_ERROR_TIMEOUT, "AXIS Timeouted");

/*---------------------------------------------------------------*/

Error:
	EXCRETHROW( pexception);
}

#undef __FUNC__
#define __FUNC__	"{IaiAxis}::SetPosition" 
SElExceptionPtr SetPosition(SaxisPtr me, int32_t axis, double position, double speed)
{
	SElExceptionPtr	pexception = NULL;
	char command[44];
	char Query[38];
	char slave_axis[5];
	char LRC[2];
	int i;
	int target_position;
	char PositionHex[4+1];
	int target_speed;
	char SpeedHex[4+1];
	char read_buffer[512];

	
/* Header 1 byte */
command[0] = HEADER;

/* axis 2 bytes */
axis = axis +1; //axis N°0 ==> '0','1'
sprintf(slave_axis, "%02d", axis);
command[1] = slave_axis[0];
command[2] = slave_axis[1];

/* Function code 2 bytes */
command[3] = '1';
command[4] = '0';

/* Start address 4 bytes */ 
command[5] = '9';
command[6] = '9';
command[7] = '0';
command[8] = '0';

/* Number of registers */
command[9] = '0';
command[10] = '0';
command[11] = '0';
command[12] = '6';

/* Number of bytes */
command[13] = '0';
command[14] = 'C';

/* Changed data 4 bytes PCMD */
command[15] = '0';
command[16] = '0';
command[17] = '0';
command[18] = '0';
if (position > 0.0 && position < 100.0 )
target_position = position * 100;
else 
target_position = 0;
	
sprintf(PositionHex, "%04X", target_position);
command[19] = PositionHex[0];
command[20] = PositionHex[1];
command[21] = PositionHex[2];
command[22] = PositionHex[3];

/* Changed data 4 bytes INP */
command[23] = '0';
command[24] = '0';
command[25] = '0';
command[26] = '0';

command[27] = '0';
command[28] = '0';
command[29] = '0';
command[30] = 'A';

	
/* Changed data 4 bytes VCMD*/
command[31] = '0';
command[32] = '0';
command[33] = '0';
command[34] = '0';
if (speed > 0.0 && speed < 500.0 )
target_speed = speed * 100;
else 
target_speed = 100;
	
sprintf(SpeedHex, "%04X", target_speed);
command[35] = SpeedHex[0];
command[36] = SpeedHex[1];
command[37] = SpeedHex[2];
command[38] = SpeedHex[3];

/* LRC Calculation */
for (i=0; i<38;i++)
	Query[i] = command[i+1];

GetLRC(Query,38,LRC);
command[39] = LRC[0];
command[40] = LRC[1];

/* End */
command[41] = CR;
command[42] = LF;
command[43] = END;

FlushInQ (me->port);
FlushOutQ (me->port);
ComWrt (me->port, command, StringLength (command));
ComWrt (me->port, command, StringLength (command));
Sleep(30);
ReturnRS232Err ();

#if 0    /* formerly excluded lines */
Sleep(10);

ComRd (me->port, read_buffer, GetInQLen (me->port));
FlushInQ (me->port);
FlushOutQ (me->port);
#endif   /* formerly excluded lines */

Error:
	EXCRETHROW( pexception);
}

#pragma warning( pop)
