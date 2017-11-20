#include <iostream>
#include <algorithm>
#include "Perlin.h"

double InverseLerp(double a, double b, double value) {
	if ((double)a != (double)b) {
		return std::clamp((((double)value - (double)a) / ((double)b - (double)a)), 0.0, 1.0);
	}
}

int main() {
	int scale = 76;
	int octaves = 4;
	double lacunarity = 3;
	double persistance = 0.51;

	bool setMaxMin = false;
	double maxNoiseHeight = 0;
	double minNoiseHeight = 0;

	double noiseMap[256][256];

	Perlin perlin;

	for (int y = 0; y < 256; ++y) {
		for (int x = 0; x < 256; ++x) {
			double amplitude = 1;
			double frequency = 1;
			double noiseHeight = 0;

			for (int i = 0; i < octaves; ++i) {
				double sampleX = x / scale * frequency;
				double sampleY = y / scale * frequency;

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

	for (int y = 0; y < 256; ++y) {
		for (int x = 0; x < 256; ++x) {
			noiseMap[y][x] = InverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[y][x]);
		}
	}

	system("pause");
}