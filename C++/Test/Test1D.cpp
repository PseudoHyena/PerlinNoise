#include <iostream>
#include <algorithm>
#include <vector>
#include <conio.h>
#include <Windows.h>
#include "Perlin.h"

#define MAP_SIZE 256
#define DISPLAY_Y_OFFSET 150
#define MULTIPLY 100
#define DISPLAY_MODE 2 //0 - num, 1 - NoiseMap, 2 - ColorMap 

double clamp(double value, double a, double b){
	if (value < a){
		return a;
	}
	else if (value > b){
		return b;
	}
	else {
		return value;
	}
}

double InverseLerp(double a, double b, double value) {
	if ((double)a != (double)b) {
		return clamp((((double)value - (double)a) / ((double)b - (double)a)), 0.0, 1.0);
	}
	else{
		return 0;
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

double maxNoiseHeight = -100000;
double minNoiseHeight = 100000;
bool firstSetMaxMinHeight = true;
bool isParCh = false;

void getMap(double* noiseMap, int offsetX, int offsetY, int& offsetScale, int& offsetOctaves, double& offsetLacunarity, double& offsetPersistance) {
	if (isParCh == true){
		firstSetMaxMinHeight = true;
	}

	int scale = 76 + offsetScale; //best 76
	int octaves = 4 + offsetOctaves; // best 4
	double lacunarity = 3 + offsetLacunarity; // best 3
	double persistance = 0.1 + offsetPersistance; // best .1

	if (scale <= 0){
		scale = 1;
		offsetScale += 1;
	}

	if (octaves <= 0){
		octaves = 1;
		offsetOctaves += 1;
	}

	Perlin perlin;

	for (int x = 0; x < MAP_SIZE; ++x) {
		double amplitude = 1;
		double frequency = 1;
		double noiseHeight = 0;

		for (int i = 0; i < octaves; ++i) {
			double sampleX = (double)(x + offsetX) / scale * frequency;

			double perlinValue = perlin.Noise(sampleX);
			noiseHeight += perlinValue * amplitude;

			amplitude *= persistance;
			frequency *= lacunarity;
		}

		if (firstSetMaxMinHeight == true){
			if (noiseHeight > maxNoiseHeight) {
				maxNoiseHeight = noiseHeight;
			}
			else if (noiseHeight < minNoiseHeight) {
				minNoiseHeight = noiseHeight;
			}
		}

		noiseMap[x] = noiseHeight;
	}

	for (int x = 0; x < MAP_SIZE; ++x) {
		noiseMap[x] = InverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[x]);
	}

	firstSetMaxMinHeight = false;
	isParCh = false;
}

void clearDisplay(HDC hdc){
	for (int y = 0; y < MAP_SIZE + 1; ++y){
		for (int x = 0; x < MAP_SIZE + 1; ++x){
			SetPixel(hdc, x, y, RGB(0, 0, 0));
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

	double* noiseMap = new double[MAP_SIZE];

	int offsetX = 0;
	int offsetY = 0;
	int offsetScale = 0;
	int offsetOctaves = 0;
	double offsetLacunarity = 0;
	double offsetPersistance = 0;
	int step = 5;

#if DISPLAY_MODE == 0
	getMap(noiseMap, offsetX, offsetY, offsetScale, offsetOctaves, offsetLacunarity, offsetPersistance);
	for (int x = 0; x < MAP_SIZE; ++x) {
		std::cout << noiseMap[y][x] << std::endl;
	}

#elif DISPLAY_MODE == 1
	int button = 0;

	while (true) {
		getMap(noiseMap, offsetX, offsetY, offsetScale, offsetOctaves, offsetLacunarity, offsetPersistance);
		for (int x = 0; x < MAP_SIZE; ++x) {
			SetPixel(hdc, x, iHeight - int(noiseMap[x] * MULTIPLY) - DISPLAY_Y_OFFSET, RGB(255, 255, 255));
		}

		bool breakFlag = false;

		if (_kbhit()) {
			button = _getch();

			switch (button)
			{
			case 'a':
				offsetX -= step;
				clearDisplay(hdc);
				break;
			case 'w':
				offsetY -= step;
				clearDisplay(hdc);
				break;
			case 'd':
				offsetX += step;
				clearDisplay(hdc);
				break;
			case 's':
				offsetY += step;
				clearDisplay(hdc);
				break;
			case 'o':
				offsetOctaves -= 1;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'O':
				offsetOctaves += 1;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'p':
				offsetPersistance -= 0.01;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'P':
				offsetPersistance += 0.01;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'l':
				offsetLacunarity -= 0.01;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'L':
				offsetLacunarity += 0.01;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'm':
				offsetScale -= 1;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'M':
				offsetScale += 1;
				isParCh = true;
				clearDisplay(hdc);
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
		getMap(noiseMap, offsetX, offsetY, offsetScale, offsetOctaves, offsetLacunarity, offsetPersistance);

		for (int x = 0; x < MAP_SIZE; ++x) {
			double currentHeight = noiseMap[x];

			for (int i = 0; i < regions.size(); ++i) {
				if (currentHeight <= regions[i].height) {
					SetPixel(hdc, x, iHeight - (currentHeight * MULTIPLY) - DISPLAY_Y_OFFSET, RGB(regions[i].color.R, regions[i].color.G, regions[i].color.B));
					break;
				}
			}
		}

		bool breakFlag = false;

		if (_kbhit()) {
			button = _getch();

			switch (button)
			{
				offsetX -= step;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'w':
				offsetY -= step;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'd':
				offsetX += step;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 's':
				offsetY += step;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'o':
				offsetOctaves -= 1;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'O':
				offsetOctaves += 1;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'p':
				offsetPersistance -= 0.01;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'P':
				offsetPersistance += 0.01;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'l':
				offsetLacunarity -= 0.01;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'L':
				offsetLacunarity += 0.01;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'm':
				offsetScale -= 1;
				isParCh = true;
				clearDisplay(hdc);
				break;
			case 'M':
				offsetScale += 1;
				isParCh = true;
				clearDisplay(hdc);
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

	std::cout << "offsetScale: " << offsetScale << std::endl;
	std::cout << "offsetOctaves: " << offsetOctaves << std::endl;
	std::cout << "offsetLacurarity: " << offsetLacunarity << std::endl;
	std::cout << "offsetPersistance: " << offsetPersistance << std::endl;

	system("pause");

	delete[] noiseMap;
}