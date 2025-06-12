//
// Created by Timofei Arefev on 09/06/2025.
//

#include <fstream>
#include <iostream>
#include <__ostream/basic_ostream.h>
#include <algorithm>

#include "shapes/Sphere.h"

#define WIDTH 900
#define HEIGHT 900

// #define FAR 7
// #define NEAR 1

#define MAX_DEPTH 10
// consts

const Vec3f rayOrigin(0, 0, 0);
Vec3f calcShadowRay(const Vec3f &hitPoint,
                    const Vec3f &normal,
                    const std::vector<std::shared_ptr<Shape>> &lightObjects,
                    const std::vector<std::shared_ptr<Shape>> &objects) {

    Vec3f totalLight(0, 0, 0);
    float bias = 1e-4;

    for (const auto& light : lightObjects) {
        Vec3f lightDir = light->getPosition() - hitPoint;
        float lightDistance = lightDir.length();
        lightDir.normalize();

        Vec3f shadowOrigin = hitPoint + normal * bias;
        bool inShadow = false;

        for (const auto& obj : objects) {
            float t0, t1;
            if (obj->intersect(shadowOrigin, lightDir, t0, t1)) {
                if (t0 > 0 && t0 < lightDistance) {
                    inShadow = true;
                    break;
                }
            }
        }

        if (!inShadow) {
            float intensity = std::max(0.f, normal.dot(lightDir));
            totalLight += light->getEmission() * intensity;
        }
    }

    return totalLight;
}


Vec3f trace(
    const Vec3f &rayorig,
    const Vec3f &raydir,
    const std::vector<std::shared_ptr<Shape>>& objects,
    const int &depth){
    if (depth == -1) {
        std::cout<<"depth should be -1"<<std::endl;
    }
    float closestT = std::numeric_limits<float>::max();
    bool hitSomething = false;
    Vec3f pixelColor(0, 0, 0);

    std::vector<std::shared_ptr<Shape>> lightObjects;
    for (const auto& object : objects) {
        if (object->hasEmission()) {
            lightObjects.push_back(object);
        }
    }


    for (const auto& object : objects) {
        float t0, t1;
        if (object->intersect(rayOrigin, raydir, t0, t1)) {
            if (t0 < 0) t0 = t1;
            if (t0 < closestT) {
                closestT = t0;
                hitSomething = true;
                Vec3f intersectionPoint = rayOrigin + raydir * t0;
                if (object->hasEmission()) {
                    pixelColor = Vec3f( object->getColor().x,  object->getColor().y, object->getColor().z);
                }
                else {
                    pixelColor = Vec3f(1, 1, 1);
                }
                // pixelColor = calcShadowRay(intersectionPoint,)
            }
        }
    }
    return hitSomething ? pixelColor : Vec3f(0, 0, 0);
}

void render(const std::vector<std::shared_ptr<Shape>>& objects) {

    Vec3f *image = new Vec3f[WIDTH * HEIGHT], *pixel = image;
    float aspect_ratio = static_cast<float>(WIDTH) / HEIGHT;


    for (int i = 0; i < HEIGHT; ++i) {
        for (int j = 0; j < WIDTH; ++j, ++pixel) {
            const float x_screen = (static_cast<float>(j) + 0.5f) / WIDTH;
            const float y_screen = (static_cast<float>(i) + 0.5f) / HEIGHT;

            const float x = (2 * x_screen - 1) * aspect_ratio;
            const float y = 1 - 2 * y_screen;
            Vec3f rayDirection = (Vec3f(x, y, -1) - rayOrigin).normalize();


            *pixel = trace(rayOrigin, rayDirection, objects, -1);
        }

    }

    std::ofstream ofs("./output.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << WIDTH << " " << HEIGHT << "\n255\n";
    for (unsigned i = 0; i < WIDTH * HEIGHT; ++i) {
        ofs << (unsigned char)(std::min(float(1), image[i].x) * 255) <<
               (unsigned char)(std::min(float(1), image[i].y) * 255) <<
               (unsigned char)(std::min(float(1), image[i].z) * 255);
    }
    ofs.close();

    std::cout << "Image saved to output.ppm\n";
}

int main() {

    std::vector<std::shared_ptr<Shape>> objects;
    objects.push_back(std::make_shared<Sphere>(
        Vec3f(-1.5, 2, -5), 1.5, Vec3f(0.90, 0.76, 0.0), 1, 0.0, Vec3f(1)
    ));
    objects.push_back(std::make_shared<Sphere>(
        Vec3f(-1.5, 3, -5), 1.5, Vec3f(0.0, 0.0, 1), 1, 0.1
    ));

    objects.push_back(std::make_shared<Sphere>(
        Vec3f(0, -0, -5), 1.5, Vec3f(0.90, 0.76, 0.0), 1, 0.0
    ));

    objects.push_back(std::make_shared<Sphere>(
        Vec3f(-2, -1, -5), 1.5, Vec3f(0.90, 0.0, 0.0), 1, 0.0
    ));

    // objects.push_back(std::make_shared<Sphere>(
    //     Vec3f(0, -1, -5), 1.5, Vec3f(0.90, 0.76, 0.0), 1, 0.0
    // ));

    // Sphere s(Sphere(Vec3f(0, 0, -5), 2.0f, Vec3f(1, 0, 0)));
    render(objects);

    return 0;
}

