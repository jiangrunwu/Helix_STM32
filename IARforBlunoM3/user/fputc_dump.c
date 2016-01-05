#include <stdio.h>


#pragma import(__use_no_semihosting_swi)
 
// 声明外部函数，在main文件中定义
extern int  DF_SendChar(int ch);

extern int  DF_GetKey(void);



FILE __stdout;

FILE __stdin;

int fputc(int ch, FILE *f) {

  return (DF_SendChar(ch));

}

int fgetc(FILE *f) {

  return (DF_SendChar(DF_GetKey()));

}

void _ttywrch(int ch) {

 DF_SendChar (ch);

}

int ferror(FILE *f) {                            // Your implementation of ferror

  return EOF;

}

void _sys_exit(int return_code) {

label:  goto label;           // endless loop

}

 