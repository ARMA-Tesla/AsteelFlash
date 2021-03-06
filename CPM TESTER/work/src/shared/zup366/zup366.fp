s??        	??   ? 	=F  `P   ?   ????                               zup366                          zup366                                                                                                ? ? ??ViInt16  ?  ? ??ViInt32  ? ? ??ViReal64     ? ??ViRsrc     	? 	??ViBoolean     	? 	??ViSession     ? ??ViStatus     ?  ViChar[]     ? ??ViChar     ? ??ViString     	?  ViInt16[]     	?  ViInt32[]     
?  	ViReal64[]     ? 	 
ViBoolean[]     ? ??ViConstString     ? ??ViAttr     ?  ViByte[]     ? ??ViInt64  ?    This instrument driver provides programming support for the Agilent N6700 DC Power Supply.  The driver contains all the functions that IVI and VXIplug&play require.  In addition, the driver contains high-level functions that configure the power supply and generate output in a single operation.  The driver also contains lower level functions that configure the power supply and initiate the output changes in separate operations.   

Note:  This driver requires the VISA and IVI libraries.  
     ?    This class contains functions that configure the instrument.  The class includes high-level functions that configure multiple instrument settings as well as low-level functions that set, get, and check individual attribute values.
     ;    This class contains functions for configuring the output.     C    This class contains functions for configuring the trigger system.     ;    This class contains functions for configuring sequencies.     F    This class contains functions for configuring output trigger system.     B    This class contains functions for configuring array measurement.     C    This class contains functions for configuring the digital system.     \    This class contains functions for defines a list of output channels as a paralleled group.     g    This class contains functions for configuring waveforms or sequences for the N6705B DC Power Analyzer     _    This class contains functions for configuring the datalogger for the N6705B DC Power Analyzer     T    This class contains sub-classes for the set, get, and check attribute functions.       ?    This class contains functions that set an attribute to a new value.  There are typesafe functions for each attribute data type.     ?    This class contains functions that obtain the current value of an attribute.  There are typesafe functions for each attribute data type.     ?    This class contains functions that obtain the current value of an attribute.  There are typesafe functions for each attribute data type.     L    This class contains high-level functions for measuring the output signal.      ?    This class contains functions and sub-classes that initiate and retrieve measurements using the current configuration.  The class contains a high-level read function for histogram.     ?    The class contains functions that give low-level control over the datalogeer and histogram.  The functions perform the following operations:

- intiate 
- fetch  
- abort      ]    This class contains functions that initiate instrument operations and report their status.
     F    This class contains functions that send and receive instrument data.    _    This class contains functions and sub-classes that control common instrument operations.  These functions include many of functions that VXIplug&play require, such as reset, self-test, revision query, error query, and error message.  This class also contains functions that access IVI error infomation, lock the session, and perform instrument I/O.
     R    This class contains functions that retrieve and clear the IVI error information.     ?    This class contains functions that retrieve coercion records.     J    This class contains functions that retrieve interchangeability warnings.     k    This class contains functions that lock and unlock IVI instrument driver sessions for multithread safefy.     M    These function are obsolete, please do not use them as possible as you can.     F    This class contains functions that send and receive instrument data.    B    This function performs the following initialization actions:

- Creates a new IVI instrument driver session.

- Opens a session to the specified device using the interface and address you specify for the Resource Name parameter.

- If the ID Query parameter is set to VI_TRUE, this function queries the instrument ID and checks that it is valid for this instrument driver.

- If the Reset parameter is set to VI_TRUE, this function resets the instrument to a known state.

- Sends initialization commands to set the instrument to the state necessary for the operation of the instrument driver.

- Returns a ViSession handle that you use to identify the instrument in all subsequent instrument driver function calls.

Note:  This function creates a new session each time you invoke it. Although you can open more than one IVI session for the same resource, it is best not to do so.  You can use the same session in multiple program threads.  You can use the zup366_LockSession and zup366_UnlockSession functions to protect sections of code that require exclusive access to the resource.

Content from the IVI specifications reproduced with permission from the IVI Foundation.  

The IVI Foundation and its member companies make no warranty of any kind with regard to this material, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose. The IVI Foundation and its member companies shall not be liable for errors contained herein or for incidental or consequential damages in connection with the furnishing, performance, or use of this material.
    ?    Pass the resource name of the device to initialize.

You can also pass the name of a virtual instrument or logical name that you configure with the IVI Configuration utility.  The virtual instrument identifies a specific device and specifies the initial settings for the session.  A logical Name identifies a particular virtual instrument.

Refer to the following table below for the exact grammar to use for this parameter.  Optional fields are shown in square brackets ([]).

Syntax
------------------------------------------------------
GPIB[board]::<primary address>[::secondary address]::INSTR
ASRL<port>::INSTR
<LogicalName>
<DriverSession>

If you do not specify a value for an optional field, the following values are used:

Optional Field - Value
------------------------------------------------------
board - 0
secondary address - none (31)

The following table contains example valid values for this parameter.

"Valid Value" - Description
------------------------------------------------------
"GPIB::22::INSTR" - GPIB board 0, primary address 22 no
                    secondary address
"GPIB::22::5::INSTR" - GPIB board 0, primary address 22
                       secondary address 5
"GPIB1::22::5::INSTR" - GPIB board 1, primary address 22
                        secondary address 5
"ASRL2::INSTR" - COM port 2
"SampleInstr" - Logical name "SampleInstr"
"xyz432" - Logical Name or Driver Session "xyz432"


Default Value: "GPIB::5::INSTR"        Specify whether you want the instrument driver to perform an ID Query.

Valid Range:
VI_TRUE  (1) - Perform ID Query (Default Value)
VI_FALSE (0) - Skip ID Query

When you set this parameter to VI_TRUE, the driver verifies that the instrument you initialize is a type that this driver supports.  

Circumstances can arise where it is undesirable to send an ID Query command string to the instrument.  When you set this parameter to VI_FALSE, the function initializes the instrument without performing an ID Query.     ?    Specify whether you want to reset the instrument during the initialization procedure.

Valid Range:
VI_TRUE  (1) - Reset Device (Default Value)
VI_FALSE (0) - Don't Reset

    ?    Returns a ViSession handle that you use to identify the instrument in all subsequent instrument driver function calls.

Notes:

(1) This function creates a new session each time you invoke it.  This is useful if you have multiple physical instances of the same type of instrument.  

(2) Avoid creating multiple concurrent sessions to the same physical instrument.  Although you can create more than one IVI session for the same resource, it is best not to do so.  A better approach is to use the same IVI session in multiple execution threads.  You can use functions zup366_LockSession and zup366_UnlockSession to protect sections of code that require exclusive access to the resource.

    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    L =   ?  ?    Resource Name                      : ?       ID Query                           =? ?       Reset Device                      ? ?C ?  ?    Instrument Handle                 !?#????  ?    Status                          ????  v??                                            "GPIB::5::INSTR"   Yes VI_TRUE No VI_FALSE   Yes VI_TRUE No VI_FALSE    	           	           KCopyright 2004-2015 National Instruments Corporation. All rights reserved.   D    This function performs the following initialization actions:

- Creates a new IVI instrument driver and optionally sets the initial state of the following session attributes:

    ZUP366_ATTR_RANGE_CHECK         
    ZUP366_ATTR_QUERY_INSTRUMENT_STATUS  
    ZUP366_ATTR_CACHE               
    ZUP366_ATTR_SIMULATE            
    ZUP366_ATTR_RECORD_COERCIONS    

- Opens a session to the specified device using the interface and address you specify for the Resource Name parameter.

- If the ID Query parameter is set to VI_TRUE, this function queries the instrument ID and checks that it is valid for this instrument driver.

- If the Reset parameter is set to VI_TRUE, this function resets the instrument to a known state.

- Sends initialization commands to set the instrument to the state necessary for the operation of the instrument driver.

- Returns a ViSession handle that you use to identify the instrument in all subsequent instrument driver function calls.

Note:  This function creates a new session each time you invoke it. Although you can open more than one IVI session for the same resource, it is best not to do so.  You can use the same session in multiple program threads.  You can use the zup366_LockSession and zup366_UnlockSession functions to protect sections of code that require exclusive access to the resource.

Content from the IVI specifications reproduced with permission from the IVI Foundation.  

The IVI Foundation and its member companies make no warranty of any kind with regard to this material, including, but not limited to, the implied warranties of merchantability and fitness for a particular purpose. The IVI Foundation and its member companies shall not be liable for errors contained herein or for incidental or consequential damages in connection with the furnishing, performance, or use of this material.

    ?    Pass the resource name of the device to initialize.

You can also pass the name of a virtual instrument or logical name that you configure with the IVI Configuration utility.  The virtual instrument identifies a specific device and specifies the initial settings for the session.  A logical Name identifies a particular virtual instrument.

Refer to the following table below for the exact grammar to use for this parameter.  Optional fields are shown in square brackets ([]).

Syntax
------------------------------------------------------
GPIB[board]::<primary address>[::secondary address]::INSTR
ASRL<port>::INSTR
<LogicalName>
<DriverSession>

If you do not specify a value for an optional field, the following values are used:

Optional Field - Value
------------------------------------------------------
board - 0
secondary address - none (31)

The following table contains example valid values for this parameter.

"Valid Value" - Description
------------------------------------------------------
"GPIB::22::INSTR" - GPIB board 0, primary address 22 no
                    secondary address
"GPIB::22::5::INSTR" - GPIB board 0, primary address 22
                       secondary address 5
"GPIB1::22::5::INSTR" - GPIB board 1, primary address 22
                        secondary address 5
"ASRL2::INSTR" - COM port 2
"SampleInstr" - Logical name "SampleInstr"
"xyz432" - Logical Name or Driver Session "xyz432"

 
Default Value:  "GPIB::5::INSTR"
        Specify whether you want the instrument driver to perform an ID Query.

Valid Range:
VI_TRUE  (1) - Perform ID Query (Default Value)
VI_FALSE (0) - Skip ID Query

When you set this parameter to VI_TRUE, the driver verifies that the instrument you initialize is a type that this driver supports.  

Circumstances can arise where it is undesirable to send an ID Query command string to the instrument.  When you set this parameter to VI_FALSE, the function initializes the instrument without performing an ID Query.     ?    Specify whether you want to reset the instrument during the initialization procedure.

Valid Range:
VI_TRUE  (1) - Reset Device (Default Value)
VI_FALSE (0) - Don't Reset

    ?    Returns a ViSession handle that you use to identify the instrument in all subsequent instrument driver function calls.

Notes:

(1) This function creates a new session each time you invoke it.  This is useful if you have multiple physical instances of the same type of instrument.  

(2) Avoid creating multiple concurrent sessions to the same physical instrument.  Although you can create more than one IVI session for the same resource, it is best not to do so.  A better approach is to use the same IVI session in multiple execution threads.  You can use functions zup366_LockSession and zup366_UnlockSession to protect sections of code that require exclusive access to the resource.

    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    ?    You can use this control to set the initial value of certain attributes for the session.  The following table lists the attributes and the name you use in this parameter to identify the attribute.

Name              Attribute Defined Constant   
--------------------------------------------
RangeCheck        ZUP366_ATTR_RANGE_CHECK
QueryInstrStatus  ZUP366_ATTR_QUERY_INSTRUMENT_STATUS   
Cache             ZUP366_ATTR_CACHE   
Simulate          ZUP366_ATTR_SIMULATE  
RecordCoercions   ZUP366_ATTR_RECORD_COERCIONS

The format of this string is, "AttributeName=Value" where AttributeName is the name of the attribute and Value is the value to which the attribute will be set.  To set multiple attributes, separate their assignments with a comma.

If you pass NULL or an empty string for this parameter and a VISA resource descriptor for the Resource Name parameter, the session uses the default values for the attributes. The default values for the attributes are shown below:

    Attribute Name     Default Value
    ----------------   -------------
    RangeCheck         VI_TRUE
    QueryInstrStatus   VI_FALSE
    Cache              VI_TRUE
    Simulate           VI_FALSE
    RecordCoercions    VI_FALSE
    

If you pass NULL or an empty string for this parameter and a virtual instrument or logical name for the Resource Name parameter, the session uses the values that you configure for virtual instrument or logical name with the IVI Configuration utility.

You can override the values of the attributes by assigning a value explicitly in a string you pass for this parameter.  You do not have to specify all of the attributes and may leave any of them out.  If you do not specify one of the attributes, its default value or the value that you configure with the IVI Configuration utility will be used.

The following are the valid values for ViBoolean attributes:

    True:     1, TRUE, or VI_TRUE
    False:    0, False, or VI_FALSE


Default Value:
"Simulate=0,RangeCheck=1,QueryInstrStatus=0,Cache=1"

Notes:
(1) For the DriverSetup parameter, you can pass the following strings:
  Module<i>: X 
  where i is slot position. It's 1-based.
  and X is the instrument type:
    N6731A
    N6732A
    N6733A
    N6734A
    N6735A
    N6742A
    N6743A
    N6744A
    N6745A
    N6751A
    N6752A
    N6753A
    N6754A
    N6761A
    N6762A
    N6731B
    N6732B
    N6733B
    N6734B
    N6735B
    N6736B
    N6741B
    N6742B
    N6743B
    N6744B
    N6745B
    N6746B

Here is an example of this option string which turns on simulation and emulates the Agilent N6700 with installed modules N6743A in slot 1 and N6762A in slot 2:

  "Simulate=1,DriverSetup=Module1:N6743A;Module2:N6762A"

(2) If you enable IDQuery and don't pass DriverSetup driver automatically detect your instrument model.

(4) If you don't pass these parameters and IDQuery is disabled default model is Agilent N6700 with installed modules N6762A,N6752A,N6742A,N6745A. It's equivalent with 
  "DriverSetup=Module1:N6762A;Module2:N6752A;Module3:N6742A;Module4:N6745A"    1I =   ?  ?    Resource Name                     7 : ?       ID Query                          9 =? ?       Reset Device                      9? ?Y ?  ?    Instrument Handle                 <?#????  ?    Status                            C ?  ? ?    Option String                   ????  v??                                            "GPIB::5::INSTR"   Yes VI_TRUE No VI_FALSE   Yes VI_TRUE No VI_FALSE    	           	           5"Simulate=0,RangeCheck=1,QueryInstrStatus=0,Cache=1"    KCopyright 2004-2015 National Instruments Corporation. All rights reserved.    l    Configures whether the signal that the power supply produces on a channel appears at the output connector.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     q    Pass the name of the channel you want to enable or disable.

Valid Channel Names:  1,2,3,4

Default Value:  ""
        Pass whether you want the signal the power supply produces on a channel to appear at the output connector.  The driver uses this value to set the ZUP366_ATTR_OUTPUT_ENABLED attribute.

Valid Values:
VI_TRUE  - Enable the output
VI_FALSE - Disable the output

Default Value: VI_TRUE

Notes: 
1) If output and sense relays are installed (Option 761), the relays will open when the output is disabled and close when the output is enabled. The optional NORelay parameter lets you turn the output state on or off and leave the state of the relays unchanged. 
2) When not specified, the relays open and close as the output is turned off and on. Relays only open and close when the output is at zero voltage; zero current. 
3) Separate delays can be programmed for the off-to-on and the on-to-off transition using OUTPut:DELay:RISE and OUTput:DELay:FALL.
4) Because of internal circuit start-up procedures and any installed relay options, the OUTPut ON command may take between 35 and 50 ms to complete its function. The OUTPut OFF command may take between 20 and 25 ms to complete its function. To mitigate this built-in delay, you can program the output to 0V rather than using the output on/off command.

Remote commands:
OUTPut[:STATe] {ON|OFF}, [NORelay],(@<chanlist>)
OUTPut[:STATe]? (@<chanlist>)    Q?-   ?  ?    Instrument Handle                 R?#????  ?    Status                            Y 3 
 ?  ?    Channel Name                      Y? 3 ? ?  ?    Enabled                                	           ""   True VI_TRUE False VI_FALSE   Y    Configures the power supply's output range on a channel.  You specify whether you want to configure the voltage or current range, and the value to which to set the range.

Notes:

1) Setting a voltage range can invalidate a previously configured current range.

2) Setting a current range can invalidate a previously configured voltage range.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     x    Pass the name of the channel on which to specify the output range.

Valid Channel Names:  1,2,3,4

Default Value:  ""
        Pass the range in which you want the power supply to operate.  The instrument driver coerces this value to the closest value the instrument supports that is greater than or equal to the value you specify.

Units: volts (for voltage range)
       amps  (for current range)

Valid Range:
(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 0.0 to 20.4 V
N6782A: 0.0 to 20.4 V
N6783A: 0.0 to 8.0 V
N6784A: 0.0 to 20.4 V       
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05 to 51 A             
N6764A: 0.02 to 20.4 A       
N6765A: 0.05 to 51 A        
N6766A: 0.02 to 17.3 A 
N6773A  0.00 to 15A
N6774A  0.00 to 8.5 A
N6775A  0.00 to 5.0 A
N6776A  0.00 to 3.0 A         
N6777A: 0.00 to 2.0 A
N6781A: -3.06 to 3.06 A
N6782A: -3.06 to 3.06 A
N6783A: 0.0 to 3.0 A
N6784A: -3.06   to 3.06 A
N6731B  0.06    to  10.2 A
N6732B  0.04    to  6.375 A
N6733B  0.01    to  2.55 A
N6734B  0.005   to  1.53 A
N6735B  0.0025  to  0.85 A
N6736B  0.0015  to  0.51 A
N6741B  0.06    to  20.4 A
N6742B  0.04    to  12.75 A
N6743B  0.01    to  5.1 A
N6744B  0.005   to  3.06 A
N6745B  0.0025  to  1.7 A
N6746B  0.0015  to  1.02 A

Default Value:  0.02 

Remote commands:
[SOURce:]CURRent:RANGe {<value>|MIN|MAX}, (@<chanlist>)
[SOURce:]VOLTage:RANGe {<value>|MIN|MAX}, (@<chanlist>)

    f    Pass the type of range you want to configure.

Valid Values:
ZUP366_VAL_RANGE_VOLTAGE - Voltage Range
ZUP366_VAL_RANGE_CURRENT - Current Range

Default Value: ZUP366_VAL_RANGE_VOLTAGE

Notes:

1) Setting a voltage range can invalidate a previously configured current range.

2) Setting a current range can invalidate a previously configured voltage range.
    a-   ?  ?    Instrument Handle                 a?#????  ?    Status                            hY 3 
 ?  ?    Channel Name                      h? 3? ?  ?    Range                             s? 3 ? ? ?    Range Type                             	           ""    0.02               BVoltage ZUP366_VAL_RANGE_VOLTAGE Current ZUP366_VAL_RANGE_CURRENT    ?    This function configures the current limit.  You specify the output current limit value and the behavior of the power supply when the output current is greater than or equal to that value.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     {    Pass the name of the channel on which to configure the current limit.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the behavior you want the power supply to exhibit when the  output current is greater than or equal to the value of the limit parameter.   The driver uses this value to set the ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR attribute.

Valid Values:
ZUP366_VAL_CURRENT_REGULATE - Regulatory limit
ZUP366_VAL_CURRENT_TRIP     - Trip limit

Default Value:  
ZUP366_VAL_CURRENT_REGULATE

Notes:
1) The current limit setting determines when the output channel goes into constant current operation.
2)  An over-current condition can be cleared with OUTPut:PROTection:CLEar after the cause of the condition is removed.

Remote commands:
[SOURce:]CURRent:PROTection:STATe {ON|OFF}, (@<chanlist>)
[SOURce:]CURRent:PROTection:STATe? (@<chanlist>)    ?    Pass the current limit you want to use. The driver uses this value to set the ZUP366_ATTR_CURRENT_LIMIT attribute.

Units: amps

Valid Range:
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A: 0 to 3.06 A        
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A   

Default Value:  0.01

Notes:
1) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority.
2) Range specified above for N6783A is for N6783A-BAT Range for  N6783A-MFG is 0.0 to 3.0 A

Remote commands:
[SOURce:]CURRent[:LEVel][:IMMediate][:AMPLitude] <NRf+>, (@chanlist)
[SOURce:]CURRent[:LEVel][:IMMediate][:AMPLitude]? (@chanlist)    w?-   ?  ?    Instrument Handle                 x[#????  ?    Status                            ~? 3 
 ?  ?    Channel Name                      m 3 ? ? ?    Behavior                          ?Q 3? ?  ?    Limit                                  	           ""               BRegulate ZUP366_VAL_CURRENT_REGULATE Trip ZUP366_VAL_CURRENT_TRIP    0.01   X    This function configures the power supply's over-voltage protection.  You specifies the over-voltage limit and the behavior of the power supply when the output voltage is greater than or equal to that value.

When the enabled parameter is VI_FALSE, the driver ignores the limit parameter and does not set the ZUP366_ATTR_OVP_LIMIT attribute.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     w    Pass the name of the channel on which to configure the OVP limit.

Valid Channel Names:  1,2,3,4

Default Value:  ""
        Pass the over-voltage protection limit you want to use. The driver uses this value to set the ZUP366_ATTR_OVP_LIMIT attribute.

Units: volts

N6731A: 0.0 to 7.0 V
N6732A: 0.0 to 10.0 V
N6733A: 0.0 to 22.0 V
N6734A: 0.0 to 38.5 V
N6735A: 0.0 to 55.0 V
N6742A: 0.0 to 10.0 V
N6743A: 0.0 to 22.0 V
N6744A: 0.0 to 38.5 V
N6745A: 0.0 to 55.0 V
N6751A: 0.0 to 55.0 V
N6752A: 0.0 to 55.0 V
N6753A: 0.0 to 22.0 V
N6754A: 0.0 to 66.0 V
N6755A: 0.0 to 22.0 V
N6756A: 0.0 to 66.0 V
N6761A: 0.0 to 55.0 V
N6762A: 0.0 to 55.0 V
N6763A: 0.0 to 22.0 V
N6764A: 0.0 to 66.0 V
N6765A: 0.0 to 22.0 V
N6766A: 0.0 to 66.0 V
N6773A: 0.0 to 22.0 V
N6774A: 0.0 to 38.5 V
N6775A: 0.0 to 66.0 V
N6776A: 0.0 to 110.0 V
N6777A: 0.0 to 165.0 V
-
-
N6783A: 0.0 to 10.0 V
-
N6731B: 0.0 to 7.5 V
N6732B: 0.0 to 10.0 V
N6733B: 0.0 to 22.0 V
N6734B: 0.0 to 38.5 V
N6735B: 0.0 to 66.0 V
N6736B: 0.0 to 110.0 V
N6741B: 0.0 to 7.5 V
N6742B: 0.0 to 10.0 V
N6743B: 0.0 to 22.0 V
N6744B: 0.0 to 38.5 V
N6745B: 0.0 to 66.0 V
N6746B: 0.0 to 110.0 V
 
Default Value: 0.0 V

Notes:
1) This command does NOT apply to models N678xA SMU (N6781A, N6782A, and N6784A). For these models, refer to VOLTage:PROTection:REMote and VOLTage:PROTection:REMote:NEGative configured in Configure SMU OV Remote Protection function.
2)An over-voltage condition can be cleared with the OUTput:PROTection:CLEar command after the condition that caused the OVP trip is removed.

Remote commands:
[SOURce:]VOLTage:PROTection:LEVel {<value>|MIN|MAX}, (@<chanlist>)
[SOURce:]VOLTage:PROTection:LEVel? (@<chanlist>)    ?    Pass whether you want to enable or disable the OVP limit.   The driver uses this value to set the ZUP366_ATTR_OVP_ENABLED attribute.

Valid Values:
VI_TRUE  - Enable OVP limit (Default Value)
VI_FALSE - Disable OVP limit

Note:
If the ZUP366_ATTR_OVP_ENABLED attribute is set to VI_FALSE, this attribute does not affect the behavior of the instrument.

Remote commands:
[SOURce:]VOLTage:PROTection:TRACking[:STATe] <Bool>, (@<chanlist>)
[SOURce:]VOLTage:PROTection:TRACking[:STATe]? (@<chanlist>)    ??-   ?  ?    Instrument Handle                 ?#????  ?    Status                            ? 3 
 ?  ?    Channel Name                      ?? 3? ?  ?    Limit                             ?? 3 ? ? ?    Enabled                                	           ""    0.0   True VI_TRUE False VI_FALSE    V    This function configures the DC voltage level the power supply attempts to generate.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     {    Pass the name of the channel on which to configure the voltage level.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the DC voltage you want the power supply to attempt to generate.  The driver uses this value to set the ZUP366_ATTR_VOLTAGE_LEVEL attribute. 

Units: volts

Valid Range:

N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A: 0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01   to  20.4 V
N6764A: 0.025  to  61.2 V
N6765A: 0.01   to  20.4 V
N6766A: 0.025  to  61.2 V
N6773A: 0.0    to  20 V
N6774A: 0.0    to  35 V
N6775A: 0.0    to  60 V
N6776A: 0.0    to  100 V
N6777A: 0.0    to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V     
N6731B  0.015   to  5.1 V
N6732B  0.015   to  8.16 V
N6733B  0.03    to  20.4 V
N6734B  0.04    to  35.7 V
N6735B  0.07    to  61.2 V
N6736B  0.1     to  102.0 V
N6741B  0.015   to  5.1 V
N6742B  0.015   to  8.16 V
N6743B  0.03    to  20.4 V
N6744B  0.04    to  35.7 V
N6745B  0.07    to  61.2 V
N6746B  0.1     to  102.0 V

Default Value:  0.02 volts

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2)Range specified above for N6783A is for N6783A-BAT
Range for  N6783A-MFG is 0.0 to 6.0 V

Remote commands:
[SOURce:]VOLTage[:LEVel][:IMMediate][:AMPLitude] <NRf+>, (@chanlist)
[SOURce:]VOLTage[:LEVel][:IMMediate][:AMPLitude]? (@chanlist)    ?;-   ?  ?    Instrument Handle                 ??#????  ?    Status                            ?? 3 
 ?  ?    Channel Name                      ? 3 ? ?  ?    Level                                  	           ""    0.02   H    This function sets the output relay polarity.

Notes:
1) This function is valid only when your instrument installs Option 760. (Polarity Reversal) available on models N6742B and N6773A and is a required option for N673xB, N674xB, N675xA, N676xA, N677xA. Not available for N6741B, N6751A, N6752A, N6761A, N6762A, and N678xA. 

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     |    Pass the name of the channel on which to configure the trigger source.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the output relay polarity. The driver sets the ZUP366_ATTR_RELAY_POLARITY attribute to this value.


Valid Values:
ZUP366_VAL_RELAY_POLARITY_NORMAL - Causes the output polarity to be the same as the output connector labeling.

ZUP366_VAL_RELAY_POLARITY_REVERSE - Causes the output polarity to be the reverse of the output connector labeling. This command briefly turns the output off while the output and sense terminal polarities are switched.

Default Value: ZUP366_VAL_RELAY_POLARITY_NORMAL

Notes:
1) When installed on models N6742B and N6773A, Option 760 (Polarity Reversal) will limit the output current of these models to a maximum of 10 A.
2) Option 760 is a required option for N673xB, N674xB, N675xA, N676xA and N677xA
Option 760 is not available on models N6741B, N6751A, N6752A N6761A, N6762A or N678xA SMU.

Remote commands:
OUTPut:RELay:POLarity {NORMal|REVerse}, (@chanlist)
OUTPut:RELay:POLarity? (@<chanlist>)    ??   ?  ?    Instrument Handle                 ??????  ?    Status                            ?% 5  ?  ?    Channel Name                      ?? 5y ? ?    Relay Polarity                         	           ""               RNormal ZUP366_VAL_RELAY_POLARITY_NORMAL Reverse ZUP366_VAL_RELAY_POLARITY_REVERSE   ?    Sets the power limit on output channels.

Notes:
1) For Agilent N6705 mainframes, the instrument will operate normally as long as the combined output power is within the power rating of the mainframe
2) For Agilent N678xA SMU, the power limit function does not apply because their maximum output power is rated at 20W.
3) For Agilent N675xA, and N676xA power modules, the power limit function limits the output power at its programmed setting.
4) For Agilent N673xB, N674xB, and N677xA power modules, the power limit function turns the output off after a power limit persists for about 1 millisecond.
5) If the power limit is left at the maximum rating, these power modules will not enable the power limit protection. The power limit protection will only activate when the power limit is set to a value less than the maximum rating of the power module and the output power subsequently exceeds this limit setting.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     w    Pass the name of the channel on which to configure the OVP limit.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    P    Sets the power limit on specified output channel.

Units: W

Valid Range:

N6731B: 0.0 to 50.0 W
N6732B: 0.0 to 50.0 W
N6733B: 0.0 to 50.0 W
N6734B: 0.0 to 50.0 W
N6735B: 0.0 to 50.0 W
N6736B: 0.0 to 50.0 W
N6741B: 0.0 to 100.0 W
N6742B: 0.0 to 100.0 W 
N6743B: 0.0 to 100.0 W
N6744B: 0.0 to 100.0 W
N6745B: 0.0 to 100.0 W
N6746B: 0.0 to 100.0 W
N6751A: 0.0 to 50.0 W
N6752A: 0.0 to 100.0 W
N6753A: 0.0 to 300.0 W
N6754A: 0.0 to 300.0 W
N6755A: 0.0 to 500.0 W
N6756A: 0.0 to 500.0 W
N6761A: 0.0 to 50.0 W
N6762A: 0.0 to 100.0 W
N6763A: 0.0 to 300.0 W
N6764A: 0.0 to 300.0 W
N6765A: 0.0 to 500.0 W
N6766A: 0.0 to 500.0 W
N6773A: 0.0 to 300.0 W
N6774A: 0.0 to 300.0 W
N6775A: 0.0 to 300.0 W
N6776A: 0.0 to 300.0 W
N6777A: 0.0 to 300.0 W
N6781A: 0.0 to 20.0 W
N6782A: 0.0 to 20.0 W
N6783A: 0.0 to 24.0 W
N6784A: 0.0 to 20.0 W
 
Default Value: 20.0 W

Notes:
1)Range specified above for N6783A is for N6783A-BAT
Range for  N6783A-MFG is 0.0 to 18.0 W
2)This command does not apply to N678xA SMU modules

Remote commands:
[SOURce:]POWer:LIMit:LEVel <value>, (@<chanlist>)
[SOURce:]POWer:LIMit? (@<chanlist>)    ?=   ?  ?    Instrument Handle                 ??????  ?    Status                            ǀ 9  ?  ?    Channel Name                      ?? 9? ?  ?    Power Limit                            	           ""    20.0    ?    Selects whether the output regulation is in voltage or current priority mode.

Note:
This function only applies to N678xA SMU models.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     w    Pass the name of the channel on which to configure the OVP limit.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    0    Selects either a voltage or current for the waveform. Only one type of waveform may be output at a time. The driver sets the ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY attribute to this value. 

Valid Values:
ZUP366_VAL_OUTPUT_REGULATION_VOLTAGE - In voltage priority mode the output is controlled by a bi-polar constant voltage feedback loop, which maintains the output voltage at its positive or negative programmed setting.
ZUP366_VAL_OUTPUT_REGULATION_CURRENT - In current priority mode the output is controlled by a bi-polar constant current feedback loop, which maintains the output sourcing or sinking current at its programmed setting.

Default Value: ZUP366_VAL_OUTPUT_REGULATION_VOLTAGE

Notes:
1) This command only applies to models N678xA SMU.

Remote Commands:
[SOURce:]FUNCtion <mode>, (@<chanlist>)
    ??   ?  ?    Instrument Handle                 Ύ????  ?    Status                            ? %  ?  ?    Channel Name                      ՚ , ? ? ?    Output Regulation                      	           ""               lVoltage Priority ZUP366_VAL_OUTPUT_REGULATION_VOLTAGE Current Priority ZUP366_VAL_OUTPUT_REGULATION_CURRENT    k    Specifies the output impedance mode at turn-off.

Note:

This function only applies to N678xA SMU models.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ~    Pass the name of the channel on which to configure the Output Impedance.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Specifies the output impedance mode. The driver sets the ZUP366_ATTR_SMU_OUTPUT_IMPEDANCE attribute to this value.

Valid Values:
ZUP366_VAL_SMU_HIGH_IMPEDANCE - High Impedance 
Sets the output to a high impedance mode at turn-off. In high impedance mode, the output relays are opened while the output remains at its set value.

ZUP366_VAL_SMU_LOW_IMPEDANCE - Low Impedance 
Sets the output to a low impedance mode at turn-off. In low impedance mode, the output is first down-programmed to zero, after which the output relays are opened. 

Default Value: ZUP366_VAL_SMU_OUTPUT_HIGH_IMPEDANCE

Notes:
1) This command is used only with models N678xA SMU (N6781A, N6782A, N6784A)

Remote Commands:
OUTPut[:STATe]:TMODe <mode>, (@<chanlist>)

    ڪ"   ?  ?    Instrument Handle                 ?`????  ?    Status                            ?? %  ?  ?    Channel Name                      ?s % ? ? ?    Output Impedance                       	           ""               YHigh Impedance ZUP366_VAL_SMU_HIGH_IMPEDANCE Low Impedance  ZUP366_VAL_SMU_LOW_IMPEDANCE    s    Sets the voltage bandwidth and the oscillation state.

