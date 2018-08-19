#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
// external/toybox$ vim toys/posix/date.c
// adb shell strace date -s SET_FORMAT 121011022015.30
int main(void)
{
	// for mtk: 2015/12/10 3:02:00
	// for qct: 2015/12/10 11:02:00
	struct timeval tv = {1449716550, 0};

	if (settimeofday(&tv, NULL) < 0) 
		printf("cannot set date");
	return 0;
}
