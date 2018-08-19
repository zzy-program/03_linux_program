#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <sys/vfs.h>

int main(int argc, char **argv)
{
	struct statfs sfs;
	int ret = 0;
	long long total = 0, free = 0, used = 0;

	if(argc != 2) {
		printf("parameter error\n");
		return -1;
	}

	printf("argv[1]=%s\n", argv[1]);

	// todo, check return value
	ret = statfs(argv[1], &sfs);
	printf("f_blocks=%d, f_bavail=%d\n", sfs.f_blocks, sfs.f_bavail);

	total = sfs.f_bsize * sfs.f_blocks;
	free = sfs.f_bsize * sfs.f_bavail;
	used = total - free;
	printf("total: %f, free: %f, used: %f\n", total/1024./1024., free/1024./1024., used/1024./1024.);

	return 0;

}
