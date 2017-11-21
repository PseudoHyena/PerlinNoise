#include <iostream>
#include <algorithm>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include "Perlin.h"

#define MAP_SIZE 256
#define DISPLAY_MODE 2 //0 - num, 1 - NoiseMap, 2 - ColorMap 

double InverseLerp(double a, double b, double value) {
	if ((double)a != (double)b) {
		return std::clamp((((double)value - (double)a) / ((double)b - (double)a)), 0.0, 1.0);
	}
}

struct Color {
	unsigned char R;
	unsigned char G;
	unsigned char B;
};

struct TerrainType {
	std::string name;
	double height;
	Color color;
};

std::vector<TerrainType> regions;

void addRegions() {
	TerrainType region;
	region.name = "DeepOcean";
	region.height = 0.3;
	region.color.R = 7;
	region.color.G = 91;
	region.color.B = 184;
	regions.push_back(region);

	region.name = "Water";
	region.height = 0.4;
	region.color.R = 10;
	region.color.G = 113;
	region.color.B = 226;
	regions.push_back(region);

	region.name = "Sand";
	region.height = 0.45;
	region.color.R = 237;
	region.color.G = 239;
	region.color.B = 135;
	regions.push_back(region);

	region.name = "Grass";
	region.height = 0.55;
	region.color.R = 114;
	region.color.G = 233;
	region.color.B = 84;
	regions.push_back(region);

	region.name = "Grass2";
	region.height = 0.6;
	region.color.R = 115;
	region.color.G = 231;
	region.color.B = 86;
	regions.push_back(region);

	region.name = "Rock";
	region.height = 0.7;
	region.color.R = 106;
	region.color.G = 92;
	region.color.B = 89;
	regions.push_back(region);

	region.name = "Rock2";
	region.height = 0.9;
	region.color.R = 90;
	region.color.G = 89;
	region.color.B = 88;
	regions.push_back(region);

	region.name = "Snow";
	region.height = 1.0;
	region.color.R = 239;
	region.color.G = 248;
	region.color.B = 248;
	regions.push_back(region);
}

void getMap(double** noiseMap, int offsetY, int offsetX) {
	int scale = 76;
	int octaves = 4;
	double lacunarity = 3;
	double persistance = 0.51;

	bool setMaxMin = false;
	double maxNoiseHeight = 0;
	double minNoiseHeight = 0;

	Perlin perlin;

	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < 256; ++x) {
			double amplitude = 1;
			double frequency = 1;
			double noiseHeight = 0;

			for (int i = 0; i < octaves; ++i) {
				double sampleX = (double)(x + offsetX) / scale * frequency;
				double sampleY = (double)(y + offsetY) / scale * frequency;

				double perlinValue = perlin.Noise2D(sampleX, sampleY);
				noiseHeight += perlinValue * amplitude;

				if (setMaxMin == false) {
					maxNoiseHeight = noiseHeight;
					minNoiseHeight = noiseHeight;
					setMaxMin = true;
				}

				amplitude *= persistance;
				frequency *= lacunarity;
			}

			if (noiseHeight > maxNoiseHeight) {
				maxNoiseHeight = noiseHeight;
			}
			else if (noiseHeight < minNoiseHeight) {
				minNoiseHeight = noiseHeight;
			}

			noiseMap[y][x] = noiseHeight;
		}
	}

	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < MAP_SIZE; ++x) {
			noiseMap[y][x] = InverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[y][x]);
		}
	}
}

int main() {
	HWND hwnd;
	char Title[1024];
	GetConsoleTitle(Title, 1024);
	hwnd = FindWindow(NULL, Title);
	RECT rc;
	GetClientRect(hwnd, &rc);
	int iWidth = rc.right;
	int iHeight = rc.bottom;
	HDC hdc = GetDC(hwnd);

	////////////////////////////////////////
	
	double** noiseMap = new double*[MAP_SIZE];
	for (int i = 0; i < MAP_SIZE; ++i) {
		noiseMap[i] = new double[MAP_SIZE];
	}

	int offsetX = 0;
	int offsetY = 0;
	int step = 5;

#if DISPLAY_MODE == 0

	for (int y = 0; y < MAP_SIZE; ++y) {
		for (int x = 0; x < MAP_SIZE; ++x) {
			std::cout << noiseMap[y][x] << std::endl;
		}
	}

#elif DISPLAY_MODE == 1
	int button = 0;

	while (true) {
		getMap(noiseMap, offsetY, offsetX);
		for (int y = 0; y < MAP_SIZE; ++y) {
			for (int x = 0; x < MAP_SIZE; ++x) {
				SetPixel(hdc, x, y, RGB(int(noiseMap[y][x] * 255), int(noiseMap[y][x] * 255), int(noiseMap[y][x] * 255)));
			}
		}

		bool breakFlag = false;

		if (_kbhit()) {
			button = _getch();

			switch (button)
			{
			case 'a':
				offsetX -= step;
				break;
			case 'w':
				offsetY -= step;
				break;
			case 'd':
				offsetX += step;
				break;
			case 's':
				offsetY += step;
				break;
			case 'z':
				breakFlag = true;
				break;
			}
		}

		if (breakFlag == true) {
			break;
		}
	}

#elif DISPLAY_MODE == 2
	int button = 0;
	addRegions();

	while (true) {
		getMap(noiseMap, offsetY, offsetX);

		for (int y = 0; y < MAP_SIZE; ++y) {
			for (int x = 0; x < MAP_SIZE; ++x) {
				double currentHeight = noiseMap[y][x];
				
				for (int i = 0; i <= regions.size(); ++i) {
					if (currentHeight <= regions[i].height) {
						SetPixel(hdc, x, y, RGB(regions[i].color.R, regions[i].color.G, regions[i].color.B));
						break;
					}
				}
			}
		}

		bool breakFlag = false;

		if (_kbhit()) {
			button = _getch();

			switch (button)
			{
			case 'a':
				offsetX += -1;
				break;
			case 'w':
				offsetY += -1;
				break;
			case 'd':
				offsetX += 1;
				break;
			case 's':
				offsetY += 1;
				break;
			case 'z':
				breakFlag = true;
				break;
			}
		}

		if (breakFlag == true) {
			break;
		}
	}

#endif

	for (int i = 0; i < MAP_SIZE; ++i) {
		delete[] noiseMap[i];
	}

	delete[] noiseMap;
}