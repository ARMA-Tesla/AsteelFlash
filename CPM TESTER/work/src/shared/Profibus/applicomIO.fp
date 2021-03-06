s??         ??     ?^  `   ?   ????                                           applicomIO                                       ? ??DWORD     ? ??BYTE     ? ??WORD     ? ??BOOL  ?    Initializing the applicomIO?  product => IO_Init

Prototype:
BOOL WINAPI IO_Init( WORD wCard, short * pStatus );

Syntax:
IO_Init( wCard , &status ) 
 


Description:
This function is used to initialize the IO mode on an applicomIO? interface. It is essential to call this function for each board configured before any other function in the library. It performs all operations required for operation of applicomIO? (checking the mapping, initializing offsets and size of each device, etc.).     /    Input parameter

16 bit integer, board number    A    Status returned

16 bit integer

Status = 0   Indicates correct initialization of the applicomIO? driver.
Status = 36  Indicates that no devices are configured.
Status = 45  Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 93  Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.    ? L   ?       wCard                             ? H ?         status                          ???? " #??                                         ???? " ???                                           5 ? ????       Success                         ???? ? ???                                                	            Input parameter    Status returned    	            Value returned    !    Status returned

32 bit integer     7    Value returned

TRUE if OK, FALSE in case of problem.     L _  ?       status                            / ? _???       Success                         ????  d??                                         ???? ? n??                                            	            	            Output parameters    Return value    !    Status returned

32 bit integer     7    Value returned

TRUE if OK, FALSE in case of problem.     O m  ?       status                            ? ? m???       Success                         ????  r??                                         ???? ? |??                                            	            	            Output parameters    Return value   ?    End of program  => IO_Exit

Prototype:
BOOL WINAPI IO_Exit( WORD wCard, short * pStatus );

Syntax:
IO_Exit( wCard , &status )
 

Description:
This function must be called at the end of the program using applicomIO? (even if IO_init returned a bad status).


Remark:
If your application has activated the watchdog, it will not be deactivated by calling this function. You can disable it if necessary by calling the function IO_SetWatchDog before the function IO_Exit.     /    Input parameter

16 bit integer, board number    A    Status returned

16 bit integer

Status = 0   Indicates correct initialization of the applicomIO? driver.
Status = 36  Indicates that no devices are configured.
Status = 45  Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 93  Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.    
? :   ?       wCard                              : ?         status                            U ? ????       Success                         ????  &??                                         ????  ???                                         ????  ???                                                	            	            Input parameter    Status returned    Value returned   ?    Input acquisition from applicomIO? interfaces => IO_RefreshInput


Prototype:
BOOL WINAPI IO_RefreshInput(WORD wCard, short * pStatus );

Syntax:
IO_RefreshInput( wCard , &status )
 

Description:
This function retrieves from the applicomIO? interfaces all input data and statuses of devices present on the network.These values are not returned directly but are stored in local buffers. All read functions of type IO_ReadXXX and the access functions to the device statuses then use these local buffers.

The function IO_RefreshInput ensures complete coherence of the data read together with maximum execution speed (the calling application can never be blocked due to synchronization problems).     /    Input parameter

16 bit integer, board number    R    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.    ? J &  ?       wCard                              J ?         status                            w ? ????       Success                         ???? $ 0??                                         ???? $ ???                                         ???? ? ???                                                	            	            Input parameter    Status returned    Value returned   ?    Reading input bits  => IO_ReadIBit


Prototype:
BOOL WINAPI IO_ReadIBit(WORD wCard, WORD wEquip, WORD Offsetbit, WORD Nb, BYTE * TabBit, short * pStatus );

Syntax:
IO_ReadIBit(wCard,wEquip,Offsetbit,Nb, TabBit, &Status );
 

Description:
This function is used to read the input bits of a device in non-packed format from the local buffer. 
N.B.?:The local buffer must be updated beforehand with the function IO_RefreshInput.

Remark:
The offsetbit parameter corresponds to the position in number of bits of the first bit to be read from the start of the device data area.
Example?: if the device contains 3 data bytes, the offset of bit 0 in the third  byte will be 16.     /    Input parameter

16 bit integer, board number    v    Status returned

16 bit integer

Status = 0   Indicates that the function was executed correctly.
Status = 32  Indicates that the parameters passed into the function are not correct (e.g.?: Number of variables too large).
Status = 36  Indicates that the slave number is not configured.
Status = 45  Indicates that the applicomIO? dialogue software is not resident in memory. In this case, intialize the board.
Status = 47  Indicates that the board number passed into the function is not valid.
Status = 93  Driver inaccessible.
Status = 255 Indicate that the local buffer was not updated before by the IO_RefreshInput function.      7    Value returned

TRUE if OK, FALSE in case of problem.     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO     Q    Input parameter

32 bit integer, address of the first bit to read int he device     t    Input parameter

16 bit integer, number of bits to read (1-MAX), (the maximum number depends on the target device)     6    Output parameter

Bit array, receiving the data read    ? : $  ?       wCard                             ' ? ?         status                            ? ? ????       Success                         ???? 
 -??                                         ???? 
 ???                                         ???? ? ???                                           ? z % ?       wEquip                            ; ? % ?       Offsetbit                         ? ? % ?       Nb                                 = ? ?       TabBit                                 	            	            Input parameters    Output parameters    Return value                	           ?    Reading input bytes => IO_ReadIByte


Prototype:
BOOL WINAPI IO_ReadIByte ( WORD wCard, WORD wEquip, WORD Offset, WORD Nb, BYTE * TabByte, short * pStatus );

Syntax:
IO_ReadIByte(wCard,wEquip,Offset,Nb, TabByte, &Status );
 
Description:
This function is used to read the input bytes of a device from the local buffer N.B.?:The local buffer must be updated beforehand using the function IO_RefreshInput.     /    Input parameter

16 bit integer, board number     7    Output parameter

Byte array, receiving the data read     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO    x    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 32     Indicates that the parameters passed into the function are not correct (e.g.?: Number of variables too large).
Status = 36 Indicates that the slave number is not configured.
Status = 45 Indicates that applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93     Driver inaccessible.
Status = 255    Indicate that the local buffer was not updated before by the IO_RefreshInput function.      U    Input parameter

32 bit integer, address of the first byte to be read in the device     |    Input parameter

16 bit integer, number of bytes to be read (1-MAX).
    (the maximum number depends on the target device)     7    Value returned

TRUE if OK, FALSE in case of problem.    k D   ?       wCard                             ? G ? ?       TabByte                           ? z  ?       wEquip                             8 ? ?         status                            "? ?  ?       Offset                            # ?  ?       Nb                                #? ????       Success                         ????  '??                                         ????  ???                                         ???? ? ???                                                	                	                    	            Input parameters    Output parameters    Return value       Reading input words => IO_ReadIWord


Prototype:
BOOL WINAPI IO_ReadIWord ( WORD wCard, WORD wEquip, WORD Offset, WORD Nb, WORD * TabWord, short * pStatus );

Syntax:
IO_ReadIWord(wCard,wEquip,Offset,Nb, TabWord, &Status );
 
Description:
This function is used to read the input words of a device from the local buffer. N.B.?:The local buffer must be updated beforehand by the function IO_RefreshInput.

Remark:
The values return take into account the data format configured with the Console (little Indian / Big Indian )     /    Input parameter

16 bit integer, board number     7    Output parameter

Word array, receiving the data read     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO    x    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 32     Indicates that the parameters passed into the function are not correct (Ex?: Number of variables too large).
Status = 36 Indicates that the slave number is not configurated.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.
Status = 255    Indicate that the local buffer was not updated before by the IO_RefreshInput function.      W    Input parameter

32 bit integer, address of the first word to be read in the device.
     ?    Input parameter

16 bit integer, number of words to be read (1-MAX), (the maximum number depends on the protocol and the target device)     7    Value returned

TRUE if OK, FALSE in case of problem.    (j : (  ?       wCard                             (? = ? ?       TabWord                           (? p ( ?       wEquip                            )7 ~ ?         status                            +? ? ( ?       Offset                            , ? ( ?       Nb                                ,? ? ????       Success                         ???? 
 0??                                         ???? 
 ???                                         ???? ? ???                                                	                	                    	            Input parameters    Output parameters    Return value       Reading double input words  => IO_ReadIDWord


Prototype:
BOOL WINAPI IO_ReadIDWord ( WORD wCard, WORD wEquip, WORD Offset, WORD Nb, DWORD * TabDWord, short * pStatus );

Syntax:
IO_ReadIDWord(wCard,wEquip,Offset,Nb, TabDWord, &Status );
 
Description:
This function is used to read the double input words of a device from the local buffer. N.B.?:The local buffer must be updated beforehand by the function IO_RefreshInput.

Remark:
The values return take into account the data format configured with the Console (little Indian / Big Indian)     /    Input parameter

16 bit integer, board number     >    Output parameter

Double word array, receiving the data read     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO    z    Status returned

16 bit integer


Status = 0   Indicates that the function was executed correctly.
Status = 32  Indicates that the parameters passed into the function are not correct (e.g.?: Number of variables too large).
Status = 36  Indicates that the slave number is not configurated.
Status = 45  Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47  Indicates that the board number passed into the function is not valid.
Status = 93  Driver inaccessible.
Status = 255 Indicate that the local buffer was not updated before by the IO_RefreshInput function. 

The other statuses correspond to communication statuses of the targeted device. The meaning of the various values returned in the ??STATUS?? variable can be found by consulting the manual relating to each protocol in chapter ??Function return statuses??.     ?    Input parameter

16 bit integer, number of double words to be read (1-MAX).
(the maximum number depends on the protocol and the target device).
     7    Value returned

TRUE if OK, FALSE in case of problem.    1? D    ?       wCard                             1? G ? ?       TabDWord                          2	 z   ?       wEquip                            2` ? ?         status                          ???? ?   ?       Offset                            5? ?   ?       Nb                                6| ? ????       Success                         ????  ???                                         ???? ? ???                                         ????  (??                                                	                	                    	            Output parameters    Return value    Input parameters   :    Writing in the DPM output area  => IO_RefreshOutput


Prototype:
BOOL WINAPI IO_RefreshOutput(WORD wCard, short * pStatus );

Syntax:
IO_RefreshOutput( wCard , &status ) 
 
Description:
This function can be used to write all the outputs set beforehand in the local write buffer (with the functions IO_WriteXXX) to the devices present on the network. The function IO_RefreshOutput ensures complete coherence of the data written. All write functions of type IO_WriteXXX only access a local buffer, thereby ensuring optimum speed before physically writing on the network.     /    Input parameter

16 bit integer, board number    R    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.    ;| :   ?       wCard                             ;? 8 ?         status                            = ? ????       Success                         ???? 
 ???                                         ???? u ???                                         ???? 
 '??                                                	            	            Output parameter    Return value    Input parameter   ?    Writing output bits => IO_WriteQBit


Prototype:
BOOL WINAPI IO_WriteQBit(WORD wCard, WORD wEquip, WORD Offsetbit, WORD Nb, BYTE * TabBit, short * pStatus );

Syntax:
IO_WriteQBit(wCard,wEquip,Offsetbit,Nb, TabBit, &Status );
 
Description:
This function is used to write output bits of a device in a local write buffer. 
N.B.?:The local buffer will then be sent on the network with the function IO_RefreshOutput.

Remark:
The offsetbit parameter corresponds to the position in number of bits of the first bit to be written, from the start of the data area in the device.
Example?: if the device contains 3 data bytes, the offset of bit 0 in the third byte will be 16.     /    Input parameter

16 bit integer, board number     @    Output parameter

Bit array, containing the data to be written     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO        Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 32 Indicates that the parameters passed into the function are not correct (Ex?: Number of variables too large).
Status = 36 Indicates that the slave number is not configurated.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     W    Input parameter

32 bit integer, address of the first bit to be written in the device     ?    Input parameter

16 bit integer, number of bits to be written (1-MAX), (the maximum number depends on the protocol and the target device)     7    Value returned

TRUE if OK, FALSE in case of problem.    A? :   ?       wCard                             A?  ?       TabBit                            B p  ?       wEquip                            BY : ?         status                            Dm ?  ?       Offsetbit                         D? ?  ?       Nb                                E_ ? ????       Success                         ???? 
 ???                                         ????  ???                                         ???? 
 &??                                                	                	                    	            Output parameters    Return value    Input parameters   ?    Writing output bytes => IO_WriteQByte


Prototype:
BOOL WINAPI IO_WriteQByte(WORD wCard, WORD wEquip, WORD Offset, WORD Nb, BYTE * TabByte, short * pStatus );

Syntax:
IO_WriteQByte(wCard,wEquip,Offset,Nb, TabByte, &Status );
 
Description:
This function is used to write output bytes of a device in a local write buffer. 
N.B.?:The local buffer will then be sent on the network with the function IO_RefreshOutput.
     /    Input parameter

16 bit integer, board number     /    Byte array, containing the data to be written     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO        Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 32     Indicates that the parameters passed into the function are not correct (Ex?: Number of variables too large).
Status = 36 Indicates that the slave number is not configurated.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     Y    Input parameter

32 bit integer, address of the first bytes to be written in the device     ?    Input parameter

16 bit integer, number of bytes to be written (1-MAX), (the maximum number depends on the protocol and the target device)     7    Value returned

TRUE if OK, FALSE in case of problem.    I? :   ?       wCard                             I?  ?       TabByte                           J4 p  ?       wEquip                            J? 9 ?         status                            L? ?  ?       Offset                            M ?  ?       Nb                                M? ? ????       Success                         ???? 
 ???                                         ???? u ???                                         ???? 
 $??                                                	                	                    	            Output parameters    Return value    Input parameters       - Writing output words  => IO_WriteQWord


Prototype:
BOOL WINAPI IO_WriteQWord(WORD wCard, WORD wEquip, WORD Offset, WORD Nb, WORD * TabWord, short * pStatus );

Syntax:
IO_WriteQWord(wCard,wEquip,Offsett,Nb, TabWord, &Status );
 
Description:
This function is used to write output words of a device in a local write buffer. 
N.B.?:The local buffer will then be sent on the network with the function IO_RefreshOutput.

Remark:
The values sent take into account the data format configured with the Console (little Indian / Big Indian )     /    Input parameter

16 bit integer, board number     A    Output parameter

Word array, containing the data to be written     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO        Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 32 Indicates that the parameters passed into the function are not correct (Ex?: Number of variables too large).
Status = 36 Indicates that the slave number is not configurated.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     X    Input parameter

32 bit integer, address of the first word to be written in the device     ?    Input parameter

16 bit integer, number of words to be written (1-MAX), (the maximum number depends on the protocol and the target device)     7    Value returned

TRUE if OK, FALSE in case of problem.    Rw :   ?       wCard                             R?  ?       TabWord                           R? p  ?       wEquip                            SN 9 ?         status                            Ub ?  ?       Offset                            U? ?  ?       Nb                                VV ? ????       Success                         ???? 
 ???                                         ???? u ???                                         ???? 
 &??                                                	                	                    	            Output parameters    Return value    Input parameters   )    Writing double output words => IO_WriteQDWord


