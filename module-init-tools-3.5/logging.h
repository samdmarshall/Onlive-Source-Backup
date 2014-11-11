#ifndef MODINITTOOLS_LOGGING_H
#define MODINITTOOLS_LOGGING_H

/* Do we use syslog for messages or stderr? */
extern int logging;

/* Number of times warn() has been called */
extern int warned;

void fatal(const char *fmt, ...);
void warn(const char *fmt, ...);

#define NOFAIL(ptr)  do_nofail((ptr), __FILE__, __LINE__, #ptr)

static inline void *do_nofail(void *ptr, const char *file, int line, const char *expr)
{
	if (!ptr) {
		fatal("Memory allocation failure %s line %d: %s.\n",
		      file, line, expr);
	}
	return ptr;
}

#endif /* MODINITTOOLS_LOGGING_H */
