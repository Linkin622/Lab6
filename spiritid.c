#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <syslog.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <errno.h>



void usr_handler(int);   
void term_handler(int);   


pid_t prev_child_pid;  
char* mole_path;   


int main(int argc, char *argv[]) {

	pid_t pid;
	pid_t sid;

	struct rlimit rl;

	int i;
	int fd0, fd1, fd2;

	char buffer[1024];

	mole_path = getcwd(buffer, 1024);
	strcat(mole_path, "/mole");  


	char *file_path;

	file_path = (char*)malloc(sizeof(getenv("HOME")));   
	strcpy(file_path, getenv("HOME"));

	file_path = (char*)realloc(file_path, sizeof(getenv("HOME")) + 10);  
	strcat(file_path, "/lab6.log");   


	FILE *logfile = fopen(file_path, "w+");	
	if (logfile == NULL)
	{
		printf("Cannot find file: lab6.log\n");
		exit(1);
	}

	fclose(logfile);

	free(file_path);



	

	pid = fork();

	if (pid > 0) {
		exit(0);  
	}


	umask(0);  


	sid = setsid();  
	if (sid < 0) {
		exit(1);
	}


	if ((chdir("/")) < 0) { 
		exit(1);
	}


	
	getrlimit(RLIMIT_NOFILE, &rl);

	if (rl.rlim_max == RLIM_INFINITY) {
		rl.rlim_max = 1024;
	}
	for (i = 0; i < rl.rlim_max; i++) {
		close(i);
	}


	
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);



	
	signal(SIGUSR1, usr_handler);
	signal(SIGTERM, term_handler);
	signal(SIGUSR2, usr_handler);


	prev_child_pid = 0; 


	while (1) {

		pause();
	}


	return 0;
}



void usr_handler(int signum) {

	signal(signum, usr_handler);


	if (prev_child_pid != 0) {

		kill(prev_child_pid, SIGKILL);  
	}


	pid_t m_pid;  

	m_pid = fork();

	if (m_pid == 0) {

		char* mole_id;

		srand(time(NULL));
		int rnd = (rand() % 2);  

		if (rnd == 1) {
			mole_id = "mole1";
		}
		else {
			mole_id = "mole2";
		}

		char *exec_args[3];  

		exec_args[0] = mole_path;	
		exec_args[1] = mole_id;		
		exec_args[2] = NULL;
		execv(exec_args[0], exec_args);

	}
	else {
		prev_child_pid = m_pid;
	}

}



void term_handler(int signum) {

	if (prev_child_pid != 0) {
		kill(prev_child_pid, SIGKILL);  
	}

	kill(getpid(), SIGKILL); 

}