Note:
This function only applies to the N678xA SMU models.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to configure the bandwidth and oscillation protection.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Enables or disables the output oscillation for protection.  If capacitive loads cause the output to oscillate on the default or any of the other bandwidth settings, this protection function will detect the oscillation and latch the output off. The driver sets the ZUP366_ATTR_SMU_ENABLE_OSCILLATION attribute to this value. 

Valid Values:
VI_TRUE  : Enable the output oscillation for protection
VI_FALSE : Disable the output oscillation for protection

Default Value: VI_FALSE

Notes:
1) This command is used only with models N678xA SMU (N6781A, N6782A, N6784A)

Remote Commands:
OUTPut:PROTection:OSCillation[:STATe] <Bool>, (@<chanlist>)

    ?    Specifies a voltage bandwidth, which lets you optimize output response time with capacitive loads. These compensation modes only apply when the unit is operating in Voltage Priority mode. The default bandwidth is optimized for stability with a wide range of output capacitors. The driver sets the ZUP366_ATTR_SMU_VOLTAGE_BANDWIDTH attribute to this value.

Valid Values:
ZUP366_VAL_SMU_LOW_BANDWIDTH   - low voltage bandwidth
ZUP366_VAL_SMU_HIGH1_BANDWIDTH - high 1 voltage bandwidth
ZUP366_VAL_SMU_HIGH2_BANDWIDTH - high 2 voltage bandwidth
ZUP366_VAL_SMU_HIGH3_BANDWIDTH - high 3 voltage bandwidth

Bandwidth Mode  Output Capacitance     Output sensing
Low             0 - 150 microfarads    Local or Remote
High 1          0 - 1 microfarads      Remote only
High 2          1 - 7 microfarads      Remote only
High 3          7 - 150 microfarads    Remote only

Default Value: ZUP366_VAL_SMU_LOW_BANDWIDTH

Notes:
1) This command is used only with models N678xA SMU (N6781A, N6782A, N6784A)
2) The High compensation modes can only be used in conjunction with remote voltage sensing. 
3) Connecting capacitive loads that fall outside the indicated ranges will result in output instability or oscillation. If output oscillation occurs, a protection function will detect the oscillation and latch the output off. The oscillation detect function is always enabled.

Remote Commands:
[SOURce:]VOLTage:BWIDth <mode>, (@<chanlist>)

    ?-   ?  ?    Instrument Handle                 ??????  ?    Status                            ?p %  ?  ?    Channel Name                      ?
 ? ?       Enable Oscillation Protection     ?? ?  ? ?    Voltage Bandwidth                      	           ""    True 1 False 0               ?Low ZUP366_VAL_SMU_LOW_BANDWIDTH High 1 ZUP366_VAL_SMU_HIGH1_BANDWIDTH High 2 ZUP366_VAL_SMU_HIGH2_BANDWIDTH High 3 ZUP366_VAL_SMU_HIGH3_BANDWIDTH    ?    Sets the state of the output resistance and resistance level on the specified channels.

Note:
This function only applies to the N6781A model.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to configure the  resistance output and level.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    8    Enables or disables output resistance programming. The driver sets the ZUP366_ATTR_SMU_ENABLE_RESISTANCE attribute to this value.

Valid Values:
VI_TRUE  
VI_FALSE 

Default Value: VI_FALSE

Notes:
1) This command only applies to model N6781A.

Remote Commands:
[SOURce:]RESistance:STATe <Bool>, (@<chanlist>)
    l    Sets the resistance level of the specified output channel.  The driver sets the ZUP366_ATTR_SMU_RESISTANCE_LEVEL attribute to this value.

Units are in ohms.

Valid Range:
-0.04 to +1.0 Ohm

Default Value: 0.0

Notes:
1) This command only applies to model N6781A.

Remote Commands:
[SOURce:]RESistance[:LEVel][:IMMediate][:AMPLitude] <resistance>, (@<chanlist>)
    ??   ?  ?    Instrument Handle                 ?U????  ?    Status                            ? .  ?  ?    Channel Name                     t . ? ?       Enable Resistance Output         ? .j ?  ?    Resistance Level                       	           ""    True 1 False 0    0.0   5    Configures the Positive Voltage Limit, Negative Voltage Limit, and Voltage Limit Tracking settings. 

Notes:
1) ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY needs to be set to ZUP366_VAL_OUTPUT_REGULATION_CURRENT in the ConfigureSMUOutputRegulation function.  
2) This function applies to N678xA SMU models.

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

         Pass the name of the channel on which to configure the SMU voltage limit.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    }    Sets the positive voltage limit of the specified output channel. Units are in volts. The driver sets the ZUP366_ATTR_SMU_POSITIVE_VOLTAGE_LIMIT attribute to this value.

Valid Values:
0.0    to  20.4 V

Default Value: 0.0

Notes:
1) This command only applies to model N678xA SMU (N6781A, N6782A, and N6784A).  
2) ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY needs to be set to ZUP366_VAL_OUTPUT_REGULATION_CURRENT in the ConfigureSMUOutputRegulation function.  
3) For model N6784A if voltage limit tracking is enabled, the negative voltage limit tracks the positive voltage limit setting. If disabled, different values can be set for the positive and negative voltage limits.
4) The voltage limit should always be set higher than the actual output voltage requirement of the external load.

Remote Commands:
[SOURce:]VOLTage:LIMit[:POSitive][:IMMediate] [:AMPLitude] <limit>, (@<chanlist>)

        Sets the voltage limit tracking state. The driver sets the ZUP366_ATTR_SMU_VOLTAGE_LIMIT_TRACKING attribute to this value.

Valid Values:
VI_TRUE  - If limit tracking is enabled, the negative voltage limit tracks the positive voltage limit setting.
VI_FALSE - If tracking is disabled, different values can be set for the positive and negative voltage limits. 

Default Value: VI_FALSE

Notes:
1) This command only applies to model N6784A.

Remote Commands:
[SOURce:]VOLTage:LIMit:COUPle <Bool>, (@<chanlist>)


    ?    Sets the negative voltage limit of the specified output channel. Units are in volts. The driver sets the ZUP366_ATTR_SMU_NEGATIVE_VOLTAGE_LIMIT attribute to this value.

Valid Values:
-20.4 to 0

Default Value: 0.0

Notes:
1) This command only applies to model N6784A.  
2) If voltage limit tracking is enabled, the negative voltage limit tracks the positive voltage limit setting. If disabled, different values can be set for the positive and negative voltage limits.
3) In order to have the full Voltage Range the Voltage range needs to be set in Configure Output Range function

Remote Commands:
[SOURce:]VOLTage:LIMit:NEGative[:IMMediate] [:AMPLitude] <limit>, (@<chanlist>)


   ?   ?  ?    Instrument Handle                S????  ?    Status                           ? %  ?  ?    Channel Name                     g ? $ ? K    Positive Voltage Limit           ? ?u ?       Enable Voltage Limit Tracking    ? ? ? ? K    Negative Voltage Limit                 	           ""    0.0    True 1 False 0    0.0   0    Configures the Positive Current Limit, Negative Current Limit, and Current Limit Tracking settings. 

Notes:
1) ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY needs to be set to ZUP366_VAL_OUTPUT_REGULATION_VOLTAGE in ConfigureSMUOutputRegulation.
2) This function applies to N678xA SMU and N6783 models.

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to configure the current limit (positive, negative, and tracking).

Valid Channel Names:  1,2,3,4

Default Value:  ""
    "    Sets the current limit tracking state. The driver sets the ZUP366_ATTR_SMU_CURRENT_LIMIT_TRACKING attribute to this value.

Valid Values:
VI_TRUE  - If limit tracking is enabled, the negative current limit tracks the positive current limit setting.
VI_FALSE - If tracking is disabled, different values can be set for the positive and negative current limits. 

Default Value: VI_FALSE

Notes:
1) This command only applies to N678xA SMU models (N6781A, N6782A, and N6784A).

Remote Commands:
[SOURce:]CURRent:LIMit:COUPle <Bool>, (@<chanlist>)

    ?    Sets the positive current limit of the specified output channel. Units are in amperes. The driver sets the ZUP366_ATTR_SMU_ASM_POSTIVE_CURRENT_LIMIT attribute to this value. 

Valid Values:
0.0 - 3.06 

Default Value: 0.0

Notes:
1) This command only applies to model N678xA SMU (N6781A, N6782A, and N6784A) and N6783 (BAT and MFG).  
2) ZUP366_ATTR_SMU_OUTPUT_REGULATION_PRIORITY needs to be set to ZUP366_VAL_OUTPUT_REGULATION_VOLTAGE in ConfigureSMUOutputRegulation.
3) For SMU models if Current limit tracking is enabled, the negative current limit tracks the positive current limit setting. If disabled, different values can be set for the positive and negative current limits. 
4) The current limit should always be set higher than the actual output current requirement of the external load.

Remote Commands:
[SOURce:]CURRent:LIMit[:POSitive][:IMMediate] [:AMPLitude] <limit>, (@<chanlist>)
    ?    Sets the negative current limit of the specified output channel. Units are in amperes. The driver sets the ZUP366_ATTR_SMU_ASM_NEGATIVE_CURRENT_LIMIT attribute to this value.

Valid Values:
-3.06 to 0  (N6781A, N6782A, and N6784A)
-2 to 0 (N6783A)

Default Value: 0.0

Notes:
1) This command only applies to model N678x SMU (N6781A, N6782A, and N6784A) and N6783A-BAT.  
2) For SMU models if Current limit tracking is enabled, the negative current limit tracks the positive current limit setting. If disabled, different values can be set for the positive and negative current limits. 

Remote Commands:
[SOURce:]CURRent:LIMit:NEGative[:IMMediate] [:AMPLitude] <limit>, (@<chanlist>)

   T   ?  ?    Instrument Handle                
????  ?    Status                            ? ,  ?  ?    Channel Name                     != , ? ?       Enable Current Limit Tracking    #g ?   ? K    Positive Current Limit           &? ? ? ? K    Negative Current Limit                 	           ""    True 1 False 0    0.0    0.0    ?    Sets the positive over-voltage remote protection (OVP) level of the output channel. 

Note:
This function only applies to the N678xA SMU models.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to configure the OVRP positive limit protection.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Sets the positive over-voltage remote protection (OVP) level of the output channel. The values are programmed in volts. The driver sets the ZUP366_ATTR_SMU_POSITIVE_OVRP attribute to this value.

Valid Range:
0 - 22 

Default Value: 0

Notes:
1) This command is used only with models N678xA SMU (N6781A, N6782A, N6784A)
2)  If the output voltage exceeds the OVP level, the output is disabled and the Questionable Condition status register OV bit is set. 
3) For Models N678xA only, the OVP circuit senses the 4-wire sense terminals rather than the output terminals. This allows for more precise overvoltage monitoring directly at the load. Since incorrect sense terminal wiring such as reversed or open sense wires could defeat this feature, the OVP will automatically trip if the voltage at the output terminals rises more than 1.5 V above the voltage at the sense terminals.

Remote Command:
[SOURce:]VOLTage:PROTection:REMote[:POSitive] [:LEVel] <voltage>, (@<chanlist>)
   +?   ?  ?    Instrument Handle                ,m????  ?    Status                           2? %  ?  ?    Channel Name                     3? x  ? K    Positive OVRP                          	           ""                          ?    Sets the negative over-voltage remote protection (OVP) level of the output channel. 

Note:
This function only applies to models N6784A.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to configure the OVRP negative protection limit.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Sets the negative over-voltage remote protection (OVP) level of the output channel. The values are programmed in volts.  The driver sets the ZUP366_ATTR_SMU_NEGATIVE_OVRP attribute to this value.


Valid Range:
-22 to 0

Default Value: 0

Notes:
1) This command is used only with model N6784A.
2)  If the output voltage exceeds the OVP level, the output is disabled and the Questionable Condition status register OV bit is set. 
3) For Models N678xA only, the OVP circuit senses the 4-wire sense terminals rather than the output terminals. This allows for more precise overvoltage monitoring directly at the load. Since incorrect sense terminal wiring such as reversed or open sense wires could defeat this feature, the OVP will automatically trip if the voltage at the output terminals rises more than 1.5 V above the voltage at the sense terminals.

Remote Command:
[SOURce:]VOLTage:PROTection:REMote:NEGative[:LEVel] <voltage>, (@<chanlist>)
   8?   ?  ?    Instrument Handle                9?????  ?    Status                           @< %  ?  ?    Channel Name                     @? v  ? K    Negative OVRP                          	           ""        ????               W    Sets the over-voltage protection delay. 

Note:
This function only applies to N6783A.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     x    Pass the name of the channel on which to configure the OVRP delay.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    B    Sets the over-voltage protection delay.  Over-voltage protection will not be triggered on the specified output channel during the delay time. After the delay time has expired, the over-voltage protection will be active. This prevents momentary changes in output status from triggering the over-current protection. The driver sets the ZUP366_ATTR_ASM_OVER_PROTECTION_DELAY.

Valid Range:
0.0 to 0.005

Default Value:0.0

Notes:
This command only applies to models N6783A (N6783A-BAT, N6783A-MFG)

Remote Commands:
[SOURce:]VOLTage:PROTection:DELay[:TIME] <time>, (@<chanlist>)
   E?   ?  ?    Instrument Handle                F?????  ?    Status                           M. %  ?  ?    Channel Name                     M? x  ? ?    Over Protection Delay                  	           ""    0.0    Y    Sets the current slew rate.

Note:
This function only applies to the N678xA SMU models.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     w    Pass the name of the channel on which to configure the slew rate.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    <    Configures the current slew rate in amps per second. The slew rate setting affects all programmed current changes, including those due to the output state turning on or off. The driver sets the ZUP366_ATTR_SMU_CURRENT_SLEW_RATE attribute to this value.

Valid Values:
0 - 9.9E+37
ZUP366_VAL_SLEW_RATE_COUNT_INFINITY

Default Value: 9.9E+37

Notes:
1) This command only applies to models N678xA SMU.
2) For very large values, the slew rate will be limited by the analog performance of the output circuit. 
3) This command is coupled to the [SOURce:]VOLTage:SLEW:MAX command. If Current Slew Rate sets the rate to INFinity, [SOURce:]VOLTage:SLEW:MAX is enabled. If the slew rate is set to any other value, [SOURce:]VOLTage:SLEW:MAX is disabled.

Remote Commands:
[SOURce:]CURRent:SLEW[:IMMediate] <slew>|INFinity, (@<chanlist>)
   QJ   ?  ?    Instrument Handle                R ????  ?    Status                           X? %  ?  ?    Channel Name                     Y ?  ?  ?    Current Slew Rate                      	           ""    	9.9E+37    @    Configures the trigger source the power supply waits for before changing the output signal.  When the power supply receives a trigger signal on the source you specify, it changes its current limit and voltage level to the values of the ZUP366_ATTR_CURRENT_LIMIT and ZUP366_ATTR_VOLTAGE_LEVEL attributes, respectively.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     |    Pass the name of the channel on which to configure the trigger source.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    k    Pass the trigger source to you want the power supply to wait for before changing the output signal.  The driver sets the ZUP366_ATTR_TRIGGER_SOURCE attribute to this value.


Valid Values:
ZUP366_VAL_TRIG_EXTERNAL - The power supply waits for a trigger on the external trigger input.

ZUP366_VAL_SOFTWARE_TRIG - The power supply waits until you call the zup366_SendSoftwareTrigger function.

ZUP366_VAL_TRIG_PIN1 - Selects an output port connector pin 1

ZUP366_VAL_TRIG_PIN2 - Selects an output port connector pin 2

ZUP366_VAL_TRIG_PIN3 - Selects an output port connector pin 3

ZUP366_VAL_TRIG_TRAN1 - Selects the transient output channels 1 as the externaltrigger source.

ZUP366_VAL_TRIG_TRAN2 - Selects the transient output channels 2 as the external trigger source.

ZUP366_VAL_TRIG_TRAN3 - Selects the transient output channels 3 as the external trigger source.

ZUP366_VAL_TRIG_TRAN4 - Selects the transient output channels 4 as the external trigger source.


Default Value: ZUP366_VAL_SOFTWARE_TRIG

Remote commands:
TRIGger:TRANsient:SOURce {BUS|EXT|PIN<pin>|TRANsient<chan>}, (@<chanlist>)
TRIGger:TRANsient:SOURce?

   ^?-   ?  ?    Instrument Handle                _D#????  ?    Status                           e? 3 
 ?  ?    Channel Name                     fU 3 ? ? ?    Source                                 	           ""           	  External ZUP366_VAL_TRIG_EXTERNAL Software ZUP366_VAL_SOFTWARE_TRIG Pin 1 ZUP366_VAL_TRIG_PIN1 Pin 2 ZUP366_VAL_TRIG_PIN2 Pin 3 ZUP366_VAL_TRIG_PIN3 Transient 1 ZUP366_VAL_TRIG_TRAN1 Transient 2 ZUP366_VAL_TRIG_TRAN2 Transient 3 ZUP366_VAL_TRIG_TRAN3 Transient 4 ZUP366_VAL_TRIG_TRAN4    ?    This function configures the power supply what happens to the output voltage when the transient system is initiated and triggered.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to configure the triggered voltage mode.

Valid Channel Names:  1,2,3,4

Default Value:  ""
        Specifies what happens to the output voltage when the transient system is initiated and triggered. The driver sets the ZUP366_ATTR_TRIGGER_VOLTAGE_MODE attribute to this value.

Valid Values:
ZUP366_VAL_TRIGGER_VOLTAGE_MODE_FIXED
  The output voltage remains at the immediate value.
ZUP366_VAL_TRIGGER_VOLTAGE_MODE_STEP
  The output goes to the triggered level when a trigger occurs.
ZUP366_VAL_TRIGGER_VOLTAGE_MODE_LIST
  The output follows the programmed list step value when a trigger occurs. This function does not apply to all models
ZUP366_VAL_TRIGGER_VOLTAGE_MODE_ARB
  The output follows the arbitrary waveform values when a trigger occurs.

Default Value: ZUP366_VAL_TRIGGER_VOLTAGE_MODE_FIXED

Notes:
1) List mode is only available with models N6761A/N6762A, or models that have Option 054 (High Speed Test Extensions).
2) Arbitrary waveform generator functionality available only with the N6705B DC Power Analyzer model.

Remote commands:
[SOURce:]VOLTage:MODE {FIXed|STEP|LIST|ARB}, (@<chanlist>)
[SOURce:]VOLTage:MODE? (@<chanlist>)   mk-   ?  ?    Instrument Handle                n!#????  ?    Status                           t? 3 
 ?  ?    Channel Name                     u; 3 ? ? ?    Voltage Mode                           	           ""               ?Fixed ZUP366_VAL_TRIGGER_VOLTAGE_MODE_FIXED Step ZUP366_VAL_TRIGGER_VOLTAGE_MODE_STEP List ZUP366_VAL_TRIGGER_VOLTAGE_MODE_LIST Waveform (Arb) ZUP366_VAL_TRIGGER_VOLTAGE_MODE_ARB    ?    This function configures the power supply what happens to the output current when the transient system is initiated and triggered.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to configure the triggered current mode.

Valid Channel Names:  1,2,3,4

Default Value:  ""
        Specifies what happens to the output current when the transient system is initiated and triggered. The driver sets the ZUP366_ATTR_TRIGGER_CURRENT_MODE attribute to this value.

Valid Values:
ZUP366_VAL_TRIGGER_CURRENT_MODE_FIXED
  The output voltage remains at the immediate value.
ZUP366_VAL_TRIGGER_CURRENT_MODE_STEP
  The output goes to the triggered level when a trigger occurs.
ZUP366_VAL_TRIGGER_CURRENT_MODE_LIST
  The output follows the programmed list step value when a trigger occurs. This function does not apply to all models
ZUP366_VAL_TRIGGER_CURRENT_MODE_ARB
  The output follows the arbitrary waveform values when a trigger occurs.

Default Value: ZUP366_VAL_TRIGGER_CURRENT_MODE_FIXED

Notes:
1) List mode is only available with models N6761A/N6762A, or models that have Option 054 (High Speed Test Extensions).
2) Arbitrary waveform generator functionality available only with the N6705B DC Power Analyzer model.

Remote commands:
[SOURce:]CURRent:MODE {FIXed|STEP|LIST|ARB}, (@<chanlist>)
[SOURce:]CURRent:MODE? (@<chanlist>)   {?-   ?  ?    Instrument Handle                |H#????  ?    Status                           ?? 3 
 ?  ?    Channel Name                     ?a 3 ? ? ?    Current Mode                           	           ""               ?Fixed ZUP366_VAL_TRIGGER_CURRENT_MODE_FIXED Step ZUP366_VAL_TRIGGER_CURRENT_MODE_STEP List ZUP366_VAL_TRIGGER_CURRENT_MODE_LIST Waveform (Arb) ZUP366_VAL_TRIGGER_CURRENT_MODE_ARB    s    This function configures the DC voltage level the power supply attempts to generate after it receives a trigger.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to configure the triggered voltage level.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the DC voltage level you want the power supply to attempt to generate after it receives a trigger.  The driver sets the ZUP366_ATTR_TRIGGERED_VOLTAGE_LEVEL attribute to this value.

Units: volts

Valid Range:

N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V       
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

Default Value:  0.02 volts

1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT
Range for  N6783A-MFG is 0.0 to 6.0 V

Remote commands:
[SOURce:]VOLTage[:LEVel]:TRIGgered[:AMPLitude]
{<value>|MIN|MAX}, (@<list>)
[SOURce:]VOLTage[:LEVel]:TRIGgered[:AMPLitude]? (@<chanlist>)   ??-   ?  ?    Instrument Handle                ?]#????  ?    Status                           ?? 3 
 ?  ?    Channel Name                     ?y 3 ? ?  ?    Triggered Level                        	           ""    0.02    k    This function configures the current limit the power supply attempts to use after it receives a trigger.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to configure the triggered current limit.


Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the current limit you want the power supply to attempt to use after it receives a trigger.  The driver sets the ZUP366_ATTR_TRIGGERED_CURRENT_LIMIT attribute to this value.

Units: amps

Valid Range:
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A: 0.0 to 3.06 A         
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A   

Default Value:  0.01 amps

Notes:
The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.

Remote commands:
[SOURce:]CURRent[:LEVel]:TRIGgered[:AMPLitude]{<value>|MIN|MAX}, (@<chanlist>)
[SOURce:]CURRent[:LEVel]:TRIGgered[:AMPLitude]? (@<chanlist>)
   ?-   ?  ?    Instrument Handle                ?5#????  ?    Status                           ?? 3 
 ?  ?    Channel Name                     ?R 3 ? ?  ?    Triggered Limit                        	           ""    0.01    ?    Configures the trigger source of the waveform function.

Note:
Waveform capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors        Selects the trigger source for the waveforms. This driver sets the ZUP366_ATTR_TRIGGER_WFM_SRC attribute to this value.

Valid Values:
ZUP366_VAL_TRIG_WFM_IMMEDIATE - Triggers the waveform immediately.
ZUP366_VAL_TRIG_WFM_EXTERNAL - Selects the rear panel Trigger In BNC connector.
ZUP366_VAL_TRIG_WFM_BUS_SW - Selects a remote interface trigger command.

Default Value: ZUP366_VAL_TRIG_WFM_IMMEDIATE

Notes:
Waveform capability is only available on the N6705B DC Power Analyzer

Remote commands:
TRIGger:ARB:SOURce <source>
   ?v   ?  ?    Instrument Handle                ?,????  ?    Status                           ?? 0  ? ?    Waveform Trigger Source                	                      rImmediate ZUP366_VAL_TRIG_WFM_IMMEDIATE External ZUP366_VAL_TRIG_WFM_EXTERNAL Software ZUP366_VAL_TRIG_WFM_BUS_SW       Configures the trigger source of the histogram function. When the power supply receives a trigger signal on the source you specify, it runs the histogram.


Notes:
1) This function only applies to model N6705B and modules N6781A and N6782A that have the Data Logger function installed.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Selects the trigger source for the histogram measurement data logger. The driver sets the ZUP366_ATTR_HISTOGRAM_TRIGGER_SRC attribute to this value.


Valid Values:
ZUP366_VAL_HISTOGRAM_IMMEDIATE -  Sends the trigger immediately (as soon as the data logger is INITiated) (as soon as the histogram measurement data logger is INITiated).
ZUP366_VAL_HISTOGRAM_EXTERNAL- Selects the Trigger In BNC connector and any digital port pin that has been configured as a trigger input.
ZUP366_VAL_HISTOGRAM_BUS_TRIG - Selects a remote interface trigger command.
ZUP366_VAL_HISTOGRAM_TRIG_PIN1 - Selects a digital port pin configured as a trigger input for pin 1.
ZUP366_VAL_HISTOGRAM_TRIG_PIN2 -  Selects a digital port pin configured as a trigger input for pin 2
ZUP366_VAL_HISTOGRAM_TRIG_PIN3 -  Selects a digital port pin configured as a trigger input for pin 3

Default Value: ZUP366_VAL_HISTOGRAM_BUS_TRIG

Notes:
1) This command only applies to models N6781A and N6782A that have the Data Logger function installed.
2) PIN<1,2,3> must be configured as trigger input before it can be used as a trigger source. See Pin Functions and Polarity commands for the Configure Pin function.

Remote commands:
TRIGger:HISTogram:SOURce <source>, (@<chanlist>)

   ?'   ?  ?    Instrument Handle                ??????  ?    Status                           ?j )  ?  ?    Channel Name                     ?? ) ? ? ?    Histogram Trigger Source               	           ""              ?Immediate ZUP366_VAL_HISTOGRAM_IMMEDIATE External ZUP366_VAL_HISTOGRAM_EXTERNAL Software ZUP366_VAL_HISTOGRAM_BUS_TRIG Pin 1 ZUP366_VAL_HISTOGRAM_TRIG_PIN1 Pin 2 ZUP366_VAL_HISTOGRAM_TRIG_PIN2 Pin 3 ZUP366_VAL_HISTOGRAM_TRIG_PIN3    ?    Configures the trigger source of the data logger. When the power supply receives a trigger signal on the source you specify, it runs the data logger.

Note:
This function only applies to the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors    ?    Selects the trigger source for the data logger.  The driver sets the ZUP366_ATTR_DATALOG_TRIGGER_SRC attribute to this value.

Valid Values:
ZUP366_VAL_TRIG_DATALOG_IMMEDIATE - Sends the trigger immediately (as soon as the data logger is INITiated)
ZUP366_VAL_TRIG_DATALOG_EXTERNAL - Selects the rear panel Trigger In BNC connector as a trigger sources.
ZUP366_VAL_TRIG_DATALOG_BUS_SW - Selects a remote interface trigger command
ZUP366_VAL_TRIG_DATALOG_CURRENT1 - Selects the measured current-level for channel 1.
ZUP366_VAL_TRIG_DATALOG_CURRENT2 -  - Selects the measured current-level for channel 2.
ZUP366_VAL_TRIG_DATALOG_CURRENT3 - Selects the measured current-level for channel 3.
ZUP366_VAL_TRIG_DATALOG_CURRENT4 - Selects the measured current-level for channel 4.
ZUP366_VAL_TRIG_DATALOG_VOLTAGE1- Selects the measured voltage-level for channel 1.
ZUP366_VAL_TRIG_DATALOG_VOLTAGE2 - Selects the measured voltage-level for channel 2.
ZUP366_VAL_TRIG_DATALOG_VOLTAGE3 - Selects the measured voltage-level for channel 3. 
ZUP366_VAL_TRIG_DATALOG_VOLTAGE4 - Selects the measured voltage-level for channel 4. 

Default Value: ZUP366_VAL_TRIG_DATALOG_IMMEDIATE

Notes:
If you have selected a voltage or current level as the trigger source, you must also configure the voltage or current level and the slope polarity.
2)Datalog capability is only available on the N6705B DC Power Analyzer

