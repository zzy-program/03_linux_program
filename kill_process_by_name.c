#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

int kill_process_by_name(char* name)
{

	const char* directory = "/proc";
	size_t      taskNameSize = 1024;
	char*       taskName = calloc(1, taskNameSize);

	DIR* dir = opendir(directory);
	int pid = -1;
	char cmd[128];

	if (dir)
	{
		struct dirent* de = 0;

		while ((de = readdir(dir)) != 0)
		{
			if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
				continue;

			int res = sscanf(de->d_name, "%d", &pid);

			if (res == 1)
			{
				// we have a valid pid

				// open the cmdline file to determine what's the name of the process running
				char cmdline_file[1024] = {0};
				sprintf(cmdline_file, "%s/%d/cmdline", directory, pid);

				FILE* cmdline = fopen(cmdline_file, "r");

				if (getline(&taskName, &taskNameSize, cmdline) > 0)
				{
					// is it the process we care about?
					if (strstr(taskName, name) != 0)
					{
						break;
					}
				}

				fclose(cmdline);
			}
		}

		closedir(dir);
	}

	// just let the OS free this process' memory!
	//free(taskName);
	if(pid > 0) {
		sprintf(cmd, "kill -9 %d", pid);
		system(cmd);
		printf("process %s:%d killed\n", name, pid);
	} else {
		printf("process %s not found\n", name);
	}

	return 0;
}

int main() {

	kill_process_by_name("com.htc.htcdialer");
	return 0;
}