Prototype:
BOOL WINAPI IO_WriteQDWord(WORD wCard, WORD wEquip, WORD Offset, WORD Nb, DWORD * TabDWord, short * pStatus );

Syntax:
IO_WriteQDWord(wCard,wEquip,Offset,Nb, TabDWord, &Status );
 
Description:
This function is used to write double output words of a device in a local write buffer. 
N.B.?:The local buffer will then be sent on the network with the function IO_RefreshOutput.

Remark:
The values sent take into account the data format configured with the Console (little Indian / Big Indian )     /    Input parameter

16 bit integer, board number     H    Output parameter

Double word array, containing the data to be written     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO        Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 32 Indicates that the parameters passed into the function are not correct (Ex?: Number of variables too large).
Status = 36 Indicates that the slave number is not configurated.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     `    Output parameter

32 bit integer, address of the first double word to be written in the device     ?    Input parameter

16 bit integer, number of double words to be written (1-MAX), (the maximum number depends on the protocol and the target device)     7    Value returned

TRUE if OK, FALSE in case of problem.    [E :    ?       wCard                             [|  ?       TabDWord                          [? p   ?       wEquip                            \# ; ?         status                            ^7 ?  ?       Offset                            ^? ?  ?       Nb                                _: ? ????       Success                         ???? 
 ???                                         ???? x ???                                         ???? 
 (??                                                	                	                    	            Output parameters    Return value    Input parameters    8    Input parameter

16 bit integer, channel number (0-31)     ;    Input parameter

16 bit integer, equipment number (0-255)     F    Input parameter

32 bit integer, parameter associated to the message     C    Input parameter

16 bit integer, number of bytes to send in BufTx     7    Input parameter

Byte array, containing the data sent     E    Output parameter

16 bit integer, number of bytes received in BufRx     <    Output parameter

Byte array, containing the data received     !    Status returned

32 bit integer     7    Value returned

TRUE if OK, FALSE in case of problem.  ????  /??                                         ????  ???                                         ???? ???                                           a? < $  ?       wChan                             b8 { $ ?       wNes                              b{ ? $ ?       dwMsgParam                        b? ? $ ?       wNbTx                             c6 $ ?       BufTx                             cS < ? ?       wNbRx                             c? ? ? ?       BufRx                             c? ? ? ?       status                            d; ????       Success                            Input parameters    Output parameters    Return value                    	            	            	            	            	           ?    Equipment status => IO_GetEquipmentStatus


Prototype:
BOOL WINAPI IO_GetEquipmentStatus(WORD wCard, WORD wEquip, WORD * EquipmentStatus,  short * pStatus );

Syntax:
IO_GetEquipmentStatus(wCard,wEquip,&EquipmentStatus,&status) 
 
Description:
This function retrieves the equipment status on the network.
N.B.?: the function IO_RefreshInput must be called before using IO_GetEquipmentStatus.     /    Input parameter

16 bit integer, board number     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO     3    Input parameter

16 bit integer, equipment status     7    Value returned

TRUE if OK, FALSE in case of problem.    ?    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 36 Indicates that the slave number is not configurated.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.    h? F   ?       wCard                             i z  ?       wEquip                            il I ? ?       EquipmentStatus                   i? ? ????       Success                         ????  ???                                         ???? ? ???                                         ????  ??                                           i? ? ?         status                                     	            	            Output parameters    Return value    Input parameters    	           o    Global status of the network => IO_GetGlobalStatus


Prototype:
BOOL WINAPI IO_GetGlobalStatus(WORD wCard, WORD * GlobalStatus,  short * pStatus ); 


Syntax:
IO_GetGlobalStatus(wCard,&GlobalStatus,&status)  
 
Description:
This function retrieves the global status on the network.
N.B.?: the function IO_RefreshInput must be called before using IO_GetGlobalStatus.     /    Input parameter

16 bit integer, board number     @    Output parameter

16 bit integer, Global status of the network    ?    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 36 Indicates that the slave number is not configurated.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.    o =   ?       wCard                             o< @ ? ?       GlobalStatus                      o? ? ?         status                            q ? ????       Success                         ???? ? ???                                         ????  ???                                         ????  ??                                                	            	            	            Return value    Output parameters    Input parameters   ?    Equipment diagnostic => IO_ReadDiag


Prototype:
BOOL WINAPI IO_ReadDiag(WORD wCard, WORD wEquip, WORD * EquipmentDiag,  short * pStatus ); 

Syntax:
IO_ReadDiag(wCard,wEquip,&EquipmentDiag,&status)  
 
Description:
This function indicates if the equipment has a problem interns (short-circuit on way ...), whereas the communication with this one is correct (status = 0).
During the exchange of the data, this one announces an internal abnormal operation. A precise diagnostic of the equipment in question can be made by the utility " DiadDP ". This functionality is not supported on protocol INTERBUS.
N.B.?: the function IO_RefreshInput must be called before using IO_ReadDiag.     /    Input parameter

16 bit integer, board number     8    Output parameter

16 bit integer, equipment Diagnostic     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO    ?    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 36 Indicates that the slave number is not configurated.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.    u? F   ?       wCard                             v I ? ?       EquipmentDiag                     vZ z  ?       wEquip                            v? ? ?         status                            xL ? ????       Success                         ????  ???                                         ???? ? ???                                         ????  ??                                                	                	            	            Output parameters    Return value    Input parameters   +    Global diagnostic of the network => IO_GetGlobalDiag


Prototype:
BOOL WINAPI IO_GetGlobalDiag(WORD wCard, WORD * GlobalDiag,  short * pStatus );

Syntax:
IO_GetGlobalDiag(wCard,&GlobalDiag,&status)   
 
Description:
This function allows to be informed so at least equipment has a problem interns (short-circuit on way ...). A precise diagnostic of the equipment in question can be made by the utility " DiadDP ". This functionality is not supported on protocol INTERBUS.
N.B.?: the function IO_RefreshInput must be called before using IO_GetGlobalDiag.     /    Input parameter

16 bit integer, board number     D    Output parameter

16 bit integer, Global diagnostic of the network     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO    ?    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 36 Indicates that the slave number is not configurated.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.    |? <   ?       wCard                             } ? ? ?       GlobalDiag                        }N p  ?       wEquip                            }? ? ?         status                            @ ? ????       Success                         ???? 
 ???                                         ???? ? ???                                         ???? 
 ??                                                	                	            	            Output parameters    Return value    Input parameters   U    List of equipment present on the network => IO_GetEquipmentList


Prototype:
BOOL WINAPI IO_GetEquipmentList( WORD wCard, WORD * pByNb, WORD * TabEquip, short * pStatus );

Syntax:
IO_GetEquipmentList(wCard,&Nb, TabEquip,&status)    
 
Description:
This function returns the number of devices present and the list of applicomIO? addresses.     /    Input parameter

16 bit integer, board number     M    Output parameter

16 bit integer, Array of equipment present on the network    R    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.     ]    Output parameter

16 bit integer, number of equipment devices present on the network r?seau    ?? <   ?       wCard                             ?  { ? ?       TabEquip                          ?u ? ?         status                            ?? ????       Success                         ???? 
 ???                                         ???? ? ???                                         ???? 
 ??                                           ? 9 ? ?       Nb                                     	            	            	            Output parameters    Return value    Input parameters    	           a    Sizes of equipment inputs and outputs   => IO_GetEquipmentInfo

Prototype:
BOOL WINAPI IO_GetEquipmentInfo( WORD wCard, WORD wEquip, WORD * InputSize, WORD * OutputSize, short * pStatus );

Syntax:
IO_GetEquipmentInfo(wCard,wEquip,&InputSize,&OutputSize,&status)    
 
Description:
This function returns the sizes in bytes of equipment inputs/outputs.     /    Input parameter

16 bit integer, board number     <    Output parameter

16 bit integer, Sizes of inputs in bytes     =    Output parameter

16 bit integer, Sizes of outputs in bytes    ?    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 33     Indicates that the equipment present on the network is not answering.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 36 Indicates that the slave number is not configurated.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.     O    Input parameter

16 bit integer, device number (0-255) configured in PCCONFIO    ?? <   ?       wCard                             ?( 9 ? ?       InputSize                         ?l { ? ?       OutputSize                        ?? ? ?         status                            ?? ????       Success                         ???? 
 ???                                         ???? ? ???                                         ???? 
 ??                                           ?? ~  ?       wEquip                                 	            	            	            	            Output parameters    Return value    Input parameters           Retrieving the digital contact => IO_GetDigitalInput

Prototype:
WORD WINAPI IO_GetDigitalInput( WORD wCard, short * pStatus );

Syntax:
IO_GetDigitalInput(Wcard,&status)     
 
Description:
This function is used to retrieve the status of the digital contact on the applicomIO? board.     /    Input parameter

16 bit integer, board number    R    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.    ?? <   ?       wCard                             ?? = ?         status                            ?= ? ????       Success                         ???? 
 ???                                         ???? u ???                                         ???? 
 ??                                                	            	            Output parameters    Return value    Input parameters   k    Defining the WatchDog activation time => IO_SetWatchDog

Prototype:
BOOL WINAPI IO_SetWatchDog( WORD wCard, WORD TimeWD, short * pStatus );

Syntax:
IO_SetWatchDog(Wcard,TimeWd,&status)      
 
Description:
This function is used to define a monitoring time, from which the WatchDog will be activated if the functions IO__RefreshInput or IO_RefreshOuput are not recalled. The countdown is reset on each call to one of these functions. The application must ensure that this monitoring time is sufficient, to avoid untimely triggering. The value 0x0000 deactivates this monitoring. 
By default, the watchdog is inactive.     q    Input parameter

16 bit integer, time in ms ? from which the WatchDog is activated.
Time base : TimeWD * 100 ms    R    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     7    Value returned

TRUE if OK, FALSE in case of problem.     /    Input parameter

16 bit integer, board number    ?? u  ?       TimeWD                            ?? 3 ?         status                            ?U ? ????       Success                         ???? 
 ???                                         ???? k ???                                         ???? 
 $??                                           ?? 3   ?       wCard                                  	            	            Output parameters    Return value    Input parameters       ?    Defining the fallback value time => IO_SetReply

Prototype:
BOOL WINAPI IO_SetReply( WORD wCard, WORD TimeReply, short * pStatus );

Syntax:
IO_SetReply(Wcard,TimeReply,&status)       
 
Description:
This function is used to define the monitoring time, from which the outputs will be set to the fallback values (zero) if the functions IO__RefreshInput or IO_RefreshOuput are not recalled. The countdown is reset on each call to one of these functions (setting to fallback values).
The application must ensure that this monitoring time is sufficient, to avoid setting the fallback values on the network. The value 0x0000 deactivates this monitoring. 
By default, the fallback time is inactive.
     /    Input parameter

16 bit integer, board number    R    Status returned

16 bit integer

Status = 0  Indicates that the function was executed correctly.
Status = 45 Indicates that the applicomIO? dialogue software is not resident in memory. In this case, initialize the board.
Status = 47 Indicates that the board number passed into the function is not valid.
Status = 93 Driver inaccessible.     d    Input parameter

16 bit integer, time in ms of the fallback time. Time base : TimeReply * 100 ms.
     7    Value returned

TRUE if OK, FALSE in case of problem.    ?V =   ?       wCard                             ?? = ?         status                            ??   ?       TimeReply                         ?S ? ????       Success                         ????  ???                                         ???? u ???                                         ????  ??                                                	                	            Output parameters    Return value    Input parameters ????          ?  t      ?H    IO_Init                         ????       ????  n     +    AuInitBus_io                    ????       ????  ?      ?8    AuExitBus_io                    ????         ?  ?      ?8    IO_Exit                         ????         &  ?      ?H    IO_RefreshInput                 ????         H  N 
    7^    IO_ReadIBit                     ????         ?  #? 
    BE    IO_ReadIByte                    ????         &W  ,? 
    (I    IO_ReadIWord                    ????         /e  6? 
    6@    IO_ReadIDWord                   ????         9:  =L      ?A    IO_RefreshOutput                ????         >?  E? 
    GF    IO_WriteQBit                    ????         H  M? 
    B9    IO_WriteQByte                   ????         PV  V? 
    D=    IO_WriteQWord                   ????         Y  _y 
    H;    IO_WriteQDWord                  ????       ????  dL     i`    AuWriteReadMsg_io               ????         gM  k?     /4    IO_GetEquipmentStatus           ????         m?  q^     '8    IO_GetGlobalStatus              ????         s2  x?     /.    IO_ReadDiag                     ????         z?       &7    IO_GetGlobalDiag                ????         ??  ?s     G(    IO_GetEquipmentList             ????         ??  ?8 	    G0    IO_GetEquipmentInfo             ????         ??  ?|      ?%    IO_GetDigitalInput              ????         ?  ??      ?;    IO_SetWatchDog                  ????         ??  ??      ?/    IO_SetReply                         ????                                  ????Initialization                       DInitializing the product             DInitializating (applicomio)          DEnd of program (applicomio)          DEnd of program                    ????Read and write functions          ????Read                                 DInput acquisition                    DReading input bits                   DReading input bytes                  DReading input words                  DReading double input words        ????Write                                DWriting in the DPM output area       DWriting output bits                  DWriting output bytes                 DWriting output words                 DWriting double output words       ????Write/Read                           DWriting and reading message       ????Status and diagnostic                DEquipment status                     DGlobal status                        DEquipment diagnostic                 DGlobal diagnostic                    DList of equipment                    DSizes of inputs and outputs          DRetrieving the digital contact       DWatchDog activation time             DFallback value time             