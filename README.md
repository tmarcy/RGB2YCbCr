# RGB2YCbCr
Basic software written in C++11 about multimedia processing.

## What the program has to do

* upload an image in PPM format according to specifications in Reference section ("rana.ppm" is used in this exercise);
* convert RGB in YCbCr according to specifications in Reference section and then save the new image converted in YCbCr in a binary file "YCBCR.bin", which contains Y, Cb,Cr pixel values in rows represented which one with exactly 1 byte (it means 3 byte for 1 pixel as RGB);
* in order to obtain Chroma subsampling from 4:4:4 to 4:2:2, downsample chrominance (Cr, Cb) with a factor of 2 and then save the results in 3 binary files respectively "Y.bin", "CB.bin", CR.bin;
* recreate previous image and then save it in a binary file "YCBCR_REC.bin", which contains Y, Cb,Cr pixel values in rows represented which one with exactly 1 byte (it means 3 byte for 1 pixel as RGB);
* convert YCbCr in RGB, according to specifications in Reference section and then save the new image converted in RGB in PPm format, according to specifications in Reference section.

## Reference
* [PPM Format Specifications](http://netpbm.sourceforge.net/doc/ppm.html)
* [JPEG File Interchange Format](https://www.w3.org/Graphics/JPEG/jfif3.pdf)

## Built With

* [Microsof Visual Studio](https://visualstudio.microsoft.com/) - IDE used
* [Irfan View](https://www.irfanview.com/) - Graphic Viewer used

## Author

**Marcella Tincani** - [Marcella](https://github.com/tmarcy)
