#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define COLUMN_FORMAT "%-29s"


void get_hostname() {
    FILE *file = fopen("/proc/sys/kernel/hostname", "r");
    if (file) {
        char hostname[256];
        if (fgets(hostname, sizeof(hostname), file)) {
            printf(COLUMN_FORMAT " %s\n", "Current Host Name:", hostname);

        } else {
            perror("Error reading hostname");
        }

        fclose(file);
    } else {
        perror("Error opening hostname");
    }
}
void get_cpu_info() {
    FILE *file = fopen("/proc/cpuinfo", "r");
    char line[256];
    char model[256] = {0};
    int cores = 0;
    char modelType[256] = {0}; 

    if (file) {
        // loop to get all line (whyyyyyy)
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "model name", 10) == 0) {
                strcpy(model, strchr(line, ':') + 2);  
                model[strcspn(model, "\n")] = 0; 
            }
             if (strncmp(line, "vendor_id", 9) == 0) {  
                strcpy(modelType, strchr(line, ':') + 2);  
                modelType[strcspn(modelType, "\n")] = 0; 
            }
            if (strncmp(line, "cpu cores", 9) == 0) {
                cores = atoi(strchr(line, ':') + 2);  
            }
        }
        fclose(file);
        
        printf(COLUMN_FORMAT " %s \n" ,"CPU Type:", model);
        printf(COLUMN_FORMAT " %d \n" ,"CPU Cores:", cores);
        printf(COLUMN_FORMAT " %s \n" ,"CPU Model:", modelType);
    } else {
        perror("Error opening cpu info");
    }
}
void get_kernel_version() {
    FILE *file = fopen("/proc/version", "r");
    if (file) {
        char version[40];

        if (fgets(version, sizeof(version), file)) {  
            printf(COLUMN_FORMAT " %s \n" ,"Kernel Version:", version);
        } else {
            perror("Error reading kernel version");
        }
        fclose(file);
    } else {
        perror("Error opening kernal");
    }
}

void get_uptime() {
    FILE *file = fopen("/proc/uptime", "r");
    if (file) {
        char line[256];
        if (fgets(line, sizeof(line), file)) {
            double uptime;
            if (sscanf(line, "%lf", &uptime) == 1) {
                int days = uptime / 86400;
                int hours = ((int)uptime % 86400) / 3600;
                int minutes = ((int)uptime % 3600) / 60;
                int seconds = (int)uptime % 60;
                printf(COLUMN_FORMAT " %d : %d :%d : %d \n", "Uptime:", days, hours, minutes, seconds);
            } else {
                perror("Error parsing uptime data");
            }
        } else {
            perror("Error reading line from uptime");
        }
        fclose(file);
    } else {
        perror("Error opening uptime");
    }
}
void get_cpu_usage() {
    FILE *file = fopen("/proc/stat", "r");
    if (file) {
        char line[256];
        char label[44];
        long user, nice, system, idle;

        if (fgets(line, sizeof(line), file)) {
            // Parsing the data from the line manually
            if (sscanf(line, "%s %ld %ld %ld %ld", label, &user, &nice, &system, &idle) == 5) {
                printf(COLUMN_FORMAT " %ld ms \n", "Time Spent in User Mode: ", user);
                printf(COLUMN_FORMAT " %ld ms \n", "Time Spent in System Mode: ", system);
                printf(COLUMN_FORMAT " %ld ms \n", "Time Spent in Idle Mode: ", idle);
            } else {
                perror("Error parsing CPU usage data");
            }
        } else {
            perror("Error reading line from stat");
        }

        fclose(file);
    } else {
        perror("Error opening stat");
    }
}

void get_memory_info() {
    FILE *file = fopen("/proc/meminfo", "r");
    char line[44];
    long mem_total = 0 ;
    long mem_available = 0;
    if (file) {
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "MemTotal", 8) == 0) {
                sscanf(line, "MemTotal: %ld", &mem_total);
            }
            if (strncmp(line, "MemAvailable", 11) == 0) {
                sscanf(line, "MemAvailable: %ld", &mem_available);
            }
        }
        printf(COLUMN_FORMAT " %ld KB \n" , "Memory Configured:", mem_total);
        printf(COLUMN_FORMAT " %ld KB \n" ,"Memory Available:", mem_available);
        fclose(file);
    }
}

int main() {
    printf("------------------------ \n");
    printf("System Information \n");
    printf("------------------------ \n");
    get_hostname();
    get_cpu_info();
    get_kernel_version();
    get_memory_info();
    get_uptime();
    get_cpu_usage();
    return 0;
}