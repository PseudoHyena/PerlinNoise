#pragma once

namespace NoiseGeneration {

	struct Vector2 {
		double x;
		double y;

		Vector2() = default;
		Vector2(double _x, double _y);
	};

	struct Vector1 {
		double x;

		Vector1() = default;
		Vector1(double _x);
	};

	class Perlin {

		double Clamp(double value, double a, double b);

		double InverseLerp(double a, double b, double value);

		static unsigned char _defaultPermutationTable[256];

		unsigned char _permutationTable[512];

		double Fade(double t);
		double GradientDotProduction(int hash, double x, double y);
		double GradientDotProduction(int hash, double x);
		double Lerp(double a, double b, double x);

	public:
		double Noise(double x);
		void Noise(double* map, size_t mapWidth, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, Vector1 offset);
		
		double Noise(double x, double y);
		void Noise(double** map, size_t mapWidth, size_t mapHeight, unsigned int seed, double scale, int octaves, double persistance, double lacunarity, Vector2 offset);

		Perlin();
	};
}