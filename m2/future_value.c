#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void calculation(double month, double interest, long year, double arrYears[], double IntArray[], double FV[]);
long convert_to_long(char *type , char *val);
double convert_to_doubles(char *type , char *val);
void printResults(long years, double arrYears[], double IntArray[], double FV[]);
void FailPrint(char *type);

int main(int argc, char* argv[]) {
    if (argc < 4) {
        FailPrint("Missing Arguments");
    }

    double Month = convert_to_doubles("Monthly-Investment", argv[1]);
    printf("Monthly Investment value: %.2f \n", Month);

    double Interest = convert_to_doubles("Annual Interest Rate", argv[2]);
    printf("Interest Rate: %.2f%% \n", Interest);

    long Years = convert_to_long("Years", argv[3]);
    printf("Years: %ld \n", Years);

    // Declare arrays to store results
    double arrYears[Years], IntArray[Years], FV[Years];

    // Perform calculation
    calculation(Month, Interest, Years, arrYears, IntArray, FV);

    // Print results
    printResults(Years, arrYears, IntArray, FV);

    return EXIT_SUCCESS;
}

void calculation(double MoneyMonth, double interest, long years, double arrYears[], double IntArray[], double FV[]) {
    double monthlyInt = interest / 12 / 100;
    long months = years * 12;
    double Future_value = 0;
    double totalDeposits = 0;

    for (long i = 1; i <= months; ++i) {
        Future_value += MoneyMonth;
        totalDeposits += MoneyMonth;
        double monthlyInterestAmount = Future_value * monthlyInt;
        Future_value += monthlyInterestAmount;

        if (i % 12 == 0) {
            int yearIndex = i / 12 - 1;
            arrYears[yearIndex] = i / 12; // Year
            IntArray[yearIndex] = Future_value - totalDeposits; // Accumulated Interest
            FV[yearIndex] = Future_value; // Future Value
        }
    }
}

void printResults(long years, double arrYears[], double IntArray[], double FV[]) {
    printf("\nYear    Accumulated Interest  Future Value\n");
    printf("===     ===================   ============\n");
    for (long i = 0; i < years; i++) {
        printf("%-6.0f   %-20.2f %.2f\n", arrYears[i], IntArray[i], FV[i]);
    }
}

void FailPrint(char *type) {
    printf("Error: Invalid Input for %s \n", type);
    printf("Usage: ./future_value <Monthly-Investment> <Annual-Interest-Rate> <Number-of-Years>\n");
    printf("Monthly-Investment: Numeric value in range [1, 1000] representing the amount invested each month.\n");
    printf("Annual-Interest-Rate: Decimal value in range [1, 15] representing the annual interest rate.\n");
    printf("Number-of-Years: Integer value in range [1, 50] representing the duration of the investment in years.\n");
    exit(EXIT_FAILURE);
}

long convert_to_long(char *type, char *strval) {
    char *endptr;
    errno = 0;
    long ival = strtol(strval, &endptr, 10);
    if (errno != 0 || *endptr != '\0') {
        FailPrint(type);
    }
    return ival;
}

double convert_to_doubles(char *type, char *strval) {
    char *endptr;
    errno = 0;
    double ival = strtod(strval, &endptr);
    if (errno != 0 || *endptr != '\0') {
        FailPrint(type);
    }
    return ival;
}
