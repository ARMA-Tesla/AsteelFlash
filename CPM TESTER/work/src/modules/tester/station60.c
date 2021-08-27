#include <stdint.h>
#include <userint.h>
#include <ansi_c.h>
#include <windows.h>
#include "tester.h"
#include "station60.h"

#define TIMEOUT_INIT() 	clock_t	_start_clock = clock();

#define TIMEOUT_CHECK(timeout) \
	if ( timeout>0 && ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( TESTER_ERROR_TIMEOUT, "Timeout"); } else
	
#define TIMEOUT_PRINTER_CHECK(timeout) \
	if ( ((clock() - _start_clock) / (double)CLOCKS_PER_SEC) > (timeout/1000.0) ) { EXCTHROW( -1, "Cycle print error"); } else
	
typedef enum _EStepPickPlace
{
	INIT = 0,
	MOVE_TO_PALETT,
	MOVE_TO_PANEL,
	TAKE_ON_PANEL,
	PICK_UP_PANEL,
	MOVE_TO_BOX,
	//ROTATE_90,
	MOVE_DOWN_TO_BOX,
	TAKE_OFF_PANEL,
	PICK_PLACE_FINIHED,
	MOVE_CONVOYEUR,
} EStepPickPlace;

typedef enum _EStepHomePickPlace
{
	UNBLOCK = 0,
	MOVE_Z_UP,
	MOVE_X_REAR,
	//ROTATE_0,
	HOME_PICK_PLACE_FINIHED,
} EStepHomePickPlace;

/*OPEN GRIPPER station 60 */
#undef __FUNC__
#define __FUNC__	"OPEN_GRIPPER"
SElExceptionPtr OPEN_GRIPPER
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_OPEN_GRIPPER, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));

	if (microCam  )
	{
	EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
	EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
	}
	
	
	if (timeout)
	{
		if(gs_open_gripper) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_open_gripper) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
	
Error:
	EXCRETHROW(pexception);
}
/*close GRIPPER station 60 */
#undef __FUNC__
#define __FUNC__	"CLOSE_GRIPPER"
SElExceptionPtr CLOSE_GRIPPER
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_CLOSE_GRIPPER, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));

	if (microCam  )
	{
	EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 0));
	EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 1));
	}
	
	
	if (timeout)
	{
		if(gs_close_gripper) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_close_gripper) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
Error:
	EXCRETHROW(pexception);
}

/*ACTUATOR X REAR station 60 */
#undef __FUNC__
#define __FUNC__	"ACTUATOR_X_REAR"
SElExceptionPtr ACTUATOR_X_REAR
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam,
					ApplicatorIsFront;
	int32_t         z_up;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_ACTUATOR_X_REAR, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &z_up));
	EXCCHECK( me->GetSensor(me, "SENSOR_APPLICATOR_BACKWARD", &ApplicatorIsFront));
   // printf(" x z_up %d\n",z_up);
	//printf(" x ApplicatorIsFront %d\n",ApplicatorIsFront);
	//printf(" x microCam %d \n",microCam);
	if (microCam)
	{
		if(z_up && !ApplicatorIsFront)
		{
			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60",0));
			EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 1));
			
			//EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60",0));
			//printf(" x rear excuted\n");
		}
	}
	
	
	if (timeout)
	{
		if(gs_x_rear) goto Error;
		/*if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_x_rear) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}*/
	}
Error:
	EXCRETHROW(pexception);
}

/*ACTUATOR X FRONT station 60 */
#undef __FUNC__
#define __FUNC__	"ACTUATOR_X_FRONT"
SElExceptionPtr ACTUATOR_X_FRONT
(
	struct _STester* me,
	int32_t			timeout
	
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t         z_up,
					ApplicatorIsFront;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_ACTUATOR_X_FRONT, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
    EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &z_up));
	EXCCHECK( me->GetSensor(me, "SENSOR_APPLICATOR_BACKWARD", &ApplicatorIsFront));
	if (microCam && z_up && !ApplicatorIsFront)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 1));
	}
	
	
	if (timeout)
	{
		if(gs_x_front) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_x_front) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
Error:
	EXCRETHROW(pexception);
}

