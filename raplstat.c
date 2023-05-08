#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>
#include <sys/time.h>
#include <time.h>

char* basepath = "/sys/class/powercap/intel-rapl:0/energy_uj";
/**
 * Returns the current time in microseconds.
 * From https://gist.github.com/w1k1n9cc/012be60361e73de86bee0bce51652aa7
 */
long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

int64_t read_energy(char* fpath)
{
    FILE *f = fopen(fpath, "r");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *buf = malloc(fsize+1);
    fread(buf, fsize, 1, f);
    fclose(f);

    //long result = strtol(buf, &eptr, 10);
    int64_t result = strtoll(buf, NULL, 10);
    free(buf);
    return result;
}

int main(int argc, char *argv[])
{
    char* efile = "/sys/class/powercap/intel-rapl:0/energy_uj";
    if(argc == 2) {
        efile = argv[1];
    }

    if(!strcmp(efile, "--help")||!strncmp(efile, "--help",
			strlen("--help"))) {
        printf("usage: raplstat <path_to_meter>\n");
        printf("path usually: /sys/class/powercap/intel-rapl:*/energy_uj\n");
        return 0;
    }

    if (access(efile, R_OK) != 0) {
        printf("error: cannot read file %s\n", efile);
        return 1;
    }
    int64_t e1 = read_energy(efile);
    long t1 = getMicrotime();
    nanosleep((const struct timespec[]){{0, 100000000L}}, NULL);
    int64_t de = read_energy(efile) - e1;
    long dt = getMicrotime() - t1;
    double power = de / (float) dt;
    printf("%.2lf\n", power);
}