#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


#define PRINT_IDS(stage) \
    do { \
        uid_t ruid, euid, suid; \
        if (getresuid(&ruid, &euid, &suid) == -1) { \
            perror("getresuid failed"); \
            exit(EXIT_FAILURE); \
        } \
        printf("%s RUID: %d, EUID: %d, SUID: %d\n", stage, ruid, euid, suid); \
    } while (0)


/*
Order of execution is important here because changing the owner resets the execute bit to 'x'
sudo chown root setuid_demo
sudo chmod u+s setuid_demo
*/



int main() {

    PRINT_IDS("T0: Running with root privileges: ");

    // =========

    if (seteuid(1000) == -1) {
        perror("Failed T1");
        exit(EXIT_FAILURE);
    }
    PRINT_IDS("T1: Dropped privileges temporarily: ");
    /// ==============

    if (seteuid(0) == -1) {
        perror("failed T2");
        exit(EXIT_FAILURE);
    }
    PRINT_IDS("T2: Restored Privileges:");

// ============

    if (seteuid(1000) == -1) {
        perror("failed T3");
        exit(EXIT_FAILURE);
    }

    PRINT_IDS("T3: Dropped Privileges again:");

    // ===============
//
    printf("T4: Permanetly dropping privileges: \n");  
    if (setresuid(getuid(), getuid(), getuid()) == -1) {
        perror("Failed to t4");
        exit(EXIT_FAILURE);
    }

      PRINT_IDS("T4: After Permanent drop:");  
    /// =============
    printf("T5: Attmepting to restore privileges: \n");  
    if(setuid(0) == -1){
        perror("T5: Failed To Restore Privileges");
        exit(EXIT_FAILURE);
    }

    PRINT_IDS("T5: After Permanent drop:");  


    return EXIT_SUCCESS;
}
