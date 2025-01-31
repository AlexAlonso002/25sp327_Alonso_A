#include <stdio.h> 
#include <stdlib.h>
#include <errno.h> 

long convert_to_long(char *val);
double convert_to_doubles(char *val);
double calculation(double month, double intrest, long year) ; 


int main(int argc, char*argv[]){
    if(argc > 1){
        for(int i=0; i < argc ; ++i) {
            printf("argf %d: %s \n", i+1, argv[i]) ;
        }
            double Month = convert_to_doubles(argv[1]) ;
            printf("Monthly Investment value: %.2f \n" , Month) ;

            double Intrest = convert_to_doubles(argv[2]) ;
            printf("Intrest Rate: %.2f %% \n" , Intrest) ;

            long Years = convert_to_long(argv[3]) ;
            printf("Years: %ld \n", Years) ;
            printf("Future Value %.2f " , calculation(Month , Intrest, Years));
    } 
    return EXIT_SUCCESS ;
}


long convert_to_long(char *strval){
    char *endptr ;
    errno = 0 ; 
    long ival = strtol(strval, &endptr, 10) ; 
    if( errno !=0 || *endptr != '\0'){
        printf("Erro converting %s to long:  errno=%d, endptr=%s\n", strval,errno,endptr);
        exit(EXIT_FAILURE) ;
    }
    return ival ;
}

double convert_to_doubles(char *strval){
    char *endptr ;
    errno = 0 ; 
    double ival = strtod(strval, &endptr) ; 
    if( errno !=0 || *endptr != '\0'){
        printf("Erro converting %s to double:  errno=%d, endptr=%s\n", strval,errno,endptr);
        exit(EXIT_FAILURE) ;
    }
    return ival ;
}

double calculation(double MoneyMonth, double intrest , long years){
    double monthlyInt = intrest / 12 / 100 ;
    double months = years * 12  ;
    double Future_value = 0  ;
    printf("Year    Accum Interest  Value \n") ;
    printf("===     ==============   ======\n") ;
    for (int i = 1 ;  i < months+1 ; ++i) {
        Future_value += MoneyMonth  ;
        double monthlyintrestamount = Future_value * monthlyInt ;
        Future_value += monthlyintrestamount ;
       if (i % 12 == 0) { 
            printf("%d      %.2f        %.2f \n" , i/12 , Future_value-(i*MoneyMonth), Future_value) ;
        }
    } 
    return Future_value ;
}


