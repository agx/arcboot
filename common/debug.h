/*
 * Debugging macros
 */

#ifdef DEBUG
#define debug_print(fmt,...) \
        printf(fmt, ##__VA_ARGS__)
#else
#define debug_printf(fmt,...) \
        do { } while (0)
#endif /* !DEBUG */

