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
#include <math.h>
/* project libraries */
#include "scheduler.h"
#include "task.h"
#include "timelib.h"

/* -- Defines -- */
#define N 100000

/* -- Functions -- */

/**
 * Initialize cyclic executive scheduler
 * @param minor Minor cycle in miliseconds (ms)
 * @return Pointer to scheduler structure
 */
scheduler_t *scheduler_init(void) {
    // Allocate memory for Scheduler structure
    scheduler_t *ces = (scheduler_t *) malloc(sizeof(scheduler_t));

    return ces;
}

/**
 * Deinitialize cyclic executive scheduler
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_destroy(scheduler_t *ces) {
    // Free memory
    free(ces);
}

/**
 * Start scheduler
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_start(scheduler_t *ces) {
    // Set timers
    timelib_timer_set(&ces->tv_started);
    timelib_timer_set(&ces->tv_cycle);
}

/**
 * Wait (sleep) till end of minor cycle
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_wait_for_timer(scheduler_t *ces) {
    int sleep_time; // Sleep time in microseconds

    // Calculate time till end of the minor cycle
    sleep_time = (ces->minor * 1000) - (int) (timelib_timer_get(ces->tv_cycle) * 1000);

    // Add minor cycle period to timer
    timelib_timer_add_ms(&ces->tv_cycle, ces->minor);

    // Check for overrun and execute sleep only if there is no
    if (sleep_time > 0) {
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
void scheduler_exec_task(scheduler_t *ces, int task_id) {
    switch (task_id) {
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

/* Executes the task with task_id once and returns its execution time in ms */
double get_exec_time(scheduler_t *ces, int task_id) {
    struct timeval t;
    timelib_timer_set(&t);
    scheduler_exec_task(ces, task_id);
    return timelib_timer_get(t);
}

/* Executes the given task and writes the time relative to start into times afterwards. */
void exec_task(double times[8][N], int count[8], scheduler_t *ces, int task_id, struct timeval start) {
    scheduler_exec_task(ces, task_id);
    double time = timelib_timer_get(start);
    times[task_id][count[task_id]++] = time;
}