Remote commands:
TRIGger:DLOG:SOURce <source>   Ɓ   ?  ?    Instrument Handle                ?7????  ?    Status                           ?? >  ? ?    Datalog Trigger Source                 	                     Immediate ZUP366_VAL_TRIG_DATALOG_IMMEDIATE External ZUP366_VAL_TRIG_DATALOG_EXTERNAL Software ZUP366_VAL_TRIG_DATALOG_BUS_SW Current Channel 1 ZUP366_VAL_TRIG_DATALOG_CURRENT1 Current Channel 2 ZUP366_VAL_TRIG_DATALOG_CURRENT2 Current Channel 3 ZUP366_VAL_TRIG_DATALOG_CURRENT3 Current Channel 4 ZUP366_VAL_TRIG_DATALOG_CURRENT4 Voltage Channel 1 ZUP366_VAL_TRIG_DATALOG_VOLTAGE1 Voltage Channel 2 ZUP366_VAL_TRIG_DATALOG_VOLTAGE2 Voltage Channel 3 ZUP366_VAL_TRIG_DATALOG_VOLTAGE3 Voltage Channel 4 ZUP366_VAL_TRIG_DATALOG_VOLTAGE4    p    Configures the trigger for the data logger.

Note:
This function only applies to the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     j    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""

    ?    Sets the trigger level of the specified output channel for a current/voltage level triggered data log. Units are in amperes/volts. The driver sets the ZUP366_ATTR_DATALOG_TRIGGER_LEVEL attribute to this value.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V      
   
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
TRIGger:DLOG:CURRent[:LEVel] <current>, (@<chanlist>)
TRIGger:DLOG:VOLTage[:LEVel] <voltage>, (@<chanlist>)
    ?    Sets the slope polarity of a current/voltage level trigger of the specified output channel for the data logger. The driver sets the ZUP366_ATTR_DATALOG_TRIGGER_SLOPE attribute to this value.

Valid Values:
ZUP366_VAL_DATALOG_SLOPE_POSITIVE            
ZUP366_VAL_DATALOG_SLOPE_NEGATIVE     

Default: ZUP366_VAL_DATALOG_SLOPE_POSITIVE 

Notes:
1) The output channel specified with this command must match the output channel specified by the TRIGger:DLOG:SOURce current/voltage (1-4) parameter.
2) Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
TRIGger:DLOG:CURRent:SLOPe <slope>, (@<chanlist>)
TRIGger:DLOG:VOLTage:SLOPe <slope>, (@<chanlist>)   ַ   ?  ?    Instrument Handle                ?m????  ?    Status                           ??    ?  ?    Channel Name                     ?l i  ?  ?    Datalog Level                    ?d i ? ? ?    Datalog Slope                          	           ""    0.0               VPositive ZUP366_VAL_DATALOG_SLOPE_POSITIVE Negative ZUP366_VAL_DATALOG_SLOPE_NEGATIVE    =    Configures the trigger source for the external data logger.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     X    Specifies the name of the channel.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Sets the trigger source for the external data logger. The driver uses this value to set the ZUP366_ATTR_ELOG_TRIGGER_SOURCE attribute.

Valid Values:
ZUP366_VAL_TRIG_BUS - Bus, selects a remote interface trigger command.
ZUP366_VAL_TRIG_EXTERNAL - External, selects ALL connector pins that have been configured as trigger sources.
ZUP366_VAL_TRIG_IMMEDIATE - Immediate, sets the trigger source true, so that as soon as the external data logger is INITiated it will send the trigger immediately.
ZUP366_VAL_TRIG_PIN1 - Pin1, selects digital port pin 1 configured as a trigger input.
ZUP366_VAL_TRIG_PIN2 - Pin2, selects digital port pin 2 configured as a trigger input.
ZUP366_VAL_TRIG_PIN3 - Pin3, selects digital port pin 3 configured as a trigger input.
ZUP366_VAL_TRIG_PIN4 - Pin4, selects digital port pin 4 configured as a trigger input.
ZUP366_VAL_TRIG_PIN5 - Pin5, selects digital port pin 5 configured as a trigger input.
ZUP366_VAL_TRIG_PIN6 - Pin6, selects digital port pin 6 configured as a trigger input.
ZUP366_VAL_TRIG_PIN7 - Pin7, selects digital port pin 7 configured as a trigger input.

Default Value: ZUP366_VAL_TRIG_BUS

SCPI Command:
TRIGger:ELOG:SOURce <source>, (@<chanlist>)    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

   ??-   ?  ?    Instrument Handle                ?? -  ?  ?    Channel Name                     ?? -? ? ?    External Logger Trigger Source   ??#????  ?    Status                                 ""            
  Bus ZUP366_VAL_TRIG_BUS External ZUP366_VAL_TRIG_EXTERNAL Immediate ZUP366_VAL_TRIG_IMMEDIATE Pin1 ZUP366_VAL_TRIG_PIN1 Pin2 ZUP366_VAL_TRIG_PIN2 Pin3 ZUP366_VAL_TRIG_PIN3 Pin4 ZUP366_VAL_TRIG_PIN4 Pin5 ZUP366_VAL_TRIG_PIN5 Pin6 ZUP366_VAL_TRIG_PIN6 Pin7 ZUP366_VAL_TRIG_PIN7    	            ?    This function configures list properties.

Notes:
1) List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     x    Pass the name of the channel on which to configure the list count.

Valid Channel Names:  1,2,3,4

Default Value:  ""
        Pass the number of times that the list is executed
before it is completed. Applies only to models with list capability. The driver sets the ZUP366_ATTR_LIST_COUNT attribute to this value.

Valid Range:
  1 to 256
  ZUP366_VAL_LIST_COUNT_INFINITY (-1) 

Default Value: 1

Notes:
1) List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
[SOURce:]LIST:COUNt {<count>|MIN|MAX|INFinity}, (@<chanlist>)
[SOURce:]LIST:COUNt? (@<chanlist>)    c    Specifies how the list responds to triggers. Applies only to models with list capability. The driver sets the ZUP366_ATTR_LIST_STEP attribute to this value.

Valid Values:
* ZUP366_VAL_LIST_STEP_ONCE - Causes the output to remain at the present step until a trigger advances it to the next step. Triggers that arrive during the dwell time are ignored.

* ZUP366_VAL_LIST_STEP_AUTO - Causes the output to automatically advance to each step, after the receipt of an initial starting trigger. The steps are paced by the dwell list. As each dwell time elapses, the next step is immediately output.

Default Value: ZUP366_VAL_LIST_STEP_AUTO

Notes:
1) List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
[SOURce:]LIST:STEP {ONCE|AUTO}, (@<chanlist>)
[SOURce:]LIST:STEP? (@<chanlist>)    +    Determines the output value when the list terminates.  Applies only to models with list capability.  When ON, the output voltage or current remains at the value of the last list step. The value of the last voltage or current list step becomes the IMMediate value when the list completes.  When OFF, and also when the list is aborted, the output returns to the settings it was at before the list started.  The driver uses this value to set the ZUP366_ATTR_LIST_TERMINATE_LAST attribute.

Valid Values:
VI_TRUE  (1) - On
VI_FALSE (0) - Off

Default Value: VI_FALSE

Notes:
1) List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
[SOURce:]LIST:TERMinate:LAST {ON|OFF}, (@<chanlist>)
[SOURce:]LIST:TERMinate:LAST? (@<chanlist>)   ?-   ?  ?    Instrument Handle                ??#????  ?    Status                           L 2 
 ?  ?    Channel Name                     ? 2 ? ?  ?    List Count                       ? 2} ? ?    List Step                        E { ? ? ?    Last Termination                       	           ""    1              >Once ZUP366_VAL_LIST_STEP_ONCE Auto ZUP366_VAL_LIST_STEP_AUTO    On VI_TRUE Off VI_FALSE   )    This function configures the dwell time for each list step.

Remote commands:
[SOURce:]LIST:DWELl <time> {,<time>}, (@<chanlist>)
[SOURce:]LIST:DWELl? (@<chanlist>)

Notes:
1)List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to configure the list.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Pass the number of elements in the ViReal64 array you specify for the Dwell List parameter.

Valid Range:
  1 to 512

Default Value: None
    ?    Pass the dwell time for each list step. Applies only
to models with list capability. Dwell time is the time that the output will remain at a specific step.

Units: seconds

Valid Range:
  0.0 to 262.143

Default Value: None

Resolution:
0 to 0.262143  1 microsecond
0 to 2.62143   10 microseconds
0 to 26.2143   100 microseconds
0 to 262.143   1 millisecond

Notes:
1) You must pass a ViReal64 array with at least List Size elements.
2) List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote Commands:
[SOURce:]LIST:DWELl <time>{,<time>}, (@<chanlist>)
[SOURce:]LIST:DWELl? (@<chanlist>)   j-   ?  ?    Instrument Handle                 #????  ?    Status                           ? G  ?  ?    Channel Name                     ) G ? ?  ?    List Size                        ? G~ ?  ?    Dewell List                            	           ""           G    This function configures the voltage setting for each list step in volts.

Remote commands:
[SOURce:]LIST:VOLTage[:LEVel] <volt> {,<volt>}, (@<list>)
[SOURce:]LIST:VOLTage[:LEVel]? (@<chanlist>)

Notes:
1)List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    }    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None

This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to configure the list.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Pass the number of elements in the ViReal64 array you specify for the Voltage List parameter.

Valid Range:
  1 to 512

Default Value: None
    D    Pass the voltage setting for each list step in volts. Applies only to models with list capability. The order in which the values are entered determines the sequence when the list executes.

Units: volts

Valid Range:
 
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V 
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7  V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

Default Value: None


Notes:
1) You must pass a ViReal64 array with at least List Size elements.
2) List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
[SOURce:]LIST:VOLTage[:LEVel] <volt> {,<volt>}, (@<list>)
[SOURce:]LIST:VOLTage[:LEVel]? (@<chanlist>)   ?-   ?  ?    Instrument Handle                 ?#????  ?    Status                           ' G  ?  ?    Channel Name                     '? G ? ?  ?    List Size                        ( G~ ?  ?    Voltage List                           	           ""           M    This function configures the current setting for each list step in amperes.

Remote Commands:
[SOURce:]LIST:CURRent[:LEVel] <curr> {,<curr>}, (@<chanlist>)
[SOURce:]LIST:CURRent[:LEVel]? (@<chanlist>)

Notes:
1)List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to configure the list.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Pass the number of elements in the ViReal64 array you specify for the Current List parameter.

Valid Range:
  1 to 512

Default Value: None
    ?    Pass the current setting for each list step in amperes. Applies only to models with list capability. The order in which the values are entered determines the sequence when the list executes.

Units: amperes

Valid Range:
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to  51 A             
N6764A: 0.02  to  20.4 A       
N6765A: 0.05  to  51 A        
N6766A: 0.02  to  17.3 A 
N6773A  0.00  to  15A
N6774A  0.00  to  8.5 A
N6775A  0.00  to  5.0 A
N6776A  0.00  to  3.0 A         
N6777A: 0.00  to  2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A       
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A      
N6731B  0.06    to  10.2 A
N6732B  0.04    to  6.375 A
N6733B  0.01    to  2.55 A
N6734B  0.005   to  1.53 A
N6735B  0.0025  to  0.85 A
N6736B  0.0015  to  0.51 A
N6741B  0.06    to  20.4 A
N6742B  0.04    to  12.75 A
N6743B  0.01    to  5.1 A
N6744B  0.005   to  3.06 A
N6745B  0.0025  to  1.7 A
N6746B  0.0015  to  1.02 A
    

Default Value: None


Note:
1) You must pass a ViReal64 array with at least List Size elements.
2) List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote Commands:
[SOURce:]LIST:CURRent[:LEVel] <curr> {,<curr>}, (@<chanlist>)
   1?-   ?  ?    Instrument Handle                2?#????  ?    Status                           9) G  ?  ?    Channel Name                     9? G ? ?  ?    List Size                        :9 G ?  ?    Current List                           	           ""           }    This function configures which list steps generate a trigger out signal at the beginning of the list step.

Remote commands:
[SOURce:]LIST:TOUTput:BOSTep[:DATA] {ON|OFF}{,{ON|OFF}}, (@<chanlist>)
[SOURce:]LIST:TOUTput:BOSTep[:DATA]? (@<chanlist>)

Notes:
1)List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions)..
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to configure the list.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Pass the number of elements in the ViBoolean array you specify for the Begin Of Step List parameter.

Valid Range:
  1 to 512

Default Value: None
    ?    Specifies which list steps generate a trigger out signal at the beginning of the list step. Applies only to models with list capability. A trigger is only generated when the state is set to VI_TRUE.

Valid Values:
  VI_FALSE (0) - Trigger is not generated.
  VI_TRUE  (1) - Trigger is generated.

Default Value: None

Note:
1) You must pass a ViBoolean array with at least List Size elements.
2) List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
[SOURce:]LIST:TOUTput:BOSTep[:DATA] {ON|OFF}{,{ON|OFF}}, (@<chanlist>)
[SOURce:]LIST:TOUTput:BOSTep[:DATA]? (@<chanlist>)   Dn-   ?  ?    Instrument Handle                E$#????  ?    Status                           K? G  ?  ?    Channel Name                     L, G ? ?  ?    List Size                        L? G~ ?  ?    Begin Of Step List                     	           ""           ?    This function configures which list steps generate a trigger out signal at the end of the list step's dwell time.

Remote commands:
[SOURce:]LIST:TOUTput:EOSTep[:DATA] {ON|OFF}{,{ON|OFF}}, (@<chanlist>)
[SOURce:]LIST:TOUTput:EOSTep[:DATA]? (@<chanlist>)

Notes:
1)List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to configure the list.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Pass the number of elements in the ViBoolean array you specify for the End Of Step List parameter.

Valid Range:
  1 to 512

Default Value: None
    ?    Specifies which list steps generate a trigger out signal at the end of the list step's dwell time. Applies only to models with list capability. A trigger is only generated when the state is set to VI_TRUE.

Valid Values:
  VI_FALSE (0) - Trigger is not generated.
  VI_TRUE  (1) - Trigger is generated.

Default Value: None

Note:
1) You must pass a ViBoolean array with at least List Size elements.
2) List mode is only available with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
[SOURce:]LIST:TOUTput:EOSTep[:DATA] {ON|OFF}{,{ON|OFF}}, (@<chanlist>)
[SOURce:]LIST:TOUTput:EOSTep[:DATA]? (@<chanlist>)   R-   ?  ?    Instrument Handle                R?#????  ?    Status                           YX G  ?  ?    Channel Name                     Y? G ? ?  ?    List Size                        Zm G~ ?  ?    End Of Step List                       	           ""               Sets the trigger offset points, the sweep points, the sample interval, and the measurement functions. 

Note:

(1) The measurement function is ignored when programming Models N6761A and N6762A. This is because these models measure both voltage and current simultaneously.

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     x    Pass the name of the channel on which to specify the output range.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Specifies the offset in a data sweep when an acquire trigger is used.  The driver sets the ZUP366_ATTR_SWEEP_OFFSET_POINTS attribute to this value.

Valid Range: -524.287 to 2000000000

Default Value: 0

Notes:
1) Positive values represent the delay after the trigger occurs but before the samples are acquired.
2) Negative values represent data samples taken prior to the trigger.
3) The maximum negative value that can be programmed is one less than the maximum allowable SENSe:SWEep:POINts value.
4) This command is used only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
:SENSe:SWEep:OFFSet:POINts {<points>|MIN|MAX}, (@<chanlist>)
:SENSe:SWEep:OFFSet:POINts? (@<chanlist>)    ?    Specifies a measurement function.  The driver sets the ZUP366_ATTR_MEASUREMENT_FUNCTION attribute to this value.

Valid Values:
ZUP366_VAL_MEASURE_VOLTAGE - Voltage Measurement
ZUP366_VAL_MEASURE_CURRENT - Current Measurement

Default Value: ZUP366_VAL_MEASURE_VOLTAGE

Notes:
1) The measurement function is ignored when programming modules N6761A, N6762A and SMU (N6781A, N6782A, and n6784A) modules. This is because these modules measure both voltage and current simultaneously. These modules would use the SENSe:FUNCtion:CURRent and SENSe:FUNCtion:VOLTage commands.

Remote commands:
:SENSe:FUNCtion {"VOLTage"|"CURRent"} (@<chanlist>)
:SENSe:FUNCtion?    ?    Specifies the number of data points in the measurement. The driver sets the ZUP366_ATTR_SWEEP_POINTS attribute to this value.

Valid Range: 1 to 524288

Default Value: 1024 (Non SMU modules)
               4883 (N678xA SMU)                

Notes:
1) This command is used only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).


Remote commands:
:SENSe:SWEep:POINts {<points>|MIN|MAX}, (@<chanlist>)
:SENSe:SWEep:POINts? (@<chanlist>)    ^    Defines the time period between samples in seconds.  The driver sets the ZUP366_ATTR_SWEEP_TIME_INTERVAL attribute to this value.

Valid Range: 0.00002048 to 40000

Default Value: 20.48e-6

Notes:
1) The shortest time interval (fastest speed) that can be specified depends on the number of parameters that are being measured and the model that is doing the measuring.
2) This command is used only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
:SENSe:SWEep:TINTerval {<interval>|MIN|MAX}, (@<chanlist>)
:SENSe:SWEep:TINTerval? (@<chanlist>)   _P-   ?  ?    Instrument Handle                `#????  ?    Status                           f? 3 
 ?  ?    Channel Name                     g ?  ?  ?    Trigger Offset Points            j 3 ? ? ?    Measurement Type                 l? ? ? ?  ?    Points                           n? ?o ?  ?    Sample Interval                        	           ""    0               FVoltage ZUP366_VAL_MEASURE_VOLTAGE Current ZUP366_VAL_MEASURE_CURRENT    1024    	20.48e-6    ?    Configures the power supply's measurement range on a channel.  You specify whether you want to configure the voltage or current range, and the value to which to set the range.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     x    Pass the name of the channel on which to specify the output range.

Valid Channel Names:  1,2,3,4

Default Value:  ""
        Pass the range in which you want the power supply to operate.  The instrument driver coerces this value to the closest value the instrument supports that is greater than or equal to the value you specify.

Units: volts (for voltage range)
       amps  (for current range)

Valid Range:
(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V         
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
 N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0.0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A        
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value:  0.02

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT
Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models. 

Remote commands:
SENSe:CURRent[:DC]:RANGe[:UPPer] {<value>|MIN|MAX}, (@<chanlist>)
SENSe:VOLTage[:DC]:RANGe[:UPPer] {<value>|MIN|MAX}, (@<chanlist>)

     ?    Pass the type of range you want to configure.

Valid Values:
ZUP366_VAL_RANGE_VOLTAGE - Voltage Range
ZUP366_VAL_RANGE_CURRENT - Current Range

Default Value: ZUP366_VAL_RANGE_VOLTAGE

   s?-   ?  ?    Instrument Handle                tQ#????  ?    Status                           z? 3 
 ?  ?    Channel Name                     {` 3? ?  ?    Range                            ?u 3 ? ? ?    Range Type                             	           ""    0.02               BVoltage ZUP366_VAL_RANGE_VOLTAGE Current ZUP366_VAL_RANGE_CURRENT       Configures the resolution used in setting the time interval between digitized measurements for time intervals above 20.48 microseconds.

Note:
This VI is used only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Sets the resolution used in setting the time interval between digitized measurements for time intervals above 20.48 microseconds. The driver uses this value to set the ZUP366_ATTR_MEASUREMENT_RESOLUTION attribute.

Valid Values:
ZUP366_VAL_RESOLUTION_20MS - 20 ms
ZUP366_VAL_RESOLUTION_40MS - 40 ms

Default Value: ZUP366_VAL_RESOLUTION_20MS

SCPI Command:
SENSe:SWEep:TINTerval:RESolution <resolution>    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

   ??-   ?  ?    Instrument Handle                ?p - ? ? ?    Measurement Resolution           ?#????  ?    Status                                            B20 ms ZUP366_VAL_RESOLUTION_20MS 40 ms ZUP366_VAL_RESOLUTION_40MS    	           X    Configures simultaneous voltage and current measurement.

Notes:
1) Simultaneous voltage and current measurement capabilities are only available on N676xA, and N678x SMU.
2) This command replaces the previous SENSe:FUNCtion command and should be used in new applications. SENSe:FUNCtion is still available for backward compatibility.
             ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    *    Enables or disables the current measurement function. It applies to models that have simultaneous voltage and current measurement capability. On modules with simultaneous measurement capability, you can enable BOTH current and voltage measurements. The driver sets the ZUP366_ATTR_ENABLE_SIMULTANEOUS_CURR_MEASUREMENT attribute to this value.

Valid Values:
VI_TRUE  - Enable the output
VI_FALSE - Disable the output

Default Value: VI_FALSE

Notes: 
1) Simultaneous measurement capability only available on N676xA, and N678x SMU models.
2) This command replaces the previous SENSe:FUNCtion command. SENSe:FUNCtion is still available for backward compatibility.
3) SENSe:FUNCtion:CURRent <Bool>, (@<chanlist>) should be used in new applications.

Remote commands:
SENSe:FUNCtion:CURRent <Bool>, (@<chanlist>)    ?    Enables or disables the voltage measurement function. It applies to models that have simultaneous voltage and current measurement capability. On modules with simultaneous measurement capability, you can enable BOTH current and voltage measurements. The driver sets the ZUP366_ATTR_ENABLE_SIMULTANEOUS_VOLT_MEASUREMENT
attribute to this value. 

Valid Values:
VI_TRUE  - Enable the output
VI_FALSE - Disable the output

Default Value: VI_FALSE

Notes: 
1) Simultaneous measurement capability only available on N676xA, and N678x SMU models.
2) This command replaces the previous SENSe:FUNCtion command. SENSe:FUNCtion is still available for backward compatibility.
3) SENSe:FUNCtion:CURRent <Bool>, (@<chanlist>) should be used in new applications.

Remote commands:
SENSe:FUNCtion:VOLTage <Bool>, (@<chanlist>)
Remote commands:
OUTPut[:STATe] {ON|OFF}, [NORelay],(@<chanlist>)
OUTPut[:STATe]? (@<chanlist>)   ??)   ?  ?    Instrument Handle                ??"????  ?    Status                           ?< ,  ?  ?    Channel Name                     ?? ? J ?  ?    Enable Current Measurement       ?? ? ? ?  ?    Enable Voltage Measurement             	           ""    True VI_TRUE False VI_FALSE    True VI_TRUE False VI_FALSE    F    This function configures digital port pin functions and polarity.


     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the pin on which to specify function and polarity.

Valid Pin Names:  PIN1,PIN2,PIN3,PIN4,PIN5,PIN6,PIN7

Default Value:  ""
    ?    Specifies the function of the digital port pin. The pin functions are saved in non-volatile memory.  The driver sets the ZUP366_ATTR_DIGITAL_PIN1_FUNCTION, ZUP366_ATTR_DIGITAL_PIN2_FUNCTION and ZUP366_ATTR_DIGITAL_PIN3_FUNCTION attributes to this value.

Valid Values:

* ZUP366_VAL_DIGITAL_PIN_FUNCTION_DIO
The pin is a general-purpose ground-referenced digital input/output. The output can be set with ZUP366_ATTR_DIGITAL_OUTPUT_DATA attribute.

* ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT 
The pin is in digital input-only mode. The digital output data of the corresponding pin is ignored.

* ZUP366_VAL_DIGITAL_PIN_FUNCTION_TOUTPUT (2)
The pin is configured as a trigger output. When configured as a trigger output, the pin will only generate output triggers if the Step or List transient system has been configured to generated trigger signals.

* ZUP366_VAL_DIGITAL_PIN_FUNCTION_TINPUT (3)
 The pin is configured as a trigger input. When configured as a trigger input, the pin can be selected as a source of measurement and transient trigger signals.

* ZUP366_VAL_DIGITAL_PIN_FUNCTION_FAULT (4)
  Applies only to pin 1. Setting FAULt means that pin 1 functions as an isolated fault output. The fault signal is true when any output is in a protected state (from OCP, OVP, OT, PF, or INH). Note also that Pin 2 serves as the isolated common for pin 1. When pin 1 is set to the FAULt function, the instrument ignores any commands to program pin 2. Queries of pin 2 will return FAULt. If pin 1 is changed from FAULt to another function, pin 2 is set to DINPut.

* ZUP366_VAL_DIGITAL_PIN_FUNCTION_INHIBIT (5)
Applies only to pin 3. When pin 3 is configured as an inhibit input, a true signal at the pin will disable all output channels.

* ZUP366_VAL_DIGITAL_PIN_FUNCTION_ONCOUPLE (6) 
Applies only to pins 4 -7. When configured as an On control, the pin will synchronize the output On state between mainframes. Only one pin can be configured as an On control. The pin will function as both an input and an output. The polarity of the pin is fixed and cannot be programmed.

* ZUP366_VAL_DIGITAL_PIN_FUNCTION_OFFCOUPLE (7)
Applies only to pins 4-7. When configured as an Off control, the pin will synchronize the output Off state between mainframes. Only one pin can be configured as an Off control. The pin will function as both an input and an output. The polarity of the pin is fixed and cannot be programmed.

Default Value: ZUP366_VAL_DIGITAL_PIN_FUNCTION_DIO

Remote commands:
[SOURce:]DIGital:PIN1:FUNCtion {DIO|DINPut|TOUTput|TINPut|FAULt}
[SOURce:]DIGital:PIN2:FUNCtion {DIO|DINPut|TOUTput|TINPut}
[SOURce:]DIGital:PIN3:FUNCtion {DIO|DINPut|TOUTput|TINPut|INHibit}
[SOURce:]DIGital:PIN4:FUNCtion {DIO|DINPut|TOUTput|TINPut|ONCouple|OFFCouple}
[SOURce:]DIGital:PIN5:FUNCtion {DIO|DINPut|TOUTput|TINPut|ONCouple|OFFCouple}
[SOURce:]DIGital:PIN6:FUNCtion {DIO|DINPut|TOUTput|TINPut|ONCouple|OFFCouple}
[SOURce:]DIGital:PIN7:FUNCtion {DIO|DINPut|TOUTput|TINPut|ONCouple|OFFCouple}
    K    Specifies the polarity of the digital port pin. The pin functions are saved in non-volatile memory.  The driver sets the ZUP366_ATTR_DIGITAL_PIN1_FUNCTION, ZUP366_ATTR_DIGITAL_PIN2_FUNCTION and ZUP366_ATTR_DIGITAL_PIN3_FUNCTION attributes to this value.

Valid Values:
* ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE
Setting the polarity positive means that a logical true signal is a voltage high at the pin. For trigger inputs and outputs, positive means a rising edge.

* ZUP366_VAL_DIGITAL_PIN_POLARITY_NEGATIVE
Setting the polarity negative means that a logical true signal is a voltage low at the pin. For trigger inputs and outputs, negative means a falling edge.

Default Value: ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE

Remote commands:
[SOURce:]DIGital:PIN1:POLarity {POSitive|NEGative}
[SOURce:]DIGital:PIN2:POLarity {POSitive|NEGative}
[SOURce:]DIGital:PIN3:POLarity {POSitive|NEGative}
[SOURce:]DIGital:PIN4:POLarity {POSitive|NEGative}
[SOURce:]DIGital:PIN5:POLarity {POSitive|NEGative}
[SOURce:]DIGital:PIN6:POLarity {POSitive|NEGative}
[SOURce:]DIGital:PIN7:POLarity {POSitive|NEGative}   ?-   ?  ?    Instrument Handle                ??#????  ?    Status                           ?b 3 
 ?  ?    Pin Name                         ?? 3 ? ? ?    Pin Function                     ?? 4s ? ?    Polarity                               	           ""              ;Digital Input/Output ZUP366_VAL_DIGITAL_PIN_FUNCTION_DIO Digital Input-only ZUP366_VAL_DIGITAL_PIN_FUNCTION_DINPUT Trigger Output ZUP366_VAL_DIGITAL_PIN_FUNCTION_TOUTPUT Trigger Input ZUP366_VAL_DIGITAL_PIN_FUNCTION_TINPUT Fault ZUP366_VAL_DIGITAL_PIN_FUNCTION_FAULT Inhibit ZUP366_VAL_DIGITAL_PIN_FUNCTION_INHIBIT               dPositive ZUP366_VAL_DIGITAL_PIN_POLARITY_POSITIVE Negative ZUP366_VAL_DIGITAL_PIN_POLARITY_NEGATIVE    x    This function sets the output data on the digital control port when that port is configured for Digital I/O operation.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    ?    Specifies the output data on the digital control port when that port is configured for Digital I/O operation. The port has three signal pins and a digital ground pin. In the binary-weighted value that is written to the port, pin 1 is controlled by bit 0, pin 2 is controlled by bit 1, and Pin 3 is controlled by bit 2.

Default Value: 7

Valid Range:
0 - 127

Remote commands:
[SOURce:]DIGital:OUTPut:DATA <value>
[SOURce:]DIGital:OUTPut:DATA?   ?\-   ?  ?    Instrument Handle                ?#????  ?    Status                           ʡ W ? ?  ?    Data                                   	           7    >    This function returns the state of the digital control port.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Returns the state of the digital control port. The attribute returns the state of pins 1, 2, and 3 in bits 0, 1, and 2 respectively.

