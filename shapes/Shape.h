//
// Created by Timofei Arefev on 09/06/2025.
//

#ifndef SHAPE_H
#define SHAPE_H

#include "../vec/Vec3.h"
#pragma once

class Shape {
public:
    virtual ~Shape() = default;

    virtual void printInfo() const = 0;

    virtual bool intersect(const Vec3f &rayorig, const Vec3f &raydir, float &t0, float &t1) const = 0;
};




#endif //SHAPE_H
