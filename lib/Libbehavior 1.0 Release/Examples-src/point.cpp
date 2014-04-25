#include "point.h"
#include <ClanLib/core.h>
#include "boost/lexical_cast.hpp"

using namespace SL;
using namespace std;

point::point(float i, float j)
{
	x = i;
	y = j;
}

point::point()
{
	x=0;
	y=0;
}

point point::offsetRect(float ox, float oy)
{
	return point(x+ox,y+oy);
}

point point::offsetPolar(CL_Angle heading, float distance)
{
	return point(x - distance * sin(-heading.to_radians()),y - distance * cos(-heading.to_radians()));
}

float point::angleTo(point p)
{
	return atan2(p.y,p.x) - atan2(y,x);
}

void point::normalize()
{
	float length = vectorSize();
	x /= length;
	y /= length;
}

std::string point::toString()
{
	return boost::lexical_cast<std::string>(x) + "," + boost::lexical_cast<std::string>(y);
}

float SL::point::vectorSize()
{
	return sqrtf(x*x + y*y);	
}

point point::operator+ (const point& right)
{
	return point(x + right.x,y+ right.y);
}

point point::operator- (const point& right)
{
	return point(x - right.x,y- right.y);
}

point point::operator*= (float k)
{
	x*=k;
	y*=k;
	return *this;
}

point point::operator/= (float k)
{
	x/=k;
	y/=k;
	return *this;
}

