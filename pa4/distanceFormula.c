#ifndef DIST
#define DIST

#include <stdio.h>
#include <math.h>

#endif

double getDistance(double x2, double y2);

double getDistance(double x2, double y2)
{
    double x1= 0.5, y1=0.5;
    double totalX = -1.0, totalY = -1.0;
    double distance = -2.0;

    totalX = (x2 - x1);
    totalX = totalX * totalX; //Square it
    totalY = (y2 - y1);
    totalY = totalY * totalY;

    distance = totalX + totalY;
    distance = sqrt(distance);
    return  distance;
}