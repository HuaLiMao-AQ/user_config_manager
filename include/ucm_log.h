#ifndef UCM_LOG_H
#define UCM_LOG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

#define _FATAL_(err, __FILE__, fmt, msgs...) fatal(err, __FILE__ ": " fmt , ##msgs)

#define _WARN_(fmt, msgs...) warn(fmt, ##msgs)

extern void fatal(const int err,const char *fmt,...);
extern void warn(const char *fmt,...);

#endif