/*ACTUATOR Z up station 60 */
#undef __FUNC__
#define __FUNC__	"ACTUATOR_Z_UP"
SElExceptionPtr ACTUATOR_Z_UP
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_ACTUATOR_Z_UP, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));

	if (microCam  )
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
	
	}
	
	
	if (timeout)
	{
		if(gs_z_up) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_z_up) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
Error:
	EXCRETHROW(pexception);
}

/*ACTUATOR Z down station 60 */
#undef __FUNC__
#define __FUNC__	"ACTUATOR_Z_DOWN"
SElExceptionPtr ACTUATOR_Z_DOWN
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t         product_present;
	
	EXCCHECK( me->eventAPI->GetEvent(me->eventAPI, EVNT_JIG_ITEM_ACTUATOR_Z_DOWN, &hevent));

	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
    EXCCHECK( me->GetSensor(me, "SENSOR_CONV_IN", &product_present ));
	if (microCam  && !product_present)
	{
	EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0));
	EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));
	}
	
	
	if (timeout)
	{
		if(gs_z_down) goto Error;
		if (WaitForSingleObject( hevent, timeout) == WAIT_TIMEOUT)
		{
			if(gs_z_down) goto Error;
			EXCTHROW( TESTER_ERROR_TIMEOUT, "Function Timeouted");
		}
	}
Error:
	EXCRETHROW(pexception);
}
 //-----------------
/* Pick & Place Panel station 60 */
#undef __FUNC__
#define __FUNC__	"PickPlacePanelStation60"
SElExceptionPtr PickPlacePanelStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			step = INIT;
	bool_t			is_product_took = FALSE;
	bool_t			cylinder_x_front,
					cylinder_x_rear,
					cylinder_z_up,
					cylinder_z_down,
					gripper_opened,
					gripper_closed,
					gripper_product_closed,
					hand_block_left,
					hand_unblock_left,
					hand_block_right,
					hand_unblock_right,
					cylinder_rotation_0,
					cylinder_rotation_90,
					sensor_cylinder_lockecd_station_60,
					productInConvoyer,
					product_present,
					sensor_cylinder_unlockecd_station_60;
	
	TIMEOUT_INIT();
	
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	EXCCHECK( me->GetSensor(me, "SENSOR_CONV_IN", &productInConvoyer));
	if(microCam == 1 && productInConvoyer==0)
	{
		
	step = MOVE_TO_PALETT;
	
	while( TRUE)
	{
		if (!is_product_took)
		{	
			//printf ("step = %d \r\n", step);
			Sleep(100);
			switch  (step)
			{

			
			case  MOVE_TO_PALETT : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &cylinder_x_rear));
				   
				   if(!cylinder_x_front) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 0));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 1));
				 			}  
				   else if (cylinder_x_front && !cylinder_x_rear) 
							{
							step ++;		
				 			} 
			break;
			
			case  MOVE_TO_PANEL : 
				   	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));	
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_z_down && cylinder_z_up && gripper_opened) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0));
							EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));
				 			}  
				   else if (cylinder_z_down && !cylinder_z_up && gripper_opened) 
							{
							step ++;		
				 			} 
			break;											 
																
			case  TAKE_ON_PANEL: 
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gripper_product_closed));
					
					if(gripper_opened && !gripper_closed && !gripper_product_closed)
					         {
							 EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 0));	 
						     EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 1));
					         }
					else if(!gripper_opened && gripper_product_closed)
					     {
						step ++;
					     }
			break;
			
			case  PICK_UP_PANEL : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_z_up && cylinder_z_down) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));	
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down) 
							{
							step ++;		
				 			} 
			break;
			
			
			case  MOVE_TO_BOX : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &cylinder_x_rear));
				   
				   if(!cylinder_x_rear && cylinder_x_front) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 0));
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 1));
				 			}  
				   else if (cylinder_x_rear && !cylinder_x_front) 
							{
							step ++;		
				 			} 
			break;

						
			case  MOVE_DOWN_TO_BOX : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60",   &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   if(!cylinder_z_down && cylinder_z_up) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));

				 			}  
				   else if (cylinder_z_down && !cylinder_z_up) 
							{
							step ++;		
				 			} 
			break;
			
			case  TAKE_OFF_PANEL: 
				
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60",			&gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", 		&gripper_closed));
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gripper_product_closed));
					
					if(gripper_product_closed)
					         {
						     EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
						     Sleep(200);
						     EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
					         }
					else if(gripper_opened && !gripper_closed && !gripper_product_closed )
					     {
						step ++;
					     }
			break;
				
			case  PICK_PLACE_FINIHED :
				
				EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
				EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60",   &cylinder_z_up));
	  			EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				/*if(!cylinder_z_down && cylinder_z_up) 
				Sleep(100);
				EXCCHECK( me->SetSensor(me, "CMD_CONVEYOR", 1));*/
				/*while(TRUE)
				{
				 Sleep(50);
				 EXCCHECK( me->GetSensor(me, "SENSOR_CONV_IN", &product_present));	
				 if(!product_present)
				 {
					EXCCHECK( me->SetSensor(me, "CMD_CONVEYOR", 0));
					break;
				 }
				} */
				 is_product_took = TRUE;
			break;
				
			}
		}

		else
			break;
		TIMEOUT_CHECK( timeout);
		Sleep(100);
	}
	

	}
	else 
	{
		if(!microCam)
		EXCTHROW( TESTER_ERROR_TIMEOUT, "Plateau pas en position!");
	    if(productInConvoyer)
		EXCTHROW( TESTER_ERROR_TIMEOUT, "Product already in Convoyer !!");	
	}
		
