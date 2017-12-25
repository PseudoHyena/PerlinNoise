#include "Test.h"
#include <Windows.h>

void Test::AddDefaultRegions() {
	TerrainType region;
	region.name = "DeepOcean";
	region.height = 0.3;
	region.color.R = 7;
	region.color.G = 91;
	region.color.B = 184;
	defaultRegions.push_back(region);

	region.name = "Water";
	region.height = 0.4;
	region.color.R = 10;
	region.color.G = 113;
	region.color.B = 226;
	defaultRegions.push_back(region);

	region.name = "Sand";
	region.height = 0.45;
	region.color.R = 237;
	region.color.G = 239;
	region.color.B = 135;
	defaultRegions.push_back(region);

	region.name = "Grass";
	region.height = 0.55;
	region.color.R = 114;
	region.color.G = 233;
	region.color.B = 84;
	defaultRegions.push_back(region);

	region.name = "Grass2";
	region.height = 0.6;
	region.color.R = 115;
	region.color.G = 231;
	region.color.B = 86;
	defaultRegions.push_back(region);

	region.name = "Rock";
	region.height = 0.7;
	region.color.R = 106;
	region.color.G = 92;
	region.color.B = 89;
	defaultRegions.push_back(region);

	region.name = "Rock2";
	region.height = 0.9;
	region.color.R = 90;
	region.color.G = 89;
	region.color.B = 88;
	defaultRegions.push_back(region);

	region.name = "Snow";
	region.height = 1.0;
	region.color.R = 239;
	region.color.G = 248;
	region.color.B = 248;
	defaultRegions.push_back(region);
}

Test::Test() {
	AddDefaultRegions();

	map_1D = new double[mapWidth_1D];

	map_2D = new double*[mapHeight_2D];
	for (int i = 0; i < mapHeight_2D; ++i) {
		map_2D[i] = new double[mapWidth_2D];
	}
}

Test::Test(size_t mapWidth) : Test::Test(){
	mapWidth_1D = mapWidth;
}

Test::Test(size_t mapWidth, size_t mapHeight) : Test::Test() {
	mapWidth_2D = mapWidth;
	mapHeight_2D = mapHeight;
}

Test::~Test() {
	delete[] map_1D;

	for (int i = 0; i < mapHeight_2D; ++i) {
		delete[] map_2D[i];
	}

	delete[] map_2D;
}

void Test::Test_1D_Noise(HDC& hdc, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector1 offset) {
	perlin.Noise(map_1D, mapWidth_1D, seed, scale, octaves, persistance, lacunarity, offset);
	perlin.Noise(map_1D, mapWidth_1D, seed, scale, octaves, persistance, lacunarity, offset);

	for (int x = 0; x < mapWidth_1D; ++x) {
		SetPixel(hdc, x, 350 - int(map_1D[x] * 100) - 150, RGB(255, 255, 255));
	}
}

void Test::Test_1D_Color(HDC& hdc, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector1 offset) {
	perlin.Noise(map_1D, mapWidth_1D, seed, scale, octaves, persistance, lacunarity, offset);

	for (int x = 0; x < mapWidth_1D; ++x) {
		double currentHeight = map_1D[x];

		for (int i = 0; i < defaultRegions.size(); ++i) {
			if (currentHeight <= defaultRegions[i].height) {
				SetPixel(hdc, x, 350 - (currentHeight * 100) - 150, RGB(defaultRegions[i].color.R, defaultRegions[i].color.G, defaultRegions[i].color.B));
				break;
			}
		}
	}
}

void Test::Test_1D_Color(HDC& hdc, std::vector<TerrainType> regions, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector1 offset) {
	perlin.Noise(map_1D, mapWidth_1D, seed, scale, octaves, persistance, lacunarity, offset);

	for (int x = 0; x < mapWidth_1D; ++x) {
		double currentHeight = map_1D[x];

		for (int i = 0; i < regions.size(); ++i) {
			if (currentHeight <= regions[i].height) {
				SetPixel(hdc, x, 350 - (currentHeight * 100) - 150, RGB(regions[i].color.R, regions[i].color.G, regions[i].color.B));
				break;
			}
		}
	}
}


void Test::Test_2D_Noise(HDC& hdc, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector2 offset) {
	perlin.Noise(map_2D, mapWidth_2D, mapHeight_2D, seed, scale, octaves, persistance, lacunarity, offset);

	for (int y = 0; y < mapHeight_2D; ++y) {
		for (int x = 0; x < mapWidth_2D; ++x) {
			SetPixel(hdc, x, y, RGB(int(map_2D[y][x] * 255), int(map_2D[y][x] * 255), int(map_2D[y][x] * 255)));
		}
	}
}

void Test::Test_2D_Color(HDC& hdc, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector2 offset) {
	perlin.Noise(map_2D, mapWidth_2D, mapHeight_2D, seed, scale, octaves, persistance, lacunarity, offset);

	for (int y = 0; y < mapHeight_2D; ++y) {
		for (int x = 0; x < mapWidth_2D; ++x) {
			double currentHeight = map_2D[y][x];

			for (int i = 0; i < defaultRegions.size(); ++i) {
				if (currentHeight <= defaultRegions[i].height) {
					SetPixel(hdc, x, y, RGB(defaultRegions[i].color.R, defaultRegions[i].color.G, defaultRegions[i].color.B));
					break;
				}
			}
		}
	}
}

void Test::Test_2D_Color(HDC& hdc, std::vector<TerrainType> regions, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector2 offset) {
	perlin.Noise(map_2D, mapWidth_2D, mapHeight_2D, seed, scale, octaves, persistance, lacunarity, offset);
	
	for (int y = 0; y < mapHeight_2D; ++y) {
		for (int x = 0; x < mapWidth_2D; ++x) {
			double currentHeight = map_2D[y][x];

			for (int i = 0; i < regions.size(); ++i) {
				if (currentHeight <= regions[i].height) {
					SetPixel(hdc, x, y, RGB(regions[i].color.R, regions[i].color.G, regions[i].color.B));
					break;
				}
			}
		}
	}
}