Remote commands:
[SOURce:]DIGital:INPut:DATA?   ?`-   ?  ?    Instrument Handle                ?#????  ?    Status                           ԥ > ? ?  ?    Data                                   	           	               This function defines a list of output channels as a paralleled group. This effectively creates a single output with higher current and power capability. You can group up to four channels per mainframe. Channels must have identical model numbers and options installed.

After the channels are wired in parallel and defined as a group, they can be addressed using any of the channel-specific SCPI commands by sending the channel number of the lowest channel in the group.

Group channel lists are stored in non-volatile memory and are unaffected by Reset. This command deletes any previously saved states. However, for the group changes to take effect, you must also reboot the unit.

Notes:
1) This function does NOT apply to models N678xA SMU.

Remote commands:
SYSTem:GROup:DEFine (@<chanlist>)     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     f    Pass the name of the channels to group together.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   ?C   ?  ?    Instrument Handle                ??????  ?    Status                           ?? J ? ?  ?    Channel Name                           	           ""   ?    This function restores a group of channels that have been grouped with zup366_ConfigureChannelGrouping back to an ungrouped state. When ungrouping channels, you must also remove all paralleled connections between the output and sense terminals of that channel.

This command deletes any previously saved states. However, for the group changes to take effect, you must also reboot the unit.

Notes:
1) This function does NOT apply to models N678xA SMU.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     ?    Pass the name of the channel to ungroup. This control is ignored if control Ungroup All is set to True.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass whether you want to enable or disable the OVP limit.   The driver uses this value to set the zup366>>Basic Operation>>OVP Enabled property.

Valid Values:
VI_TRUE  (1) - Ungroup all channel groups.
VI_FALSE (0) - Ungroup only one channel group 

Default Value: VI_FALSE

Notes:
1) These commands do NOT apply to models N678xA SMU.

Remote commands:
VI_TRUE  - SYSTem:GROup:DELete:ALL
VI_FALSE - SYSTem:GROup:DELete <channel>   ?v   ?  ?    Instrument Handle                ?,????  ?    Status                           ?? S k ?  ?    Channel Name                     ?^ Oi ?       Ungroup All                            	           ""    Yes VI_TRUE No VI_FALSE   n    Configures parameters for Waveforms. Sets the waveform type, shape, count and the output value when the waveform terminates. 


Notes:
1) Waveform capability is only available on the N6705B DC Power Analyzer
2) If ZUP366_VAL_WAVEFORM_SEQUENCE is selected as a Waveform Shape the ConfigureSequenceParameters function needs run to configure the Sequence Parameters.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Determines the output value when the waveform terminates. The driver sets the ZUP366_ATTR_WAVEFORM_TERMINATION attribute to this value.
 

Valid Values:
VI_TRUE  (1) - On -  When ON, the output current/voltage remains at the last waveform value. The last current/voltage waveform value becomes the IMMediate value when the waveform completes.
VI_FALSE (0) - Off - When OFF, and also when the waveform is aborted, the output returns to the settings that were in effect before the waveform started.

Default Value: VI_FALSE

Notes:
Waveform capability is only available on the N6705B DC Power Analyzer

Remote commands:
[SOURce:]ARB:TERMinate:LAST <Bool>, (@<chanlist>)    ?    Sets the number of times that the waveform is repeated. The driver sets the ZUP366_ATTR_WAVEFORM_COUNT attribute to this value.

Valid Range:
1 - 16,777,216 
1 - 256 (for constant dwell waveforms only)
ZUP366_VAL_WAVEFORM_COUNT_INFINITY (-1) 

Default Value:1

Notes:
1) Use the INFinity parameter to repeat the waveform continuously.
2) Waveform capability is only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:COUNt <count>, (@<chanlist>)
    ?    Sets the shape for of the waveform. The driver sets the ZUP366_ATTR_WAVEFORM_SHAPE attribute to this value.

Valid Values:
ZUP366_VAL_WAVEFORM_NONE - Specifies no waveform.
ZUP366_VAL_WAVEFORM_CONSTANTDWELL - Specifies a constant-dwell waveform.
ZUP366_VAL_WAVEFORM_EXPONENTIAL - Specifies an exponential waveform
ZUP366_VAL_WAVEFORM_PULSE - Specifies a pulse.
ZUP366_VAL_WAVEFORM_RAMP - Specifies a ramp.
ZUP366_VAL_WAVEFORM_SEQUENCE - Specifies a sequence of waveforms.
ZUP366_VAL_WAVEFORM_SINSUOID - Specifies a sine wave.
ZUP366_VAL_WAVEFORM_STAIRCASE - Specifies a staircase.
ZUP366_VAL_WAVEFORM_STEP - Specifies a step.
ZUP366_VAL_WAVEFORM_TRAPEZOID - Specifies a trapezoid.
ZUP366_VAL_WAVEFORM_UDEFINED - Specifies a user-defined waveform.

Default: ZUP366_VAL_WAVEFORM_NONE 

Notes:
Waveform capability is only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:FUNCtion:SHAPe <function>, (@<chanlist>)    }    Selects either a voltage or current for the waveform. Only one type of waveform may be output at a time. The driver sets the ZUP366_ATTR_WAVEFORM_MEASUREMENT attribute to this value.

Valid Values:
ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE
ZUP366_VAL_WFM_MEAS_TYPE_CURRENT

Default Value: ZUP366_VAL_MEAS_WFM_TYPE_VOLTAGE

Remote Commands:
[SOURce:]ARB:FUNCtion:TYPE <type>, (@<chanlist>)   ??"   ?  ?    Instrument Handle                ?F????  ?    Status                           ?? %  ?  ?    Channel Name                     ?D  ? ? ?    Enable Waveform Termination      ??   ? d    Waveform Count                   ?? % ? ? ?    Waveform Shape                   ?u %j ? ?    Waveform Measurement Type              	           ""    On VI_TRUE Off VI_FALSE    1              ?None ZUP366_VAL_WAVEFORM_NONE Constant Dwell ZUP366_VAL_WAVEFORM_CONSTANTDWELL Exponential ZUP366_VAL_WAVEFORM_EXPONENTIAL Pulse ZUP366_VAL_WAVEFORM_PULSE Ramp ZUP366_VAL_WAVEFORM_RAMP Sequence ZUP366_VAL_WAVEFORM_SEQUENCE Sinsuoid ZUP366_VAL_WAVEFORM_SINSUOID Staircase ZUP366_VAL_WAVEFORM_STAIRCASE Step ZUP366_VAL_WAVEFORM_STEP Trapezoid ZUP366_VAL_WAVEFORM_TRAPEZOID Undefined ZUP366_VAL_WAVEFORM_UDEFINED               RVoltage ZUP366_VAL_WFM_MEAS_TYPE_VOLTAGE Current ZUP366_VAL_WFM_MEAS_TYPE_CURRENT   M    Sets the shape, pacing, step number, step repeat count, the sequence repeat count and the output value when the sequence terminates.

Notes:
1) Sequence capability is only available on the N6705B DC Power Analyzer.
2) zup366_ConfigureWaveformParameters needs to be set to
ZUP366_VAL_WAVEFORM_SEQUENCE before running this function.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Determines the output value when the sequence terminates. The driver sets the ZUP366_ATTR_SEQUENCE_TERMINATION attribute to this value.

Valid Values:
VI_TRUE  (1) - On -  When ON, the output current/voltage remains at the last sequence value. The last voltage or current waveform value becomes the IMMediate value when the sequence completes.
VI_FALSE (0) - Off - When OFF, and also when the sequence is aborted, the output returns to the settings that were in effect before the sequence started.

Default Value: VI_FALSE

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote commands:
[SOURce:]ARB:SEQuence:TERMinate:LAST <Bool>, (@<chanlist>)    ?    Sets the number of times that the sequence is repeated. The driver sets the ZUP366_ATTR_SEQUENCE_COUNT
attribute to this value.


Valid Range:
1 - 4096 
ZUP366_VAL_SEQUENCE_COUNT_INFINITY (-1) 

Default Value:1

Notes:
1)Use the INFinity parameter to repeat the sequence continuously.
2)Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:SEQuence:COUNt <count>|INFinity, (@<chanlist>)
    ?    Specifies the waveform step within the sequence to be programmed.  The driver sets the ZUP366_ATTR_SEQUENCE_STEP_NUMBER attribute to this value.


Valid Range:
0 - 100

Default Value: 0

Notes:
1) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values.
2) New sequence steps must be specified sequentially. 
3) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence. 
4) When a step is added, all parameters of the step waveform are reset to their default values.
5) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

    ;    Creates a new sequence step or changes the waveform of an existing sequence step. The driver sets the ZUP366_ATTR_SEQUENCE_SHAPE attribute to this value.


Valid Values:
ZUP366_VAL_SEQUENCE_EXPONENTIAL - Specifies an exponential waveform
ZUP366_VAL_SEQUENCE_PULSE - Specifies a pulse.
ZUP366_VAL_SEQUENCE_RAMP - Specifies a ramp.
ZUP366_VAL_SEQUENCE_SINSUOID - Specifies a sine wave.
ZUP366_VAL_SEQUENCE_STAIRCASE - Specifies a staircase.
ZUP366_VAL_SEQUENCE_STEP - Specifies a step.
ZUP366_VAL_SEQUENCE_TRAPEZOID - Specifies a trapezoid.
ZUP366_VAL_SEQUENCE_UDEFINED - Specifies a user-defined waveform.

Default: ZUP366_VAL_SEQUENCE_PULSE

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:SEQuence:STEP:FUNCtion:SHAPe <function>, <step#>, (@<chanlist>)    ?    Specifies the type of pacing for the specified step number. The driver sets the  ZUP366_ATTR_SEQUENCE_PACING attribute to this value.


Valid Values:
ZUP366_VAL_SEQUENCE_PACING_TYPE_DWELL - When dwell paced, the step moves to the next step when the dwell time is finished.
ZUP366_VAL_SEQUENCE_PACING_TYPE_TRIGGERED - When trigger paced, the step waits at the last value of the step until a trigger is received. The next step is started upon receipt of the trigger.

Default Value: ZUP366_VAL_SEQUENCE_PACING_TYPE_DWELL

Notes:
1) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values..
2) New sequence steps must be specified sequentially. 
3) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence + 1 (see Query Total Steps In Sequence). 
4) When a step is added, all parameters of the step waveform are reset to their default values.
5) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:SEQuence:STEP:PACing <pacing>, <step#>, (@<chanlist>)
    A    Sets the number of times that the sequence step is repeated.  The driver sets the ZUP366_ATTR_SEQUENCE_STEP_COUNT attribute to this value.


Valid Range:
1 - 16,777,216 
ZUP366_VAL_SEQUENCE_STEP_COUNT_INFINITY (-1) 


Default Value:1

Notes:
1) Use the INFinity parameter to repeat the waveform continuously
2) Cannot be set to continuous ZUP366_VAL_SEQUENCE_STEP_COUNT_INFINITY when Dwell paced.
3) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer


Remote Commands:
[SOURce:]ARB:SEQuence:STEP:COUNt <count>|INFinity, <step#>, (@<chanlist>)
     ?    Selects either a voltage or current measurement for the sequence. 

Valid Values:
ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE
ZUP366_VAL_SEQ_MEAS_TYPE_CURRENT

Default Value: ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE
   *   ?  ?    Instrument Handle                ?#????  ?    Status                           E %  ?  ?    Channel Name                     ? ? ? ? ?    Enable Sequence Termination      q ?  ? d    Sequence Count                   : p ! ? K    Sequence Step Number              p ? ? d    Sequence Shape                   G p ? ? K    Pacing                           ? pj ? d    Sequence Step Repeat Count       $ % ? ? ?    Sequence Measurement Type              	           ""    On VI_TRUE Off VI_FALSE    1       d                            %Exponential ZUP366_VAL_SEQUENCE_EXPONENTIAL Pulse ZUP366_VAL_SEQUENCE_PULSE Ramp ZUP366_VAL_SEQUENCE_RAMP Sinsuoid ZUP366_VAL_SEQUENCE_SINSUOID Staircase ZUP366_VAL_SEQUENCE_STAIRCASE Step ZUP366_VAL_SEQUENCE_STEP Trapezoid ZUP366_VAL_SEQUENCE_TRAPEZOID Undefined ZUP366_VAL_SEQUENCE_UDEFINED               `Dwell ZUP366_VAL_SEQUENCE_PACING_TYPE_DWELL Triggered ZUP366_VAL_SEQUENCE_PACING_TYPE_TRIGGERED    1               RVoltage ZUP366_VAL_SEQ_MEAS_TYPE_VOLTAGE Current ZUP366_VAL_SEQ_MEAS_TYPE_CURRENT    ?    Configures the exponential waveform or sequence.

Note:
The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    R    Specifies the initial current/voltage (before the exponential waveform starts) in amperes/volts. The driver sets the ZUP366_ATTR_EXP_START_LEVEL attribute to this value.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0   to  20.4 V
        6V  range 0.0   to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0   to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V      
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 A to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 A to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 A to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT
Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:EXPonential:STARt[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:EXPonential:STARt[:LEVel] <voltage>, (@<chanlist>)
    L    Specifies the end level current/voltage (after the exponential waveform ends) in amperes/volts.  The driver sets the ZUP366_ATTR_EXP_END_LEVEL attribute to this value.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.
 
 (for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V       
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT
Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:EXPonential:END[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:EXPonential:END[:LEVel] <voltage> (@<chanlist>)

    ?    Specifies the delay in seconds after the trigger is received, but before the exponential waveform occurs. The driver sets the ZUP366_ATTR_EXP_START_TIME attribute to this value.

Valid Range:
0 - 262.144 

Default Value: 0.0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:EXPonential:STARt:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:EXPonential:STARt:TIMe <time>, (@<chanlist>)
    ?    Specifies the time constant of the curve. The driver sets the ZUP366_ATTR_EXP_TIME_CONSTANT attribute to this value.

Valid Range:
0 - 262.144 

Default Value: 1.0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer


Remote Commands:
[SOURce:]ARB:CURRent:EXPonential:TCONstant <constant>, (@<chanlist>)
[SOURce:]ARB:VOLTage:EXPonential:TCONstant <constant>, (@<chanlist>)
    ?    Specifies the total time for the current or voltage to go from the starting level to the ending level in seconds. The driver sets the ZUP366_ATTR_EXP_TOTAL_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 1.0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:EXPonential:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:EXPonential:TIMe <time>, (@<chanlist>)
    ?    Specifies the waveform step within the sequence to be programmed.

Valid Range:
0 - 100

Default Value: 0

Notes:
1) This command is used when selected Waveform Shape is ZUP366_VAL_WAVEFORM_SEQUENCE specified in zup366_ConfigureWaveformParameters.
2) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values.
3) New sequence steps must be specified sequentially. 
4) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence.
5) When a step is added, all parameters of the step waveform are reset to their default values.
   $?   ?  ?    Instrument Handle                %?????  ?    Status                           ,7 %  ?  ?    Channel Name                     ,? % ? ? K    Exponential Start Level          ; %g ? K    Exponential End Level            IV h  ? K    Exponential Start Time           K1 h ? ? K    Exponential Time Constant        L? hL ? K    Exponential Total Time           N? ? % ? F    Sequence Step Number                   	           ""    0.0    0.0    0.0    1.0    1.0       d                   ?    Configures the pulse waveform or sequence.

Note:
The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    O    Specifies the current or voltage before and after the pulse occurs in amperes or volts. The driver sets the  ZUP366_ATTR_PULS_START_LEVEL attribute to this value.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V      

N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
        
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT
Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4)  Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:PULSe:STARt[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:PULSe:STARt[:LEVel] <voltage>, (@<chanlist>)

    ?    Specifies the amplitude (top level) current or voltage of the pulse in amperes or volts. The driver sets the ZUP366_ATTR_PULS_AMPL_TLEV attribute to this value.


Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.
 
 (for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 0.0 to 20.4 V
N6782A: 0.0 to 20.4 V
N6783A-BAT: 0.0 to 8.0 V
N6783A-MFG: 0.0 to 6.0 V
N6784A: 0.0 to 20.4 V       
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: -3.06 to 3.06 A
N6782A: -3.06 to 3.06 A
N6783A-BAT: -2.0 to + 3.0 A
N6783A-MFG: -2.0 to + 3.0 A
N6784A: -3.06    to  3.06 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:PULSe:TOP[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:PULSe:TOP[:LEVel] <voltage>, (@<chanlist>)



    ?    Specifies the delay in seconds after the trigger is received, but before the pulse occurs. The driver sets the ZUP366_ATTR_PULS_START_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 0.0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:PULSe:STARt:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:PULSe:STARt:TIMe <time>, (@<chanlist>)

    ?    Specifies the time of the pulse in seconds. The driver sets the ZUP366_ATTR_PULS_WIDTH_TTIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 1.0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:PULSe:TOP:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:PULSe:TOP:TIMe <time>, (@<chanlist>)
    ?    Specifies the time in seconds, after the pulse completes, that the start-level current persists. The driver sets the ZUP366_ATTR_PULS_END_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 0.0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:PULSe:END:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:PULSe:END:TIMe <time>, (@<chanlist>)
    ?    Specifies the waveform step within the sequence to be programmed.

Valid Range:
0 - 100

Default Value: 0

Notes:
1) This command is used when selected Waveform Shape is ZUP366_VAL_WAVEFORM_SEQUENCE specified in zup366_ConfigureWaveformParameters.
2) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values.
3) New sequence steps must be specified sequentially. 
4) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence.
5) When a step is added, all parameters of the step waveform are reset to their default values.
   T   ?  ?    Instrument Handle                T?????  ?    Status                           [Z %  ?  ?    Channel Name                     [? % ? ? K    Pulse Start Level                j" %N ? K    Pulse Amplitude                  u? d  ? K    Pulse Start Time                 w? d ? ? K    Pulse Width                      yI dN ? K    Pulse End Time                   { ? + ? K    Sequence Step Number                   	           ""    0.0    0.0    0.0    1.0    0.0       d                   ?    Configures the ramp waveform or sequence.

Note:
The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    C    Specifies the initial current or voltage (before the ramp occurs) in amperes or volts. The driver sets the ZUP366_ATTR_RAMP_START_LEVEL attribute to this value.


Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V        
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT
Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:RAMP:STARt[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:RAMP:STARt[:LEVel] <voltage>, (@<chanlist>)

    B    Specifies the end-level current or voltage (after the ramp occurs) in amperes or volts. The driver sets the ZUP366_ATTR_RAMP_END_LEVEL attribute to this value.


Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.
 
 (for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V       
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT
Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models. 
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:RAMP:END[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:RAMP:END[:LEVel] <voltage>, (@<chanlist>)



    ?    Configures the delay in seconds after the trigger is received, but before the ramp occurs. The driver sets the ZUP366_ATTR_RAMP_START_TIME attribute to this value.
 

Valid Range:
0 - 262.144 

Default Value: 0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:RAMP:STARt:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:RAMP:STARt:TIMe <time>, (@<chanlist>)


    ?    Specifies the rise time of the ramp in seconds. The driver sets the ZUP366_ATTR_RAMP_RISE_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 1

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:RAMP:RTIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:RAMP:RTIMe <time>, (@<chanlist>)
    ?    Specifies the time in seconds, after the ramp completes, that the end-level current persists. The driver sets the ZUP366_ATTR_RAMP_END_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:RAMP:END:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:RAMP:END:TIMe <time>, (@<chanlist>)

    ?    Specifies the waveform step within the sequence to be programmed.

Valid Range:
0 - 100

Default Value: 0

Notes:
1) This command is used when selected Waveform Shape is ZUP366_VAL_WAVEFORM_SEQUENCE specified in zup366_ConfigureWaveformParameters.
2) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values.
3) New sequence steps must be specified sequentially. 
4) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence .
5) When a step is added, all parameters of the step waveform are reset to their default values.
   ?s   ?  ?    Instrument Handle                ?)	????  ?    Status                           ?? %  ?  ?    Channel Name                     ?' % ? ? K    Ramp Start Level                 ?r %6 ? K    Ramp End Level                   ?? v  ? K    Ramp Start Time                  ?} v6 ? K    Ramp Rise Time                   ? v ? ? K    Ramp End Time                    ?? ? - ? K    Sequence Step Number                   	           ""    0.0    0.0    0.0    1.0    0.0       d                   ?    Configures the sine waveform or sequence.

Note:
The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    
?    Specifies the amplitude or peak value of the sine wave in amperes or volts. The driver sets the ZUP366_ATTR_AMPLITUDE
attribute to this value.


Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the module.

alid Range:
(for voltage)
N6731A/B: 0.0 to 2.5 V
N6732A/B: 0.0 to 4.0 V
N6733A/B: 0.0 to 10.0 V
N6734A/B: 0.0 to 17.5 V
N6735A: 0.0 to 25.0 V
N6735B: 0.0 to 30.0 V
N6736B: 0.0 to 50.0 V
N6741B: 0.0 to 2.5 V
N6742A/B: 0.0 to 4.0 V 
N6743A/B: 0.0 to 10.0 V
N6744A/B: 0.0 to 17.5 V
N6745A: 0.0 to 25.0 V
N6745B: 0.0 to 30.0 V
N6746B: 0.0 to 50.0 V
N6773A: 0.0 to 10.0 V
N6774A: 0.0 to 17.5 V
N6775A: 0.0 to 30.0 V
N6776A: 0.0 to 50.0 V
N6777A: 0.0 to 75.0 V
N6751A: 0.0 to 25.0 V
N6752A: 0.0 to 25.0 V
N6753A: 0.0 to 10.0 V
N6754A: 0.0 to 30.0 V
N6755A: 0.0 to 10.0 V
N6756A: 0.0 to 30.0 V
N6761A: 0.0 to 25.0 V
N6762A: 0.0 to 25.0 V
N6763A: 0.0 to 10.0 V
N6764A: 0.0 to 30.0 V
N6765A: 0.0 to 10.0 V
N6766A: 0.0 to 30.0 V
N6781A: 0.0 to 10.0 V
N6782A: 0.0 to 10.0 V
N6783A: 0.0 to 4.0 V
N6784A: -20.4 to  20.4 V  (if negative voltage configured)    
       
(for current)
N6731A/B: 0.0 to 5.0 A
N6732A/B: 0.0 to 3.125 A
N6733A/B: 0.0 to 1.25 A
N6734A/B: 0.0 to 0.75 A
N6735A/B: 0.0 to 0.4 A
N6736B: 0.0 to 0.25 A
N6741B: 0.0 to 10.0 A
N6742A: 0.0 to 5.0 A
N6742B: 0.0 to 6.25 A
N6743A/B: 0.0 to 2.5 A
N6744A/B: 0.0 to 1.5 A
N6745A/B: 0.0 to 0.8 A
N6746B: 0.0 to 0.5 A
N6773A: 0.0 to 7.5 A
N6774A: 0.0 to 4.25 A
N6775A: 0.0 to 2.5 A
N6776A: 0.0 to 1.5 A
N6777A: 0.0 to 1.0 A
N6751A: 0.0 to 2.5 A
N6752A: 0.0 to 5.0 A
N6753A: 0.0 to 25.0 A
N6754A: 0.0 to 10.0 A
N6755A: 0.0 to 25.0 A
N6756A: 0.0 to 8.5 A
N6761A: 0.0 to 0.75 A
N6762A: 0.0 to 1.5 A
N6763A: 0.0 to 25.0 A
N6764A: 0.0 to 10.0 A
N6765A: 0.0 to 25.0 A
N6766A: 0.0 to 8.5 A
N6781A: -3.06 to 3.06 A (if negative current configured)       
N6782A: -3.06 to 3.06 A (if negative current configured)
N6783A: -2.00 to 3.06 A (if negative current configured)           
N6784A: -3.06 to 3.06 A (if negative current configured)  

Default Value: 0.0

Notes:
1) Range specified above for N6783A is for N6783A-BAT Range for  N6783A-MFG is 0.0 to 3.0 V 
2) Except for model N6784A (-20.0 to +20.0 V), the output cannot generate negative voltages. 
3) Except for models N678xA SMU (-3.0 to +3.0 A), the output cannot generate negative currents.
4) In order to get the Maximum range for current/voltage Configure Output Range function will have to run before other current/voltage range functions.
5) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:SINusoid:AMPLitude <amplitude>, (@<chanlist>)
[SOURce:]ARB:VOLTage:SINusoid:AMPLitude <amplitude>, (@<chanlist>)

    L    Specifies the offset of the sine wave from zero, in amperes or volts. The driver sets the ZUP366_ATTR_FREQUENCY
attribute to this value.


Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.
 
 (for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 0.0 to  20.4 V       
N6782A: 0.0  to  20.4 V
N6783A: 0.0 to 8.0 V
N6784A: -20.4 to  20.4 V  (if negative voltage configured)       
       
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: -3.06 to 3.06 A (if negative current configured)       
N6782A: -3.06 to 3.06 A (if negative current configured)
N6783A: -2.00 to 3.06 A (if negative current configured)           
N6784A: -3.06 to 3.06 A (if negative current configured)           
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1) Except for model N6784A (-20.0 to +20.0 V), the output cannot generate negative voltages. 
2) Except for models N678xA SMU (-3.0 to +3.0 A),  the output cannot generate negative currents. Therefore, the offset value cannot be less than the amplitude of the waveform.
3)In order to get the Maximum range for current/voltage Configure Output Range function will have to run before other current/voltage range functions 
4) Range specified above for N6783A is for N6783A-BAT Range for  N6783A-MFG is 0.0 to 6.0 V  
5) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:SINusoid:OFFSet <offset>, (@<chanlist>)
[SOURce:]ARB:VOLTage:SINusoid:OFFSet <offset>, (@<chanlist>)




    ?    Specifies the delay in seconds after the trigger is received, but before the ramp occurs. The driver sets the ZUP366_ATTR_OFFSET attribute to this value.

Valid Range:
3.8147E-5 - 1.0E4 

Default Value: 1

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:SINusoid:FREQuency <frequency>, (@<chanlist>)
[SOURce:]ARB:VOLTage:SINusoid:FREQuency <frequency>, (@<chanlist>)


    ?    Specifies the waveform step within the sequence to be programmed.

Valid Range:
0 - 100

Default Value: 0

Notes:
1) This command is used when selected Waveform Shape is ZUP366_VAL_WAVEFORM_SEQUENCE specified in zup366_ConfigureWaveformParameters.
2) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values.
3) New sequence steps must be specified sequentially. 
4) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence.
5) When a step is added, all parameters of the step waveform are reset to their default values.

   ?*   ?  ?    Instrument Handle                ??
????  ?    Status                           ?m -  ?  ?    Channel Name                     ?? p  ? K    Amplitude                        ?? p ? ? K    Offset                           ?? p ? ? K    Frequency                        Ѯ ?  ? K    Sequence Step Number                   	           ""    0.0    0.0    1       d                   ?    Configures the staircase waveform or sequence.

Note:
The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    P    Specifies the initial current or voltage (before the staircase occurs) in amperes or volts. The driver sets the ZUP366_ATTR_STAIR_START_LEVEL attribute to this value.


Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V    
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STAircase:STARt[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STAircase:STARt[:LEVel] <voltage>, (@<chanlist>)


    ?    Specifies the delay in seconds after the trigger is received, but before the staircase occurs. The driver sets the ZUP366_ATTR_STAIR_START_TIME attribute to this value.
 
Valid Range:
0 - 262.144 

Default Value: 0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STAircase:STARt:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STAircase:STARt:TIMe <time>, (@<chanlist>)


    ?    Specifies the end level current or voltage (after the staircase occurs) in amperes/volts. The current or voltage difference between the starting current or voltage and the ending current or voltage is divided equally between the steps. The driver sets the ZUP366_ATTR_STAIR_END_LEVEL attribute to this value.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V   
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STAircase:END[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STAircase:END[:LEVel] <voltage>, (@<chanlist>)

    ?    Specifies the time in seconds, after the staircase completes, that the end-level current persists. The driver sets the ZUP366_ATTR_STAIR_END_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STAircase:END:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STAircase:END:TIMe <time>, (@<chanlist>)


    ?    Specifies the total time to complete all of the staircase steps in seconds. The driver sets the ZUP366_ATTR_STAIR_TOTAL_TIME
attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 1

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STAircase:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STAircase:TIMe <time>, (@<chanlist>)



    ?    Specifies the number of steps in the staircase. The driver sets the ZUP366_ATTR_STAIR_NSTEPS attribute to this value.

Valid Range:
1 - 500

Default Value: 10

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STAircase:NSTeps <steps>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STAircase:NSTeps <steps>, (@<chanlist>)



    ?    Specifies the waveform step within the sequence to be programmed.

Valid Range:
0 - 100

Default Value: 0

Notes:
1) This command is used when selected Waveform Shape is ZUP366_VAL_WAVEFORM_SEQUENCE specified in zup366_ConfigureWaveformParameters.
2) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values.
3) New sequence steps must be specified sequentially. 
4) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence .
5) When a step is added, all parameters of the step waveform are reset to their default values.
   ֢   ?  ?    Instrument Handle                ?X????  ?    Status                           ?? %  ?  ?    Channel Name                     ?V % ? ? K    Staircase Start Level            ?? z  ? K    Staircase Start Time             ?} %b ? K    Staircase End Level              ?\ z ? ? K    Staircase End Time               ?) z ? ? K    Staircase Total Time              ? z\ ? K    Staircase N Steps                l ?  ? K    Sequence Step Number                   	           ""    0.0    0.0    0.0    0.0    1.0      ?       
              d                   ?    Configures the step waveform or sequence.

Note:
The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    +    Specifies the initial current or voltage level in amperes or volts. The driver sets the ZUP366_ATTR_STEP_START_LEVEL
attribute to this value.


Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V  
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STEP:STARt[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STEP:STARt[:LEVel] <voltage>, (@<chanlist>)


        Specifies the step level in amperes or volts. The driver sets the ZUP366_ATTR_STEP_END_LEVEL attribute to this value.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.
 
 (for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V        
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STEP:END[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STEP:END[:LEVel] <voltage>, (@<chanlist>)




    ?    Specifies the delay in seconds after the trigger is received, but before the step occurs. The driver sets the ZUP366_ATTR_STEP_START_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STEP:STARt:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STEP:STARt:TIMe <time>, (@<chanlist>)



    ?    Specifies the delay in seconds after the trigger is received, but after the step occurs. The driver sets the ZUP366_ATTR_STEP_END_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:STEP:END:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:STEP:END:TIMe <time>, (@<chanlist>)



    ?    Specifies the waveform step within the sequence to be programmed.

Valid Range:
0 - 100

Default Value: 0

Notes:
1) This command is used when selected Waveform Shape is ZUP366_VAL_WAVEFORM_SEQUENCE specified in zup366_ConfigureWaveformParameters.
2) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values.
3) New sequence steps must be specified sequentially. 
4) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence .
5) When a step is added, all parameters of the step waveform are reset to their default values.
   !   ?  ?    Instrument Handle                ?v???  ?    Status                           d +  ?  ?    Channel Name                     ? u  ? K    Step Start Level                  u x ? K    Step End Level                   ,( u ? ? K    Step Start Time                  -? uF ? K    Step End Time                    /? ? # ? K    Sequence Step Number                   	           ""    0.0    0.0    0.0    0.0       d                   ?    Configures the trapezoid waveform or sequence.

Note:
The Waveform and Sequence capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     j    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""

    S    Specifies the initial and final current or voltage level of the trapezoid in amperes or volts. The driver sets the ZUP366_ATTR_TRAP_START_LEVEL attribute to this value.


Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V    
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:TRAPeziod:STARt[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:TRAPeziod:STARt[:LEVel] <voltage>, (@<chanlist>)



    ?    Specifies the delay in seconds after the trigger is received, but before the rising ramp occurs. The driver sets the ZUP366_ATTR_TRAP_START_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:TRAPezoid:STARt:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:TRAPezoid:STARt:TIMe <time>, (@<chanlist>)


    >    Specifies the top level current or voltage of the trapezoid in amperes or volts. The driver sets the ZUP366_ATTR_TRAP_PEAK_TLEV attribute to this value.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V     
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)The additional 600 mV range (0 to 612 mV) is only available in Voltage Priority mode for models N6781A, N6782A, and N6784A.
2) Range specified above for N6783A is for N6783A-BAT Range for  N6783A-MFG is 0.0 to 6.0 V
3) The additional 300 mA (N6781A, N6782A), 100 mA, and 10 mA (N6784A) ranges are only available in Current Priority mode for models.
4) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:TRAPezoid:TOP[:LEVel] <current>, (@<chanlist>)
[SOURce:]ARB:VOLTage:TRAPezoid:TOP[:LEVel] <voltage>, (@<chanlist>)


    ?    Specifies the time in seconds, after the trapezoid completes, that the start-level current persists. The driver sets the ZUP366_ATTR_TRAP_END_TIME attribute to this value.

