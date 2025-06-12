//
// Created by Timofei Arefev on 09/06/2025.
//

#ifndef SHERE_H
#define SHERE_H

#pragma once
#include <__math/roots.h>

#include "Shape.h"

#include "../vec/Vec3.h"

class Sphere : public Shape {
public:
    Vec3f center;
    float radius, radius2;
    Vec3f surfaceColor, emissionColor;
    float transparency, reflection;

    Sphere(
        const Vec3f &c,
        const float &r,
        const Vec3f &sc,
        const float &refl = 0,
        const float &transp = 0,
        const Vec3f &ec = 0) :
        center(c), radius(r), radius2(r * r), surfaceColor(sc),
        emissionColor(ec), transparency(transp), reflection(refl)
    {}

    bool intersect(const Vec3f &rayorig, const Vec3f &raydir, float &t0, float &t1) const override {
        Vec3f l = center - rayorig;
        float tca = l.dot(raydir);
        if (tca < 0) return false;
        float d2 = l.dot(l) - tca * tca;
        if (d2 > radius2) return false;
        float thc = std::sqrt(radius2 - d2);
        t0 = tca - thc;
        t1 = tca + thc;
        return true;
    }

    void printInfo() const override {
        std::cout << "Sphere at " << center << " with radius " << radius << "\n";
    }
    Vec3f getColor() const override {
        return surfaceColor;
    }
    Vec3f getEmission() const override {
        return emissionColor;
    }

    bool hasEmission() const override {
        return emissionColor.x > 0 || emissionColor.y > 0 || emissionColor.z > 0;
    }

    Vec3f getPosition() const override {
        return center;
    }

    Vec3f getNormal(const Vec3f &point) const override {
        return (point - center).normalize();
    }

};

#endif //SHERE_H
