/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#ifndef __READER_H__
#define __READER_H__

#define IO_ERROR 0
#define IO_SUCCESS 1
int lineNo, colNo;
int currentChar;

int readChar(void);
int jump(int distance);
long where();
int openInputStream(char *fileName);
void closeInputStream(void);

#endif
