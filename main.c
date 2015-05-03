#include <sys/types.h> /* pid_t */
#include <sys/wait.h>  /* waitpid */
#include <stdio.h>     /* printf, perror */
#include <stdlib.h>    /* exit */
#include <unistd.h>    /* _exit, fork */
#include <syslog.h>    /* Syslog functions */ 
#include <signal.h>    /* Signal handler functions */


void signal_handler(int signal) {
  syslog(LOG_INFO, "Received signal: %d", signal);
}


int main(void)
{
  pid_t pid = fork();
  if (pid == -1) {
    // For has failed and we need to exit
    fprintf(stderr, "ERROR: Unable to fork to child process, exiting.\n");
    exit(20);
  }

  // If we are the parent just leave
  if (pid > 0)
    exit(0); 

  // Reaching this point means we are the child and that we will soon
  // be disconnected from the parent that will terminate.

  // Open a syslog facility to log our progress
  openlog("ichidaemon", LOG_PERROR|LOG_PID, 0);
  syslog(LOG_INFO, "Fork successful");
  
  // Attach signal handler to sighup
  signal(SIGHUP, signal_handler);
  signal(SIGCHLD, signal_handler);
  signal(SIGTERM, signal_handler);
  signal(SIGABRT, signal_handler);

  // Sleep for a while then exit
  sleep(20);
  syslog(LOG_INFO, "Done, will now exit");
  closelog();

  return (0);
}
 
