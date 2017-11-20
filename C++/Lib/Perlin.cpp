#include "Perlin.h"

double Perlin::Fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double Perlin::GradientDotProduction(int hash, double x, double y) {
	switch (hash & 0x3)
	{
	case 0x0: return  x;
	case 0x1: return -x;
	case 0x2: return  y;
	case 0x3: return -y;
	default: return 0;
	}
}

double Perlin::Lerp(double a, double b, double x) {
	return a + x * (b - a);
}

double Perlin::Noise2D(double x, double y) {
	int x_int = (int)x & 255;
	int y_int = (int)y & 255;

	double distanceX = x - (int)x;
	double distanceY = y - (int)y;

	double smoothX = Fade(distanceX);
	double smoothY = Fade(distanceY);

	int aa, ab, ba, bb;
	aa = _permutationTable[_permutationTable[x_int] + y_int];
	ab = _permutationTable[_permutationTable[x_int] + y_int + 1];
	ba = _permutationTable[_permutationTable[x_int + 1] + y_int];
	bb = _permutationTable[_permutationTable[x_int + 1] + y_int + 1];

	double x1, x2;
	x1 = Lerp(GradientDotProduction(aa, distanceX, distanceY),           
		GradientDotProduction(ba, distanceX - 1, distanceY),            
		smoothX);                                 
	x2 = Lerp(GradientDotProduction(ab, distanceX, distanceY - 1),          
		GradientDotProduction(bb, distanceX - 1, distanceY - 1),      
		smoothX);

	return (Lerp(x1, x2, smoothY) + 1) / 2;
}

Perlin::Perlin() {
	for (int i = 0; i < 256; ++i) {
		_permutationTable[i] = _defaultPermutationTable[i & 255];
	}
}
