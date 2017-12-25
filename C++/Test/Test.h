#pragma once
#include "PerlinLib.h"
#include <string>
#include <vector>
#include <Windows.h>

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

class Test {

	double* map_1D = nullptr;
	double** map_2D = nullptr;

	size_t mapWidth_1D = 256;

	size_t mapWidth_2D = 256;
	size_t mapHeight_2D = 256;

	std::vector<TerrainType> defaultRegions;

	NoiseGeneration::Perlin perlin;
	void AddDefaultRegions();
public:

	void Test_1D_Noise(HDC& hdc, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector1 offset);
	void Test_1D_Color(HDC& hdc, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector1 offset);
	void Test_1D_Color(HDC& hdc, std::vector<TerrainType> regions, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector1 offset);

	void Test_2D_Noise(HDC& hdc, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector2 offset);
	void Test_2D_Color(HDC& hdc, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector2 offset);
	void Test_2D_Color(HDC& hdc, std::vector<TerrainType> regions, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, NoiseGeneration::Vector2 offset);

	Test(); 
	Test(size_t mapWidth);
	Test(size_t mapWidth, size_t mapHeight);

	~Test();
};

