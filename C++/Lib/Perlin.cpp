#include "Perlin.h"

unsigned char Perlin::_defaultPermutationTable[256] = { 151, 160, 137, 91, 90, 15,
131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180 };

double Perlin::Fade(double t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

double Perlin::GradientDotProduction(int hash, double x) {
	switch (hash & 0x1)
	{
	case 0x0: return  x;
	case 0x1: return -x;
	default: return 0;
	}
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

double Perlin::Noise(double x){
	int x_int = (int)x & 255;

	double distanceX = x - (int)x;

	double smoothX = Fade(distanceX);

	int a, b;
	a = _permutationTable[_permutationTable[x_int]];
	b = _permutationTable[_permutationTable[x_int + 1]];

	double x1;
	x1 = Lerp(GradientDotProduction(a, distanceX),
		GradientDotProduction(b, distanceX - 1), smoothX);

	return (x1 + 1) / 2;
}

double Perlin::Noise(double x, double y) {
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

Perlin::Perlin()
{
	for (int i = 0; i < 256; ++i) {
		_permutationTable[i] = _defaultPermutationTable[i & 255];
	}
}