Valid Range:
0 - 262.144 

Default Value: 0

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:TRAPezoid:END:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:TRAPezoid:END:TIMe <time>, (@<chanlist>)



    ?    Specifies the time of the top level of the trapezoid in seconds.   The driver sets the ZUP366_ATTR_TRAP_WIDTH_TTIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 1

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:TRAPezoid:TOP:TIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:TRAPezoid:TOP:TIMe <time>, (@<chanlist>)



    ?    Specifies the fall time of the trapezoid in seconds. The driver sets the ZUP366_ATTR_TRAP_FALL_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 1

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:TRAPezoid:FTIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:TRAPezoid:FTIMe <time>, (@<chanlist>)



    ?    Specifies the rise time of the trapezoid in seconds. The driver sets the ZUP366_ATTR_TRAP_RISE_TIME attribute to this value.


Valid Range:
0 - 262.144 

Default Value: 1

Notes:
Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:TRAPezoid:RTIMe <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:TRAPezoid:RTIMe <time>, (@<chanlist>)




    ?    Specifies the waveform step within the sequence to be programmed.

Valid Range:
0 - 100

Default Value: 0

Notes:
1) This command is used when selected Waveform Shape is ZUP366_VAL_WAVEFORM_SEQUENCE specified in zup366_ConfigureWaveformParameters.
2) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values.
3) New sequence steps must be specified sequentially. 
4) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence .
5) When a step is added, all parameters of the step waveform are reset to their default values.
   4?$   ?  ?    Instrument Handle                5?????  ?    Status                           <   ?  ?    Channel Name                     <? \ % ? K    Trapezoid Start Level            J? \ ? ? K    Trapezoid Start Time             L? \ ? ? K    Trapezoid Peak                   Z? ? 	? K    Trapezoid End Time               \? ? % ? K    Trapezoid Width                  ^w ? ? ? K    Trapezoid Fall Time              ` ^q ? K    Trapezoid Rise Time              a? ? - ? K    Sequence Step Number                   	           ""    0.0    0.0    0.0    0.0    1.0    1.0    1.0       d                   w    Configures the constant dwell waveform.

Note:
Waveform capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Pass the number of elements in the ViReal64 array you specify for the Constant Dwell Level parameter.

Valid Range:
  1 to 512

Default Value: None
    ?    Specifies the level of each point of a constant-dwell waveform.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

Units: volts (for voltage range)
       amps  (for current range)

Valid Range:
(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 0.0 to 20.4 V
N6782A: 0.0 to 20.4 V
N6783A: 0.0 to 8.0 V
N6784A: 0.0 to 20.4 V       
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05 to 51 A             
N6764A: 0.02 to 20.4 A       
N6765A: 0.05 to 51 A        
N6766A: 0.02 to 17.3 A 
N6773A  0.00 to 15A
N6774A  0.00 to 8.5 A
N6775A  0.00 to 5.0 A
N6776A  0.00 to 3.0 A         
N6777A: 0.00 to 2.0 A
N6781A: -3.06 to 3.06 A
N6782A: -3.06 to 3.06 A
N6783A: 0.0 to 3.0 A
N6784A: -3.06   to 3.06 A
N6731B  0.06    to  10.2 A
N6732B  0.04    to  6.375 A
N6733B  0.01    to  2.55 A
N6734B  0.005   to  1.53 A
N6735B  0.0025  to  0.85 A
N6736B  0.0015  to  0.51 A
N6741B  0.06    to  20.4 A
N6742B  0.04    to  12.75 A
N6743B  0.01    to  5.1 A
N6744B  0.005   to  3.06 A
N6745B  0.0025  to  1.7 A
N6746B  0.0015  to  1.02 A


Notes:
1) You must pass a ViReal64 array with at least List Size elements.
2) Current and voltage constant-dwell waveforms share settings, so setting the current constant-dwell waveform resets the voltage constant-dwell waveform level to its default value and vice-versa.
3) A list of up to 65,535 points may be programmed.
4) Waveform capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:[CURRent:CDWell[:LEVel] <current>{,<current>}, (@<chanlist>)
[SOURce:]ARB:VOLTage:CDWell[:LEVel] <voltage>{,<voltage>}, (@<chanlist>)    k    Specifies the dwell time for each point of a constant-dwell waveform. 

Units: seconds

Valid Range:
0.00001024 - 0.30 

Default Value: 0.001 (with 1 point)

Notes:
1) Values are rounded to the nearest 10.24-microsecond increment.
2) Current and voltage constant-dwell waveforms share settings, so setting this parameter for a current constant-dwell waveform also changes the voltage dwell value and vice versa.
3) Waveform capabilities are only available on the N6705B DC Power Analyzer

Remote commands:
[SOURce:]ARB:CURRent:CDWell:DWELl <time>, (@<chanlist>)
[SOURce:]ARB:VOLTage:CDWell:DWELl <time>, (@<chanlist>)   gz   ?  ?    Instrument Handle                h0????  ?    Status                           n? (  ?  ?    Channel Name                     o. f  ?  ?    Constant Dwell Level Wfm Size    o? f ? ?  ?    Constant Dwell Level             |g ?  ?  ?    Constant Dwell Time                    	           ""            0.001    u    Configures the user defined waveform.

Note:
Waveform capability is only available on the N6705B DC Power Analyzer.     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Pass the number of elements in the ViReal64 array you specify for the User Defined Level parameter.

Valid Range:
  1 to 512

Default Value: None
    ?    Specifies the current/voltage setting for each user-defined waveform point in amperes/volts.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

 (for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 0.0 to 20.4 V
N6782A: 0.0 to 20.4 V
N6783A: 0.0 to 8.0 V
N6784A: 0.0 to 20.4 V       
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: -3.06 to 3.06 A
N6782A: -3.06 to 3.06 A
N6783A: 0.0 to 3.0 A
N6784A: -3.06    to  3.06 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Notes:
1) You must pass a ViReal64 array with at least List Size elements.
2) Waveform capabilities are only available on the N6705B DC Power Analyzer


Remote Commands:
[SOURce:]ARB:SEQuence:STEP:CURRent:UDEFined:LEVel <current>{,<current>}, (@<chanlist>)
[SOURce:]ARB:SEQuence:STEP:VOLTage:UDEFined:LEVel <voltage>{,<voltage>}, (@<chanlist>)

     ?    Pass the number of elements in the ViReal64 array you specify for the User Defined Dwell Time List parameter.

Valid Range:
  1 to 512

Default Value: None
    ?    Specifies the dwell time for each user-defined current or voltage waveform point. Dwell time is the time that the output will remain at a specific point.

Units: seconds

Valid Range:
0 - 262.144 (All except N678xA SMU) 
0 - 268.435 (N678xA SMU only)

(N678xA SMU only)
Range                   Resolution
  0 to 0.268435        1.024 microsecond
  0 to 2.68435         10.24 microseconds
  0 to 26.8435         102.4 microseconds
  0 to 268.435         1.024 millisecond

(All Models except N678xA)
Range                   Resolution
  0 to 0.262144        1 microsecond
  0 to 2.62144         10 microseconds
  0 to 26.2144         100 microseconds
  0 to 262.144         1 millisecond


Notes:
1) You must pass a ViReal64 array with at least List Size elements.
2) At the end of the dwell time, the output state of the unit depends upon the Enable Wavefrom Termination (Configure Waveform Parameters function) settings.
3) The order in which the dwell values are entered determines the sequence of execution.
4) To create a valid user-defined waveform, the User Defined Dwell Time, Beginning Step, and User Defined Level lists must be either all the same length or have a length of 1.
5) Waveform capabilities are only available on the N6705B DC Power Analyzer

Remote commands:
[SOURce:]ARB:SEQuence:STEP:CURRent:UDEFined:DWELl <time>{,<time>}, (@<chanlist>)
[SOURce:]ARB:SEQuence:STEP:VOLTage:UDEFined:DWELl <time>{,<time>}, (@<chanlist>)

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors    ?    Specifies which user-defined waveform points or steps generate a trigger-out signal at the beginning of the step (BOSTep).

Valid Values:
0 or 1

Default Value: 1

Notes:
1) You must pass a ViReal64 array with at least List Size elements.
2) Waveform capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:CURRent:UDEFined:BOSTep[:DATA] <Bool>{,<Bool>}, (@<chanlist>)
[SOURce:]ARB:VOLTage:UDEFined:BOSTep[:DATA] <Bool>{,<Bool>}, (@<chanlist>)




     ?    Pass the number of elements in the ViReal64 array you specify for the Beginning Step parameter.

Valid Range:
  1 to 512

Default Value: None
   ?? %  ?  ?    Channel Name                     ?- ?  ?  ?    User Defined Level Size          ?? ? ? ?  ?    User Defined Level               ?n f  ?  ?    User Defined Dwell Time Size     ? f ? ?  ?    User Defined Dwell Time          ??#   ?  ?    Instrument Handle                ?x????  ?    Status                           ? %y ? K    User Defined Beginning Step      ?? % ? ?  ?    User Defined BOST Size             ""                        	                   u    Configures the user defined sequence.

Note:
Sequence capability is only available on the N6705B DC Power Analyzer.     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Pass the number of elements in the ViReal64 array you specify for the Sequence UDef Level List parameter.

Valid Range:
  1 to 512

Default Value: None
    ?    Specifies the current/voltage setting for each user-defined waveform point in amperes/volts.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

 (for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 0.0 to 20.4 V
N6782A: 0.0 to 20.4 V
N6783A: 0.0 to 8.0 V
N6784A: 0.0 to 20.4 V       
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: -3.06 to 3.06 A
N6782A: -3.06 to 3.06 A
N6783A: 0.0 to 3.0 A
N6784A: -3.06    to  3.06 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Notes:
1) You must pass a ViReal64 array with at least List Size elements.
2) Waveform/Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote Commands:
[SOURce:]ARB:SEQuence:STEP:CURRent:UDEFined:LEVel <current>{,<current>}, (@<chanlist>)
[SOURce:]ARB:SEQuence:STEP:VOLTage:UDEFined:LEVel <voltage>{,<voltage>}, (@<chanlist>)

     ?    Pass the number of elements in the ViReal64 array you specify for the Sequence UDef Dwell Time List parameter.

Valid Range:
  1 to 512

Default Value: None
    ?    Specifies the dwell time for each user-defined current\voltage waveform point. Dwell time is the time that the output will remain at a specific point.

Units: seconds

Valid Range:
0 - 262.144 (all except N678xA SMU) 
0 - 268.435 (N678xA SMU only)

N678xA SMU only)
Range                   Resolution
  0 to 0.268435        1.024 microsecond
  0 to 2.68435         10.24 microseconds
  0 to 26.8435         102.4 microseconds
  0 to 268.435         1.024 millisecond

(All Models except N678xA)
Range                   Resolution
  0 to 0.262144        1 microsecond
  0 to 2.62144         10 microseconds
  0 to 26.2144         100 microseconds
  0 to 262.144         1 millisecond


Notes:
1) You must pass a ViReal64 array with at least List Size elements.
2) At the end of the dwell time, the output state of the unit depends upon the Enable Wavefrom Termination (Configure Waveform Parameters function) settings.
3) The order in which the dwell values are entered determines the sequence of execution.
4) To create a valid user-defined waveform, the User Defined Dwell Time, Beginning Step, and User Defined Level lists must be either all the same length or have a length of 1.
5) Sequence capabilities are only available on the N6705B DC Power Analyzer

Remote commands:
[SOURce:]ARB:SEQuence:STEP:CURRent:UDEFined:DWELl <time>{,<time>}, (@<chanlist>)
[SOURce:]ARB:SEQuence:STEP:VOLTage:UDEFined:DWELl <time>{,<time>}, (@<chanlist>)

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors        Specifies which user-defined waveform points or steps generate a trigger-out signal at the beginning of the step (BOSTep).

Valid Values:
0 or 1

Default Value: 1

Notes:
1) You must pass a ViReal64 array with at least List Size elements.
2) Sequence capabilities are only available on the N6705B DC Power Analyzer


Remote Commands:
[SOURce:]ARB:SEQuence:STEP:CURRent:UDEFined:BOSTep[:DATA] <Bool>{,<Bool>}, (@<chanlist>)
[SOURce:]ARB:SEQuence:STEP:VOLTage:UDEFined:BOSTep[:DATA] <Bool>{,<Bool>}, (@<chanlist>)




     ?    Pass the number of elements in the ViReal64 array you specify for the Sequence Beginning Step parameter.

Valid Range:
  1 to 512

Default Value: None
        Specifies the waveform step within the sequence to be programmed.

Valid Range:
0 - 100

Default Value: 0

Notes:
1) If the Waveform Step specified is an existing sequence step, that step will be changed to the specified waveshape with all parameters reset to default values.
2) New sequence steps must be specified sequentially. 
3) To create a new sequence step, Waveform Step should be specified to be the current length of the sequence .
4) When a step is added, all parameters of the step waveform are reset to their default values.
   ? ,  ?  ?    Channel Name                     ?? ?  ?  ?    Sequence UDef Dwell Level Size   ?) ? ? ?  ?    Sequence UDef Dwell Level        ?? y  ?  ?    Sequence UDef Dwell Time Size    ?} y ? ?  ?    Sequence UDef Dwell Time         ?'   ?  ?    Instrument Handle                ??????  ?    Status                           ?j ,? ? K    Sequence UDef Beginning Step     ?x - ? ?  ?    Sequence UDef BOST Size          ? ?j ? K    Sequence Step Number               ""                        	                      d                   ?    Configures the data log type, duration, period, offset and sets the state of the data log function.

Note:
This function only applies to the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     j    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""

    i    Configures the datalog trigger offset as a percent of the total datalog duration. Programming a trigger offset lets you specify the percent of pre-trigger data that will be logged to the datalog file. The driver sets the ZUP366_ATTR_DATALOG_OFFSET attribute to this value.

  0% trigger occurs at the beginning  
100% trigger occurs at the end of the running datalog.

Valid Range: 
0-100%

Default Value: 0

Notes:
1) The offset must be set to zero when interleaved data logging is in effect.
2)  Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
SENSe:DLOG:OFFSet <percent>


    ?    Specifies the period of the datalog samples in seconds. The driver sets the ZUP366_ATTR_DATALOG_PERIOD attribute to this value.

Valid Range: 
0.000020 - 60.0  

Default Value: 0.1

Notes:
1) The actual minimum is (20.48 microseconds x number of parameters being logged at each interval). So for example if average voltage, minimum voltage, maximum voltage, and average current are being logged for a single channel, the minimum logging period is (4 parameters X 20.48) = 81.9 microseconds.
2) This function cannot be set to values less than 75 milliseconds when interleaved data logging is in effect.
3) Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
SENSe:DLOG:PERiod <seconds>


    +    Specifies the duration of the datalog in seconds. The driver sets the ZUP366_ATTR_DATALOG_DURATION attribute to this value.

Valid Range: 
1 - 3,599,964,000 

Default Value: 30

Notes:
1)The resulting datalog file cannot exceed 2E9 bytes (1.87 Gbytes in Microsoft Windows units). If the duration value causes the file to exceeds this size, the time interval will automatically be adjusted to a longer interval to reduce the file size.
2)Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
SENSe:DLOG:TIME <seconds>



    ?    Enables current or voltage data logging On or Off. The driver sets the ZUP366_ATTR_DATALOG_ENABLE attribute to this value.

Valid Values:
VI_TRUE  - Enable data logging
VI_FALSE - Disable data logging

Default Value: VI_FALSE

Notes:
Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
SENSe:DLOG:FUNCtion:VOLTage <Bool>, (@<chanlist>)
SENSe:DLOG:FUNCtion:CURRent <Bool>, (@<chanlist>)


     ?    Selects either a voltage or current measurement for the datalogger. 

Valid Values:
ZUP366_VAL_DL_MEAS_TYPE_VOLTAGE
ZUP366_VAL_DL_MEAS_TYPE_CURRENT

Default Value: ZUP366_VAL_DL_MEAS_TYPE_VOLTAGE   ?D   ?  ?    Instrument Handle                ??????  ?    Status                           ɇ %  ?  ?    Channel Name                     ?? ? % ? K    Datalog Offset                   ?j p ? ? K    Datalog Period                   ?F p ? ? K    Datalog Duration                 ?y p 6 ?       Datalog Enabled                  ?* % ? ? ?    Datalog Meaurement Type                	           ""       d                   0.1    30    True 1 False 0              PVoltage ZUP366_VAL_DL_MEAS_TYPE_VOLTAGE Current ZUP366_VAL_DL_MEAS_TYPE_CURRENT    y    Configures the measurement range of the data logger.

Note:
This function only applies to the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Sets the internal data log current or voltage range measurement range on models that have multiple ranges. The value that you enter must be the highest value in amperes or volts that you expect to measure. The driver sets the ZUP366_ATTR_DATALOG_RANGE attribute to this value.

Valid Range:
 0 - maximum The maximum value is dependent on the current/voltage rating of the power module.

(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V         
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V

(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0.0

Notes:
1)  Range specified above for N6783A for voltage is for N6783A-BAT the range for N6783A-MFG is 0.0 to 6.0 V
2) Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
SENSe:DLOG:CURRent[:DC]:RANGe <range>, (@<chanlist>)
SENSe:DLOG:VOLTage[:DC]:RANGe <range>, (@<chanlist>)

    e    Enables or disables the internal datalog for current or voltage autoranging measurement. Autoranging provides seamless measurement ranging, which results in a wide dynamic range with no data being lost across ranges. The driver sets the ZUP366_ATTR_DATALOG_AUTO_RANGE attribute to this value.

Valid Values:
VI_TRUE  - Enable Auto ranging for datalog measurement
VI_FALSE - Disable Auto ranging for datalog measurement

Default Value: VI_FALSE

Notes:
1) This command only applies to models N6781A and N6782A.
2) When enabled, this command overrides the Datalog Range setting.
3) Autoranging does not include the 10 ?A range, which must be selected individually.
4) Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
SENSe:DLOG:CURRent[:DC]:RANGe:AUTO <Bool>, (@<chanlist>)
SENSe:DLOG:VOLTage[:DC]:RANGe:AUTO <Bool>, (@<chanlist>)
   ??"   ?  ?    Instrument Handle                ׁ????  ?    Status                           ? %  ?  ?    Channel Name                     ? ? ? ? }    Datalog Manual Range             ?& ? C ?       Datalog Auto Range                     	           ""    0.0    True 1 False 0    s    Sets the position of the data logger markers. 

Note:
This function only applies to the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors        Sets the position of the data logger markers. Positions are set in seconds relative to the trigger position. A negative time gives positions before the trigger. The marker positions are adjusted to the closest actual sample point. The driver sets the ZUP366_ATTR_DATALOG_MARKER1 attribute to this value.


Valid Range:
3,599,964,000 to +3,599,964,000

Default Value:            
 7.5 s - for marker 1 

Notes:
Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
SENSe:DLOG:MARKer1:POINt <seconds>

        Set the position of the data logger markers. Positions are set in seconds relative to the trigger position. A negative time gives positions before the trigger. The marker positions are adjusted to the closest actual sample point. The driver sets the ZUP366_ATTR_DATALOG_MARKER2 attribute to this value.

Valid Range:
3,599,964,000 to +3,599,964,000

Default Value:            
27.5 s - for marker 2

Notes:
Datalog capability is only available on the N6705B DC Power Analyzer

Remote Commands:
SENSe:DLOG:MARKer2:POINt <seconds>

   ?F   ?  ?    Instrument Handle                ??????  ?    Status                           ?? p W ?  K    Datalog Marker 1                 ?? p ? ?  K    Datalog Marker 2                       	           7.5    27.5    #    Configures the external data log.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     X    Specifies the name of the channel.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Passes the type of external data log you want to configure.

Valid Values:
ZUP366_VAL_ELOG_TYPE_CURRENT - Current
ZUP366_VAL_ELOG_TYPE_VOLTAGE - Voltage

Default Value: 
ZUP366_VAL_ELOG_TYPE_VOLTAGE - Voltage    u    Enables or disables the external datalog current/voltage measurement autoranging. The driver uses this value to set the ZUP366_ATTR_AUTO_ELOG_CURRENT_RANGE attribute.

Valid Values:
VI_TRUE - Enable
VI_FALSE - Disable

Default Value: VI_FALSE

SCPI Command:
SENSe:ELOG:CURRent[:DC]:RANGe:AUTO <Bool>, (@<chanlist>)
SENSe:ELOG:VOLTage[:DC]:RANGe:AUTO <Bool>, (@<chanlist>)    ?    Sets the external datalog voltage or current measurement range on models that have multiple ranges. The value that you enter must be the highest value in volts or amperes that you expect to measure. The driver uses this value to set the ZUP366_ATTR_ELOG_CURRENT_RANGE attribute.

Units: volts (for voltage range)
           amps (for current range)

Valid Range:
(for voltage)
N6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V         
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7 V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V
N6785A  0.00   to  20.0 V
N6786A  0.00   to  20.0 V


(for current)
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to 51 A             
N6764A: 0.02  to 20.4 A       
N6765A: 0.05  to 51 A        
N6766A: 0.02  to 17.3 A 
N6773A  0.00  to 15A
N6774A  0.00  to 8.5 A
N6775A  0.00  to 5.0 A
N6776A  0.00  to 3.0 A         
N6777A: 0.00  to 2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6785A  0.00     to  8.0 A
N6786A  0.00     to  8.0 A
N6731B  0.06     to  10.2 A
N6732B  0.04     to  6.375 A
N6733B  0.01     to  2.55 A
N6734B  0.005    to  1.53 A
N6735B  0.0025   to  0.85 A
N6736B  0.0015   to  0.51 A
N6741B  0.06     to  20.4 A
N6742B  0.04     to  12.75 A
N6743B  0.01     to  5.1 A
N6744B  0.005    to  3.06 A
N6745B  0.0025   to  1.7 A
N6746B  0.0015   to  1.02 A 

Default Value: 0

SCPI Command:
SENSe:ELOG:CURRent[:DC]:RANGe[:UPPer] <range>, (@<chanlist>)
SENSe:ELOG:VOLTage[:DC]:RANGe[:UPPer] <range>, (@<chanlist>)    6    Turns external voltage/current data logging ON or OFF on the specified output channel. The driver uses this value to set the ZUP366_ATTR_ENABLE_CURRENT_ELOG attribute.

Valid Values:
VI_TRUE - Enable
VI_FALSE - Disable

Default Value: VI_FALSE

SCPI Command:
SENSe:ELOG:FUNCtion:VOLTage <Bool>, (@<chanlist>)    ?    Turns external voltage/current data logging of the minimum and maximum voltage/current measurements ON or OFF on the specified output channel. The driver uses this value to set the ZUP366_ATTR_ENABLE_CURRENT_MINMAX_ELOG attribute.

Valid Values:
VI_TRUE - Enable
VI_FALSE - Disable

Default Value: VI_FALSE

SCPI Command:
SENSe:ELOG:FUNCtion:CURRent:MINMax <Bool>, (@<chanlist>)
SENSe:ELOG:FUNCtion:VOLTage:MINMax <Bool>, (@<chanlist>)        Specifies the logging period (or integration) time of an external datalog measurement. The driver uses this value to set the ZUP366_ATTR_ELOG_PERIOD attribute.

Units: seconds

Valid Range:
0.0001024 to 60

Default Value: 0.1

SCPI Command:
SENSe:ELOG:PERiod <seconds>, (@<chanlist>)    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

   ??-   ?  ?    Instrument Handle                ?? -  ?  ?    Channel Name                     ?? - ? ? ?    External Data Log Type           ?? -? ?  ?    Auto Range                       O ?  ?  ?    Measurement Range                ? ? ?  ?    Enable External Data Log         ? ?? ?  ?    Enable Min Max Log               ? ?  ?  ?    External Data Log Period         ?#????  ?    Status                                 ""              JCurrent ZUP366_VAL_ELOG_TYPE_CURRENT Voltage ZUP366_VAL_ELOG_TYPE_VOLTAGE    True VI_TRUE False VI_FALSE    0    True VI_TRUE False VI_FALSE    True VI_TRUE False VI_FALSE    0.1    	            ?    Configures the histogram function of the power analyzer.
        
Notes:  
1) Only applies to models N6781A and N6782A that have the Data Logger installed.
2) The histogram function only applies to the N6705B DC Power Analyzer.          
    ?    Selects the current histogram measurement range. The value that you enter must be the highest value in amperes that you expect to measure. The driver sets the ZUP366_ATTR_HISTOGRAM_RANGE attribute to this value.

Units: amps

Valid Range:
-3.00 to 3.00 A

Default Value: 0.0

Notes:
1) This command only applies to models N6781A and N6782A that have the Data Logger function installed. 

Remote commands:
SENSe:HISTogram:CURRent[:DC]:RANGe <range>, (@<chanlist>)     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Turns the current histogram function On or Off on the specified output channel. The driver sets the ZUP366_ATTR_HISTOGRAM_ENABLED attribute to this value.

Valid Values:
VI_TRUE  
VI_FALSE 

Default Value: VI_FALSE

Notes:
1) Only applies to models N6781A and N6782A that have the Data Logger function installed.

Remote Commands:
SENSe:HISTogram:FUNCtion:CURRent <Bool>, (@<chanlist>)
    R    Enables or disables histogram current measurement autoranging. Autoranging provides seamless measurement ranging, which results in a wide dynamic range with no data being lost across ranges. The driver sets the ZUP366_ATTR_HISTOGRAM_AUTO_RANGE attribute to this value.

Valid Values:
VI_TRUE  
VI_FALSE 

Default Value: VI_FALSE

Notes:
1) Only applies to models N6781A and N6782A that have the Data Logger function installed.
2) Autoranging does not include the 10 ?A range, which must be selected individually.

Remote Commands:
SENSe:HISTogram:CURRent[:DC]:RANGe:AUTO <Bool>, (@<chanlist>)   ? ? ? ?  ?    Histogram Range                  ?   ?  ?    Instrument Handle                ~w???  ?    Status                           & %  ?  ?    Channel Name                     &| % ? ?       Enable Historgram                ( ? 5 ?       Histogram Auto Range               0.0        	           ""    True 1 False 0    True 1 False 0   ?    This function sets the value of a ViInt32 attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none    C    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViInt32 type.   
  If you choose to see all IVI attributes, the data types appear
  to the right of the attribute names in the list box. 
  Attributes with data types other than ViInt32 are dim.  If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
        If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   0?-   ?  ?    Instrument Handle                1j#????  ?    Status                           7? ? ? ?  ?    Attribute Value                 ???? ? ???                                          : = ? ? ?    Attribute ID                     ?^ =  ?  ?    Channel Name                           	               .Press <ENTER> for a list of 
value constants.                0    ""   ?    This function sets the value of a ViReal64 attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none    B    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViReal64
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box.
  Attributes with data types other than ViReal64 are dim.  If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
        If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   F?-   ?  ?    Instrument Handle                G?#????  ?    Status                           N ? ? ?  ?    Attribute Value                 ???? ? ???                                          P6 = ? ? ?    Attribute ID                     U? =  ?  ?    Channel Name                           	               .Press <ENTER> for a list of 
value constants.                0    ""   ?    This function sets the value of a ViString attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
       
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none    A    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViString
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box.
  Attributes with data types other than ViString are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
        If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   \?-   ?  ?    Instrument Handle                ]?#????  ?    Status                           d; ? ? ?  ?    Attribute Value                 ???? ? ???                                          fV = ? ? ?    Attribute ID                     k? =  ?  ?    Channel Name                           	               .Press <ENTER> for a list of 
value constants.                0    ""   ?    This function sets the value of a ViBoolean attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none    C    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViBoolean
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box.
  Attributes with data types other than ViBoolean are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
        If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   s-   ?  ?    Instrument Handle                s?#????  ?    Status                           z] ? ? ?  ?    Attribute Value                 ???? ? ???                                          |x = ? ? ?    Attribute ID                     ?? =  ?  ?    Channel Name                           	               .Press <ENTER> for a list of 
value constants.                0    ""   ?    This function sets the value of a ViSession attribute.

This is a low-level function that you can use to set the values of instrument-specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid or is different than the value you specify. 

This instrument driver contains high-level functions that set most of the instrument attributes.  It is best to use the high-level driver functions as much as possible.  They handle order dependencies and multithread locking for you.  In addition, they perform status checking only after setting all of the attributes.  In contrast, when you set multiple attributes using the SetAttribute functions, the functions check the instrument status after each call.

Also, when state caching is enabled, the high-level functions that configure multiple attributes perform instrument I/O only for the attributes whose value you change.  Thus, you can safely call the high-level functions without the penalty of redundant instrument I/O.


     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

        Pass the value to which you want to set the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none        If the attribute is channel-based, this parameter specifies the name of the channel on which to set the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    C    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViSession
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box.
  Attributes with data types other than ViSession are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   ?=-   ?  ?    Instrument Handle                ??#????  ?    Status                           ?? ? ? ?  ?    Attribute Value                  ?? =  ?  ?    Channel Name                     ?? = ? ? ?    Attribute ID                    ???? ? ???                                                	               ""                0    .Press <ENTER> for a list of 
value constants.   ?    This function queries the value of a ViInt32 attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    ?    Returns the current value of the attribute.  Pass the address of a ViInt32 variable.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViInt32 type. 
  If you choose to see all IVI attributes, the data types appear
  to the right of the attribute names in the list box.  
  Attributes with data types other than ViInt32 are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   ?I-   ?  ?    Instrument Handle                ??#????  ?    Status                           ?? ? ? ?  ?    Attribute Value                  ?K =  ?  ?    Channel Name                     ?X = ? ? ?    Attribute ID                           	           	            ""                0   ?    This function queries the value of a ViReal64 attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
       
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    ?    Returns the current value of the attribute.  Pass the address of a ViReal64 variable.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViReal64
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViReal64 are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   ??-   ?  ?    Instrument Handle                ??#????  ?    Status                           ?5 ? ? ?  ?    Attribute Value                  ?? =  ?  ?    Channel Name                     ? = ? ? ?    Attribute ID                           	           	           ""                0   4    This function queries the value of a ViString attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

You must provide a ViChar array to serve as a buffer for the value.  You pass the number of bytes in the buffer as the Buffer Size parameter.  If the current value of the attribute, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you want to call this function just to get the required buffer size, you can pass 0 for the Buffer Size and VI_NULL for the Attribute Value buffer.  

If you want the function to fill in the buffer regardless of the   number of bytes in the value, pass a negative number for the Buffer Size parameter.  


     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    D    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

If the current value of the return buffer, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    &    The buffer in which the function returns the current value of the attribute.  The buffer must be of type ViChar and have at least as many bytes as indicated in the Buffer Size parameter.

If the current value of the attribute, including the terminating NUL byte, contains more bytes that you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you specify 0 for the Buffer Size parameter, you can pass VI_NULL for this parameter.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the number of bytes in the ViChar array you specify for the Attribute Value parameter.  

If the current value of the attribute, including the terminating NUL byte, contains more bytes that you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Attribute Value buffer parameter.

Default Value: 512    ?    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViString
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViString are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   ?-   ?  ?    Instrument Handle                ??#????  ?    Status or Required Size          ? ? L ? ?    Attribute Value                  ?K =  ?  ?    Channel Name                     ?X =? ?  ?    Buffer Size                      ?N = ? ? ?    Attribute ID                           	           	            ""    512                0   ?    This function queries the value of a ViBoolean attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
       
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    ?    Returns the current value of the attribute.  Pass the address of a ViBoolean variable.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViBoolean
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViBoolean are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   ?'-   ?  ?    Instrument Handle                ??#????  ?    Status                           ?i ? ? ?  ?    Attribute Value                  ?) =  ?  ?    Channel Name                     ?6 = ? ? ?    Attribute ID                           	           	            ""                0   ?    This function queries the value of a ViSession attribute.

You can use this function to get the values of instrument- specific attributes and inherent IVI attributes.  If the attribute represents an instrument state, this function performs instrument I/O in the following cases:

- State caching is disabled for the entire session or for the particular attribute.

- State caching is enabled and the currently cached value is invalid. 

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    ?    Returns the current value of the attribute.  Pass the address of a ViSession variable.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has named constants as valid values, you can view a
  list of the constants by pressing <ENTER> on this control.  
  Select a value by double-clicking on it or by selecting it and 
  then pressing <ENTER>.  
        If the attribute is channel-based, this parameter specifies the name of the channel on which to obtain the value of the attribute. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Help text is
  shown for each attribute.  Select an attribute by 
  double-clicking on it or by selecting it and then pressing
  <ENTER>.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViSession
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViSession are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   ??-   ?  ?    Instrument Handle                ??#????  ?    Status                           ? ? ? ?  ?    Attribute Value                  ?? =  ?  ?    Channel Name                     ?? = ? ? ?    Attribute ID                           	           	            ""                0    S    This function checks the validity of a value you specify for a ViInt32 attribute.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     ?    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    @    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViInt32 type. 
  If you choose to see all IVI attributes, the data types appear
  to the right of the attribute names in the list box. 
  Attributes with data types other than ViInt32 are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   ?!-   ?  ?    Instrument Handle                ??#????  ?    Status                           e ? ? ?  ?    Attribute Value                  ? =  ?  ?    Channel Name                    ???? ? ???                                          ? = ? ? ?    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    T    This function checks the validity of a value you specify for a ViReal64 attribute.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
       
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     ?    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    B    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViReal64
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViReal64 are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   ?-   ?  ?    Instrument Handle                ?#????  ?    Status                            ? ? ?  ?    Attribute Value                  K =  ?  ?    Channel Name                    ???? ? ???                                          P = ? ? ?    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    T    This function checks the validity of a value you specify for a ViString attribute.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    }    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None

This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     ?    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    B    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViString
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViString are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   !?-   ?  ?    Instrument Handle                "G#????  ?    Status                           (? ? ? ?  ?    Attribute Value                  *? =  ?  ?    Channel Name                    ???? ? ???                                          , = ? ? ?    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    U    This function checks the validity of a value you specify for a ViBoolean attribute.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     ?    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    D    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViBoolean
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViBoolean are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   3C-   ?  ?    Instrument Handle                3?#????  ?    Status                           :? ? ? ?  ?    Attribute Value                  <? =  ?  ?    Channel Name                    ???? ? ???                                          =? = ? ? ?    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    U    This function checks the validity of a value you specify for a ViSession attribute.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

    (    Pass the value which you want to verify as a valid value for the attribute.

From the function panel window, you can use this control as follows.

- If the attribute currently showing in the Attribute ID ring
  control has constants as valid values, you can view a list of
  the constants by pressing <ENTER> on this control.  Select a
  value by double-clicking on it or by selecting it and then
  pressing <ENTER>.  

  Note:  Some of the values might not be valid depending on the
  current settings of the instrument session.

Default Value: none     ?    If the attribute is channel-based, this parameter specifies the name of the channel on which to check the attribute value. If the attribute is not channel-based, then pass VI_NULL or an empty string.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    D    Pass the ID of an attribute.

From the function panel window, you can use this control as follows.

- Click on the control or press <ENTER>, <spacebar>, or
  <ctrl-down arrow>, to display a dialog box containing a
  hierarchical list of the available attributes.  Attributes 
  whose value cannot be set are dim.  Help text is shown for 
  each attribute.  Select an attribute by double-clicking on it  
  or by selecting it and then pressing <ENTER>.

  Read-only attributes appear dim in the list box.  If you 
  select a read-only attribute, an error message appears.

  A ring control at the top of the dialog box allows you to see 
  all IVI attributes or only the attributes of the ViSession
  type.  If you choose to see all IVI attributes, the data types
  appear to the right of the attribute names in the list box. 
  Attributes with data types other than ViSession are dim. If
  you select an attribute data type that is dim, LabWindows/CVI
  transfers you to the function panel for the corresponding
  function that is consistent with the data type.

- If you want to enter a variable name, press <CTRL-T> to change
  this ring control to a manual input box.

- If the attribute in this ring control has named constants as  
  valid values, you can view the constants by moving to the 
  Attribute Value control and pressing <ENTER>.
   E-   ?  ?    Instrument Handle                E?#????  ?    Status                           LF ? ? ?  ?    Attribute Value                  Nv =  ?  ?    Channel Name                    ???? ? ???                                          O{ = ? ? ?    Attribute ID                           	               ""    .Press <ENTER> for a list of 
value constants.                0    X    This function takes a measurement on the output signal and returns the measured value.        Pass the measurement you want the power supply to take.

Valid Values:
ZUP366_VAL_MEASURE_VOLTAGE - DC voltage measurement
ZUP366_VAL_MEASURE_CURRENT - DC current measurement
ZUP366_VAL_MEASURE_POWER   - DC power measurement
 

Default Value: ZUP366_VAL_MEASURE_VOLTAGE
    ?    Returns the measured value.

Units: volts (for voltage measurement)
       amps  (for current measurement)


Notes:
1) The time interval, number of points, and windowing function can be programmed on models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
MEASure[:SCALar]:CURRent[:DC]? (@<chanlist>)
MEASure[:SCALar]:VOLTage[:DC]? (@<chanlist>)

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   V? 3 ? ? ?    Measurement Type                 W? 3? ?  ?    Measurement                      Yq-   ?  ?    Instrument Handle                Z'#????  ?    Status                           `? 3 
 ?  ?    Channel Name                                  eVoltage ZUP366_VAL_MEASURE_VOLTAGE Current ZUP366_VAL_MEASURE_CURRENT Power ZUP366_VAL_MEASURE_POWER    	               	           ""   f    This function initiates an acquisition on the channels that you specify.  It then waits for the acquisition to complete, and returns the measurement for the channel you specify.  You call the zup366_InitiateAcquisition, zup366_FetchArray functions to obtain the measurement for each of the remaining enabled channels without initiating another acquisition.        Pass the measurement you want the power supply to take.

