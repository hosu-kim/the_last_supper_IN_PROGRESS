#include <sys/time.h>
#include <stdio.h>

int	main()
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == 0)
	{
		printf("Sec: %ld\n", tv.tv_sec);
		printf("Microsec: %ld\n", tv.tv_usec);
	}
	else
	{
		perror("gettimeofday() failed");
		return (1);
	}
	return (0);
}
