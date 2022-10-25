#include "log.h"
#include "uart.h"
#include "stdio.h"
#include "stdarg.h"

#define filename(x) strrchr(x,'\\')?strrchr(x,'\\')+1:x


char *EM_LOGlevelGet(const int level){
  if(level==LOG_DEBUG){
    return "DEBUG";
  }else if(level==LOG_INFO)
  {
    return "INFO";
  }else if(level==LOG_WARN)
  {
    return "WARN";
  }else if(level==LOG_ERROR)
  {
    return "ERROR";
  }
  return "unknow";
  
}

void EM_LOG(const int level,const char* fun,const int line,const char *fmt,...)
{
#ifdef OPEN_LOG
  va_list arg;
  va_start(arg, fmt);
  //char buf[vsnprintf(NULL,0,fmt,arg)+1];
  char buf[50] = {0};
  va_start(arg,fmt);
  vsnprintf(buf, sizeof(buf), fmt, arg);
  
  va_end(arg);
  if(level>=LOG_LEVEL)
    printf("[%s][%s][%s %d]%s \r\n",EM_LOGlevelGet(level),filename(__FILE__),fun,line,buf);
    
  #endif
}