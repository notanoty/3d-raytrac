//
// Created by Timofei Arefev on 09/06/2025.
//

#include <fstream>
#include <iostream>
#include <__ostream/basic_ostream.h>
#include <algorithm>

#include "shapes/Sphere.h"

void render(const std::vector<std::shared_ptr<Shape>>& objects) {

    const int width = 800;
    const int height = 600;
    Vec3f *image = new Vec3f[width * height], *pixel = image;
    float aspect_ratio = static_cast<float>(width) / height;

    Vec3f rayOrigin(0, 0, 0);

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j, ++pixel) {
            const float x_screen = (static_cast<float>(j) + 0.5f) / width;
            const float y_screen = (static_cast<float>(i) + 0.5f) / height;

            const float x = (2 * x_screen - 1) * aspect_ratio;
            const float y = 1 - 2 * y_screen;
            Vec3f rayDirection = (Vec3f(x, y, -1) - rayOrigin).normalize();


            float closestT = std::numeric_limits<float>::max();
            bool hitSomething = false;
            Vec3f pixelColor(0, 0, 0);

            for (const auto& object : objects) {
                float t0, t1;
                if (object->intersect(rayOrigin, rayDirection, t0, t1)) {
                    if (t0 < 0) t0 = t1;
                    if (t0 < closestT) {
                        closestT = t0;
                        hitSomething = true;
                        float brightness = 1 - (t0 - 1) / (10 - 1);

                        if (brightness < 0.0f) brightness = 0.0f;
                        else if (brightness > 1.0f) brightness = 1.0f;

                        pixelColor = Vec3f(brightness * object->getColor().x, brightness * object->getColor().y, brightness * object->getColor().z);
                    }
                }
            }
            *pixel = hitSomething ? pixelColor : Vec3f(0, 0, 0);
        }

    }

    std::ofstream ofs("./output.ppm", std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (unsigned i = 0; i < width * height; ++i) {
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
        Vec3f(-1.5, 2, -5), 1.5, Vec3f(0.90, 0.76, 0.0), 1, 0.0
    ));
    objects.push_back(std::make_shared<Sphere>(
        Vec3f(-1.5, 3, -5), 1.5, Vec3f(0.0, 0.0, 1), 1, 0.1
    ));

    objects.push_back(std::make_shared<Sphere>(
        Vec3f(0, -1, -5), 1.5, Vec3f(0.90, 0.76, 0.0), 1, 0.0
    ));
    objects.push_back(std::make_shared<Sphere>(
        Vec3f(-2, -1, -5), 1.5, Vec3f(0.90, 0.0, 0.0), 1, 0.0
    ));

    objects.push_back(std::make_shared<Sphere>(
        Vec3f(0, -1, -5), 1.5, Vec3f(0.90, 0.76, 0.0), 1, 0.0
    ));
    objects.push_back(std::make_shared<Sphere>(
        Vec3f(-2, -1, -5), 1.5, Vec3f(0.90, 0.0, 0.0), 1, 0.0
    ));

    // Sphere s(Sphere(Vec3f(0, 0, -5), 2.0f, Vec3f(1, 0, 0)));
    render(objects);

    return 0;
}