Valid Values:
ZUP366_VAL_MEASURE_VOLTAGE - DC voltage measurement
ZUP366_VAL_MEASURE_CURRENT - DC current measurement
ZUP366_VAL_MEASURE_POWER   - DC power measurement
 

Default Value: ZUP366_VAL_MEASURE_VOLTAGE
    ?    Returns the measurement array that the DC Power acquires.  

Units:  volts

Units: volts (for voltage measurement)
       amps  (for current measurement)


Notes:
1) This command is used only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).


Remote commands:
MEASure:ARRay:CURRent[:DC]? (@<chanlist>)
MEASure:ARRay:VOLTage[:DC]? (@<chanlist>)     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     V    Pass the number of elements in the Measurement Array parameter.

Default Value: None     X    Indicates the number of points the function places in the Measurement Array parameter.    ?    Pass the maximum length of time in which to allow the read measurement operation to complete.    

If the operation does not complete within this time interval, the function returns the ZUP366_ERROR_MAX_TIME_EXCEEDED (0xBFFA2003) error code.  When this occurs, you can call zup366_AbortAcquisition to cancel the read waveform operation and return the oscilloscope to the Idle state.

Units:  milliseconds.  

Defined Values:
ZUP366_VAL_MAX_TIME_INFINITE (-1)             ZUP366_VAL_MAX_TIME_IMMEDIATE (0)          

Default Value: 5000 (ms)

Notes:

(1) The Maximum Time parameter applies only to this function.  It has no effect on other timeout parameters or attributes.
   d< 3 ? ? ?    Measurement Type                 eT ? ? ?  ?    Measurement Array                f?-   ?  ?    Instrument Handle                g?#????  ?    Status                           n% 3 
 ?  ?    Channel Name                     n? ?  ?  ?    Array Size                       n? ?? ?  ?    Actual Points                    o] 4? ?  ?    Maximum Time (ms)                             eVoltage ZUP366_VAL_MEASURE_VOLTAGE Current ZUP366_VAL_MEASURE_CURRENT Power ZUP366_VAL_MEASURE_POWER    	                	           ""        	            5000    X    This function takes a measurement on the output signal and returns the measured value.        Pass the measurement you want the power supply to take.

Valid Values:
ZUP366_VAL_MEASURE_VOLTAGE - DC voltage measurement
ZUP366_VAL_MEASURE_CURRENT - DC current measurement
ZUP366_VAL_MEASURE_POWER   - DC power measurement 

Default Value: ZUP366_VAL_MEASURE_VOLTAGE
    W    Returns the measured value.

Units: volts (for voltage measurement)
       amps  (for current measurement)

Notes:
1) This command is used only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
FETCh[:SCALar]:CURRent[:DC]? (@<chanlist>)
FETCh[:SCALar]:VOLTage[:DC]? (@<chanlist>)
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   t? 3 ? ? ?    Measurement Type                 u? 3? ?  ?    Measurement                      w4-   ?  ?    Instrument Handle                w?#????  ?    Status                           ~x 3 
 ?  ?    Channel Name                                  eVoltage ZUP366_VAL_MEASURE_VOLTAGE Current ZUP366_VAL_MEASURE_CURRENT Power ZUP366_VAL_MEASURE_POWER    	               	           ""    ?    This function returns the measurement the DC Power supply acquires for the channel you specify.  The waveform is from an acquisition that you previously initiated.          Pass the measurement you want the power supply to take.

Valid Values:
ZUP366_VAL_MEASURE_VOLTAGE - DC voltage measurement
ZUP366_VAL_MEASURE_CURRENT - DC current measurement
ZUP366_VAL_MEASURE_POWER   - DC power measurement
 

Default Value: ZUP366_VAL_MEASURE_VOLTAGE
        Returns the measurement array that the DC Power acquires.  

Units:  volts

Units: volts (for voltage measurement)
       amps  (for current measurement)

Notes:
1) This command is used only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
FETCh:ARRay:CURRent[:DC]? (@<chanlist>)
FETCh:ARRay:VOLTage[:DC]? (@<chanlist>)     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     V    Pass the number of elements in the Measurement Array parameter.

Default Value: None     X    Indicates the number of points the function places in the Measurement Array parameter.   ?@ 3 ? ? ?    Measurement Type                 ?X ? ? ?  ?    Measurement Array                ??-   ?  ?    Instrument Handle                ??#????  ?    Status                           ?# 3 
 ?  ?    Channel Name                     ?? ?  ?  ?    Array Size                       ?? ?? ?  ?    Actual Points                                 eVoltage ZUP366_VAL_MEASURE_VOLTAGE Current ZUP366_VAL_MEASURE_CURRENT Power ZUP366_VAL_MEASURE_POWER    	                	           ""        	            ?    Stops the currently running internal data log and returns the trigger system back to the Idle state.

Note:
This function only applies to the N6705B DC Power Analyzer.

Remote Commands:
ABORt:DLOG
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ?C????  ?    Status                           ??   ?  ?    Instrument Handle                  	               |    Stops the external data log and returns the trigger system back to the Idle state.

SCPI Command:
ABORt:ELOG (@<chanlist>)     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     X    Specifies the name of the channel.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

   ??-   ?  ?    Instrument Handle                ?: - ? ?  ?    Channel Name                     ??#????  ?    Status                                 ""    	               Abort histogram current measurement.

Note:
1) This function only applies to models N678xA SMU that have the Data Logger function installed.
2) The histogram function only applies to the N6705B DC Power Analyzer.

Remote Commands:
ABORt:HISTogram (@<chanlist>)
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     u    Pass the name of the channel on which to abort a the histogram.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ??{???  ?    Status                           ?y %  ?  ?    Channel Name                     ??   ?  ?    Instrument Handle                  	           ""        _    Initiates the data logger.

Note:
This function only applies to the N6705B DC Power Analyzer.    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    This command initiates the data logger. When the data logger is initiated, an event on a selected data log trigger source starts a datalog.

Valid Values: ASCII string
"internal:\\data1.dlog" - instrument?s internal memory. 
"external:\\data1.dlog" - external USB drive

Default Value: "internal:\\data1.dlog"

Notes:
1)Memory source (internal:\\ or external:\\) must be specified.
2)Filename and path must be enclosed in single or double quotes.
3)The filename in which to save the data should be the full path and filename. It can only be saved to an existing folder.
4)All data log files must have the extension .dlog  If the data logger is not initiated, all triggers are ignored.

Remote Commands:
INITiate[:IMMediate]:DLOG <"filename">
   ??????  ?    Status                           ?U   ?  ?    Instrument Handle                ? ? ? ?  ?    Datalog File Path                  	               "internal:\\data1.dlog"   ?    Initiates an external data log. When the external data log is initiated, an event on a selected external data log trigger source starts the data log. If the external data log is not initiated, all triggers are ignored.

Notes:
1) The external data log function is not accessible from the instrument's front panel.
2) All external data log functions are controlled using SCPI commands.

SCPI Command:
INITiate[:IMMediate]:ELOG (@<chanlist>)     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     X    Specifies the name of the channel.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

   ??-   ?  ?    Instrument Handle                ?< - ? ?  ?    Channel Name                     ??#????  ?    Status                                 ""    	           ?    Initiates a histogram current measurement.
A histogram measurement provides a statistical measurement for profiling the signal levels of a sense function. The acquisition generates an array of frequency of occurrence vs. amplitude data.

Notes:
1)This command only applies to models N678xA SMU that have the Data Logger function installed. 

Remote Commands:
INITiate[:IMMediate]:HISTogram (@<chanlist>)
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     v    Pass the name of the channel on which to initiate the histogram.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ?|w???  ?    Status                           ?
 ,  ?  ?    Channel Name                     ǈ   ?  ?    Instrument Handle                  	           ""        ?    Returns the peak to peak value of the data logger between marker 1 and marker 2.

Note:

This function only applies to the N6705B DC Power Analyzer.    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     ?    Returns the peak-to-peak voltage between markers.

Remote Commands:
FETCh:DLOG:CURRent:PTPeak? (@<chanlist>)    
FETCh:DLOG:VOLTage:PTPeak? (@<chanlist>)        
   ɑw???  ?    Status                           ? %  ?  ?    Channel Name                     Й   ?  ?    Instrument Handle                ?O Tj ?  ?    Peak to Peak                       	           ""        	           ?    Returns the DC output current or voltage average on the selected data log trace. 

Note: 
This function only applies to the N6705B DC Power Analyzer.      ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     ?    Returns the peak-to-peak voltage between markers.

Remote Commands:
FETCh:DLOG:CURRent:PTPeak? (@<chanlist>)    
FETCh:DLOG:VOLTage:PTPeak? (@<chanlist>)        
   Ӓw???  ?    Status                           ?  %  ?  ?    Channel Name                     ښ   ?  ?    Instrument Handle                ?P Tj ?  ?    Datalog Measurement                	           ""        	           t    Returns external datalog data from the instrument's buffer.

SCPI Command:
FETCh:ELOG? <maxrecords>, (@<chanlist>)     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     X    Specifies the name of the channel.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    Specifies the maximum number of records of external datalog data that the controller will return for each channel.

Valid Range:
1 to 16384

Default Value: 
1    ?    Returns the external datalog data from the instrument???s buffer.

A record is one set of voltage and current readings for one time interval. The exact format of a record depends on which functions have been enabled for external datalog sensing. If all functions are enabled, then one record will contain:Current average, Current minimum, Current maximum, Voltage average, Voltage minimum, Voltage maximum,
in that order.    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

   ?n-   ?  ?    Instrument Handle                ?$ -  ?  ?    Channel Name                     ބ - ? ?  ?    Max Records                      ?, -? ?  ?    External Log                     ??#????  ?    Status                                 ""    1    	            	            ?    This query fetches the histogram current bin data. 

Note:
This command only applies to models N678xA SMU that have the Data Logger function installed.

Remote Commands:
FETCh:HISTogram:CURRent? <binrange>, (@<chanlist>)
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    s    Selects bin range.  There are two bin ranges, with 4096 bins in each of the ranges.

Valid Values:
8 A range - has 4096 bins with minimum and maximum bin amplitudes of -8A and +8A and a bin size of 3.9mA (16 / 4096). -  0.0039 - A range has 4096 bins with minimum and maximum bin amplitudes of -3.9mA and +3.9ma and a bin size of 1.9uA (7.8ma / 4096).

Default Value: 8     ?    Returns the histogram current bin data for the specified channel and selected range.

Remote Commands:
FETCh:HISTogram:CURRent? <binrange>, (@<chanlist>)   ?????  ?    Status                           ? %  ?  ?    Channel Name                     ??   ?  ?    Instrument Handle                ?= p  ? ?    Bin Range                        ?? p? ?  ?    Bin Current Data                   	           ""                   !8A Range 8 0.0039 A Range 0.0039    	           ?    Reads and returns the bin data in the histogram function.

Notes:
1) This function only applies to models N678xA SMU that have the Data Logger function installed.
2) The histogram function only applies to the N6705B DC Power Analyzer. 
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     r    Pass the name of the channel on which to take a measurement.

Valid Channel Names:  1,2,3,4

Default Value:  ""
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    s    Selects bin range.  There are two bin ranges, with 4096 bins in each of the ranges.

Valid Values:
8 A range - has 4096 bins with minimum and maximum bin amplitudes of -8A and +8A and a bin size of 3.9mA (16 / 4096). -  0.0039 - A range has 4096 bins with minimum and maximum bin amplitudes of -3.9mA and +3.9ma and a bin size of 1.9uA (7.8ma / 4096).

Default Value: 8     V    Returns the histogram current bin data for the specified channel and selected range.   ??????  ?    Status                           ?: %  ?  ?    Channel Name                     ??   ?  ?    Instrument Handle                ?j p  ? ?    Bin Range                        ?? p8 ?  ?    Bin Current Data                   	           ""                   !8A Range 8 0.0039 A Range 0.0039    	          ?    This function initiates output changes that you previously specified.

If the power supply is not currently waiting for a trigger, the power supply waits for the trigger you specify with the zup366_ConfigureTriggerSource function after you call this function.  After the power supply detects the trigger, it updates its voltage level and current limit to the values you specify with the zup366_ConfigureTriggeredVoltageLevel and zup366_ConfigureTriggeredCurrentLimit functions.

If the power supply is already waiting for a trigger, this function does nothing and returns VI_SUCCESS.

Notes:
1)Initiates Transient trigger on all occupied channels. 
2)ConfigureTriggeredCurrentMode or ConfigureTriggeredVoltageMode (depending what measurement is set for specific channel) has to be set to STEP for all channels. Otherwise, a "Cannot Initiate Voltage/Current in FIXED mode error will be generated.

Remote commands:
INITiate[:IMMediate]:TRANsient (@<chanlist>)    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   f#????  ?    Status                           
?-   ?  ?    Instrument Handle                  	              *    This function initiates a measurement acquisition.  After you call this function, the DC Power supply leaves the Idle state and acquires a measurements for each channel you specify.


Notes:

1) This function does not check the instrument status.   Typically, you call this function only in a sequence of calls to other low-level driver functions.  The sequence performs one operation.  You use the low-level functions to optimize one or more aspects of interaction with the instrument.  If you want to check the instrument status, call the zup366_error_query function at the conclusion of the sequence.

2) For mainframe N6700 this command is used only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).


Remote commands:
INITiate[:IMMediate]:ACQuire (@<chanlist>)    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    +    Pass the name of the channel on which to initiates a measurement.

Valid Channel Names:  1,2,3,4

Default Value:  ""


Note:

1) If you set NULL or empty string, acquisition will be started on the all available channels.

2) You can specify more channels, separated with comma. For example: "1,3"
   W#????  ?    Status                           ?-   ?  ?    Instrument Handle                ? F ? ?  ?    Channel Name                       	               ""       If the power supply is currently waiting for a trigger to change the output signal, this function aborts all pending output changes.

If the power supply is not waiting for a trigger, this function does nothing and returns VI_SUCCESS.

Remote commands:
ABORt:TRANsient (@<chanlist>)    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ?#????  ?    Status                            7-   ?  ?    Instrument Handle                  	              w    This function aborts an acquisition and returns the DC Power supply to the Idle state.  You initiate an acquisition with the zup366_InitiateAcquisition function.

Notes:

(1) This function does not check the instrument status.   Typically, you call this function only in a sequence of calls to other low-level driver functions.  The sequence performs one operation.  You use the low-level functions to optimize one or more aspects of interaction with the instrument.  If you want to check the instrument status, call the zup366_error_query function at the conclusion of the sequence.

Remote commands:
ABORt:ACQuire (@<chanlist>)    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    '    Pass the name of the channel on which to abort an acquisition.

Valid Channel Names:  1,2,3,4

Default Value:  ""


Note:

1) If you set NULL or empty string, acquisition will be aborted on the all available channels.

2) You can specify more channels, separated with comma. For example: "1,3"   #?#????  ?    Status                           *u-   ?  ?    Instrument Handle                ++ F ? ?  ?    Channel Name                       	               ""   5    This function sends a command to trigger the power supply.  Call this function if you configure the power supply to respond to software triggers.  If the power supply is not configured to respond to software triggers, this function returns the error ZUP366_ERROR_TRIGGER_NOT_SOFTWARE.

Remote commands:
*TRG    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   .L#????  ?    Status                           4?-   ?  ?    Instrument Handle                  	                  This function sends an immediate trigger to the measurement
system. When the trigger system is initiated, a measurement trigger causes the power system to measure the output voltage or current and store the results in a buffer. The measured quantity, voltage or current is specified by the zup366_ConfigureMeasurement function.

Notes:
1) This command is used only with models N676xA, N678xA SMU, or models that have Option 054 (High Speed Test Extensions).

Remote commands:
:TRIGger:ACQuire[:IMMediate] (@<chanlist>)    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    )    Pass the name of the channel on which to trigger an acquisition.

Valid Channel Names:  1,2,3,4

Default Value:  ""


Note:

1) If you set NULL or empty string, acquisition will be aborted on the all available channels.

2) You can specify more channels, separated with comma. For example: "1,3"   8#????  ?    Status                           >?-   ?  ?    Instrument Handle                ?^ W ? ?  ?    Channel Name                       	               ""   F    This function generates an immediate transient trigger regardless of the selected trigger source. Output triggers affect the following functions: voltage, current, and current limit. You must initiate the output trigger system before you can send any triggers. 

When sent, the output trigger will:
* Initiate an output change as specified by the Current Triggered or Voltage Triggered commands.
* Clears the WTG-tran bit in the Status Operation Condition
register after the transient trigger sequence has completed.

Remote commands:
:TRIGger:TRANsient[:IMMediate], (@<chanlist>)    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    )    Pass the name of the channel on which to trigger an acquisition.

Valid Channel Names:  1,2,3,4

Default Value:  ""


Note:

1) If you set NULL or empty string, acquisition will be aborted on the all available channels.

2) You can specify more channels, separated with comma. For example: "1,3"   C?#????  ?    Status                           J -   ?  ?    Instrument Handle                J? W ? ?  ?    Channel Name                       	               ""   :    Sends an immediate trigger signal to the data logger. This will trigger the data log regardless of the selected trigger source.

Notes:
1) The data logger must be initiated before triggered.
2) This function only applies to the N6705B DC Power Analyzer.               

Remote Commands:
TRIGger:DLOG[:IMMediate]
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   M?????  ?    Status                           T?   ?  ?    Instrument Handle                  	                  Sends an immediate trigger signal to the external data logger. This will trigger the external data log regardless of the selected trigger source.

Note:
The external data logger must be initiated before triggered.

SCPI Command:
TRIGger:ELOG[:IMMediate] (@<chanlist>)     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     X    Specifies the name of the channel.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

   V?-   ?  ?    Instrument Handle                W? - ? ?  ?    Channel Name                     W?#????  ?    Status                                 ""    	           ?    Sends an immediate trigger signal to the histogram current measurement data logger. This will trigger the histogram measurement regardless of the selected trigger source.

Notes:
1) The histogram function only applies to the N6705B Analyzer.
2)This command only applies to models N6781A and N6782A that have the Data Logger function installed.
3) The histogram function must be initiated before triggered.

Remote Command:
TRIGger:HISTogram[:IMMediate] (@<chanlist>)
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    )    Pass the name of the channel on which to trigger an acquisition.

Valid Channel Names:  1,2,3,4

Default Value:  ""


Note:

1) If you set NULL or empty string, acquisition will be aborted on the all available channels.

2) You can specify more channels, separated with comma. For example: "1,3"   a????  ?    Status                           g?   ?  ?    Instrument Handle                hJ %  ?  ?    Channel Name                       	               ""   +    This function returns whether the power supply is in a particular output state.

A constant voltage condition occurs when the output voltage is equal to the value of the ZUP366_ATTR_VOLTAGE_LEVEL attribute and the current is less than or equal to the value of the ZUP366_ATTR_CURRENT_LIMIT attribute.

A constant current condition occurs when the output current is equal to the value of the ZUP366_ATTR_CURRENT_LIMIT attribute and the ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR attribute is set to ZUP366_VAL_CURRENT_REGULATE.

An unregulated condition occurs when the output voltage is less than the value of the ZUP366_ATTR_VOLTAGE_LEVEL attribute and the current is less than the value of the ZUP366_ATTR_CURRENT_LIMIT attribute.

An over-voltage condition occurs when the output voltage is equal to or greater than the value of the ZUP366_ATTR_OVP_LIMIT attribute and the ZUP366_ATTR_OVP_ENABLED attribute is set to VI_TRUE.

An over-current condition occurs when the output current is equal to or greater than the value of the ZUP366_ATTR_CURRENT_LIMIT attribute and the ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR attribute is set to ZUP366_VAL_CURRENT_TRIP.  

When either an over-voltage condition or an over-current condition occurs, the power supply's output protection disables the output.  If the power supply is in an over-voltage or over-current state, it does not produce power until the output protection is reset.  The zup366_ResetOutputProtection function resets the output protection.  Once the output protection is reset, the power supply resumes generating a power signal.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     y    Pass the name of the channel on which to query for an output state.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the output state for which you want to query.

Valid Values:
ZUP366_VAL_OUTPUT_CONSTANT_VOLTAGE - Constant Voltage State

ZUP366_VAL_OUTPUT_CONSTANT_CURRENT - Constant Current State

ZUP366_VAL_OUTPUT_UNREGULATED - Unregulated State

ZUP366_VAL_OUTPUT_OVER_VOLTAGE - Over-voltage State

ZUP366_VAL_OUTPUT_OVER_CURRENT - Over-current State


Default Value:
ZUP366_VAL_OUTPUT_CONSTANT_VOLTAGE


Remote commands:
:STATus:QUEStionable:CONDition? (@<chanlist>)
:STATus:OPERtion:CONDition? (@<chanlist>)
     ?    This parameter returns VI_TRUE if the power supply is currently in the state you specify with the outputState parameter, and VI_FALSE if it is not.
   pc-   ?  ?    Instrument Handle                q#????  ?    Status                           w? 3 
 ?  ?    Channel Name                     x' 3 ? ? ?    Output State                     z( 3? ?  ?    In State                               	           ""               ?Constant Voltage ZUP366_VAL_OUTPUT_CONSTANT_VOLTAGE Constant Current ZUP366_VAL_OUTPUT_CONSTANT_CURRENT Unregulated ZUP366_VAL_OUTPUT_UNREGULATED Over Voltage ZUP366_VAL_OUTPUT_OVER_VOLTAGE Over Current ZUP366_VAL_OUTPUT_OVER_CURRENT    	            ?    This function returns the maximum programmable current limit that the power supply accepts for a particular voltage level on a channel for the output range to which the power supply is currently configured.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to query for the maximum programmable current limit.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Pass the voltage level for which to determine the maximum programmable current limit.  

Units: volts (V).

Valid Range: 
6731A: 0.02  to  5.1 V
N6732A: 0.02  to 8.16 V
N6733A: 0.02  to 20.4 V
N6734A: 0.02  to 35.7 V
N6735A: 0.02  to 51.0 V
N6742A: 0.02  to 8.16 V
N6743A: 0.02  to 20.4 V
N6744A: 0.02  to 35.7 V
N6745A: 0.02  to 51.0 V
N6751A: 0.02  to 51.0 V
N6752A: 0.02  to 51.0 V
N6753A: 0.01  to 20.4 V
N6754A: 0.025 to 61.2 V
N6755A: 0.01  to 20.4 V
N6756A:  0.025 to 61.2 V
N6761A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6762A: high range           0.015 to 51.0 V
        low range (<= 5.5V)  0.012 to 5.5 V
