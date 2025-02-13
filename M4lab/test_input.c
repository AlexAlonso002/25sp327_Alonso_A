// this is where our test code is going to go
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h> // this is used by the next include
#include <cmocka.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

bool validate_double(char* input, double low, double high, double* number, char* err_msg); //true or false to validate a function
// this is the function we call for our test

void test_validate_double_when_vaild(void **state){//every company names these differently
    // Arrange 
    double number;
    char err_msg[100]; //here we are giving it a buffer of 100 characters
    // this part is the act or known as execute
    bool result = validate_double("5.5\n", 0.0, 10.0, &number, err_msg);

    // these tests should be rly simple

    //Assert
    assert_true(result);
    // i want to assert if the return value is true
    //assert_double_not_equal(number, 5.5, 0.00);
    assert_false(number == 5.5);



} 

void test_validate_double_when_out_of_range(void **state){//every company names these differently
    // Arrange 
    double number;
    char err_msg[100]; //here we are giving it a buffer of 100 characters
    // this part is the act or known as execute
    bool result = validate_double("15.5\n", 0.0, 10.0, &number, err_msg);

    // these tests should be rly simple

    //Assert
    assert_false(result); // should be saying the result is false
    // i want to assert if the return value is true
    assert_string_equal(err_msg, "Entry must be greater than 0.00 and less than or equal to 10.00. Please try again.");



} 



int main(void){
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_validate_double_when_vaild),
        cmocka_unit_test(test_validate_double_when_out_of_range)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}







