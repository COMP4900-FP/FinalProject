#include <time.h>

#define BOOL_TO_LIGHT(B) (B==1?"ON":"OFF")

unsigned long start;
#define SECONDS_SINCE_START (time(NULL) - start)

#define HOUR_OF_DAY ((SECONDS_SINCE_START >> 1) % 24)
#define MINUTE_OF_HOUR ((SECONDS_SINCE_START & 0b1) * 30)

#define MIN_BOUND(A,B) (A = (((A)<(B))?(B):(A)))
#define MAX_BOUND(A,B) (A = (((A)>(B))?(B):(A)))
#define MIN_MAX_BOUND(A,B,C) (MIN_BOUND(A,B); MAX_BOUND(A,C))

#define INVALID -1