Error:
	EXCRETHROW(pexception);
}
/* Home Pick & Place Panel station 60 */
#undef __FUNC__
#define __FUNC__	"HomePickPlacePanelStation60"
SElExceptionPtr HomePickPlacePanelStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			step = INIT;
	bool_t			is_pick_place_homed = FALSE;
	bool_t			cylinder_x_front,
					cylinder_x_rear,
					cylinder_z_up,
					gripper_opened,
					gripper_closed,
					cylinder_z_down,
					hand_block_left,
					hand_unblock_left,
					hand_block_right,
					hand_unblock_right,
					cylinder_rotation_0,
					productPresentInConvoyer,
					gripper_product_closed,
					cylinder_rotation_90,
					productAtEndConvoyer;

	
	TIMEOUT_INIT();
	step = UNBLOCK;
	
	EXCCHECK( me->GetSensor(me, "SENSOR_CONV_IN", &productPresentInConvoyer));
	EXCCHECK( me->GetSensor(me, "SENSOR_CONV_OUT", &productAtEndConvoyer));		
	while(TRUE)
	{
		if (!is_pick_place_homed )
		{
			//printf ("step = %d \r\n", step);
			switch  (step)
			{
			case  UNBLOCK :
				
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gripper_product_closed));
					EXCCHECK( me->GetSensor(me, "SENSOR_CONV_IN", &productPresentInConvoyer));
					EXCCHECK( me->GetSensor(me, "SENSOR_CONV_OUT", &productAtEndConvoyer));
					//printf ("SENSOR_GRIPPER_OPENED_STATION_60 = %d \r\n", gripper_opened);
					//printf ("SENSOR_GRIPPER_CLOSED_STATION_60 = %d \r\n", gripper_closed);
					//printf ("SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60 = %d \r\n", gripper_product_closed);
					//printf ("SENSOR_CONV_IN = %d \r\n", productPresentInConvoyer);
					//printf ("SENSOR_CONV_OUT = %d \r\n", productAtEndConvoyer);
					if(!gripper_product_closed)
							{
				   			EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
				 			}  
							
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &cylinder_x_rear));
					//printf ("SENSOR_CYLINDER_Z_UP_STATION_60 = %d \r\n", cylinder_z_up);
					//printf ("SENSOR_CYLINDER_Z_DOWN_STATION_60 = %d \r\n", cylinder_z_down);
					//printf ("SENSOR_CYLINDER_X_FRONT_STATION_60 = %d \r\n", cylinder_x_front);
					//printf ("SENSOR_CYLINDER_X_REAR_STATION_60 = %d \r\n", cylinder_x_rear);
					if(/*cylinder_z_down && */!cylinder_z_up && gripper_opened && !gripper_product_closed)
						   {
						    EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
						    EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
					       }
				    if(cylinder_z_up)
						   {
						    EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 0));
					       }
				    if(!productPresentInConvoyer && gripper_product_closed)
						   {
							   if(cylinder_z_up && cylinder_x_rear)
							   {
								   EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0))
								   EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));
							   }
						   }
				   
				   if(!cylinder_z_up && cylinder_z_down && (gripper_product_closed || gripper_closed )) 
							{
							EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
						
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down && gripper_opened && !gripper_closed && !gripper_product_closed ) 
							{
							step ++;		
				 			} 
					
			break;
			
			case  MOVE_Z_UP :
			
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", 	 &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down)); 
				    if (cylinder_z_up && !cylinder_z_down) 
						{
							step ++;		
				 		} 
					else
						{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
			 			} 
			break;
					
			case  MOVE_X_REAR : //FRONT
				   		
	  				/*EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &cylinder_x_rear));   
				    if (cylinder_x_rear && !cylinder_x_front) 
						{
							step ++;		
				 		} 
							
					else
						{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 0));
				 		}  */
					step ++;

			break;

							
			case  HOME_PICK_PLACE_FINIHED :
				 /*EXCCHECK( me->SetSensor(me, "CMD_CONVEYOR", 1));
				while(TRUE)
				{
				 Sleep(100);
				 EXCCHECK( me->GetSensor(me, "SENSOR_CONV_IN", &product_present));	
				 if(!product_present)
				 {
					EXCCHECK( me->SetSensor(me, "CMD_CONVEYOR", 0));
					break;
				 }
				} */
				 
				 is_pick_place_homed = TRUE;
			break;
				
			}
		}

		else
		{
			//if (productPresentInConvoyer)
			//EXCTHROW( TESTER_ERROR_TIMEOUT, "Convoyer is FULL !!");		
			break;
		}
		TIMEOUT_CHECK( timeout);
		Sleep(500);
	}
	
