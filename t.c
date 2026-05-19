#include <stdio.h>
#include <sys/time.h> // Required header for gettimeofday()

int main() {
    struct timeval tv;

    // The second argument (timezone) is obsolete and should be NULL
    if (gettimeofday(&tv, NULL) == 0) {
        printf("Seconds since Epoch: %ld\n", tv.tv_sec);
        printf("Microseconds: %ld\n", tv.tv_usec);
    } else {
        perror("gettimeofday");
        return 1;
    }

    return 0;
}
