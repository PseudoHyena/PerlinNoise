#include "Perlin.h"

float Perlin::Noise2D(float x, float y){
	return 0;
}

float Perlin::Lerp(float node0, float node1, float weight){
	return ((float)1.0 - weight) * node0 + weight * node1;
}

Perlin::gradientVectors Perlin::GetPseudoRandomGradientVector(int floor_x, int floor_y){

}

float Perlin::DotGridGradient(int floor_x, int floor_y, float x, float y){
	//grad vect

	float distance_x = x - (float)floor_x;
	float distance_y = y - (float)floor_y;

	return 0;
}

Perlin::Perlin()
{
}


Perlin::~Perlin()
{
}