/* Write the saved times to file */
void save_times(double times[8][N], int counts[8], int periods[8], double victims[24][3]) {
    FILE *fp = fopen("exec.csv", "w");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < N; j++) {
            fprintf(fp, "%f,", times[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

    FILE *meta = fopen("period.csv", "w");
    for (int i = 0; i < 8; ++i) {
        fprintf(meta, "%d\n", periods[i]);
    }
    fclose(meta);

    FILE *acc = fopen("acc.csv", "w");
    for (int i = 0; i < 24; ++i) {
        fprintf(acc, "%d,%d,%f\n", (int) victims[i][0], (int) victims[i][1], victims[i][2]);
    }
    fclose(acc);
}

typedef struct MY_VICTIM_STRUCT {
    victim_t victim;
    int nr;
} my_victim_t;

/* Returns the true victim position of the found victim with id (based on the pdf) */
my_victim_t get_victim(char const *id) {
    victim_t v;
    int nr;

    if (!strcmp(id, "020058F5BD")) {
        v.x = 340;
        v.y = 340;
        nr = 1;
    }
    else if (!strcmp(id, "020053A537")) {
        v.x = 975;
        v.y = 1115;
        nr = 2;
    }
    else if (!strcmp(id, "020053E0BA")) {
        v.x = 1845;
        v.y = 925;
        nr = 3;
    }
    else if (!strcmp(id, "01004B835E")) {
        v.x = 2670;
        v.y = 335;
        nr = 4;
    }
    else if (!strcmp(id, "020053C80E")) {
        v.x = 3395;
        v.y = 870;
        nr = 5;
    }
    else if (!strcmp(id, "020058100D")) {
        v.x = 4645;
        v.y = 910;
        nr = 6;
    }
    else if (!strcmp(id, "0200580B96")) {
        v.x = 4800;
        v.y = 250;
        nr = 7;
    }
    else if (!strcmp(id, "02005345B6")) {
        v.x = 5395;
        v.y = 1060;
        nr = 8;
    }
    else if (!strcmp(id, "020058F121")) {
        v.x = 5830;
        v.y = 1895;
        nr = 9;
    }
    else if (!strcmp(id, "0200581B9E")) {
        v.x = 5110;
        v.y = 2390;
        nr = 10;
    }
    else if (!strcmp(id, "020058066F")) {
        v.x = 5770;
        v.y = 3790;
        nr = 11;
    }
    else if (!strcmp(id, "020058212D")) {
        v.x = 4500;
        v.y = 3190;
        nr = 12;
    }
    else if (!strcmp(id, "020058022D")) {
        v.x = 3415;
        v.y = 3200;
        nr = 13;
    }
    else if (!strcmp(id, "0200581542")) {
        v.x = 4150;
        v.y = 1810;
        nr = 14;
    }
    else if (!strcmp(id, "0200534E5C")) {
        v.x = 3720;
        v.y = 3710;
        nr = 15;
    }
    else if (!strcmp(id, "020053AB2C")) {
        v.x = 2580;
        v.y = 3770;
        nr = 16;
    }
    else if (!strcmp(id, "01004A11E8")) {
        v.x = 2970;
        v.y = 2805;
        nr = 17;
    }
    else if (!strcmp(id, "020053E282")) {
        v.x = 3030;
        v.y = 2070;
        nr = 18;
    }
    else if (!strcmp(id, "0200553505")) {
        v.x = 3120;
        v.y = 1965;
        nr = 19;
    }
    else if (!strcmp(id, "01004751A2")) {
        v.x = 2880;
        v.y = 1840;
        nr = 20;
    }
    else if (!strcmp(id, "02005097C0")) {
        v.x = 1890;
        v.y = 2580;
        nr = 21;
    }
    else if (!strcmp(id, "020053BF78")) {
        v.x = 985;
        v.y = 3020;
        nr = 22;
    }
    else if (!strcmp(id, "020056D0EF")) {
        v.x = 730;
        v.y = 3175;
        nr = 23;
    }
    else if (!strcmp(id, "01004BDF7B")) {
        v.x = 320;
        v.y = 1800;
        nr = 24;
    }

        // Id doesnt exist
    else {
        fprintf(stderr, "There is no victim with id %s", id);
        exit(1);
    }

    my_victim_t truth;
    truth.victim = v;
    truth.nr = nr;
    return truth;
}

/**
 * Run scheduler
 * @param ces Pointer to scheduler structure
 * @return Void
 */
void scheduler_run(scheduler_t *ces) {
    /* --- Set minor cycle period --- */
    ces->minor = 125;

    /* --- Write your code here --- */

    /* Avoids first execution takes far longer than the average */
    scheduler_exec_task(ces, s_TASK_AVOID_ID);

    /* Define major cycle and the periods for the tasks */
    int major_cycle = 3000;

    static int periods[8];
    periods[0] = 0;
    periods[s_TASK_MISSION_ID] = 250;
    periods[s_TASK_NAVIGATE_ID] = 375;
    periods[s_TASK_CONTROL_ID] = 375;
    periods[s_TASK_REFINE_ID] = 250;
    periods[s_TASK_REPORT_ID] = 250;
    periods[s_TASK_COMMUNICATE_ID] = 1000;
    periods[s_TASK_AVOID_ID] = 125;

    /* Matrix of the tasks execution time relative to start */
    static double times[8][N];
    static int counts[8];

    int robot = 7;
   
    struct timeval tv_now;
    gettimeofday(&tv_now, NULL);
    int sleep_time = 1000000 - tv_now.tv_usec;
    usleep(sleep_time);
    usleep(125*1000*(robot-1));

    /* Start a timer */
    struct timeval start;
    timelib_timer_set(&start);

    /* Run M major cycles */
    unsigned M = 1000;
    scheduler_start(ces);
    for (unsigned i = 0; i < M * major_cycle; i += ces->minor) {
        //printf("Starting period %d at %f\n", i, timelib_timer_get(start));

        if (i % periods[s_TASK_COMMUNICATE_ID] == 0) {
            exec_task(times, counts, ces, s_TASK_COMMUNICATE_ID, start);
        }

        if (i % periods[s_TASK_MISSION_ID] == 0) {
            exec_task(times, counts, ces, s_TASK_MISSION_ID, start);
        }

        /* Always run navigate and control together */
        if (i % periods[s_TASK_NAVIGATE_ID] == 0) {
            exec_task(times, counts, ces, s_TASK_NAVIGATE_ID, start);
            exec_task(times, counts, ces, s_TASK_CONTROL_ID, start);
        }

        if (i % periods[s_TASK_AVOID_ID] == 0) {
            exec_task(times, counts, ces, s_TASK_AVOID_ID, start);
        }

        /* Always run refine and report together */
        if (i % periods[s_TASK_REFINE_ID] == 0) {
            exec_task(times, counts, ces, s_TASK_REFINE_ID, start);
            exec_task(times, counts, ces, s_TASK_REPORT_ID, start);
        }

        //double cur = timelib_timer_get(start);
        //printf("Ended tasks for this minor cycle at %fms with %fms to spare \n", cur,
        //       (double) (i + ces->minor + wait_ms) - cur);
        /* Wait for minor cycle to finish */
        printf("Sent %d bytes of pheromones\n", bytes_send[s_DATA_STRUCT_TYPE_PHEROMONE]);
        scheduler_wait_for_timer(ces);
    }

    gettimeofday(&tv_now, NULL);
    double seconds_ran = (double)(tv_now.tv_sec - start.tv_sec);
    printf("Ran for %fs", seconds_ran);

    static double victims[24][3];
    /* Measure accuracy of found victims */
    for (int i = 0; i < g_task_mission_data.victim_count; ++i) {
        /* Prediction */
        victim_t v1 = g_task_mission_data.victims[i];
        /* Truth */
        my_victim_t truth = get_victim(v1.id);
        victim_t v2 = truth.victim;
        int nr = truth.nr;

        /* Euclidean distance */
        double distance = sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));


        victims[nr - 1][0] = v2.x;
        victims[nr - 1][1] = v2.y;
        victims[nr - 1][2] = distance;

        printf("Predicted victim %s at (%d, %d). True position is (%d, %d). Distance = %f", v1.id, v1.x, v1.y, v2.x,
               v2.y, distance);
    }

    /* Write the times, periods, and victim predictions to file */
    save_times(times, counts, periods, victims);

    /* Save how many packets we sent and how many bytes that was */
    FILE *fp = fopen("packets.csv", "w");
    for (int i = 0; i < 5; ++i) {
      fprintf(fp, "%d,%d\n", sent[i], bytes_send[i]);
    }
    fprintf(fp, "%d,%f", aheads, seconds_ran);
    fclose(fp);

    fp = fopen("packets_individual.csv", "w");
    for (size_t i = 0; i < CALL_MAX; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            fprintf(fp, "%d, %d, %d, %d \n", found_i[j][i], sent_i[j][i], bytes_sent_i[j][i], aheads_i[i]);
        }
    }
    fclose(fp);
}

/* int sent[5]; */
/* int bytes_send[5]; */
/* int aheads; */

