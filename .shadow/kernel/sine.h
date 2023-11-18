#define PI 3.14159265359

float sine(float x) {
    float result = x;
    float numerator = x * x * x;
    float denominator = 6.0;
    int sign = -1;

    for (int i = 3; i <= 15; i += 2) {
        result += sign * (numerator / denominator);
        numerator *= x * x;
        denominator *= (i + 1) * (i + 2);
        sign *= -1;
    }

    return result;
}

void generate_waveform(int w, int h, float* pointsX, float* pointsY, int numPoints) {
    float stepX = (float)w / (numPoints - 1);
    float amplitude = h / 2.0;

    for (int i = 0; i < numPoints; i++) {
        pointsX[i] = i * stepX;
        float angle = pointsX[i] * (2.0 * PI / w);
        pointsY[i] = amplitude * sine(angle);
    }
}