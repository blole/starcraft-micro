#ifndef UTIL_H
#define UTIL_H

inline int round(double x)
{
return int(x > 0.0 ? x + 0.5 : x - 0.5);
}

/// unlike fmod, this will always return a positive value
inline float mod(float x, float y) { return x - y * floor(x / y); }

const float PI = 3.14159265358979323846264338f;


#endif