/**
 * @file	scheduler.c
 * @author  Eriks Zaharans and Massimiiliano Raciti
 * @date    1 Jul 2013
 *
 * @section DESCRIPTION
 *
 * Cyclic executive scheduler library.
 */

/* -- Includes -- */
/* system libraries */
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
/* project libraries */
#include "scheduler.h"
#include "task.h"
#include "timelib.h"

/* -- Defines -- */
#define N 10000

/* -- Functions -- */

/**
 * Initialize cyclic executive scheduler
 * @param minor Minor cycle in miliseconds (ms)
 * @return Pointer to scheduler structure
 */
scheduler_t *scheduler_init(void)
{
	// Allocate memory for Scheduler structure
	scheduler_t *ces = (scheduler_t *) malloc(sizeof(scheduler_t));

	return ces;
}

/**
 * Deinitialize cyclic executive scheduler
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_destroy(scheduler_t *ces)
{
	// Free memory
	free(ces);
}

/**
 * Start scheduler
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_start(scheduler_t *ces)
{
	// Set timers
	timelib_timer_set(&ces->tv_started);
	timelib_timer_set(&ces->tv_cycle);
}

/**
 * Wait (sleep) till end of minor cycle
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_wait_for_timer(scheduler_t *ces)
{
	int sleep_time; // Sleep time in microseconds

	// Calculate time till end of the minor cycle
	sleep_time = (ces->minor * 1000) - (int)(timelib_timer_get(ces->tv_cycle) * 1000);

	// Add minor cycle period to timer
	timelib_timer_add_ms(&ces->tv_cycle, ces->minor);

	// Check for overrun and execute sleep only if there is no
	if(sleep_time > 0)
	{
		// Go to sleep (multipy with 1000 to get miliseconds)
		usleep(sleep_time);
	}
}

/**
 * Execute task
 * @param ces Pointer to scheduler structure
 * @param task_id Task ID
 * @return Void
 */
void scheduler_exec_task(scheduler_t *ces, int task_id)
{
	switch(task_id)
	{
	// Mission
	case s_TASK_MISSION_ID :
		task_mission();
		break;
	// Navigate
	case s_TASK_NAVIGATE_ID :
		task_navigate();
		break;
	// Control
	case s_TASK_CONTROL_ID :
		task_control();
		break;
	// Refine
	case s_TASK_REFINE_ID :
		task_refine();
		break;
	// Report
	case s_TASK_REPORT_ID :
		task_report();
		break;
	// Communicate
	case s_TASK_COMMUNICATE_ID :
		task_communicate();
		break;
	// Collision detection
	case s_TASK_AVOID_ID :
		task_avoid();
		break;
	// Other
	default :
		// Do nothing
		break;
	}
}

double get_exec_time(scheduler_t *ces, int task_id) {
	clock_t start, end;
	struct timeval t;
	timelib_timer_set(&t);
	scheduler_exec_task(ces, task_id);
	double time = timelib_timer_get(t);
	// double time = (double)(end - start) / CLOCKS_PER_SEC;
	// printf("Ran task %d in %f", task_id, time);
	return time;
}

void save_time(double times[8][N], int count[8], scheduler_t* ces, int task_id) {
	times[task_id][count[task_id]++] = get_exec_time(ces, task_id);	
}


/**
 * Run scheduler
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_run(scheduler_t *ces)
{
	/* --- Local variables (define variables here) --- */

	/* --- Set minor cycle period --- */
	ces->minor = 1000;
	/* --- Write your code here --- */

	static int count[8];
	// memset(count, 0, sizeof(int) * 8);
	static double times[8][N];
	// memset(times, 0, sizeof(int) * 8 * N);

	scheduler_exec_task(ces, s_TASK_AVOID_ID);
	for (int i = 0; i < 20; i++) {
		scheduler_start(ces);
		printf("Starting iteration %d", i);

		save_time(times, count, ces, s_TASK_AVOID_ID);
		save_time(times, count, ces, s_TASK_MISSION_ID);

		save_time(times, count, ces, s_TASK_NAVIGATE_ID);
		save_time(times, count, ces, s_TASK_CONTROL_ID);
		save_time(times, count, ces, s_TASK_AVOID_ID);

		for (int x = 0; x < 10; x++) {

		}

		save_time(times, count, ces, s_TASK_REFINE_ID);
		save_time(times, count, ces, s_TASK_AVOID_ID);
		save_time(times, count, ces, s_TASK_REPORT_ID);
		save_time(times, count, ces, s_TASK_AVOID_ID);

		save_time(times, count, ces, s_TASK_NAVIGATE_ID);
		save_time(times, count, ces, s_TASK_CONTROL_ID);
		save_time(times, count, ces, s_TASK_AVOID_ID);

		save_time(times, count, ces, s_TASK_COMMUNICATE_ID);
		save_time(times, count, ces, s_TASK_AVOID_ID);
		

		// times[s_TASK_MISSION_ID][i] = get_exec_time(ces, s_TASK_MISSION_ID);
		// times[s_TASK_NAVIGATE_ID][i] = get_exec_time(ces, s_TASK_NAVIGATE_ID);
		// times[s_TASK_CONTROL_ID][i] = get_exec_time(ces, s_TASK_CONTROL_ID);		
		// times[s_TASK_AVOID_ID][i] = get_exec_time(ces, s_TASK_AVOID_ID);

		// times[s_TASK_NAVIGATE_ID][i] = get_exec_time(ces, s_TASK_NAVIGATE_ID);
		// times[s_TASK_CONTROL_ID][i] = get_exec_time(ces, s_TASK_CONTROL_ID);
		// ran_twice[s_TASK_AVOID_ID][i] = get_exec_time(ces, s_TASK_AVOID_ID);
		
		// times[s_TASK_REFINE_ID][i] = get_exec_time(ces, s_TASK_REFINE_ID);
		// times[s_TASK_REPORT_ID][i] = get_exec_time(ces, s_TASK_REPORT_ID);
		// times[s_TASK_COMMUNICATE_ID][i] = get_exec_time(ces, s_TASK_COMMUNICATE_ID);
		
		double total = 0; 
		for (int j = 0; j < 8; j++) {
			total += times[j][i];
		}

		printf("Ended tasks for iteration %d\n", i);
		scheduler_wait_for_timer(ces);
	}
	FILE *fp = fopen("times.csv", "a");
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < N; j++) {
			fprintf(fp, "%f,", times[i][j]);
		}
		fprintf(fp, "\n");
	}
	fclose(fp);

}

