#ifndef _LOG_H
#define _LOG_H

#include <stdarg.h>
#include <stdio.h>

//日志打印
#define OPEN_LOG 1
#define LOG_LEVEL   LOG_DEBUG//LOG_WARN//LOG_DEBUG
#define LOG_SACE 1
//行号打印

typedef enum
{
  LOG_DEBUG = 0,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
} E_LOGLEVEL;


char *EM_LOGlevelGet(const int level);
void EM_LOG(const int level, const char *fun, const int line, const char *fmt, ...);

#define  EMlog(level,fmt...) EM_LOG(level,__FUNCTION__,__LINE__,fmt)

#endif
