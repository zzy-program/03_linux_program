#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <linux/ashmem.h>

// https://gist.github.com/sanmarcos/991042
// http://man7.org/linux/man-pages/man2/mmap.2.html
// http://www.111cn.net/sj/android/83793.htm
// http://blog.csdn.net/luoshengyang/article/details/6664554

#define ASHMEM_DEVICE   "/dev/ashmem"
/*
 * ashmem_create_region - creates a new ashmem region and returns the file
 * descriptor, or <0 on error
 *
 * `name' is an optional label to give the region (visible in /proc/pid/maps)
 * `size' is the size of the region, in page-aligned bytes
 */
static int ashmem_create_region(const char *name, size_t size)
{
	int fd, ret;
	
	fd = open(ASHMEM_DEVICE, O_RDWR);
	if (fd < 0)
	        return fd;
	
	if (name) {
	        char buf[ASHMEM_NAME_LEN] = {0};
	
	        strlcpy(buf, name, sizeof(buf));
	        ret = ioctl(fd, ASHMEM_SET_NAME, buf);
	        if (ret < 0)
	                goto error;
	}
	
	ret = ioctl(fd, ASHMEM_SET_SIZE, size);
	if (ret < 0)
	        goto error;
	
	return fd;

error:
	close(fd);
	return ret;
}

#define LEN 1024*1024
int main(int argc, const char *argv[])
{
	int i;
	int count = 0;
	int fd;
	int ret;

	/* Open a file for writing.
  *  - Creating the file if it doesn't exist.
  *  - Truncating it to 0 size if it already exists. (not really needed)
  *
  * Note: "O_WRONLY" mode is not sufficient when mmaping.
  */
loop:
	fd = ashmem_create_region("zzytest", LEN);
	if (fd < 0)
	{
	    perror("Error opening file for writing");
	    exit(EXIT_FAILURE);
	}

	char ash_name[ASHMEM_NAME_LEN] = {0};
	ret = ioctl(fd, ASHMEM_GET_NAME, ash_name);
	if(ret >= 0) {
		printf("ash_name=%s\n", ash_name);	
	}

	// Now the file is ready to be mmapped.
	char *map = mmap(0, LEN, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (map == MAP_FAILED)
	{
	    close(fd);
	    perror("Error mmapping the file");
	    exit(EXIT_FAILURE);
	}
	
	char s[256], name[256];
	memset(s, 0, 256);
	memset(name, 0, 256);
	snprintf(s, 255, "/proc/%d/fd/%d", getpid(), fd);
	readlink(s, name, 255);
	printf("Succeeded to mmap program binaries. File descriptor is %d, and path is %s\n", fd, name);

	printf("memset data\n");
	memset(map, '1', LEN-1);
	//printf("%c\n", map[count%textsize]);
	
	// Write it now to disk
	if (msync(map, LEN, MS_SYNC) == -1)
	{
	    perror("Could not sync the file to disk");
	}
	sleep(1);
	printf("count=%d\n", count++);
	printf("fd=%d\n", fd);
	printf("map=%x\n", map);
	
	// close fd
	close(fd);
	
	// Don't forget to free the mmapped memory
	#if 1
	if (munmap(map, LEN) == -1)
	{
	    close(fd);
	    perror("Error un-mmapping the file");
	    exit(EXIT_FAILURE);
	}
	#endif
goto loop;

	return 0;
}