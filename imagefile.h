#if !defined IMAGEFILE_H
#define IMAGEFILE_H

#include <string>

#include "image.h"
#include "types.h"


//PGM
bool readPGM(const std::string& inFilename, image<byte>& img);
bool writePGM(const std::string& outFilename, const image<byte>& img, bool binary);

//PPM
bool readPPM(const std::string& inFilename, image<vec3b>& img);
bool writePPM(const std::string& outFilename, const image<vec3b>& img, bool binary);

#endif // IMAGEFILE_H