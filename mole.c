#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {

	char *file_path;

	file_path = (char*)malloc(sizeof(getenv("HOME")));   
	strcpy(file_path, getenv("HOME"));

	file_path = (char*)realloc(file_path, sizeof(getenv("HOME")) + 10);  
	strcat(file_path, "/lab6.log");  


	FILE *logfile = fopen(file_path, "a");	
	if (logfile == NULL)
	{
		exit(1);
	}

	fprintf(logfile, "Pop %s\n", argv[1]);

	fclose(logfile);

	free(file_path);

	while (1) {
		pause();
	}
}
