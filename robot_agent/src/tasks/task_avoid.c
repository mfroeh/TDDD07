/**
 * @file	task_avoid.c
 * @author  Eriks Zaharans
 * @date    31 Oct 2013
 *
 * @section DESCRIPTION
 *
 * Collision detection (avoid) Task.
 */

/* -- Includes -- */
/* system libraries */

/* project libraries */
#include "task.h"
  
 /**
 * Check the bump sensors for collision
 */
void task_avoid(void)
{
	if(g_task_avoid.enabled == s_TRUE)
	{
		// Update Bump sensor
#ifndef s_CONFIG_TEST_ENABLE
		openinterface_sensors_update(g_ois, s_OI_SENSOR_PACKET_100, s_OI_SENSOR_PACKET_100_SIZE);
#endif


		/*// Check collision
        if(g_ois->oiss->wheeldrop_bump > 0)
		{
			debug_printf("Old Bumper %d \n", g_ois->oiss->wheeldrop_bump);
		}*/


		// Check collision
        if(g_ois->oiss->light_bumper > 0)
		{
			debug_printf("Light Bumper %d \n", g_ois->oiss->light_bumper);
			
			
			
			    // Right Bump
		    if((g_ois->oiss->light_bumper >=8) && (g_ois->oiss->light_bumper  <=32))
		    {
			    openinterface_drive(g_ois, g_config.robot_speed, 0x0001);
			    return;
		    }
		    // Left Bump
		    else if(g_ois->oiss->light_bumper  <=4)
		    {
			    openinterface_drive(g_ois, g_config.robot_speed, 0xFFFF );
			    return;
		    }
		     else if(g_ois->oiss->light_bumper  >64)
		     {
		        printf("ROBOT INTERFACE ERROR!! Exiting! \n");
		       // openinterface_drive(g_ois, 0, 0x0001 );
		        openinterface_stop(g_ois);
		        //openinterface_start(g_ois);
			    exit(1);
			    }
			
			
			
		}
		
	

		// Choose direction of rotation depending on the side it hit
		// Right Bump
		/*if(g_ois->oiss->wheeldrop_bump == 1)
		{
			openinterface_drive(g_ois, g_config.robot_speed, 0x0001);
			return;
		}
		// Left Bump
		else if(g_ois->oiss->wheeldrop_bump == 2)
		{
			openinterface_drive(g_ois, g_config.robot_speed, 0xFFFF);
			return;
		}/*
		// Center Bump
		else if(g_ois->oiss->wheeldrop_bump == 3)
		{
			if((rand() % 10) > 5)
				openinterface_drive(g_ois, g_config.robot_speed, 0xFFFF);
			else
				openinterface_drive(g_ois, g_config.robot_speed, 0x0001);

			return;
		}*/
	}
}
