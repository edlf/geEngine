#ifndef _IMAGETOOLS_H_
#define _IMAGETOOLS_H_

#include <string>

unsigned char * reverseImage(unsigned char* data, const int width, const int height, const int channels);

void takeSnapshot(const int width, const int height);

unsigned char *loadRGBImage(const std::string& filename, int *w, int *h);

#endif
