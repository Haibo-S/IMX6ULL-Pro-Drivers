#include "my_printf.h"
#include "uart.h"

// Macros for handling variable arguments
typedef char * va_list;
#define _INTSIZEOF(n)   ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

#define va_start(ap, v)  (ap = (va_list)&v + _INTSIZEOF(v))
#define va_arg(ap, t)    (*(t *)(ap = ap + _INTSIZEOF(t), ap - _INTSIZEOF(t)))
#define va_end(ap)       (ap = (va_list)0)

// Lookup table for converting numbers to hexadecimal format
unsigned char hex_tab[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

// Outputs a single character to UART
static int outc(int c) {
    PutChar(c);
    return 0;
}

// Outputs a string to UART
static int outs(const char *s) {
    while (*s != '\0') {
        PutChar(*s++);
    }
    return 0;
}

// Outputs a number in a specified base with optional leading characters and width
static int out_num(long n, int base, char lead, int maxwidth) {
    unsigned long m = (n < 0) ? -n : n;
    char buf[MAX_NUMBER_BYTES], *s = buf + sizeof(buf);
    int count = 0;

    *--s = '\0';
    do {
        *--s = hex_tab[m % base];
        count++;
    } while ((m /= base) != 0);

    while (maxwidth-- > count) {
        *--s = lead;
    }

    if (n < 0) {
        *--s = '-';
    }

    return outs(s);
}

// Custom vprintf implementation for formatted output
static int my_vprintf(const char *fmt, va_list ap) {
    char lead = ' ';
    int maxwidth = 0;

    for (; *fmt != '\0'; fmt++) {
        if (*fmt != '%') {
            outc(*fmt);
            continue;
        }

        fmt++;  // skip '%'
        lead = (*fmt == '0') ? '0' : ' ';
        if (*fmt == '0') fmt++;

        while (*fmt >= '0' && *fmt <= '9') {
            maxwidth = maxwidth * 10 + (*fmt - '0');
            fmt++;
        }

        switch (*fmt) {
            case 'd': out_num(va_arg(ap, int), 10, lead, maxwidth); break;
            case 'o': out_num(va_arg(ap, unsigned int), 8, lead, maxwidth); break;
            case 'u': out_num(va_arg(ap, unsigned int), 10, lead, maxwidth); break;
            case 'x': out_num(va_arg(ap, unsigned int), 16, lead, maxwidth); break;
            case 'c': outc(va_arg(ap, int)); break;
            case 's': outs(va_arg(ap, char *)); break;
            default:  outc(*fmt); break;
        }
    }
    return 0;
}

// Custom printf implementation
int printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    my_vprintf(fmt, ap);
    va_end(ap);
    return 0;
}

// Test function for printf
int my_printf_test(void) {
    printf("This is www.100ask.org   my_printf test\n\r");
    printf("test char           = %c,%c\n\r", 'A', 'a');
    printf("test decimal number = %d\n\r", 123456);
    printf("test hex number     = 0x%x\n\r", 0x55aa55aa);
    printf("test string         = %s\n\r", "www.100ask.org");
    printf("num = %08d\n\r", 12345);
    return 0;
}