N6763A: 0.01 to 20.4 V
N6764A: 0.025 to 61.2 V
N6765A: 0.01 to 20.4 V
N6766A: 0.025 to 61.2 V
N6773A: 0.0 to  20 V
N6774A: 0.0 to 35 V
N6775A: 0.0 to 60 V
N6776A: 0.0 to 100 V
N6777A: 0.0 to  150 V
N6781A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6782A: 
       20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V
N6783A: 0.0    to 8.0 V
N6784A: 
        20V range 0.0    to  20.4 V
        6V  range 0.0    to  6.12  V  
N6731B  0.015  to  5.1 V
N6732B  0.015  to  8.16 V
N6733B  0.03   to  20.4 V
N6734B  0.04   to  35.7  V
N6735B  0.07   to  61.2 V
N6736B  0.1    to  102.0 V
N6741B  0.015  to  5.1 V
N6742B  0.015  to  8.16 V
N6743B  0.03   to  20.4 V
N6744B  0.04   to  35.7 V
N6745B  0.07   to  61.2 V
N6746B  0.1    to  102.0 V


Default Value: 0.02
     ?    This parameter returns the maximum programmable current limit for the voltage level you specify.

Units: amps (A)

Remote commands:
[SOURce:]CURRent[:LEVel][:IMMediate][:AMPLitude]? MAX,(@chanlist)   }?-   ?  ?    Instrument Handle                ~y#????  ?    Status                           ? 3 
 ?  ?    Channel Name                     ?? 3 ? ? ?    Voltage Level                    ?b 3? ?  ?    Max Current Limit                      	           ""    0.02    	           ?    This function returns the maximum programmable voltage level that the power supply accepts for a particular current limit on a channel for the output range to which the power supply is currently configured.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
      
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to query for the maximum programmable voltage level.

Valid Channel Names:  1,2,3,4

Default Value:  ""
        Pass the current limit for which to determine the maximum programmable voltage level.  

Units: amps (A).

Valid Range: 
N6731A: 0.01   to  10.2 A
N6732A: 0.01   to  6.375 A
N6733A: 0.01   to  2.55 A
N6734A: 0.01   to  1.53 A
N6735A: 0.01   to  0.816 A
N6742A: 0.01   to  10.2 A
N6743A: 0.01   to  5.1 A
N6744A: 0.01   to  3.06 A
N6745A: 0.01   to  1.63 A
N6751A: 0.01   to  5.1 A
N6752A: 0.01   to  10.2 A
N6753A: 0.05   to  51.0 A
N6754A: 0.02   to  20.4 A
N6755A: 0.05   to  51.0 A
N6756A: 0.02   to  17.3 A
N6761A: high range          0.001  to  1.53 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6762A: high range          0.001  to  3.06 A
        low range (<= 0.1A) 0.1e-3 to  0.1 A
N6763A: 0.05  to  51 A             
N6764A: 0.02  to  20.4 A       
N6765A: 0.05  to  51 A        
N6766A: 0.02  to  17.3 A 
N6773A  0.00  to  15A
N6774A  0.00  to  8.5 A
N6775A  0.00  to  5.0 A
N6776A  0.00  to  3.0 A         
N6777A: 0.00  to  2.0 A
N6781A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6782A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6783A:  0 to 3.06 A     
N6784A: 
        3A range -3.06 to 3.06 A
        1A range -1.02 A to 1.02 A
N6731B  0.06    to  10.2 A
N6732B  0.04    to  6.375 A
N6733B  0.01    to  2.55 A
N6734B  0.005   to  1.53 A
N6735B  0.0025  to  0.85 A
N6736B  0.0015  to  0.51 A
N6741B  0.06    to  20.4 A
N6742B  0.04    to  12.75 A
N6743B  0.01    to  5.1 A
N6744B  0.005   to  3.06 A
N6745B  0.0025  to  1.7 A
N6746B  0.0015  to  1.02 A

Default Value: 0.01
     ?    This parameter returns the maximum programmable voltage level for the current limit you specify.

Units: volts (V)

Remote commands:
[SOURce:]VOLTage[:LEVel][:IMMediate][:AMPLitude]? MAX,(@chanlist)   ?=-   ?  ?    Instrument Handle                ??#????  ?    Status                           ?~ 3 
 ?  ?    Channel Name                     ? 3 ? ? ?    Current Limit                    ?+ 3? ?  ?    Max Voltage Level                      	           ""    0.01    	          ?    This function resets the power supply's output protection after an over-voltage, over-current, or other output protection condition occurs.

An over-voltage condition occurs when the output voltage is equal to or greater than the value of the ZUP366_ATTR_OVP_LIMIT attribute and the ZUP366_ATTR_OVP_ENABLED attribute is set to VI_TRUE.  

An over-current condition occurs when the output current is equal to or greater than the value of the ZUP366_ATTR_CURRENT_LIMIT attribute and the ZUP366_ATTR_CURRENT_LIMIT_BEHAVIOR attribute is set to ZUP366_VAL_CURRENT_TRIP.  

When either an over-voltage condition or an over-current condition occurs, the output protection of the power supply disables the output.  Once the output protection is reset, the power supply resumes generating a power signal.

You use the zup366_QueryOutputState function to determine if the power supply is in an output-protection state.

Remote commands:
OUTPut:PROTection:CLEar (@<chanlist>)
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     e    Pass the name of the channel you want to reset.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   ??-   ?  ?    Instrument Handle                ??#????  ?    Status                           ?@ 3 
 ?  ?    Channel Name                           	           ""    ?    This function returns a list of output channels which are currently grouped together.

Notes:
1) This function does NOT apply to models N678xA SMU.    I    Gives the currently grouped channels, which are enclosed in quotes.  For example, the string "1,2,3", "4", indicates that channels 1, 2 and 3 are grouped. Channel 4 is not grouped, as it appears by itself in the quote string.


Notes:
1) This command does NOT apply to models N678xA SMU.

Remote Commands:
SYSTem:GROup:CATalog?    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ?? OQ ?  ?    Grouped Channels                 ?P????  ?    Status                           ??   ?  ?    Instrument Handle                  	            	               ?    Queries the quality of the waveforms in the waveform sequence.

Note:
Sequence capability is only available on the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    Pass the name of the channel on which to query the waveform quality state.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    ?    Returns the quality of the waveforms in the waveform sequence. This is a number between 16 (worst quality) to 100 (best quality) this indicates how well waveforms are represented. It is the number of points used to represent waveforms that are smooth curves (ramp, trapezoid, exponential, and sine).   


Notes:
1) The quality of a given waveform is determined by the total number and type of waveforms that comprise the sequence. The maximum number of points allowed in a sequence is 511. As more waveforms are added to the sequence, the quality of the waveforms is reduced to accommodated the 511-point limit.

Remote Commands:
[SOURce:]ARB:SEQuence:QUALity? (@<chanlist>)

   ??    ?  ?    Instrument Handle                ??u???  ?    Status                           ?' %  ?  ?    Channel Name                     ?? %u ?  ?    Waveform Quality                       	           ""    	               Queries least significant bit (LSB) weight of a histogram, histogram offset, and current bin ranges.

Notes:
1) This function only applies to models N6781A and N6782A that have the Data Logger function installed.
2) The histogram function only applies to the N6705B DC Power Analyzer.     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     s    Pass the name of the channel on which to query the histogram.

Valid Channel Names:  1,2,3,4

Default Value:  ""
    }    This queries the LSB weight of a histogram current bin in amperes per bin number. 

Notes:
1) This command only applies to models N6781A and N6782A that have the Data Logger function installed.
2) The gains and offsets vary slightly between units of the same model because of software calibration.

Remote Command:
SENSe:HISTogram:CURRent[:DC]:BIN:GAIN? <binrange>, (@<chanlist>)    |    This queries the offset of a histogram current bin in amperes per bin number. 

Notes:
1) This command only applies to models N6781A and N6782A that have the Data Logger function installed.
2) The gains and offsets vary slightly between units of the same model because of software calibration.

Remote Command:
SENSe:HISTogram:CURRent[:DC]:BIN:OFFSet? <binrange>, (@<chanlist>)
    V    This queries the values of the current bin ranges. 

The following values are returned:
Binrange specifies one of the two histogram ranges: 
-8 - The 8 A range has 4096 bins with minimum and maximum bin amplitudes of -8A and +8A and a bin size of 3.9mA (16 / 4096). -0.0039 - The 0.0039 A range has 4096 bins with minimum and maximum bin amplitudes of -3.9mA and +3.9ma and a bin size of 1.9uA (7.8ma / 4096).

Notes:
1) This command only applies to models N6781A and N6782A that have the Data Logger function installed..

Remote Command:
SENSe:HISTogram:CURRent[:DC]:BIN:RANGes? (@<chanlist>)


    s    Selects bin range.  There are two bin ranges, with 4096 bins in each of the ranges.

Valid Values:
8 A range - has 4096 bins with minimum and maximum bin amplitudes of -8A and +8A and a bin size of 3.9mA (16 / 4096). -  0.0039 - A range has 4096 bins with minimum and maximum bin amplitudes of -3.9mA and +3.9ma and a bin size of 1.9uA (7.8ma / 4096).

Default Value: 8   ?y+   ?  ?    Instrument Handle                ?/}???  ?    Status                           ȼ '  ?  ?    Channel Name                     ?7 ' ? ?  ?    Current Bin Gain                 ʼ p ? ?  ?    Current Bin Offset               ?@ ? ? ?  ?    Current Bin Range                Ξ z  ? ?    Bin Range                              	           ""    	           	           	                      !8A Range 8 0.0039 A Range 0.0039   ?    This function writes a command string directly to an instrument using VISA I/O. The function bypasses the attribute state caching mechanism and therefore always invalidates all attribute cache values for the session.

The function assumes that the IVI_ATTR_SYSTEM_IO_SESSION attribute for the IVI session you specify holds a valid VISA session for the instrument.

The function calls Ivi_SetNeedToCheckStatus with VI_TRUE.

Note: This function is supported with the IVI Compliance Package (ICP) 4.6.1 and later. If you are using an earlier version of ICP, you can still use the WriteInstrData and ReadInstrData functions under the Obsolete folder.
     9    Specify the string you want to send to the instrument.
     0    Returns the number of bytes actually written.
     ?    Specifies the number of bytes to write to the instrument. The value of this parameter must be less than or equal to the size of buffer.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors   Ԉ -  ?  ?    Write Buffer                     ?? -? ?  ?    Number of Bytes Written          ? - ? ?  ?    Number of Bytes to Write         Փ   ?  ?    Instrument Handle                ?H????  ?    Status                             ""    	                    	               This function reads data directly from an instrument using VISA I/O. The function bypasses the attribute state caching mechanism.

The function assumes that the IVI_ATTR_SYSTEM_IO_SESSION attribute for the IVI session you specify holds a valid VISA session for the instrument.

If data is not available at the instrument's output buffer when you call this function, the instrument might hang up. In that case, the function does not return until the VISA I/O call times out. If you have disabled the timeout, the function hangs indefinitely.

Note: This function is supported with the IVI Compliance Package (ICP) 4.6.1 and later. If you are using an earlier version of ICP, you can still use the WriteInstrData and ReadInstrData functions under the Obsolete folder.
    ?    Specify the maximum number of bytes to read from the instrument. The Read Buffer parameter must be a ViChar buffer that contains at least the number of bytes you specify.

If the number of bytes you specify is less than the number of bytes in the instrument's output buffer, you must call this function again to empty the output buffer. If you do not empty the instrument's output buffer, the instrument might return invalid data in response to subsequent requests.

If data is not available at the instrument's output buffer when you call this function, the instrument might hang up. In that case, the function does not return until the VISA I/O call times out. If you have disabled the timeout, the function hangs indefinitely.
    j    A buffer in which the function places the data it receives from the instrument.

The buffer must be a ViChar array that has at least as many bytes as you specify in the Number Bytes To Read parameter.

This function does not write an ASCII NUL byte to terminate the data, nor does it clear the buffer beyond the bytes it actually receives from the instrument.
    ?    This control returns the actual number of bytes the function received from the instrument. This is the value that the VISA viRead function returns.

If the actual number of bytes received is less than the number of bytes you specify in the Number Bytes To Read parameter, the instrument's output buffer has probably emptied.

If the number of bytes received is 0, the most probable cause is that no data was available at the instrument's output buffer.
     ?    The ViSession handle that you obtain from the @REPLACE[Prefix]_init or @REPLACE[Prefix]_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the @REPLACE[Prefix]_error_message function.  To obtain additional information about the error condition, call the @REPLACE[Prefix]_GetError function.  To clear the error information from the driver, call the @REPLACE[Prefix]_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors   ? -  ?  ?    Number of Bytes to Read          ?? - ? ?  ?    Read Buffer                      ?c -? ?  ?    Number of Bytes Read             ?2   ?  ?    Instrument Handle                ??????  ?    Status                                 	            	                	          &    This function resets the instrument to a known state and sends initialization commands to the instrument.  The initialization commands set instrument settings such as Headers Off, Short Command form, and Data Transfer Binary to the state necessary for the operation of the instrument driver.
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ?
#????  ?    Status                           ??-   ?  ?    Instrument Handle                  	                   This function resets the instrument and applies initial user specified settings from the Logical Name which was used to initialize the session.  If the session was created without a Logical Name, this function is equivalent to the zup366_reset function.
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ??#????  ?    Status                           ?\-   ?  ?    Instrument Handle                  	               ?    This function places the instrument in a quiescent state where it has minimal or no impact on the system to which it is connected.

Note: This function is not supported.      ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    A#????  ?    Status                           ?-   ?  ?    Instrument Handle                  	               W    This function runs the instrument's self test routine and returns the test result(s).    &    This control contains the value returned from the instrument self test.  Zero means success.  For any other code, see the device's operator's manual.

Self-Test Code    Description
---------------------------------------
   0              Passed self test
   1              Self test failed

     ?    Returns the self-test response string from the instrument. See the device's operation manual for an explanation of the string's contents.

You must pass a ViChar array with at least 256 bytes.    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ] =  ?  ?    Self Test Result                 	? = ? ? ,    Self-Test Message                
U#????  ?    Status                           ?-   ?  ?    Instrument Handle                  	           	            	               ^    This function returns the revision numbers of the instrument driver and instrument firmware.     ?    Returns the instrument driver software revision numbers in the form of a string.

You must pass a ViChar array with at least 256 bytes.     ?    Returns the instrument firmware revision numbers in the form of a string.

You must pass a ViChar array with at least 256 bytes.
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ? = 3 ?  ?    Instrument Driver Revision       ? =6 ?  ?    Firmware Revision                #????  ?    Status                           ?-   ?  ?    Instrument Handle                  	            	            	                  This function converts the specified  waveform to a list of points in the user-defined current or voltage waveform format. 

Notes:
1) Waveform capability is only available on the N6705B DC Power Analyzer.
2) To use this command you must first specify a Waveform Shape and Waveform Measurement Type (Configure Waveform Parameters function).                                                
3) If the Waveform Shape of the specified channel is already set to a User-Defined waveform, the command is ignored and the conversion is not performed.
4) If the Waveform Shape of the specified channel is set to "NONE", the command is ignored and the conversion is not performed.

Remote Commands:
[SOURce:]ARB:CURRent:CONVert (@<chanlist>)
[SOURce:]ARB:VOLTage:CONVert (@<chanlist>)
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   e   ?  ?    Instrument Handle                 ????  ?    Status                           &? 2  ?  ?    Channel Name                           	           ""    ?    Resets the sequence to its power-on default setting: Step = 1 Shape = Pulse.

Notes:
1) Sequence capability is only available on the N6705B DC Power Analyzer.

Remote Commands:
[SOURce:]ARB:SEQuence:RESet (@<chanlist>)

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
   (?   ?  ?    Instrument Handle                )j????  ?    Status                           /? %  ?  ?    Channel Name                           	           ""    ?    Recalls or saves the sequence settings on the N6705B model.

Note:
Sequence capability is only available on the N6705B DC Power Analyzer.    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     i    Pass the name of the channel you want to configure.

Valid Channel Names:  1,2,3,4

Default Value:  ""
        This command saves (stores) a waveform sequence to a file or recalls (loads) waveform sequence from a file. 

Valid Values:
ZUP366_VAL_SEQUENCE_ACTION_SAVE - Stores the sequence
ZUP366_VAL_SEQUENCE_ACTION_RECALL - Loads the sequence

Default Value:
ZUP366_VAL_SEQUENCE_ACTION_SAVE
    ?    Specify the memory source, full path and filename for saving or recalling sequences.

Valid Values: ASCII string
"internal:\\filename.seq" - instruments internal memory. 


Default Value:"internal:\\data1.seq"

Notes
1)Memory source (internal:\\ or external:\\) must be specified. 
2)Filename and path must be enclosed in single or double quotes.
3)The filename should be the full path and filename. The sequence file path must include a .seq file extension 
and can only be saved to an existing folder. 
4)All waveform sequence data is saved in a proprietary format (.seq) which is not editable.

Remote Commands:
MMEMory:STOR:ARB:SEQuence <"filename">, (@<channel>)
MMEMory:LOAD:ARB:SEQuence <"filename">, (@<chanlist>)
   1?????  ?    Status                           8=   ?  ?    Instrument Handle                8? %  ?  ?    Channel Name                     9d z  ? U    Sequence Action                  :? z ? ?  ?    Sequence File Path                 	               ""               NSave ZUP366_VAL_SEQUENCE_ACTION_SAVE Recall ZUP366_VAL_SEQUENCE_ACTION_RECALL    "internal:\\data1.seq"    y    Exports a datalog file or deletes a specified file.

Note:
This function only applies to the N6705B DC Power Analyzer.
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
        Selects the operation to perform  

Valid Values:
ZUP366_VAL_FILE_OPERATION_DELETE - Deletes the specified file
ZUP366_VAL_FILE_OPERATION_EXPORT - Exports the data that is currently loaded in the instrument's datalog viewer to a file.

Default Value:
ZUP366_VAL_FILE_OPERATION_EXPORT
    H    Specify the memory source, full path and filename for deleting a file or exporting datalog.

Valid Values: ASCII string
"internal:\\filename.ext" - instrument?s internal memory. 


Default Value:"internal:\\dataexport1.csv"

Notes
1)Memory source (internal:\\ or external:\\) must be specified. 
2)Filename and path must be enclosed in single or double quotes.
3)The filename should be the full path and filename. The exported data log operation requires a .csv (comma separated values) file extension.

Remote Commands:
MMEMory:DELete <"filename">
MMEMory:EXPort:DLOG <"filename">
   ?z????  ?    Status                           F   ?  ?    Instrument Handle                F? z  ? _    Operation                        G? z ? ?  ?    File Path                          	                         ]Delete File ZUP366_VAL_FILE_OPERATION_DELETE Export Datalog ZUP366_VAL_FILE_OPERATION_EXPORT    "internal:\\dataexport1.csv"    V    This function reads an error code and a message from the instrument's error queue.

     B    Returns the error code read from the instrument's error queue.

     ?    Returns the error message string read from the instrument's error message queue.

You must pass a ViChar array with at least 256 bytes.
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   L =  ?  ?    Error Code                       LK = ? ? ,    Error Message                    L?#????  ?    Status                           Sj-   ?  ?    Instrument Handle                  	           	            	               n    This function converts a status code returned by an instrument driver function into a user-readable string.     %    Pass the Status parameter that is returned from any of the instrument driver functions.
          
Default Value:  0  (VI_SUCCESS)
          
zup366 Status Codes:
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
  
IviDCPwr Status Codes:
Status    Description
-------------------------------------------------
ERRORS:
BFFA1001  The trigger source is not software trigger.

IVI Engine Status Codes:
Status    Description
-------------------------------------------------
ERRORS:
BFFA0001  Instrument error. Call zup366_error_query.
BFFA0002  Cannot open file.
BFFA0003  Error reading from file.
BFFA0004  Error writing to file.
BFFA0005  Driver module file not found.
BFFA0006  Cannot open driver module file for reading.
BFFA0007  Driver module has invalid file format or invalid data.
BFFA0008  Driver module contains undefined references.
BFFA0009  Cannot find function in driver module.
BFFA000A  Failure loading driver module.
BFFA000B  Invalid path name.
BFFA000C  Invalid attribute.
BFFA000D  IVI attribute is not writable.
BFFA000E  IVI attribute is not readable.
BFFA000F  Invalid parameter.
BFFA0010  Invalid value.
BFFA0011  Function not supported.
BFFA0012  Attribute not supported.
BFFA0013  Value not supported.
BFFA0014  Invalid type.
BFFA0015  Types do not match.
BFFA0016  Attribute already has a value waiting to be updated.
BFFA0017  Specified item already exists.
BFFA0018  Not a valid configuration.
BFFA0019  Requested item does not exist or value not available.
BFFA001A  Requested attribute value not known.
BFFA001B  No range table.
BFFA001C  Range table is invalid.
BFFA001D  Object or item is not initialized.
BFFA001E  Non-interchangeable behavior.
BFFA001F  No channel table has been built for the session.
BFFA0020  Channel name specified is not valid.
BFFA0021  Unable to allocate system resource.
BFFA0022  Permission to access file was denied.
BFFA0023  Too many files are already open.
BFFA0024  Unable to create temporary file in target directory.
BFFA0025  All temporary filenames already used.
BFFA0026  Disk is full.
BFFA0027  Cannot find configuration file on disk.
BFFA0028  Cannot open configuration file.
BFFA0029  Error reading configuration file.
BFFA002A  Invalid ViInt32 value in configuration file.
BFFA002B  Invalid ViReal64 value in configuration file.
BFFA002C  Invalid ViBoolean value in configuration file.
BFFA002D  Entry missing from configuration file.
BFFA002E  Initialization failed in driver DLL.
BFFA002F  Driver module has unresolved external reference.
BFFA0030  Cannot find CVI Run-Time Engine.
BFFA0031  Cannot open CVI Run-Time Engine.
BFFA0032  CVI Run-Time Engine has invalid format.
BFFA0033  CVI Run-Time Engine is missing required function(s).
BFFA0034  CVI Run-Time Engine initialization failed.
BFFA0035  CVI Run-Time Engine has unresolved external reference.
BFFA0036  Failure loading CVI Run-Time Engine.
BFFA0037  Cannot open DLL for read exports.
BFFA0038  DLL file is corrupt.
BFFA0039  No DLL export table in DLL.
BFFA003A  Unknown attribute name in default configuration file.
BFFA003B  Unknown attribute value in default configuration file.
BFFA003C  Memory pointer specified is not known.
BFFA003D  Unable to find any channel strings.
BFFA003E  Duplicate channel string.
BFFA003F  Duplicate virtual channel name.
BFFA0040  Missing virtual channel name.
BFFA0041  Bad virtual channel name.
BFFA0042  Unassigned virtual channel name.
BFFA0043  Bad virtual channel assignment.
BFFA0044  Channel name required.
BFFA0045  Channel name not allowed.
BFFA0046  Attribute not valid for channel.
BFFA0047  Attribute must be channel based.
BFFA0048  Channel already excluded.
BFFA0049  Missing option name (nothing before the '=').
BFFA004A  Missing option value (nothing after the '=').
BFFA004B  Bad option name.
BFFA004C  Bad option value.
BFFA004D  Operation only valid on a class driver session.
BFFA004E  "ivi.ini" filename is reserved.
BFFA004F  Duplicate run-time configuration entry.
BFFA0050  Index parameter is one-based.
BFFA0051  Index parameter is too high.
BFFA0052  Attribute is not cacheable.
BFFA0053  You cannot export a ViAddr attribute to the end-user.
BFFA0054  Bad channel string in channel string list.
BFFA0055  Bad prefix name in default configuration file.
         
VISA Status Codes:
Status    Description
-------------------------------------------------
WARNINGS:
3FFF0002 Event enabled for one or more specified mechanisms.
3FFF0003 Event disabled for one or more specified mechanisms.
3FFF0004 Successful, but queue already empty.
3FFF0005 Specified termination character was read.
3FFF0006 Number of bytes transferred equals input count.
3FFF0077 Configuration non-existant or could not be loaded.
3FFF007D Open successful, but the device not responding.
3FFF0080 Wait successful, but more event objects available.
3FFF0082 Specified object reference is uninitialized.
3FFF0084 Attribute value not supported.
3FFF0085 Status code could not be interpreted.
3FFF0088 Specified I/O buffer type not supported.
3FFF0098 Successful,  but invoke no handlers for this event.
3FFF0099 Successful but session has nested shared locks.
3FFF009A Successful but session has nested exclusive locks.
3FFF009B Successful but operation not asynchronous.
         
ERRORS:
BFFF0000 Unknown system error (miscellaneous error).
BFFF000E Session or object reference is invalid.
BFFF000F Resource is locked.
BFFF0010 Invalid expression specified for search.
BFFF0011 Resource is not present in the system.
BFFF0012 Invalid resource reference specified.  Parsing error.
BFFF0013 Invalid access mode.
BFFF0015 Timeout expired before operation completed.
BFFF0016 Unable to deallocate session data structures.
BFFF001B Specified degree is invalid.
BFFF001C Specified job identifier is invalid.
BFFF001D Attribute is not supported by the referenced object.
BFFF001E Attribute state not supported by the referenced object.
BFFF001F Specified attribute is read-only.
BFFF0020 Lock type lock not supported by this resource.
BFFF0021 Invalid access key.
BFFF0026 Specified event type not supported by the resource.
BFFF0027 Invalid mechanism specified.
BFFF0028 A handler was not installed.
BFFF0029 Handler reference either invalid or was not installed.
BFFF002A Specified event context invalid.
BFFF002D Event queue for specified type has overflowed.
BFFF002F Event type must be enabled in order to receive.
BFFF0030 User abort during transfer.
BFFF0034 Violation of raw write protocol during transfer.
BFFF0035 Violation of raw read protocol during transfer.
BFFF0036 Device reported output protocol error during transfer.
BFFF0037 Device reported input protocol error during transfer.
BFFF0038 Bus error during transfer.
BFFF0039 Unable to queue asynchronous operation.
BFFF003A Unable to start operation because setup is invalid.
BFFF003B Unable to queue the asynchronous operation.
BFFF003C Insufficient resources to perform memory allocation.
BFFF003D Invalid buffer mask specified.
BFFF003E I/O error.
BFFF003F Format specifier invalid.
BFFF0041 Format specifier not supported.
BFFF0042 Trigger line is currently in use.
BFFF004A Service request not received for the session.
BFFF004E Invalid address space specified.
BFFF0051 Invalid offset specified.
BFFF0052 Invalid access width specified.
BFFF0054 Offset not accessible from this hardware.
BFFF0055 Source and destination widths are different.
BFFF0057 Session not currently mapped.
BFFF0059 Previous response still pending.
BFFF005F No listeners condition detected.
BFFF0060 Interface not currently the controller in charge.
BFFF0061 Interface not the system controller.
BFFF0067 Session does not support this operation.
BFFF006A A parity error occurred during transfer.
BFFF006B A framing error occurred during transfer.
BFFF006C An overrun error occurred during transfer.
BFFF0070 Offset not properly aligned for operation access width.
BFFF0071 Specified user buffer not valid.
BFFF0072 Resource valid, but VISA cannot access it.
BFFF0076 Width not supported by this hardware.
BFFF0078 Invalid parameter value, parameter unknown.
BFFF0079 Invalid protocol.
BFFF007B Invalid window size.
BFFF0080 Session currently contains a mapped window.
BFFF0081 Operation not implemented.
BFFF0083 Invalid length.
BFFF0091 Invalid mode.
BFFF009C Session did not have a lock on the resource.
BFFF009D The device does not export any memory.
BFFF009E VISA-required code library not located or not loaded.
         
VXIPnP Driver Status Codes:
Status    Description
-------------------------------------------------
WARNINGS:
3FFC0101  Instrument does not have ID Query capability.
3FFC0102  Instrument does not have Reset capability.
3FFC0103  Instrument does not have Self-Test capability.
3FFC0104  Instrument does not have Error Query capability.
3FFC0105  Instrument does not have Revision Query capability.
          
ERRORS:
BFFC0001  Parameter 1 out of range, or error trying to set it.
BFFC0002  Parameter 2 out of range, or error trying to set it.
BFFC0003  Parameter 3 out of range, or error trying to set it.
BFFC0004  Parameter 4 out of range, or error trying to set it.
BFFC0005  Parameter 5 out of range, or error trying to set it.
BFFC0006  Parameter 6 out of range, or error trying to set it.
BFFC0007  Parameter 7 out of range, or error trying to set it.
BFFC0008  Parameter 8 out of range, or error trying to set it.
BFFC0011  Instrument failed the ID Query.
BFFC0012  Invalid response from instrument.     ?    Returns the user-readable message string that corresponds to the status code you specify.

You must pass a ViChar array with at least 256 bytes.
    }    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None

This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

        The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

You can pass VI_NULL for this parameter.  This is useful when one of the initialize functions fail.

