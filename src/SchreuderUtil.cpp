#include <cstdarg>
#include <cstdio>
#include <cstdlib>

#include "OpenGL.h"

#include "SchreuderUtil.h"

int CreateShader(GLenum type, char* file)
{
	//  Create the shader
	int shader = glCreateShader(type);

	//  Load source code from file
	char* source = ReadText(file);
	glShaderSource(shader, 1, (const char**)&source, NULL);
	free(source);

	//  Compile the shader
	fprintf(stderr, "Compile %s\n", file);
	glCompileShader(shader);

	//  Check for errors
	PrintShaderLog(shader, file);

	//  Return name
	return shader;
}

int CreateShaderProg(char* VertFile, char* FragFile)
{
	//  Create program
	int prog = glCreateProgram();

	//  Create and compile vertex shader
	int vert = CreateShader(GL_VERTEX_SHADER, VertFile);

	//  Create and compile fragment shader
	int frag = CreateShader(GL_FRAGMENT_SHADER, FragFile);

	//  Attach vertex shader
	glAttachShader(prog, vert);

	//  Attach fragment shader
	glAttachShader(prog, frag);

	//  Link program
	glLinkProgram(prog);

	//  Check for errors
	PrintProgramLog(prog);

	//  Return name
	return prog;
}

void ErrCheck(const char* where)
{
	int err = glGetError();
	if (err)
		fprintf(stderr, "ERROR: (%d) %s [%s]\n", err, gluErrorString(err), where);
}

void Fatal(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(1);
}

static void Reverse(void* x, const int n)
{
	int k;
	char* ch = (char*)x;
	for (k = 0; k < n / 2; k++)
	{
		char tmp = ch[k];
		ch[k] = ch[n - 1 - k];
		ch[n - 1 - k] = tmp;
	}
}

unsigned char* ReadBMP(const char* file, unsigned int* dx, unsigned int* dy) {
	unsigned int   texture;    // Texture name
	FILE*          f;          // File pointer
	unsigned short magic;      // Image magic
	unsigned int   size; // Image dimensions
	unsigned short nbp, bpp;    // Planes and bits per pixel
	unsigned char* image;      // Image data
	unsigned int   k;          // Counter
	int            max;        // Maximum texture dimensions

	//  Open file
	f = fopen(file, "rb");
	if (!f)
		Fatal("Cannot open file %s\n", file);

	//  Check image magic
	if (fread(&magic, 2, 1, f) != 1)
		Fatal("Cannot read magic from %s\n", file);

	if (magic != 0x4D42 && magic != 0x424D)
		Fatal("Image magic not BMP in %s\n", file);

	//  Seek to and read header
	if (fseek(f, 16, SEEK_CUR) || fread(dx, 4, 1, f) != 1 || fread(dy, 4, 1, f) != 1 ||
		fread(&nbp, 2, 1, f) != 1 || fread(&bpp, 2, 1, f) != 1 || fread(&k, 4, 1, f) != 1)
		Fatal("Cannot read header from %s\n", file);

	//  Reverse bytes on big endian hardware (detected by backwards magic)
	if (magic == 0x424D)
	{
		Reverse(dx, 4);
		Reverse(dy, 4);
		Reverse(&nbp, 2);
		Reverse(&bpp, 2);
		Reverse(&k, 4);
	}

	//  Check image parameters
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
	if (*dx<1 || *dx>max)
		Fatal("%s image width %d out of range 1-%d\n", file, dx, max);

	if (*dy<1 || *dy>max)
		Fatal("%s image height %d out of range 1-%d\n", file, dy, max);

	if (nbp != 1)
		Fatal("%s bit planes is not 1: %d\n", file, nbp);

	if (bpp != 24)
		Fatal("%s bits per pixel is not 24: %d\n", file, bpp);

	if (k != 0)
		Fatal("%s compressed files not supported\n", file);

#ifndef GL_VERSION_2_0
	//  OpenGL 2.0 lifts the restriction that texture size must be a power of two
	for (k = 1; k < *dx; k *= 2);
	if (k != *dx) Fatal("%s image width not a power of two: %d\n", file, *dx);
	for (k = 1; k < *dy; k *= 2);
	if (k != *dy) Fatal("%s image height not a power of two: %d\n", file, *dy);
#endif

	//  Allocate image memory
	size = 3 * (*dx) * (*dy);
	image = (unsigned char*)malloc(size);

	if (!image)
		Fatal("Cannot allocate %d bytes of memory for image %s\n", size, file);

	//  Seek to and read image
	if (fseek(f, 20, SEEK_CUR) || fread(image, size, 1, f) != 1)
		Fatal("Error reading data from image %s\n", file);

	fclose(f);

	//  Reverse colors (BGR -> RGB)
	for (k = 0; k < size; k += 3)
	{
		unsigned char temp = image[k];
		image[k] = image[k + 2];
		image[k + 2] = temp;
	}

	return image;
}

