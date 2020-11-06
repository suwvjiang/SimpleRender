#include "TGAImage.h"
#include <iostream>
#include <fstream>
#include <cstring>

TGAImage::TGAImage()
	:data(), width(0), height(0), bytespp(0) {}
TGAImage::TGAImage(const int w, const int h, const int bpp)
	: data(w* h* bpp, 0), width(w), height(h), bytespp(bpp) {}

bool TGAImage::read_tga_file(const std::string fileName)
{
	std::ifstream in;
	in.open(fileName, std::ios::binary);
	if (!in.is_open())
	{
		std::cerr << "can't open file " << fileName << "\n";
		in.close();
		return false;
	}

	TGA_Header header;
	in.read(reinterpret_cast<char*>(&header), sizeof(header));
	if (!in.good())
	{
		in.close();
		std::cerr << "an error cocured while reading the header\n";
		return false;
	}

	width = header.width;
	height = header.height;
	bytespp = header.bitsperpixel >> 3;
	if ( width <= 0 || height<= 0 || 
		(bytespp != GRAYSCALE && bytespp != RGB && bytespp != RGBA))
	{
		in.close();
		std::cerr << "bad bpp(or width/height) value\n";
		return false;
	}

	size_t nbytes = bytespp * width * height;
	data = std::vector<std::uint8_t>(nbytes, 0);
	if (header.datatypecode == 3 || header.datatypecode == 2)
	{
		in.read(reinterpret_cast<char*>(data.data()), nbytes);
		if (!in.good())
		{
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else if (header.datatypecode == 10 || header.datatypecode == 11)
	{
		if (!load_rle_data(in))
		{
			in.close();
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
	}
	else
	{
		in.close();
		std::cerr << "unkown file format " << (int)header.datatypecode << "\n";
		return false;
	}

	if (!(header.imagedescriptor & 0x20))
		flip_vertically();
	if (header.datatypecode & 0x10)
		flip_horizontally();

	std::cerr << width << "x" << height << "/" << bytespp * 8 << "\n";
	in.close();
	return true;
}

bool TGAImage::load_rle_data(std::ifstream& in)
{
	size_t pixelCount = width * height;
	size_t currentPixel = 0;
	size_t currentByte = 0;
	TGAColor colorBuffer;

	do 
	{
		std::uint8_t chunkheader = 0;
		chunkheader = in.get();
		if (!in.good())
		{
			std::cerr << "an error occured while reading the data\n";
			return false;
		}
		if (chunkheader < 128)
		{
			chunkheader++;
			for (int i = 0; i < chunkheader; ++i)
			{
				in.read(reinterpret_cast<char*>(colorBuffer.bgra), bytespp);
				if (!in.good())
				{
					std::cerr << "an error occured while reading the header\n";
					return false;
				}
			}
		} 
		else
		{
		}
	} while (currentPixel < pixelCount);
}