Default Value:  VI_NULL
   U? =  ?  h    Error Code                       z? = ? ? ?    Error Message                    {M#????  ?    Status                           ??-   ?  ?    Instrument Handle                  0    	            	           VI_NULL    P    This function invalidates the cached values of all attributes for the session.    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ?D#????  ?    Status                           ??-   ?  ?    Instrument Handle                  	              b    This function returns the highest-level channel name that corresponds to the specific driver channel string that is in the channel table at an index you specify.  By passing 0 for the buffer size, the caller can ascertain the buffer size required to get the entire channel name string and then call the function again with a sufficiently large buffer.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None    %    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

If the current value of the return buffer, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

To obtain a text description of the status code, or if the status code is not listed below, call the zup366_error_message function.  To obtain additional information about the error condition, use the zup366_GetError and zup366_ClearError functions.

The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors
     <    A 1-based index into the channel table.

Default Value: 1
        Returns the highest-level channel name that corresponds to the specific driver channel string that is in the channel table at an index you specify..

The buffer must contain at least as many elements as the value you specify with the Buffer Size parameter.  If the channel name description, including the terminating NUL byte, contains more bytes than you indicate with the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass 0 for the Buffer Size, you can pass VI_NULL for this parameter.    ?    Pass the number of bytes in the ViChar array you specify for the Channel Name parameter.

If the channel name, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies BufferSize - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Channel Name buffer parameter.

Default Value:  None   ?k-   ?  ?    Instrument Handle                ? #????  ?    Status or Required Size          ?M = 0 ?  ?    Index                            ?? ?6 ?  ?    Channel Name                     ?? ? 1 ?  ?    Buffer Size                            	           1    	               ?    This function retrieves and then clears the IVI error information for the session or the current execution thread. One exception exists: If the BufferSize parameter is 0, the function does not clear the error information. By passing 0 for the buffer size, the caller can ascertain the buffer size required to get the entire error description string and then call the function again with a sufficiently large buffer.

If the user specifies a valid IVI session for the InstrumentHandle parameter, Get Error retrieves and then clears the error information for the session.  If the user passes VI_NULL for the InstrumentHandle parameter, this function retrieves and then clears the error information for the current execution thread.  If the InstrumentHandle parameter is an invalid session, the function does nothing and returns an error. Normally, the error information describes the first error that occurred since the user last called zup366_GetError or zup366_ClearError.
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

If the current value of the return buffer, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Pass the number of bytes in the ViChar array you specify for the Description parameter.

If the error description, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies BufferSize - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Description buffer parameter.

Default Value:  None     ?    Returns the error code for the session or execution thread.

If you pass 0 for the Buffer Size, you can pass VI_NULL for this parameter.
    ?    Returns the error description for the IVI session or execution thread.  If there is no description, the function returns an empty string.

The buffer must contain at least as many elements as the value you specify with the Buffer Size parameter.  If the error description, including the terminating NUL byte, contains more bytes than you indicate with the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass 0 for the Buffer Size, you can pass VI_NULL for this parameter.   ??#????  ?    Status or Required Size          ?w-   ?  ?    Instrument Handle                ?- 3< ?  ?    BufferSize                       ? 3 P ?  ?    Code                             ?? ? Q ? ?    Description                        	                   	           	           ^    This function clears the error code and error description for the IVI session. If the user specifies a valid IVI session for the InstrumentHandle parameter, this function clears the error information for the session. If the user passes VI_NULL for the Vi parameter, this function clears the error information for the current execution thread. If the Vi parameter is an invalid session, the function does nothing and returns an error.
The function clears the error code by setting it to VI_SUCCESS.  If the error description string is non-NULL, the function de-allocates the error description string and sets the address to VI_NULL.

Maintaining the error information separately for each thread is useful if the user does not have a session handle to pass to the zup366_GetError function, which occurs when a call to zup366_init or zup366_InitWithOptions fails.    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   ?7#????  ?    Status                           ??-   ?  ?    Instrument Handle                  	              ?    This function returns the coercion information associated with the IVI session.  This function retrieves and clears the oldest instance in which the instrument driver coerced a value you specified to another value.

If you set the ZUP366_ATTR_RECORD_COERCIONS attribute to VI_TRUE, the instrument driver keeps a list of all coercions it makes on ViInt32 or ViReal64 values you pass to instrument driver functions.  You use this function to retrieve information from that list.

If the next coercion record string, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Coercion Record buffer parameter.

The function returns an empty string in the Coercion Record parameter if no coercion records remain for the session.

    C    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

If the current value of the return buffer, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init function.  The handle identifies a particular instrument session.

Default Value:  None    ?    Returns the next coercion record for the IVI session.  If there are no coercion records, the function returns an empty string.

The buffer must contain at least as many elements as the value you specify with the Buffer Size parameter.  If the next coercion record string, including the terminating NUL byte, contains more bytes than you indicate with the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

This parameter returns an empty string if no coercion records remain for the session.
    ?    Pass the number of bytes in the ViChar array you specify for the Coercion Record parameter.

If the next coercion record string, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Coercion Record buffer parameter.

Default Value:  None

   ??#????  ?    Status or Required Size          ?-   ?  ?    Instrument Handle                ʲ ? Q ? ?    Coercion Record                  ͸ = ? ?  ?    Buffer Size                        	               	               ,    This function returns the interchangeability warnings associated with the IVI session. It retrieves and clears the oldest instance in which the class driver recorded an interchangeability warning.  Interchangeability warnings indicate that using your application with a different instrument might cause different behavior. You use this function to retrieve interchangeability warnings.

The driver performs interchangeability checking when the ZUP366_ATTR_INTERCHANGE_CHECK attribute is set to VI_TRUE.

The function returns an empty string in the Interchange Warning parameter if no interchangeability warnings remain for the session.

In general, the instrument driver generates interchangeability warnings when an attribute that affects the behavior of the instrument is in a state that you did not specify.    #    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

If the current value of the return buffer, including the terminating NUL byte, is larger than the size you indicate in the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

To obtain a text description of the status code, or if the status code is not listed below, call the zup366_error_message function.  To obtain additional information about the error condition, use the zup366_GetError and zup366_ClearError functions.

The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Pass the number of bytes in the ViChar array you specify for the Interchange Warning parameter.

If the next interchangeability warning string, including the terminating NUL byte, contains more bytes than you indicate in this parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value. For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

If you pass a negative number, the function copies the value to the buffer regardless of the number of bytes in the value.

If you pass 0, you can pass VI_NULL for the Interchange Warning buffer parameter.

Default Value:  None
        Returns the next interchange warning for the IVI session. If there are no interchange warnings, the function returns an empty  string.

The buffer must contain at least as many elements as the value you specify with the Buffer Size parameter. If the next interchangeability warning string, including the terminating NUL byte, contains more bytes than you indicate with the Buffer Size parameter, the function copies Buffer Size - 1 bytes into the buffer, places an ASCII NUL byte at the end of the buffer, and returns the buffer size you must pass to get the entire value.  For example, if the value is "123456" and the Buffer Size is 4, the function places "123" into the buffer and returns 7.

This parameter returns an empty string if no interchangeability
warnings remain for the session.

   ??#????  ?    Status or Required Size          ? -   ?  ?    Instrument Handle                ݶ = ? ?  ?    Buffer Size                      ?? ? Q ? ?    Interchange Warning                	                   	            A    This function clears the list of current interchange warnings.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

   ?-   ?  ?    Instrument Handle                ??#????  ?    Status                                 	          ?    When developing a complex test system that consists of multiple test modules, it is generally a good idea to design the test modules so that they can run in any order.  To do so requires ensuring that each test module completely configures the state of each instrument it uses.  If a particular test module does not completely configure the state of an instrument, the state of the instrument depends on the configuration from a previously executed test module.  If you execute the test modules in a different order, the behavior of the instrument and therefore the entire test module is likely to change.  This change in behavior is generally instrument specific and represents an interchangeability problem.

You can use this function to test for such cases.  After you call this function, the interchangeability checking algorithms in the specific driver ignore all previous configuration operations.  By calling this function at the beginning of a test module, you can determine whether the test module has dependencies on the operation of previously executed test modules.

This function does not clear the interchangeability warnings from the list of previously recorded interchangeability warnings.  If you want to guarantee that the zup366_GetNextInterchangeWarning function only returns those interchangeability warnings that are generated after calling this function, you must clear the list of interchangeability warnings.  You can clear the interchangeability warnings list by repeatedly calling the zup366_GetNextInterchangeWarning function until no more interchangeability warnings are returned.  If you are not interested in the content of those warnings, you can call the zup366_ClearInterchangeWarnings function.
     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

   ??-   ?  ?    Instrument Handle                ?\#????  ?    Status                                 	          "    This function obtains a multithread lock on the instrument session.  Before it does so, it waits until all other execution threads have released their locks on the instrument session.

Other threads might have obtained a lock on this session in the following ways:

- The user's application called zup366_LockSession.

- A call to the instrument driver locked the session.

- A call to the IVI engine locked the session.

After your call to zup366_LockSession returns successfully, no other threads can access the instrument session until you call zup366_UnlockSession.

Use zup366_LockSession and zup366_UnlockSession around a sequence of calls to instrument driver functions if you require that the instrument retain its settings through the end of the sequence.

You can safely make nested calls to zup366_LockSession within the same thread.  To completely unlock the session, you must balance each call to zup366_LockSession with a call to zup366_UnlockSession.  If, however, you use the Caller Has Lock parameter in all calls to zup366_LockSession and zup366_UnlockSession within a function, the IVI Library locks the session only once within the function regardless of the number of calls you make to zup366_LockSession.  This allows you to call zup366_UnlockSession just once at the end of the function. 
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
         
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    This parameter serves as a convenience.  If you do not want to use this parameter, pass VI_NULL. 

Use this parameter in complex functions to keep track of whether you obtain a lock and therefore need to unlock the session.  Pass the address of a local ViBoolean variable.  In the declaration of the local variable, initialize it to VI_FALSE.  Pass the address of the same local variable to any other calls you make to zup366_LockSession or zup366_UnlockSession in the same function.

The parameter is an input/output parameter.  zup366_LockSession and zup366_UnlockSession each inspect the current value and take the following actions:

- If the value is VI_TRUE, zup366_LockSession does not lock the session again.  If the value is VI_FALSE, zup366_LockSession obtains the lock and sets the value of the parameter to VI_TRUE.

- If the value is VI_FALSE, zup366_UnlockSession does not attempt to unlock the session.  If the value is VI_TRUE, zup366_UnlockSession releases the lock and sets the value of the parameter to VI_FALSE.
 
Thus, you can, call zup366_UnlockSession at the end of your function without worrying about whether you actually have the lock.  

Example:

ViStatus TestFunc (ViSession vi, ViInt32 flags)
{
    ViStatus error = VI_SUCCESS;
    ViBoolean haveLock = VI_FALSE;

    if (flags & BIT_1)
        {
        viCheckErr( zup366_LockSession(vi, &haveLock));
        viCheckErr( TakeAction1(vi));
        if (flags & BIT_2)
            {
            viCheckErr( zup366_UnlockSession(vi, &haveLock));
            viCheckErr( TakeAction2(vi));
            viCheckErr( zup366_LockSession(vi, &haveLock);
            } 
        if (flags & BIT_3)
            viCheckErr( TakeAction3(vi));
        }

Error:
    /* 
       At this point, you cannot really be sure that 
       you have the lock.  Fortunately, the haveLock 
       variable takes care of that for you.          
    */
    zup366_UnlockSession(vi, &haveLock);
    return error;
}   	 ?#????  ?    Status                           	-   ?  ?    Instrument Handle                	? H ? ?  ?    Caller Has Lock                    	               	            ?    This function releases a lock that you acquired on an instrument session using zup366_LockSession.  Refer to zup366_LockSession for additional information on session locks.
    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
   
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
    ?    This parameter serves as a convenience.  If you do not want to use this parameter, pass VI_NULL. 

Use this parameter in complex functions to keep track of whether you obtain a lock and therefore need to unlock the session. 
Pass the address of a local ViBoolean variable.  In the declaration of the local variable, initialize it to VI_FALSE.  Pass the address of the same local variable to any other calls you make to zup366_LockSession or zup366_UnlockSession in the same function.

The parameter is an input/output parameter.  zup366_LockSession and zup366_UnlockSession each inspect the current value and take the following actions:

- If the value is VI_TRUE, zup366_LockSession does not lock the session again.  If the value is VI_FALSE, zup366_LockSession obtains the lock and sets the value of the parameter to VI_TRUE.

- If the value is VI_FALSE, zup366_UnlockSession does not attempt to unlock the session.  If the value is VI_TRUE, zup366_UnlockSession releases the lock and sets the value of the parameter to VI_FALSE.
 
Thus, you can, call zup366_UnlockSession at the end of your function without worrying about whether you actually have the lock.  

Example:

ViStatus TestFunc (ViSession vi, ViInt32 flags)
{
    ViStatus error = VI_SUCCESS;
    ViBoolean haveLock = VI_FALSE;

    if (flags & BIT_1)
        {
        viCheckErr( zup366_LockSession(vi, &haveLock));
        viCheckErr( TakeAction1(vi));
        if (flags & BIT_2)
            {
            viCheckErr( zup366_UnlockSession(vi, &haveLock));
            viCheckErr( TakeAction2(vi));
            viCheckErr( zup366_LockSession(vi, &haveLock);
            } 
        if (flags & BIT_3)
            viCheckErr( TakeAction3(vi));
        }

Error:
    /* 
       At this point, you cannot really be sure that 
       you have the lock.  Fortunately, the haveLock 
       variable takes care of that for you.          
    */
    zup366_UnlockSession(vi, &haveLock);
    return error;
}   	?#????  ?    Status                           	?-   ?  ?    Instrument Handle                	8 H ? ?  ?    Caller Has Lock                    	               	           ?    This function performs the following operations:

- Closes the instrument I/O session.

- Destroys the instrument driver session and all of its attributes.

- Deallocates any memory resources the driver uses.

Notes:

(1) You must unlock the session before calling zup366_close.

(2) After calling zup366_close, you cannot use the instrument driver again until you call zup366_init or zup366_InitWithOptions.

    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
           
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   	"M#????  ?    Status                           	(?-   ?  ?    Instrument Handle                  	               ?    This function writes a user-specified string to the instrument.

Note:  This function bypasses IVI attribute state caching.  Therefore, when you call this function, the cached values for all attributes will be invalidated.     2    Pass the string to be written to the instrument.    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
          
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
   	*? A ? ?  ?    Write Buffer                     	+/#????  ?    Status                           	1?-   ?  ?    Instrument Handle                      	               /    This function reads data from the instrument.     c    After this function executes, this parameter contains the data that was read from the instrument.    ?    Returns the status code of this operation.  The status code  either indicates success or describes an error or warning condition.  You examine the status code from each call to an instrument driver function to determine if an error occurred.

To obtain a text description of the status code, call the zup366_error_message function.  To obtain additional information about the error condition, call the zup366_GetError function.  To clear the error information from the driver, call the zup366_ClearError function.
          
The general meaning of the status code is as follows:

Value                  Meaning
-------------------------------
0                      Success
Positive Values        Warnings
Negative Values        Errors

This driver defines the following status codes:
          
Status    Description
-------------------------------------------------
WARNINGS:
None

ERRORS:
None
        
This instrument driver also returns errors and warnings defined by other sources.  The following table defines the ranges of additional status codes that this driver can return.  The table lists the different include files that contain the defined constants for the particular status codes:
          
Numeric Range (in Hex)   Status Code Types    
-------------------------------------------------
3FFA2000 to 3FFA3FFF     IviDCPwr Warnings
3FFA0000 to 3FFA1FFF     IVI      Warnings
3FFF0000 to 3FFFFFFF     VISA     Warnings
3FFC0000 to 3FFCFFFF     VXIPnP   Driver Warnings
          
BFFA2000 to BFFA3FFF     IviDCPwr Errors
BFFA0000 to BFFA1FFF     IVI      Errors
BFFF0000 to BFFFFFFF     VISA     Errors
BFFC0000 to BFFCFFFF     VXIPnP   Driver Errors

     ?    The ViSession handle that you obtain from the zup366_init or zup366_InitWithOptions function.  The handle identifies a particular instrument session.

Default Value:  None
     ?    Pass the maximum number of bytes to read from the instruments.  

Valid Range:  0 to the number of elements in the Read Buffer.

Default Value:  0

     ^    Returns the number of bytes actually read from the instrument and stored in the Read Buffer.   	3^ @ ?  ?    Read Buffer                      	3?#????  ?    Status                           	:V-   ?  ?    Instrument Handle                	; @ C ?  ?    Number of Bytes To Read          	;? ?' ?  ?    Num Bytes Read                     	            	               0    	         ????           (             K.        init                                                                                                                                    ????         )?  O             K.        InitWithOptions                                                                                                                         ????         Q`  ^?             K.        ConfigureOutputEnabled                                                                                                                  ????         _?  uf             K.        ConfigureOutputRange                                                                                                                    ????         v?  ??             K.        ConfigureCurrentLimit                                                                                                                   ????         ?i  ??             K.        ConfigureOVP                                                                                                                            ????         ??  ??             K.        ConfigureVoltageLevel                                                                                                                   ????         ??  ?V             K.        ConfigureRelayPolarity                                                                                                                  ????         ??  ?W             K.        LimitChannelPower                                                                                                                       ????         ?I  ??             K.        ConfigureSMUOutputRegulation                                                                                                            ????         ?7  ?`             K.        ConfigureSMUOutputImpedance                                                                                                             ????         ??  ?4             K.        ConfigureSMUBWOscillation                                                                                                               ????         ? (             K.        ConfigureSMUResistance                                                                                                                  ????        ` ?             K.        ConfigureSMUVoltageLimit                                                                                                                ????         )?             K.        ConfigureSMUCurrentLimit                                                                                                                ????        + 7f             K.        ConfigureSMUOVRPosProtectection                                                                                                         ????        8g D?             K.        ConfigureSMUOVRNegProtectection                                                                                                         ????        E? O?             K.        ConfigureSMUOVRemoteDelay                                                                                                               ????        P? \P             K.        ConfigureSMUCurrentSlewRate                                                                                                             ????        ]F j?             K.        ConfigureTriggerSource                                                                                                                  ????        l? yZ             K.        ConfigureTriggeredVoltageMode                                                                                                           ????        { ??             K.        ConfigureTriggeredCurrentMode                                                                                                           ????        ?, ?             K.        ConfigureTriggeredVoltageLevel                                                                                                          ????        ? ??             K.        ConfigureTriggeredCurrentLimit                                                                                                          ????        ?? ??             K.        ConfigureTriggerWaveformSource                                                                                                          ????        ?  ??             K.        ConfigureHistogramTriggerSource                                                                                                         ????        ş ?k             K.        ConfigureDatalogTriggerSource                                                                                                           ????        ?? ?             K.        ConfigureDatalogCurrVoltTrigger                                                                                                         ????        ?? ?D             K.        ConfigureExternalDatalogTriggerSource                                                                                                   ????        ?Q x             K.        ConfigureList                                                                                                                           ????        9 \             K.        ConfigureDwellList                                                                                                                      ????        ? /i             K.        ConfigureVoltageList                                                                                                                    ????        0? A?             K.        ConfigureCurrentList                                                                                                                    ????        B? Oc             K.        ConfigureBOSTList                                                                                                                       ????        P? ]             K.        ConfigureEOSTList                                                                                                                       ????        ^5 p?             K.        ConfigureMeasurement                                                                                                                    ????        r? ?8             K.        ConfigureMeasurementRange                                                                                                               ????        ?? ??             K.        ConfigureMeasurementResolution                                                                                                          ????        ?? ?r             K.        ConfigureSimulatanMeasurement                                                                                                           ????        ?? ?              K.        ConfigurePin                                                                                                                            ????        ?? ?f             K.        WriteToOuputPort                                                                                                                        ????        ? ?b             K.        ReadInputPort                                                                                                                           ????        ? ??             K.        ConfigureChannelGrouping                                                                                                                ????        ?? ?             K.        UngroupChannels                                                                                                                         ????        ?  ?             K.        ConfigureWaveformParameters                                                                                                             ????        ? ?         
    K.        ConfigureSequenceParameters                                                                                                             ????        $[ QW         	    K.        ConfigureExponentialWfmSeq                                                                                                              ????        S? }?         	    K.        ConfigurePulseWfmSeq                                                                                                                    ????        ? ?k         	    K.        ConfigureRampWfmSeq                                                                                                                     ????        ?? ?X             K.        ConfigureSinusoidWfmSeq                                                                                                                 ????        ?          
    K.        ConfigureStaircaseWfmSeq                                                                                                                ????        ? 2K             K.        ConfigureStepWfmSeq                                                                                                                     ????        4< dV             K.        ConfigureTrapezoidWfmSeq                                                                                                                ????        f? ~?             K.        ConfigureConstantDwellWaveform                                                                                                          ????        ?? ??         	    K.        ConfigureUserDefinedWaveform                                                                                                            ????        ?? ?=         
    K.        ConfigureUserDefinedSequence                                                                                                            ????        ?? ??             K.        ConfigureDatalog                                                                                                                        ????        ?J ??             K.        ConfigureDatalogMeasurement                                                                                                             ????        ?? ??             K.        ConfigureDatalogMarkers                                                                                                                 ????        ?? C         	    K.        ConfigureExternalDatalog                                                                                                                ????        ? *b             K.        ConfigureHistogram                                                                                                                      ????        +? @h             K.        SetAttributeViInt32                                                                                                                     ????        B V?             K.        SetAttributeViReal64                                                                                                                    ????        X% l?             K.        SetAttributeViString                                                                                                                    ????        nD ??             K.        SetAttributeViBoolean                                                                                                                   ????        ?h ??             K.        SetAttributeViSession                                                                                                                   ????        ?? ??             K.        GetAttributeViInt32                                                                                                                     ????        ?5 ??             K.        GetAttributeViReal64                                                                                                                    ????        ?? ??             K.        GetAttributeViString                                                                                                                    ????        ?h ??             K.        GetAttributeViBoolean                                                                                                                   ????        ? ??             K.        GetAttributeViSession                                                                                                                   ????        ?? ?             K.        CheckAttributeViInt32                                                                                                                   ????        } ?             K.        CheckAttributeViReal64                                                                                                                  ????        !5 1K             K.        CheckAttributeViString                                                                                                                  ????        2? C	             K.        CheckAttributeViBoolean                                                                                                                 ????        D? T?             K.        CheckAttributeViSession                                                                                                                 ????        Vb a/             K.        Measure                                                                                                                                 ????        b? r             K.        MeasureArray                                                                                                                            ????        t^ ~?             K.        Fetch                                                                                                                                   ????        ?? ?[             K.        FetchArray                                                                                                                              ????        ?t ??             K.        AbortDatalog                                                                                                                            ????        ?  ?'             K.        AbortExternalDatalog                                                                                                                    ????        ?? ??             K.        AbortHistogram                                                                                                                          ????        ?a ??             K.        InitiateDatalog                                                                                                                         ????        ?? ?)             K.        InitiateExternalDatalog                                                                                                                 ????        ?? ?>             K.        InitiateHistogram                                                                                                                       ????        ?? ??             K.        FetchDatalog                                                                                                                            ????        ?? ??             K.        FetchDatalogMeasurement                                                                                                                 ????        ?? ?h             K.        FetchExternalDatalog                                                                                                                    ????        ?? ?[             K.        FetchHistogram                                                                                                                          ????        ?? ?C             K.        ReadHistogram                                                                                                                           ????         ? ?             K.        Initiate                                                                                                                                ????        % ?             K.        InitiateAcquisition                                                                                                                     ????        ?  ?             K.        Abort                                                                                                                                   ????        !g ,Z             K.        AbortAcquisition                                                                                                                        ????        - 5?             K.        SendSoftwareTrigger                                                                                                                     ????        6
 @?             K.        ImmediateAcquisitionTrigger                                                                                                             ????        AD L             K.        ImmediateTransientTrigger                                                                                                               ????        L? UB             K.        ImmediateDatalogTrigger                                                                                                                 ????        U? ^t             K.        ImmediateExternalDatalogTrigger                                                                                                         ????        _) i{             K.        ImmediateHistogramTrigger                                                                                                               ????        j0 z?             K.        QueryOutputState                                                                                                                        ????        |? ?1             K.        QueryMaxCurrentLimit                                                                                                                    ????        ?d ??             K.        QueryMaxVoltageLevel                                                                                                                    ????        ?. ??             K.        ResetOutputProtection                                                                                                                   ????        ?b ??             K.        ChannelGroupingStatus                                                                                                                   ????        ?N ?]             K.        QueryWaveformQuality                                                                                                                    ????        ?S ?             K.        QueryHistogram                                                                                                                          ????        ?? ??             K.        viWrite                                                                                                                                 ????        ? ??             K.        viRead                                                                                                                                  ????        ?? ?M             K.        reset                                                                                                                                   ????        ?? ?             K.        ResetWithDefaults                                                                                                                       ????        ?? ?             K.        Disable                                                                                                                                 ????        ? ?             K.        self_test                                                                                                                               ????        ? Y             K.        revision_query                                                                                                                          ????        U '             K.        ConvertWaveformToList                                                                                                                   ????        '? 0h             K.        ResetSequence                                                                                                                           ????        1 =c             K.        SaveRecallSequence                                                                                                                      ????        >? J3             K.        FileManagement                                                                                                                          ????        K? T              K.        error_query                                                                                                                             ????        U ??             K.        error_message                                                                                                                           ????        ?? ??             K.        InvalidateAllAttributes                                                                                                                 ????        ? ?|             K.        GetChannelName                                                                                                                          ????        ?? ??             K.        GetError                                                                                                                                ????        ?? ?|             K.        ClearError                                                                                                                              ????        ?? Э             K.        GetNextCoercionRecord                                                                                                                   ????        ѡ ??             K.        GetNextInterchangeWarning                                                                                                               ????        ?? ?`             K.        ClearInterchangeWarnings                                                                                                                ????        ?? ??             K.        ResetInterchangeCheck                                                                                                                   ????        ?d 	?             K.        LockSession                                                                                                                             ????        	C 	?             K.        UnlockSession                                                                                                                           ????        	 ? 	)?             K.        close                                                                                                                                   ????        	* 	2t             K.        WriteInstrData                                                                                                                          ????        	3' 	<             K.        ReadInstrData                                                                                                                                 9                                                                                     ?Initialize                                                                           ?Initialize With Options                                                             0Configuration Functions                                                             !Output                                                                               ?Configure Output Enabled                                                             ?Configure Output Range                                                               ?Configure Current Limit                                                              ?Configure OVP                                                                        ?Configure Voltage Level                                                              ?Configure Relay Polarity                                                             ?Limit Channel Power                                                                  ?Configure SMU Output Regulation                                                      ?Configure SMU Output Impedance                                                       ?Configure SMU BW Oscillation                                                         ?Configure SMU Resistance                                                             ?Configure SMU Voltage Limit                                                          ?Configure SMU Current Limit                                                          ?Configure SMU OVR Pos Protect                                                        ?Configure SMU OVR Neg Protect                                                        ?Configure SMU OV Remote Delay                                                        ?Configure SMU Current Slew Rate                                                     dTriggering                                                                           ?Configure Trigger Source                                                             ?Configure Triggered Voltage Mod                                                      ?Configure Triggered Current Mod                                                      ?Configure Triggered Voltage Lev                                                      ?Configure Triggered Current Lim                                                      ?Configure Trigger Waveform Src                                                       ?Configure Histogram Trigger Src                                                      ?Configure Datalog Trigger Src                                                        ?ConfigureDatalogCurrVoltTrigger                                                      ?Configure External Datalog Trigger Source                                           ?List                                                                                 ?Configure List                                                                       ?Configure Dwell List                                                                 ?Configure Voltage List                                                               ?Configure Current List                                                              ?Trigger Output                                                                       ?Configure BOST List                                                                  ?Configure EOST List                                                                 @Measurement                                                                          ?Configure Measurement                                                                ?Configure Measurement Range                                                          ?Configure Measurement Resolution                                                     ?Configure Simultaneous Measure                                                      ?Digital                                                                              ?Configure Pin                                                                        ?Write To Ouput Port                                                                  ?Read Input Port                                                                     ?Group                                                                                ?Configure Channel Grouping                                                           ?Ungroup Channels                                                                    9Waveform/Sequence                                                                    ?Configure Waveform Parameters                                                        ?Configure Sequence Parameters                                                        ?Configure Exponential Wfm Seq                                                        ?Configure Pulse Wfm Seq                                                              ?Configure Ramp Wfm Seq                                                               ?Configure Sinusoid Wfm Seq                                                           ?Configure Staircase Wfm Seq                                                          ?Configure Step Wfm Seq                                                               ?Configure Trapezoid Wfm Seq                                                          ?Configure CDwell Waveform                                                            ?Configure User Defined Waveform                                                      ?Configure User Defined Sequence                                                     ?Datalog                                                                              ?Configure Datalog                                                                    ?Configure Datalog Measurement                                                        ?Configure Datalog Markers                                                            ?Configure External Datalog                                                           ?Configure Histogram                                                                 Set/Get/Check Attribute                                                             kSet Attribute                                                                        ?Set Attribute ViInt32                                                                ?Set Attribute ViReal64                                                               ?Set Attribute ViString                                                               ?Set Attribute ViBoolean                                                              ?Set Attribute ViSession                                                             ?Get Attribute                                                                        ?Get Attribute ViInt32                                                                ?Get Attribute ViReal64                                                               ?Get Attribute ViString                                                               ?Get Attribute ViBoolean                                                              ?Get Attribute ViSession                                                             	?Check Attribute                                                                      ?Check Attribute ViInt32                                                              ?Check Attribute ViReal64                                                             ?Check Attribute ViString                                                             ?Check Attribute ViBoolean                                                            ?Check Attribute ViSession                                                           
Measure Output                                                                       ?Measure                                                                              ?Measure Array                                                                        ?Fetch                                                                                ?Fetch Array                                                                         
lMeasurement                                                                         *Low Level                                                                            ?Abort Datalog                                                                        ?Abort External Datalog                                                               ?Abort Histogram                                                                      ?Initiate Datalog                                                                     ?Initiate External Datalog                                                            ?Initiate Histogram                                                                   ?Fetch Datalog                                                                        ?Fetch Datalog Measurement                                                            ?Fetch External Datalog                                                               ?Fetch Histogram                                                                      ?Read Histogram                                                                      ?Action/Status Functions                                                              ?Initiate                                                                             ?Initiate Acquisition                                                                 ?Abort                                                                                ?Abort Acquisition                                                                    ?Send Software Trigger                                                                ?Immediate Acquisition Trigger                                                        ?Immediate Transient Trigger                                                          ?Immediate Datalog Trigger                                                            ?Immediate External Datalog Trigger                                                   ?Immediate Histogram Trigger                                                          ?Query Output State                                                                   ?Query Max Current Limit                                                              ?Query Max Voltage Level                                                              ?Reset Output Protection                                                              ?Channel Grouping Status                                                              ?Query Waveform Quality                                                               ?Query Histogram                                                                     DSystem                                                                               ?Write Bytes                                                                          ?Read Bytes                                                                          ?Utility Functions                                                                    ?Reset                                                                                ?Reset With Defaults                                                                  ?Disable                                                                              ?Self-Test                                                                            ?Revision Query                                                                       ?Convert Waveform To UDef List                                                        ?Reset Sequence                                                                       ?Save Recall Sequence                                                                 ?File Management                                                                      ?Error-Query                                                                          ?Error Message                                                                        ?Invalidate All Attributes                                                            ?Get Channel Name                                                                    ?Error Info                                                                           ?Get Error                                                                            ?Clear Error                                                                         SCoercion Info                                                                        ?Get Next Coercion Record                                                            ?Interchangeability Info                                                              ?Get Next Interchange Warning                                                         ?Clear Interchange Warnings                                                           ?Reset Interchange Check                                                             ?Locking                                                                              ?Lock Session                                                                         ?Unlock Session                                                                       ?Close                                                                               _Obsolete                                                                            ?Instrument I/O                                                                       ?Write Instrument Data                                                                ?Read Instrument Data                                                            