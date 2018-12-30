#include <string>
#include <fstream>
#include <iostream>

#include "imagefile.h"

using namespace std;

bool readPGM(const std::string& inFilename, image<byte>& img) {

	ifstream is(inFilename, ios::binary);
	if (!is)
		return false;

	string magic;
	bool binary;
	getline(is, magic);
	if (magic == "P5")
		binary = true;
	else if (magic == "P2")
		binary = false;
	else
		return false;

	if (is.peek() == '#') {
		string comment;
		getline(is, comment);
		if (comment[0] != '#' || comment.empty())
			return false;
	}

	unsigned w, h, maxval;
	is >> w >> h >> maxval;

	if (maxval != 255)
		return false;

	//ultimo LF
	is.get();

	img.resize(w, h);

	if (binary == true) {
		is.read(img.data(), img.data_size());
		return is.good();
	}

	else if (binary == false) {
		for (unsigned y = 0; y < img.height(); ++y) {
			for (unsigned x = 0; x < img.width(); ++x) {
				unsigned val;
				if (!(is >> val) || val > 255)
					return false;
				img(x, y) = static_cast<byte>(val);
			}
		}
		return true;
	}

}
bool writePGM(const std::string& outFilename, const image<byte>& img, bool binary) {

	ofstream os(outFilename, ios::binary);
	if (!os)
		return false;

	os << ((binary == true) ? "P5\n" : "P2\n");
	os << "#PGM creato nel corso di Sdem\n";
	os << img.width() << " " << img.height() << "\n";
	os << "255\n";

	if (binary == true) {
		os.write(img.data(), img.data_size());
		return os.good();
	}

	else if (binary == false) {
		for (unsigned y = 0; y < img.height(); ++y) {
			for (unsigned x = 0; x < img.width(); ++x) {
				os << static_cast<int>(img(x, y)) << " ";
			}
			os << "\n";
		}
		return true;
	}
}

bool readPPM(const std::string& inFilename, image<vec3b>& img) {

	ifstream is(inFilename, ios::binary);
	if (!is)
		return false;

	string magic;
	bool binary;
	if (!getline(is, magic))
		return false;

	if (magic == "P6")
		binary = true;
	else if (magic == "P3")
		binary = false;
	else
		return false;

	if (is.peek() == '#') {
		string comment;
		if (!getline(is, comment))
			return false;
		if (comment.empty() || comment[0] != '#')
			return false;
	}

	unsigned w, h, maxval;
	is >> w >> h >> maxval;

	if (maxval != 255)
		return false;

	is.get();

	img.resize(w, h);

	if (binary == true) {
		is.read(img.data(), img.data_size());
		return is.good();
	}

	else if (binary == false) {
		for (unsigned y = 0; y < img.height(); ++y) {
			for (unsigned x = 0; x < img.width(); ++x) {
				unsigned val;
				if (!(is >> val) || (val>255))
					return false;
				img(x, y)[0] = static_cast<byte>(val);
				if (!(is >> val) || (val>255))
					return false;
				img(x, y)[1] = static_cast<byte>(val);
				if (!(is >> val) || (val>255))
					return false;
				img(x, y)[2] = static_cast<byte>(val);
			}
		}
		return true;
	}
}
bool writePPM(const std::string& outFilename, const image<vec3b>& img, bool binary) {

	ofstream os(outFilename, ios::binary);
	if (!os)
		return false;

	os << ((binary == true) ? "P6\n" : "P3\n");
	os << "#PPM creato nel corso di sdem!\n";
	os << img.width() << " " << img.height() << "\n";
	os << "255\n";

	if (binary) {
		os.write(img.data(), img.data_size());
		return os.good();
	}

	else {
		for (unsigned y = 0; y < img.height(); ++y) {
			os << static_cast<int>(img(0, y)[0]) << " " << static_cast<int>(img(0, y)[1]) << " " << static_cast<int>(img(0, y)[2]);
			for (unsigned x = 1; x < img.width(); ++x) {
				os << " " << static_cast<int>(img(x, y)[0]) << " " << static_cast<int>(img(x, y)[1]) << " " << static_cast<int>(img(x, y)[2]);
			}
			os << "\n";
		}
		return false;
	}

}

