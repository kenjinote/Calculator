#ifndef STRING_CALCULATOR_H
#define STRING_CALCULATOR_H

#include <string>

// Add support for exponential and square roots

/*
convert a character digit to an integer value
input: digit - a character representing a digit, e.g. '7'
output: the corresponding integer value of the digit, e.g. 7
throws: std::invalid_argument if the input is not a valid digit, e.g. '0' -- '9'
*/
unsigned int digit_to_decimal(wchar_t digit);

/*
convert an integer value to a character digit
input: decimal - an integer value of a digit, e.g. 7
output: the corresponding character of the digit, e.g. '7'
throws: std::invalid_argument if the input is not a valid digit, e.g. 0 -- 9
*/
wchar_t decimal_to_digit(unsigned int decimal);

/*
remove the leading zeros from a number
input: num - a wstring representing a number with zero or more leading 0s
output: a copy of the input with all leading 0s removed
throws: none 
*/
std::wstring trim_leading_zeros(std::wstring num);

/*
add two numbers 
input: lhs,rhs - strings representing numbers, e.g. "1", "2"
output: the result of adding rhs to lhs: lhs + rhs e.g. "3"
throws: none
*/
std::wstring add(std::wstring lhs, std::wstring rhs);

/*
subtract two numbers
input: lhs,rhs - strings representing numbers, e.g. "1", "2"
output: the result of subtracting rhs from lhs: lhs - rhs, e.g. "-1"
throws: none
*/
std::wstring subtract(std::wstring lhs, std::wstring rhs);

/*
muplity two numbers
input: lhs,rhs - strings representing numbers, e.g. "1", "2"
output: the result of multiplying lhs by rhs: lhs * rhs, e.g. "2"
throws: none
*/
std::wstring multiply(std::wstring lhs, std::wstring rhs);

/*
calculates the number of times a number can go into another number
input: lhs,result - strings representing numbers, e.g. "1", "2"
output: the number of times lhs goes into result: "2" into "6" gives "3"
throws: none
*/
std::wstring numTimes(std::wstring lhs, std::wstring result);

/*
compares two strings
input: lhs,rhs - strings representing numbers, e.g. "1", "2"
output: true if lhs > rhs, false otherwise: 47 > 46 gives true
throws: none
*/
bool compare(std::wstring lhs, std::wstring rhs);

/*
divides two numbers
input: dividend,divisor - strings representing numbers, e.g. "1", "2"
output: the result of integer division of dividend by divisor: dividend / divisor, e.g. "2"
throws: std::invalid_argument if the dividend is 0: cannot divide by 0
*/
std::wstring division(std::wstring dividend, std::wstring divisor);

/*
modulo division of two numbers
input: dividend,divisor - strings representing numbers, e.g. "1", "2"
output: the result of integer division of dividend by divisor: dividend / divisor, e.g. "2"
throws: std::invalid_argument if the dividend is 0 or if the divisor/dividend are not numbers: cannot divide by 0
*/
std::wstring modulus(std::wstring dividend, std::wstring divisor);

std::wstring power(std::wstring lhs, std::wstring rhs);

#endif  // STRING_CALCULATOR_H