Error:
	EXCRETHROW(pexception);
}

/*Ready Cycle print */
#undef __FUNC__
#define __FUNC__	"ReadyPrint"
SElExceptionPtr ReadyPrint
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	static HANDLE			hevent;
	DWORD 					result;

	bool_t				printer_in_position = FALSE, 
						printer_input1 = FALSE,
						printer_input2 = FALSE,
						printer_input3 = FALSE,
						printer_input4 = FALSE,
						printer_input5 = FALSE,
						microCam;
	
	
	//wait cycle_print_finished OFF
	TIMEOUT_INIT();
	do{
		me->GetSensor(me, "PRINTER_INPUT1", &printer_input1);//Print Job Ready
		me->GetSensor(me, "PRINTER_INPUT2", &printer_input2);//Printer Ready
		me->GetSensor(me, "PRINTER_INPUT3", &printer_input3);//End Of Ribbon
		me->GetSensor(me, "PRINTER_INPUT4", &printer_input4);//Error Message
		me->GetSensor(me, "MICRO_CAM", &microCam);
		
	if(!printer_input1 && 
	   printer_input2 && 
	   printer_input3 && 
	   printer_input4 
	  )
		{
		/* START = 0 */
		me->SetSensor(me, "PRINTER_START", 0);
		break;
		}
	
	else if (microCam)
	{
	/* Electovalve AIR PRINTER */
	EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE_ACTIVATE", 1));
	Sleep(100);
	EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE_DESACTIVATE", 0));
	
	/* Reset Errors */
	EXCCHECK( me->SetSensor(me, "PRINTER_RESET", 1));
	Sleep(100);
	}

	TIMEOUT_PRINTER_CHECK(timeout);
	
	}while(TRUE);

	
Error:
	me->SetSensor(me, "PRINTER_RESET", 0);
	EXCRETHROW(pexception);
}

