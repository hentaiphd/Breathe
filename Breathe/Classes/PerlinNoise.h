//
//  PerlinNoise.h
//  Breathe
//
//  Created by Emmett Butler on 5/12/13.
//
//

#ifndef __Breathe__PerlinNoise__
#define __Breathe__PerlinNoise__

#include "cocos2d.h"

class PerlinNoise
{
public:
    static double noise(double x, double y);
private:
    static double interpolate(double a, double b, double x);
    static double findNoise(double x, double y);
};

#endif /* defined(__Breathe__PerlinNoise__) */
