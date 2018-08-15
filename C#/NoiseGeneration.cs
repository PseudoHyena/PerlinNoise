namespace NoiseGeneration {

    public struct Vector1 {
        public float x;

        public Vector1(float _x) {
           x = _x;
        }
    }

    public struct Vector2 {
        public float x;
        public float y;

        public Vector2(float _x, float _y) {
            x = _x;
            y = _y;
        }
    }

    static class Perlin
    {
        private static float Clamp(float value, float a, float b) {
            if (value < a) {
                return a;
            }
            else if (value > b) {
                return b;
            }
            else {
                return value;
            }
        }

        private static float InverseLerp(float a, float b, float value) {
            if ((double)a != (double)b) {
                return Clamp(((value - a) / (b - a)), 0.0f, 1.0f);
            }
            else {
                return 0;
            }
        }

        private static byte[] _permutationTable = new byte[512] { 151, 160, 137, 91, 90, 15,
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
        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180, 151, 160, 137, 91, 90, 15,
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
        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180};

        private static float Fade(float t)
        {
            return t * t * t * (t * (t * 6 - 15) + 10);
        }

        private static float Lerp(float a, float b, float x)
        {
            return a + x * (b - a);
        }

        private static float GradientDotProduction(int hash, float x)
        {
            switch (hash & 0x1)
            {
                case 0x0: return x;
                case 0x1: return -x;
                default: return 0;
            }
        }

        private static float GradientDotProduction(int hash, float x, float y)
        {
            switch (hash & 0x3)
            {
                case 0x0: return x;
                case 0x1: return -x;
                case 0x2: return y;
                case 0x3: return -y;
                default: return 0;
            }
        }

        public static float Noise(float x)
        {
            if (x < 0)
            {
                x *= -1;
            }

            int x_int = (int)x & 255;

            float distanceX = x - (int)x;

            float smoothX = Fade(distanceX);

            int a, b;
            a = _permutationTable[_permutationTable[x_int]];
            b = _permutationTable[_permutationTable[x_int + 1]];

            float x1;
            x1 = Lerp(GradientDotProduction(a, distanceX),
                GradientDotProduction(b, distanceX - 1), smoothX);

            return (x1 + 1) / 2;
        }

        public static float[] Noise(int mapWidth, int seed, float scale, int octaves, float persistance, float lacunarity, Vector1 offset) {
            float[] noiseMap = new float[mapWidth];

            System.Random prng = new System.Random(seed);
            Vector1[] octaveOffsets = new Vector1[octaves];
            for (int i = 0; i < octaves; i++) {
                float offsetX = prng.Next(-100000, 100000) + offset.x;
                octaveOffsets[i] = new Vector1(offsetX);
            }

            if (scale <= 0) {
                scale = 0.0001f;
            }

            float maxNoiseHeight = float.MinValue;
            float minNoiseHeight = float.MaxValue;

            float halfWidth = mapWidth / 2f;

            for (int x = 0; x < mapWidth; x++) {

                float amplitude = 1;
                float frequency = 1;
                float noiseHeight = 0;

                for (int i = 0; i < octaves; i++) {
                    float sampleX = (x - halfWidth) / scale * frequency + octaveOffsets[i].x;

                    float perlinValue = Noise(sampleX) * 2 - 1;
                    noiseHeight += perlinValue * amplitude;

                    amplitude *= persistance;
                    frequency *= lacunarity;
                }

                if (noiseHeight > maxNoiseHeight) {
                    maxNoiseHeight = noiseHeight;
                }
                else if (noiseHeight < minNoiseHeight) {
                    minNoiseHeight = noiseHeight;
                }
                noiseMap[x] = noiseHeight;
            }

            for (int x = 0; x < mapWidth; x++) {
                noiseMap[x] = InverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[x]);
            }

            return noiseMap;
        }

        public static float Noise(float x, float y) {
            if (x < 0) {
                x *= -1;
            }

            if (y < 0) {
                y *= -1;
            }

            int x_int = (int)x & 255;
            int y_int = (int)y & 255;

            float distanceX = x - (int)x;
            float distanceY = y - (int)y;

            float smoothX = Fade(distanceX);
            float smoothY = Fade(distanceY);

            int aa, ab, ba, bb;
            aa = _permutationTable[_permutationTable[x_int] + y_int];
            ab = _permutationTable[_permutationTable[x_int] + y_int + 1];
            ba = _permutationTable[_permutationTable[x_int + 1] + y_int];
            bb = _permutationTable[_permutationTable[x_int + 1] + y_int + 1];

            float x1, x2;
            x1 = Lerp(GradientDotProduction(aa, distanceX, distanceY),
                GradientDotProduction(ba, distanceX - 1, distanceY),
                smoothX);
            x2 = Lerp(GradientDotProduction(ab, distanceX, distanceY - 1),
                GradientDotProduction(bb, distanceX - 1, distanceY - 1),
                smoothX);

            return (Lerp(x1, x2, smoothY) + 1) / 2;
        }

        public static float[,] Noise(int mapWidth, int mapHeight, int seed, float scale, int octaves, float persistance, float lacunarity, Vector2 offset) {
            float[,] noiseMap = new float[mapWidth, mapHeight];

            System.Random prng = new System.Random(seed);
            Vector2[] octaveOffsets = new Vector2[octaves];
            for (int i = 0; i < octaves; i++) {
                float offsetX = prng.Next(-100000, 100000) + offset.x;
                float offsetY = prng.Next(-100000, 100000) + offset.y;
                octaveOffsets[i] = new Vector2(offsetX, offsetY);
            }

            if (scale <= 0) {
                scale = 0.0001f;
            }

            float maxNoiseHeight = float.MinValue;
            float minNoiseHeight = float.MaxValue;

            float halfWidth = mapWidth / 2f;
            float halfHeight = mapHeight / 2f;


            for (int y = 0; y < mapHeight; y++) {
                for (int x = 0; x < mapWidth; x++) {

                    float amplitude = 1;
                    float frequency = 1;
                    float noiseHeight = 0;

                    for (int i = 0; i < octaves; i++) {
                        float sampleX = (x - halfWidth) / scale * frequency + octaveOffsets[i].x;
                        float sampleY = (y - halfHeight) / scale * frequency + octaveOffsets[i].y;

                        float perlinValue = Noise(sampleX, sampleY) * 2 - 1;
                        noiseHeight += perlinValue * amplitude;

                        amplitude *= persistance;
                        frequency *= lacunarity;
                    }

                    if (noiseHeight > maxNoiseHeight) {
                        maxNoiseHeight = noiseHeight;
                    }
                    else if (noiseHeight < minNoiseHeight) {
                        minNoiseHeight = noiseHeight;
                    }
                    noiseMap[x, y] = noiseHeight;
                }
            }

            for (int y = 0; y < mapHeight; y++) {
                for (int x = 0; x < mapWidth; x++) {
                    noiseMap[x, y] = InverseLerp(minNoiseHeight, maxNoiseHeight, noiseMap[x, y]);
                }
            }

            return noiseMap;
        }
    }
}