/*Start Cycle print */
#undef __FUNC__
#define __FUNC__	"CyclePrint"
SElExceptionPtr CyclePrint
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	static HANDLE			hevent;
	DWORD 					result;

	bool_t				printer_input1 = TRUE,
						printer_input2 =FALSE,
						SENSOR_APPLICATOR_FORWARD,
						microCam,
						cylinder_z_up,
						gripper_product_closed;
	
	
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gripper_product_closed));
	EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60",          &cylinder_z_up));
	
	if(microCam && gripper_product_closed && cylinder_z_up)
	{
	   /* PRINTER_START */
		EXCCHECK( me->SetSensor(me, "PRINTER_START", 1));
		Sleep(200);
		EXCCHECK( me->SetSensor(me, "PRINTER_START", 0));                                                                                                                                                                               
		

		TIMEOUT_INIT();

			//wait cycle_print_finished OFF
  			do{                                                            
				me->GetSensor(me, "MICRO_CAM", &microCam);
				if(microCam == 0) 
				{
					/* Electovalve AIR PRINTER */
					EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE_ACTIVATE", 0));
					Sleep(100);
					EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE_DESACTIVATE", 1));		
				}
	  		
				me->GetSensor(me, "PRINTER_INPUT1", &printer_input1);
				me->GetSensor(me, "SENSOR_APPLICATOR_BACKWARD", &SENSOR_APPLICATOR_FORWARD);
	   			if (!printer_input1 && !SENSOR_APPLICATOR_FORWARD)
					break;
				
        		TIMEOUT_PRINTER_CHECK(timeout);
    		}while(TRUE);
		
			Sleep (200);
			

			//wait cycle_print_finished ON
  			do{
				me->GetSensor(me, "MICRO_CAM", &microCam);
				if(microCam == 0) 
				{
					/* Electovalve AIR PRINTER */
					EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE_ACTIVATE", 0));
					Sleep(200);
					EXCCHECK( me->SetSensor(me, "PRINTER_AIR_VALVE_DESACTIVATE", 1));		
				}

	  			me->GetSensor(me, "PRINTER_INPUT2", &printer_input2);
	   			if (printer_input2)
				{   
					//printf(" printer input\n");
					break;
				}
        		TIMEOUT_PRINTER_CHECK(timeout);
    		}while(TRUE);

		}
	
	else 
	{
		if(!microCam)
	      EXCTHROW( TESTER_ERROR_TIMEOUT, "plateau pas en position");
		if(!gripper_product_closed)
		  EXCTHROW( TESTER_ERROR_TIMEOUT, "produit n'est pas pris");	
	}
	
	
Error:
	me->SetSensor(me, "PRINTER_START", 0);
	EXCRETHROW(pexception);
}

//--------------------------------------------
 //-----------------
