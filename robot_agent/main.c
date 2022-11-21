/**
 * @file	main.c
 * @author	Eriks Zaharans
 * @date	1 Jul 2013
 *
 * @section DESCRIPTION
 *
 * Application main file.
 */


/* -- Includes -- */
/* system libraries */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
/* project libraries */
#include "src/config.h"
#include "src/def.h"
#include "src/scheduler.h"
#include "src/task.h"
#include "src/timelib.h"


#include "src/robot.h"
#include "src/doublylinkedlist.h"

/* -- Functions -- */

 /**
 * @brief Main application
 */
 
 static scheduler_t *ces;
 int sig_handler(int signo);
 
 
int main()
{
	// Say hello!
	printf("Hello world!\n");
	
	// Register the signal handler
	
	if (signal(SIGINT, (void(*)(int))sig_handler) == SIG_ERR)
    	{
        fprintf(stderr, "Warning: won't catch SIGINT\n");
    	}

	// Initialization
	// Load Configuration
	config_load();
	// Init tasks
	task_init(1);
	// Init scheduler (Set minor and mayor cycle)
	ces = scheduler_init();

	// Run scheduler
	scheduler_run(ces);

	// Before end application deinitialize and free memory
	// Deinit tasks
	task_destroy();
	// Deinit scheduler
	
	scheduler_destroy(ces);

	// Say goodbye!
	printf("Goodbye!\n");

	// End application
	return 0;
}


/* -- Functions -- */
/*
 * Function:	    sig_handler
 * Brief:	        The signal handler function to use when aborting our program with Ctrl-C or  Ctrl-Z.
 *                  Its main purpose is to properly close the connection with the robot and avoid gettin it stuck
 *                  even if the program was interrupted (if a SIGINT signal was sent via Ctrl-C or a SIGTSTOP as sent via Ctrl-Z).
 * @param signo:	The signal to catch
 * Returns:	        Shall return 0 for the moment
*/
int sig_handler(int signo)
{
    if (signo == SIGINT || signo ==SIGSTOP)
    {
        fprintf(stderr, "SIGINT received!\n");

        // Deinit tasks
        task_destroy();
        // Destroy scheduler
        scheduler_destroy(ces);
        // And say goodbye! 
        printf("Goodbye from signal handler!\n");
        // Exit our program
        exit(SIGINT);
    }
    exit(signo);
    }