unsigned int LoadTexBMP(const char* file)
{
	unsigned int   texture;    // Texture name
	FILE*          f;          // File pointer
	unsigned short magic;      // Image magic
	unsigned int   dx, dy, size; // Image dimensions
	unsigned short nbp, bpp;    // Planes and bits per pixel
	unsigned char* image;      // Image data
	unsigned int   k;          // Counter
	int            max;        // Maximum texture dimensions

	image = ReadBMP(file, &dy, &dx);

	//  Sanity check
	ErrCheck("LoadTexBMP");

	//  Generate 2D texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//  Copy image
	glTexImage2D(GL_TEXTURE_2D, 0, 3, dx, dy, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	if (glGetError())
		Fatal("Error in glTexImage2D %s %dx%d\n", file, dx, dy);

	//  Scale linearly when image size doesn't match
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//  Free image memory
	free(image);

	//  Return texture name
	return texture;
}

unsigned int LoadTexDecalBMP(const char* file, const char* alpha) {
	unsigned int   texture;    // Texture name

	unsigned int   imageWidth, imageHeight; // Image dimensions
	unsigned char* image = ReadBMP(file, &imageWidth, &imageHeight);

	unsigned int   maskWidth, maskHeight; // Image dimensions
	unsigned char* mask = ReadBMP(alpha, &maskWidth, &maskHeight);

	// 2015-06-13 GEL Make sure mask and image are the same size.
	if (imageWidth != maskWidth || imageHeight != maskHeight) {
		free(image);
		free(mask);
		Fatal("Mask and image dimensions must be identical.");
	}

	//  Sanity check
	ErrCheck("LoadTexBMP");

	//  Generate 2D texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//  Copy image
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imageWidth, imageHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	if (glGetError())
		Fatal("Error in glTexImage2D %s %dx%d\n", file, imageWidth, imageHeight);

	//  Scale linearly when image size doesn't match
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//  Free image memory
	free(image);

	// 2015-06-13 GEL Allocate memory for the new combined image.
	unsigned char* newImage = (unsigned char*)malloc(imageWidth * imageHeight * 4);

	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, newImage);
	if (glGetError()) {
		free(mask);
		free(newImage);

		Fatal("Error in glGetTexImage %s %dx%d\n", file, imageWidth, imageHeight);
	}

	// 2015-06-13 GEL Go through and update the alpha channel in the newImage 
	// given the red channel of the mask (which assumes that all channels are 
	// the same.)
	for (int row = 0; row < imageHeight; row++) {
		for (int column = 0; column < imageWidth; column++) {
			*(
				newImage				// base pointer position
				+ row * imageWidth * 4	// skip over number of rows (each "entry" is a 4 byte pixel)
				+ column * 4			// skip to the start of this pixel
				+ 3						// access the alpha part
				) = *(
				mask					// mask base position
				+ row * imageWidth * 3	// skip over  rows (each "entry" is a 3 byte pixel)
				+ column * 3			// skip to start of this pixel
				+ 0						// access the R part.
				);
		}
	}

	// 2015-06-13 GEL Recopy the newly created image back into memory.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, newImage);
	if (glGetError())
		Fatal("Error in glTexImage2D %s %dx%d\n", file, imageWidth, imageHeight);

	// already freed image.
	free(mask);
	free(newImage);

	//  Return texture name
	return texture;
}

#define LEN 8192  //  Maximum amount of text
void Print(const char* format, ...)
{
	char    buf[LEN]; // Text storage
	char*   ch = buf;   // Text pointer
	//  Create text to be display
	va_list args;
	va_start(args, format);
	vsnprintf(buf, LEN, format, args);
	va_end(args);
	//  Display text string
	while (*ch)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *ch++);
}

void PrintProgramLog(int obj)
{
	int len = 0;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &len);
	if (len>1)
	{
		int n = 0;
		char* buffer = (char *)malloc(len);
		if (!buffer) Fatal("Cannot allocate %d bytes of text for program log\n", len);
		glGetProgramInfoLog(obj, len, &n, buffer);
		fprintf(stderr, "%s\n", buffer);
	}

	glGetProgramiv(obj, GL_LINK_STATUS, &len);
	if (!len)
		Fatal("Error linking program\n");
}

void PrintShaderLog(int obj, char* file)
{
	int len = 0;
	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &len);
	if (len>1)
	{
		int n = 0;
		char* buffer = (char *)malloc(len);
		if (!buffer) Fatal("Cannot allocate %d bytes of text for shader log\n", len);
		glGetShaderInfoLog(obj, len, &n, buffer);
		fprintf(stderr, "%s:\n%s\n", file, buffer);
		free(buffer);
	}

	glGetShaderiv(obj, GL_COMPILE_STATUS, &len);

	if (!len)
		Fatal("Error compiling %s\n", file);
}

char* ReadText(char *file)
{
	int   n;
	char* buffer;
	
	//  Open file
	FILE* f = fopen(file, "rt");
	if (!f) 
		Fatal("Cannot open text file %s\n", file);

	//  Seek to end to determine size, then rewind
	fseek(f, 0, SEEK_END);
	n = ftell(f);
	rewind(f);

	//  Allocate memory for the whole file
	buffer = (char*)malloc(n + 1);
	if (!buffer) 
		Fatal("Cannot allocate %d bytes for text file %s\n", n + 1, file);

	//  Snarf the file
	if (fread(buffer, n, 1, f) != 1) 
		Fatal("Cannot read %d bytes for text file %s\n", n, file);

	buffer[n] = 0;

	//  Close and return
	fclose(f);
	
	return buffer;
}