/***************************************************
 *        Copyright Keysight Technologies 2012-2014
 **************************************************/

#include "stdafx.h"
#include "Ag34450.h"
// Link to: <Program Files>\IVI Foundation\IVI\Lib\msc\Ag34450.lib

///
/// Ag34450 IVI-C Driver Example Program
/// 
/// Initializes the driver, reads a few Identity interface properties, and checks the instrument error queue.
/// May include additional instrument specific functionality.
/// 
/// See driver help topic "Programming with the IVI-C Driver in Various Development Environments"
/// for additional programming information.
///
/// Runs in simulation mode without an instrument.
/// 

int _tmain(int argc, _TCHAR* argv[])
{
	ViStatus status;
	ViSession session;
	ViChar str[128];
	ViInt32 ErrorCode;
	ViChar ErrorMessage[256];
	ViBoolean simulate;
	ViReal64 data;

	// Edit resource and options as needed.  resource is ignored if option Simulate=true
	char resource[] = "GPIB0::22::INSTR";
	//char resource[] = "TCPIP0::<ip or host name>::INSTR";

	char options[]  = "QueryInstrStatus=true, Simulate=true, DriverSetup= Model=, Trace=true, TraceName=c:\\temp\\traceOut";

	ViBoolean idQuery = VI_TRUE;
	ViBoolean reset   = VI_TRUE;

	printf("  C_Example1\n\n");

	// Initialize the driver.  See driver help topic "Initializing the IVI-C Driver" for additional information
	status = Ag34450_InitWithOptions(resource, idQuery, reset, options, &session);
	if(status)
	{
		// Initialization failed
		Ag34450_GetError(session, &ErrorCode, 255, ErrorMessage);
		printf("** InitWithOptions() Error: %d, %s\n", ErrorCode, ErrorMessage);
		printf("\nDone - Press Enter to Exit");
		getchar();  
		return ErrorCode;
	}
	assert(session != VI_NULL);
	printf("Driver Initialized \n");

	// Read and output a few attributes
	// Return status checking omitted for example clarity
	status = Ag34450_GetAttributeViString(session, "", AG34450_ATTR_SPECIFIC_DRIVER_PREFIX, 127, str);
	assert(status == VI_SUCCESS);
	printf("DRIVER_PREFIX:      %s\n", str);
	status = Ag34450_GetAttributeViString(session, "", AG34450_ATTR_SPECIFIC_DRIVER_REVISION, 127, str);
	assert(status == VI_SUCCESS);
	printf("DRIVER_REVISION:    %s\n", str);
	status = Ag34450_GetAttributeViString(session, "", AG34450_ATTR_SPECIFIC_DRIVER_VENDOR, 127, str);
	assert(status == VI_SUCCESS);
	printf("DRIVER_VENDOR:      %s\n", str);
	status = Ag34450_GetAttributeViString(session, "", AG34450_ATTR_SPECIFIC_DRIVER_DESCRIPTION, 127, str);
	assert(status == VI_SUCCESS);
	printf("DRIVER_DESCRIPTION: %s\n", str);
	status = Ag34450_GetAttributeViString(session, "", AG34450_ATTR_INSTRUMENT_MODEL, 127, str);
	assert(status == VI_SUCCESS);
	printf("INSTRUMENT_MODEL:   %s\n", str);
	status = Ag34450_GetAttributeViString(session, "", AG34450_ATTR_INSTRUMENT_FIRMWARE_REVISION, 127, str);
	assert(status == VI_SUCCESS);
	printf("FIRMWARE_REVISION:  %s\n", str);
	status = Ag34450_GetAttributeViString(session, "", AG34450_ATTR_SYSTEM_SERIAL_NUMBER, 127, str);
	assert(status == VI_SUCCESS);
	printf("SERIAL_NUMBER:      %s\n", str);
	status = Ag34450_GetAttributeViBoolean(session, "", AG34450_ATTR_SIMULATE, &simulate);
	assert(status == VI_SUCCESS);
	if (simulate == VI_TRUE)
		wprintf(L"\nSIMULATE:           True\n\n");
	else
		wprintf(L"SIMULATE:           False\n\n");


			//Reset the DMM
		status = Ag34450_reset(session);

		//Set up the DMM for immediate trigger
		Ag34450_SetAttributeViInt32(session,"", AG34450_ATTR_TRIGGER_SOURCE ,AG34450_VAL_IMMEDIATE);

		
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		//---------------------DC Volt measurements------------------------//
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		
		//Configure the meter to 10V range and fast mode (least resolution)
		status = Ag34450_DCVoltageConfigure(session, 10, AG34450_VAL_RESOLUTION_LOW);
		
		status = Ag34450_Read(session,5000,&data);
		
		//Display the Raw DC Volts Measurement
		printf("\nRaw DC Volts measurement: %0.15g Volts", data);

			
		
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		//---------------------AC Volt measurements------------------------//
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		
		//Configure the meter to 10VAC range and fast mode (least resolution)
		
		status = Ag34450_ACVoltageConfigure(session, 10, AG34450_VAL_RESOLUTION_LOW);

		status = Ag34450_Read(session,5000,&data);

		//Display the Raw AC Volts Measurement
		printf("\nRaw AC Volts measurement: %0.15g Volts", data);

		
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		//-------------------DC Current measurements-----------------------//
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		
		//Configure the meter to 100mA range and fast mode (least resolution)
		
		status = Ag34450_DCCurrentConfigure(session, 100E-3, AG34450_VAL_RESOLUTION_LOW);

		status = Ag34450_Read(session,5000,&data);
		printf("\nDC Current measurement: %0.15g Amps", data);


		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		//-------------------AC Current measurements-----------------------//
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//

		//Configure the meter to 100mA range and fast mode (least resolution)
		
		status = Ag34450_ACCurrentConfigure(session, 100E-3, AG34450_VAL_RESOLUTION_LOW);

		status = Ag34450_Read(session,5000,&data);
		printf("\nAC Current measurement: %0.15g Amps", data);
		

		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		//--------------------Frequency measurements-----------------------//
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//

		//Configure the meter to read frequency
		
		status = Ag34450_FrequencyConfigure(session, 100, AG34450_VAL_RESOLUTION_LOW);
		status = Ag34450_SetAttributeViReal64(session,"",AG34450_ATTR_FREQUENCY_VOLTAGE_RANGE,10);

		status = Ag34450_Read(session,5000,&data);
		printf("\nFrequency measurement: %0.15g Hz", data);

		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		//-------------------Resistance measurements-----------------------//
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//

		//Configure the meter to 10k ohm range and fast mode (least resolution)
		
		status = Ag34450_ResistanceConfigure(session, 10E+3, AG34450_VAL_RESOLUTION_LOW);

		status = Ag34450_Read(session,5000,&data);
		printf("\n2-Wire Resistance measurement: %0.15g Ohms", data);
		

		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		//-------------------Temperature measurements----------------------//
		//"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""//
		
		//Configure the meter to 5k ohm range
		
		status = Ag34450_TempratureConfigure(session);
		
		status = Ag34450_Read(session,5000,&data);
		printf("\nTemprature measurement: %0.15g degrees C", data);


	// Check instrument for errors
	ErrorCode = -1;
	printf("\n");
	while(ErrorCode!=0)
	{
		status = Ag34450_error_query( session, &ErrorCode, ErrorMessage);
		assert(status == VI_SUCCESS);
		printf("error_query: %d, %s\n", ErrorCode, ErrorMessage);
	}

	// Close the driver
	status = Ag34450_close(session);
	assert(status == VI_SUCCESS);
	session = VI_NULL;
	printf("Driver Closed \n");

	printf("\nDone - Press Enter to Exit");
	getchar();

	return 0;
}
