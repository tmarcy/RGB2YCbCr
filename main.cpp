#include <fstream>
#include <string>
#include <iostream>

#include "imagefile.h"

using namespace std;

void error(const string& msg){
	cout << msg << endl;
	exit(EXIT_FAILURE);
}

void saturate(byte& val, int min, int max) {
	if (val < min)
		val = min;
	else if (val > max)
		val = max;

}

image<byte> reduce(const image<byte>& img) {
	unsigned w2 = img.width() / 2;
	unsigned h2 = img.height() / 2;
	unsigned w2e = (img.width() + 1) / 2;
	unsigned h2e = (img.height() + 1) / 2;

	image<byte> iout(w2e, h2e);
	for (unsigned y = 0; y<h2; ++y) {
		for (unsigned x = 0; x < w2; ++x) {
			iout(x, y) = (img(x * 2, y * 2) + img(x * 2 + 1, y * 2) + img(x * 2, y * 2 + 1) + img(x * 2 + 1, y * 2 + 1)) / 4;
		}
	}

	//ultima riga

	if (w2 != w2e) {
		for (unsigned y = 0; y < h2; ++y) {
			iout(w2, y) = (img(w2 * 2, y * 2) + img(w2 * 2, y * 2 + 1)) / 2;
		}

	}

	// ultima colonna
	if (h2 != h2e) {
		for (unsigned x = 0; x < w2; ++x) {
			iout(x, h2) = (img(x * 2, h2 * 2) + img(x * 2 + 1, h2 * 2)) / 2;
		}

	}

	//ultimo pixel in fondo a dx
	if (w2 != w2e && h2 != h2e) {
		iout(w2, h2) = img(w2 * 2, h2 * 2);
	}

	return iout;
}

image<byte> expand(const image<byte>& img, unsigned w, unsigned h) {
	image<byte> out(w, h);

	for (unsigned y = 0; y < h; ++y) {
		for (unsigned x = 0; x < w; ++x) {
			out(x, y) = img(x / 2, y / 2);
		}
	}
	return out;
}

image<byte> expand(const image<byte>& img) {

	image<byte> out;
	unsigned w, h, w2, h2, w2e, h2e;
	w = img.width();
	h = img.height();
	w2 = w * 2;
	h2 = h * 2;

	out.resize(w2-1, h2-1);

	for (unsigned y = 0; y < h2; ++y) {
		for (unsigned x = 0; x < w2; ++x) {
			out(x, y) = img(x / 2, y / 2);
		}
	}

	return out;
}


int main(int argc, char **argv) {

	if (argc != 2)
		error("errore nel numero dei parametri!");

	string input(argv[1]);
	image<vec3b> img, img_ycbcr, ycbcr_rec, rgb;
	image<byte> ipsilon, cibi, cierre;
	
	readPPM(input, img);

	img_ycbcr.resize(img.width(), img.height());
	ipsilon.resize(img.width(), img.height());
	cibi.resize(img.width(), img.height());
	cierre.resize(img.width(), img.height());
	ycbcr_rec.resize(img.width(), img.height());
	rgb.resize(img.width(), img.height());

	for (unsigned y = 0; y < img.height(); ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			byte R, G, B;
			R = img(x, y)[0];
			G = img(x, y)[1];
			B = img(x, y)[2];

			saturate(R, 0, 255);
			saturate(G, 0, 255);
			saturate(B, 0, 255);

			byte Y, Cb, Cr;
			Y = 0.299*R + 0.587*G + 0.114*B;
			Cb = -0.1687*R - 0.3313*G + 0.5*B + 128;
			Cr = 0.5*R - 0.4187*G - 0.0813*B + 128;

			saturate(Y, 16, 235);
			saturate(Cb, 16, 240);
			saturate(Cr, 16, 240);		

			img_ycbcr(x, y)[0] = Y;
			img_ycbcr(x, y)[1] = Cb;
			img_ycbcr(x, y)[2] = Cr;

			ipsilon(x, y) = Y;
			cibi(x, y) = Cb;
			cierre(x, y) = Cr;
		}
	}

	{
		ofstream os("YCBCR.bin", ios::binary);
		if (!os)
			exit(EXIT_FAILURE);
		
		os.write(img_ycbcr.data(), img_ycbcr.data_size());
	}

	auto cibi_red = reduce(cibi);
	auto cierre_red = reduce(cierre);

	{
		ofstream os("Y.bin", ios::binary);
		if (!os)
			return false;

		os.write(ipsilon.data(), ipsilon.data_size());
	}

	{
		ofstream os("Cb.bin", ios::binary);
		if (!os)
			return false;

		os.write(cibi_red.data(), cibi_red.data_size());
	}

	{
		ofstream os("Cr.bin", ios::binary);
		if (!os)
			return false;

		os.write(cierre_red.data(), cierre_red.data_size());
	}

	auto cbrec = expand(cibi_red, img.width(), img.height());
	auto crrec = expand(cierre_red, img.width(), img.height());

	for (unsigned y = 0; y < img.height(); ++y) {
		for (unsigned x = 0; x < img.width(); ++x) {
			ycbcr_rec(x, y)[0] = ipsilon(x, y);
			ycbcr_rec(x, y)[1] = cbrec(x, y);
			ycbcr_rec(x, y)[2] = crrec(x, y);

			saturate(ycbcr_rec(x, y)[0], 16, 235);
			saturate(ycbcr_rec(x, y)[1], 16, 240);
			saturate(ycbcr_rec(x, y)[2], 16, 240);

			byte R, G, B;

			R = ycbcr_rec(x, y)[0] + 1.402*(ycbcr_rec(x, y)[2] - 128);
			G = ycbcr_rec(x, y)[0] - 0.34414* (ycbcr_rec(x, y)[1] - 128) - 0.71414 *(ycbcr_rec(x, y)[2] - 128);
			B = ycbcr_rec(x, y)[0] + 1.772*(ycbcr_rec(x, y)[1] - 128);

			saturate(R, 0, 255);
			saturate(G, 0, 255);
			saturate(B, 0, 255);

			rgb(x, y)[0] = R;
			rgb(x, y)[1] = G;
			rgb(x, y)[2] = B;

		}
	}


	{
		ofstream os("ycbcr_rec.bin", ios::binary);
		if (!os)
			return false;

		os.write(ycbcr_rec.data(), ycbcr_rec.data_size());
	}

	writePPM("output.ppm", rgb, true);

	



	

	

}