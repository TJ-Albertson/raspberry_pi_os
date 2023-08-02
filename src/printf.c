/*
File: printf.c

modified. original Kustaa Nyholm http://www.sparetimelabs.com/tinyprintf/tinyprintf.php
*/

#include "printf.h"

typedef void (*putcf)(void *, char);
static putcf stdout_put_character;
static void *stdout_put_pointer;

// convert unsigned int to string
// base = 10 or 16(hex)
// if upper_case is nonzero then uppercase letters will be used for hex
static void unsigned_int_to_string(unsigned int u_int, unsigned int base, int upper_case, char *buffer)
{
    int count = 0;
    unsigned int divisor = 1;

    // Find the largest power of the base that is less than or equal to num. e.g. u_int == 312, divisor == 10^2
    while (u_int / divisor >= base)
        divisor *= base;

    // iterate through each digit of number
    while (divisor != 0)
    {
        int current_digit = u_int / divisor;    // save first digit of u_int       e.g: u_int == 312, current_digit = 3
        u_int %= divisor;                       // remove first digit of u_int     e.g: u_int == 312, after, u_int = 12
        divisor /= base;                        // take divisor down one power     e.g: divisor == 10^2, after, divisor == 10

        // Check if the digit is non-zero or if 'count' is already non-zero (to include leading zeros).
        // Also, include the digit if it is the last digit (divisor == 0) even if it is zero.
        if (count || current_digit > 0 || divisor == 0)
        {
            // conver current digit to char and add to buffer, with clause for hex
            *buffer++ = current_digit + (current_digit < 10 ? '0' : (upper_case ? 'A' : 'a') - 10);
            ++count;
        }
    }

    // add null terminator '\0' to end of string
    *buffer = 0;
}

static void int_to_string(int num, char *buffer)
{
    // if int is negative then put '-' in buffer and just perform unsigned_int_to_string
    if (num < 0)
    {
        num = -num;
        *buffer++ = '-';
    }

    unsigned_int_to_string(num, 10, 0, buffer);
}


static int character_to_int(char ch)
{
    if (ch >= '0' && ch <= '9')
        return ch - '0';

    // for hex
    else if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    else if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    else
        return -1;
}

// ch == first character
static char string_to_int(char ch, char **format_string, int base, int *width)
{
    char *copy_format_string = *format_string;
    int num = 0;
    int digit;

    while ((digit = character_to_int(ch)) >= 0)
    {
        if (digit > base)
            break;
        num = num * base + digit;
        ch = *copy_format_string++;
    }

    *format_string = copy_format_string;
    *width = num;
    return ch;
}

// the args str_pointer and put_function are for sprintf to work also
static void putchw(void *str_pointer, putcf put_function, int width, char leading_zero, char *buffer)
{
    char fc = leading_zero ? '0' : ' ';
    char ch;
    char *p = buffer;

    while (*p++ && width > 0)
        width--;

    while (width-- > 0)
        put_function(str_pointer, fc);

    while ((ch = *buffer++))
        put_function(str_pointer, ch);
}

// the args str_pointer and put_function are for sprintf to work also
void tfp_format(void *str_pointer, putcf put_function, char *format_string, va_list va)
{
    char buffer[12];

    char ch;

    while ((ch = *(format_string++)))
    {
        if (ch != '%')
            put_function(str_pointer, ch);
        else
        {
            char leading_zero = 0;

            int width = 0;
            ch = *(format_string++);

            if (ch == '0')
            {
                ch = *(format_string++);
                leading_zero = 1;
            }

            if (ch >= '0' && ch <= '9')
            {
                ch = string_to_int(ch, &format_string, 10, &width);
            }

            switch (ch)
            {
                case 0:
                    goto abort;

                // unsigned int
                case 'u':
                    unsigned_int_to_string(va_arg(va, unsigned int), 10, 0, buffer);
                    putchw(str_pointer, put_function, width, leading_zero, buffer);
                    break;

                // int
                case 'd':
                    int_to_string(va_arg(va, int), buffer);
                    putchw(str_pointer, put_function, width, leading_zero, buffer);
                    break;

                // lower and uppercase hexidecimal
                case 'x': case 'X':
                    unsigned_int_to_string(va_arg(va, unsigned int), 16, (ch == 'X'), buffer);
                    putchw(str_pointer, put_function, width, leading_zero, buffer);
                    break;

                // char
                case 'c':
                    put_function(str_pointer, (char)(va_arg(va, int)));
                    break;

                // string
                case 's':
                    putchw(str_pointer, put_function, width, 0, va_arg(va, char *));
                    break;

                case '%':
                    put_function(str_pointer, ch);

                default:
                    break;
            }
        }
    }
abort:;
}

// Set
void init_printf(void *putp, void (*putf)(void *, char))
{
    stdout_put_character = putf;
    stdout_put_pointer = putp;
}

void tfp_printf(char *format_string, ...)
{
    va_list variable_args;
    va_start(variable_args, format_string);
    tfp_format(stdout_put_pointer, stdout_put_character, format_string, variable_args);
    va_end(variable_args);
}

static void put_char_pointer(void *p, char c)
{
    // this stores c at the pointer location and increments the pointers address to the next address in memory. I think...
    *(*((char **)p))++ = c;
}

void tfp_sprintf(char *string, char *format_string, ...)
{
    va_list variable_args;
    va_start(variable_args, format_string);
    tfp_format(&string, put_char_pointer, format_string, variable_args);

    // append null terminator '\0'
    put_char_pointer(&string, 0);
    va_end(variable_args);
}
