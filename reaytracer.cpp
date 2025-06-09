//
// Created by Timofei Arefev on 09/06/2025.
//

#include <fstream>
#include <iostream>
#include <__ostream/basic_ostream.h>

#include "shapes/Sphere.h"

void render(Sphere s) {

    const int width = 800;
    const int height = 600;

    float aspect_ratio = static_cast<float>(width) / height;

    Vec3f rayOrigin(0, 0, 0);
    // Create and open a .ppm file
    std::ofstream image("output.ppm");

    // Write the PPM header
    image << "P3\n" << width << " " << height << "\n255\n";

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            const float x_screen = (static_cast<float>(j) + 0.5f) / width;
            const float y_screen = (static_cast<float>(i) + 0.5f) / height;

            const float x = (2 * x_screen - 1) * aspect_ratio;
            const float y = 1 - 2 * y_screen;
            Vec3f rayDirection = (Vec3f(x, y, -1) - rayOrigin).normalize();

            float t0, t1;
            const bool hit = s.intersect(rayOrigin, rayDirection, t0, t1);
            if (hit) {
                std::cout << "Ray hit the sphere at t0 = " << t0 << ", t1 = " << t1 << "\n";
                 float brightness = 1 - (t0 - 1) / (5 - 1);


                int brightness255 = static_cast<int>(brightness * 255);
                if (brightness255 > 255) brightness255 = 255;
                if (brightness255 < 0) brightness255 = 0;
                image << brightness255 << " " << brightness255 << " " << brightness255 << " ";

                // image << static_cast<int>(255 - (t0 * 10))<<" 255 255 ";
            }
            else {
                image << "0 0 0 ";
            }
        }
        image << "\n";
    }


    image.close();
    std::cout << "Image saved to output.ppm\n";
}

int main() {

    Sphere s(Vec3f(0, 0, -5), 2.0f, Vec3f(1, 0, 0));
    render(s);

    return 0;
}

