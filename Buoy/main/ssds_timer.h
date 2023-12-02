
#ifndef __SSDS_TIMER
#define __SSDS_TIMER

#define TIMER_INTERVAL  5 // Pause in seconds

// Public-facing method declarations

/*
 * Initializes the timer thread.  After this is called, run with resume_thread_timer().
 * Once running, this thread will collect and store one datapoint every
 * TIMER_INTERVAL seconds.
 */
void init_thread_timer();

/*
 * Attempts to determine the current hour of the day, logging it to the
 * monitor and noting if it's night (10 PM to 6 AM) or day (6 AM to 10 PM)
 */
void check_if_day();

/*
 * Pauses the timer thread. No datapoints will be collected or stored until resumed.
 * If already paused, the timer remains paused.
 */
void pause_thread_timer();

/*
 * Resumes the timer thread. If already running, the timer remains running.
 */
void resume_thread_timer();

#endif
