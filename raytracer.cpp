//
// Created by Timofei Arefev on 09/06/2025.
//

#include <fstream>
#include <iostream>
#include <__ostream/basic_ostream.h>

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


            float t0, t1;
            for (const auto & object : objects) {

                const bool hit = object->intersect(rayOrigin, rayDirection, t0, t1);
                // if (hit) {
                    std::cout << "Ray hit the sphere at t0 = " << t0 << ", t1 = " << t1 << "\n";
                    float brightness = 1 - (t0 - 1) / (5 - 1);


                    int brightness255 = static_cast<int>(brightness * 255);
                    if (brightness255 > 255) brightness255 = 255;
                    if (brightness255 < 0) brightness255 = 0;

                    *pixel = Vec3f(brightness, brightness, brightness);
                // }
                // else {
                //     *pixel =  Vec3f(0, 0, 0);
                // }
            }
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
        Vec3f(-1.5, 2, -4), 1, Vec3f(0.90, 0.76, 0.46), 1, 0.0
    ));
    objects.push_back(std::make_shared<Sphere>(
        Vec3f(-1.5, 3, -4), 1, Vec3f(0.90, 0.76, 0.46), 1, 0.1
    ));

    objects.push_back(std::make_shared<Sphere>(
        Vec3f(0, -0, -5), 1.5, Vec3f(0.90, 0.76, 0.46), 1, 0.0
    ));
    objects.push_back(std::make_shared<Sphere>(
        Vec3f(-2, -0, -5), 1.5, Vec3f(0.90, 0.76, 0.46), 1, 0.0
    ));


    // Sphere s(Sphere(Vec3f(0, 0, -5), 2.0f, Vec3f(1, 0, 0)));
    render(objects);

    return 0;
}

