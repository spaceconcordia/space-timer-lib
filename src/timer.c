#include "timer.h"

timer_t timer_get()
{
	timer_t timer;
    struct sigevent sev;

    /*
    struct sigaction sa;
    // register to signal handler
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = signal_handler;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIG, &sa, NULL) == -1)
        errExit("Failed to register to signal handler");
    */

	// create timer
    sev.sigev_notify = SIGEV_NONE;

//	 if (timer_create(CLOCK_MONOTONIC, &sev, &timer) == -1) {
	 if (timer_create(CLOCK_REALTIME, &sev, &timer) == -1) {
		perror("\nCould not create timer\n");
		exit(EXIT_FAILURE);
	 }

	 return timer;

}

void timer_start(timer_t * timer, time_t timeout_s, time_t timeout_ms)
{
	printf("\ntimer %lx starting with timeout %lx s\n", (long)*timer, (long)timeout_s);
    struct itimerspec it_val;
    it_val.it_value.tv_sec = timeout_s;
    it_val.it_value.tv_nsec = timeout_ms * 1000000; // timeout in miliseconds

    // timer expires once
    it_val.it_interval.tv_sec = 0;
    it_val.it_interval.tv_nsec = 0;

	 if (timer_settime(*timer, 0, &it_val, NULL) == -1) {
			perror("\nCould not start timer\n");
			exit(EXIT_FAILURE);
	 }
}


bool timer_complete(timer_t * timer)
{
	struct itimerspec curr_val;
	if(timer_gettime(*timer, &curr_val) == -1) {
		perror("\nCould not get time\n");
		exit(EXIT_FAILURE);
	}

	if(curr_val.it_value.tv_nsec == 0 && curr_val.it_value.tv_sec == 0) {
		printf("\nTimer %lx Complete!\n", (long)*timer);
		return true;
	}
	else
		return false;
}