/* Pick & Print station 60 */
#undef __FUNC__
#define __FUNC__	"PickPrintPanelStation60"
SElExceptionPtr PickPrintPanelStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			step = INIT;
	bool_t			is_product_marked = FALSE;
	bool_t			cylinder_x_front,
					cylinder_x_rear,
					cylinder_z_up,
					cylinder_z_down,
					gripper_opened,
					gripper_closed,
					ApplicatorIsFront,
					gripper_product_closed,
					product_present;
	
	TIMEOUT_INIT();
	
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	EXCCHECK( me->GetSensor(me, "SENSOR_APPLICATOR_BACKWARD", &ApplicatorIsFront));
	if(microCam && !ApplicatorIsFront)
	{
		
	step = INIT;
	
	while( TRUE)
	{
		if (!is_product_marked)
		{	
			//printf ("step = %d \r\n", step);
			//Sleep(100);
			switch  (step)
			{
			case  INIT :
			        EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gripper_product_closed));
					  if(!gripper_opened)
							{
				   			EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
				 			}  
							
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &cylinder_x_rear));
					if(cylinder_z_down && !cylinder_z_up && gripper_opened && !gripper_product_closed)
						   {
						    EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
						    EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
					       }
					if(gripper_product_closed && cylinder_z_up && cylinder_x_rear)
						   {
						    EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0));
						    EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));
					       }
					if(gripper_product_closed && cylinder_z_up && !cylinder_x_rear)
						   {
						    EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 0));
					       }
				   
				   if(!cylinder_z_up && cylinder_z_down && gripper_product_closed) 
							{
							EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
						
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down && gripper_opened && !gripper_closed && !gripper_product_closed ) 
							{
							step ++;		
				 			} 
			break;
			
			case  MOVE_TO_PALETT : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &cylinder_x_rear));
				   
				   if(!cylinder_x_front && cylinder_x_rear) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 0));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 1));
				 			}  
				   else if (cylinder_x_front && !cylinder_x_rear) 
							{
							step ++;		
				 			} 
			break;
			
			case  MOVE_TO_PANEL : 
				   	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));	
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_z_down && cylinder_z_up && gripper_opened) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0));
				 			}  
				   else if (cylinder_z_down && !cylinder_z_up && gripper_opened) 
							{
							step ++;		
				 			} 
			break;											 
																
			case  TAKE_ON_PANEL: 
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gripper_product_closed));
					
					if(gripper_opened && !gripper_product_closed)
					         {
						     EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 1));
						     //Sleep(200);
						     EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 0));
							 EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
						     Sleep(300);
					         }
					else if(!gripper_opened && gripper_product_closed && !gripper_closed )
					     {
						step ++;
					     }
			break;
			
			case  PICK_UP_PANEL : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_z_up && cylinder_z_down) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
				   			
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down) 
							{
							step ++;		
				 			} 
			break;
			
			
			case  MOVE_TO_BOX : 
				   		
	  				/*EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &cylinder_x_rear));
				   
				   if(!cylinder_x_rear && cylinder_x_front) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 0));
				 			}  
				   else if (cylinder_x_rear && !cylinder_x_front) 
							{
							step ++;		
				 			} */
					step ++;
			break;

						
			case  MOVE_DOWN_TO_BOX : 
				   		
	  				/*EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   if(!cylinder_z_down && cylinder_z_up) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0));
				 			}  
				   else if (cylinder_z_down && !cylinder_z_up) 
							{
							step ++;		
				 			} */
					step ++;
			break;
			
			case  TAKE_OFF_PANEL: 
					/*EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));
					
					if(!gripper_opened && gripper_closed)
					         {
						     EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
						     //Sleep(200);
						     EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
						    // Sleep(1000);
					         }
					else if(gripper_opened && !gripper_closed)
					     {
						step ++;
					     }*/
					step ++;
			break;
				
			case  PICK_PLACE_FINIHED :
				
				 is_product_marked = TRUE;
			break;
				
			}
		}

		else
			break;
		TIMEOUT_CHECK( timeout);
		Sleep(100);
	}
	

	}
	else 
	{
		if(!microCam)
		EXCTHROW( TESTER_ERROR_TIMEOUT, "Table is not indexed !");
	    if(ApplicatorIsFront)
		EXCTHROW( TESTER_ERROR_TIMEOUT, "Applicator is active !!");	
	}
		
