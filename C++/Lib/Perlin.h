#pragma once
#include <random>

class Perlin {
	static unsigned char _defaultPermutationTable[256];

	unsigned char _permutationTable[512];

	double Fade(double t);
	double GradientDotProduction(int hash, double x, double y);
	double GradientDotProduction(int hash, double x);
	double Lerp(double a, double b, double x);

public:
	double Noise(double x);
	double Noise(double x, double y);

	Perlin();
};