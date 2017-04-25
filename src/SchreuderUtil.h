#ifndef __SchreuderUtil_H__
#define __SchreuderUtil_H__

#include "OpenGL.h"


// The following convenience methods were written by W. Schreuder for CSCI 5229 
// unless otherwise noted.

/*
*  Create Shader (From Example 27)
*/
int CreateShader(GLenum type, char* file);

/*
*  Create Shader Program (From Example 27)
*/
int CreateShaderProg(char* VertFile, char* FragFile);

/*
*  Function to print any errors encountered
*/
void ErrCheck(const char* where);

/*
 * Prints the message to stderr and terminates the application.
 */
void Fatal(const char* format, ...);

/*
*  Load texture from BMP file
*/
unsigned int LoadTexBMP(const char* file);

/*
* Texture with alpha channel. (GEL addition 2015-06-13)
*/
unsigned int LoadTexDecalBMP(const char* file, const char* alpha);

/*
* Convenience function for text
*/
void Print(const char* format, ...);

/*
*  Print Program Log (From Example 27)
*/
void PrintProgramLog(int obj);

/*
*  Print Shader Log (From Example 27)
*/
void PrintShaderLog(int obj, char* file);

/*
* Loads a bitmap into memory given by the file name, and outputs the width and
* height to dx and dy and a pointer to the RGB data.
*/
unsigned char* ReadBMP(const char* file, unsigned int* dx, unsigned int* dy);

/*
*  Read text file (From Example 27)
*/
char* ReadText(char *file);

#endif