Error:
	EXCRETHROW(pexception);
}
//-----------------------------------------------
/* RESET Pick & Print station 60 */
#undef __FUNC__
#define __FUNC__	"ResetPickPrintPanelStation60"
SElExceptionPtr ResetPickPrintPanelStation60
(
	struct _STester* me,
	int32_t			timeout
)
{
	SElExceptionPtr	pexception = NULL;
	HANDLE			hevent = NULL;
	int32_t			microCam;
	int32_t			step = INIT;
	bool_t			is_product_marked = FALSE;
	bool_t			cylinder_x_front,
					cylinder_x_rear,
					cylinder_z_up,
					cylinder_z_down,
					gripper_opened,
					gripper_closed,
					ApplicatorIsFront,
					gripper_product_closed,
					product_present;
	
	TIMEOUT_INIT();
	
	EXCCHECK( me->GetSensor(me, "MICRO_CAM", &microCam));
	EXCCHECK( me->GetSensor(me, "SENSOR_APPLICATOR_BACKWARD", &ApplicatorIsFront));
	if(microCam && !ApplicatorIsFront)
	{
		
	step = INIT;
	
	while( TRUE)
	{
		if (!is_product_marked)
		{	
			
			switch  (step)
			{
			case  INIT :
			        EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gripper_product_closed));
					  /*
					  if((!gripper_opened && gripper_closed)||(!gripper_opened && gripper_product_closed))
							{
				   			EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
				 			}  */
							
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_z_up && cylinder_z_down ) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down && !gripper_opened && gripper_product_closed ) 
							{
							step ++;		
				 			} 
			break;
			
			case  MOVE_TO_PALETT : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &cylinder_x_rear));
					EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_x_front && cylinder_x_rear && cylinder_z_up) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 0));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 1));
				 			}  
				   else if (cylinder_x_front && !cylinder_x_rear) 
							{
							step ++;		
				 			} 
			break;
			
			case  MOVE_TO_PANEL : 
				   	EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gripper_product_closed));	
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_z_down && cylinder_z_up && gripper_product_closed) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0));
				 			}  
				   else if (cylinder_z_down && !cylinder_z_up) 
							{
							step ++;		
				 			} 
			break;											 
																
			case  TAKE_ON_PANEL: 
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));
					EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_PRODUCT_STATION_60", &gripper_product_closed));
					
					if(!gripper_opened /*&& gripper_product_closed*/)
					         {
						     EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
							 Sleep(100);
						     EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
							 Sleep(100);
						     EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
							 EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));

						    // Sleep(1000);
					         }
					else if(gripper_opened /*&& !gripper_product_closed*/)
					     {
						step ++;
					     }
			break;
			
			case  PICK_UP_PANEL : 
				   		
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_z_up && cylinder_z_down) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 0));
				 			}  
				   else if (cylinder_z_up && !cylinder_z_down) 
							{
							step ++;		
				 			} 
			break;
			
			
			case  MOVE_TO_BOX : 
				   		
	  				/*EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_FRONT_STATION_60", &cylinder_x_front));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_X_REAR_STATION_60", &cylinder_x_rear));
					EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   
				   if(!cylinder_x_rear && cylinder_x_front && cylinder_z_up && !cylinder_z_down ) 
							{
							EXCCHECK( me->SetSensor(me, "CYLINDER_X_REAR_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_X_FRONT_STATION_60", 0));
				 			}  
				   else if (cylinder_x_rear && !cylinder_x_front) 
							{
							step ++;		
				 			} */
					step ++;
			break;

						
			case  MOVE_DOWN_TO_BOX : 
				   		
	  				/*EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_UP_STATION_60", &cylinder_z_up));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_CYLINDER_Z_DOWN_STATION_60", &cylinder_z_down));
				   if(!cylinder_z_down && cylinder_z_up) 
							{
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_DOWN_STATION_60", 1));
				   			EXCCHECK( me->SetSensor(me, "CYLINDER_Z_UP_STATION_60", 0));
				 			}  
				   else if (cylinder_z_down && !cylinder_z_up) 
							{
							step ++;		
				 			} */
					step ++;
			break;
			
			case  TAKE_OFF_PANEL: 
					/*EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_OPENED_STATION_60", &gripper_opened));
	  				EXCCHECK( me->GetSensor(me, "SENSOR_GRIPPER_CLOSED_STATION_60", &gripper_closed));
					
					if(!gripper_opened && gripper_closed)
					         {
						     EXCCHECK( me->SetSensor(me, "CLOSE_GRIPPER_STATION_60", 0));
						     //Sleep(200);
						     EXCCHECK( me->SetSensor(me, "OPEN_GRIPPER_STATION_60", 1));
						    // Sleep(1000);
					         }
					else if(gripper_opened && !gripper_closed)
					     {
						step ++;
					     }*/
					step ++;
			break;
				
			case  PICK_PLACE_FINIHED :
				
				 is_product_marked = TRUE;
			break;
				
			}
		}

		else
			break;
		TIMEOUT_CHECK( timeout+1000);
		Sleep(100);
	}
	

	}
	else 
	{
		if(!microCam)
		EXCTHROW( TESTER_ERROR_TIMEOUT, "Table is not indexed !");
	    if(ApplicatorIsFront)
		EXCTHROW( TESTER_ERROR_TIMEOUT, "Applicator is active !!");	
	}
		
Error:
	EXCRETHROW(pexception);
}