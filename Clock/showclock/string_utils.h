#ifndef _STRING_UTILS_H
#define _STRING_UTILS_H

#include "uart.h"

/**
 * Converts a string to an unsigned long integer.
 * 
 * @param cp The string to be converted.
 * @param endptr Pointer to the character that stops the conversion.
 * @param base The numerical base (2 to 36 or 0) that determines the valid characters and their interpretation.
 * @return The converted unsigned long integer value or zero if no valid conversion could be performed.
 */
unsigned long simple_strtoul(const char *cp, char **endp, unsigned int base);

/**
 * Converts a string to a long integer.
 * 
 * @param cp The string to be converted.
 * @param endptr Pointer to the character that stops the conversion.
 * @param base The numerical base (2 to 36 or 0) that determines the valid characters and their interpretation.
 * @return The converted long integer value or zero if no valid conversion could be performed.
 */
long simple_strtol(const char *cp, char **endp, unsigned int base);

/**
 * Reads a string from UART until a newline is encountered.
 * 
 * @param s The buffer where the input string will be stored.
 * @return A pointer to the buffer containing the string.
 */
char *gets(char s[]);

/**
 * Reads characters from UART, converts them into a long integer, and returns the value.
 * 
 * @return The converted long integer.
 */
int get_int(void);

/**
 * Reads characters from UART, converts them into an unsigned long integer, and returns the value.
 * 
 * @return The converted unsigned long integer.
 */
unsigned int get_uint(void);

#endif /* _STRING_UTILS_H */
