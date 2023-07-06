//
// Created by ljgdsq on 2023/7/6.
//

#include <iostream>
#include <cmath>
#include "vector"
#include "geometry.h"


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include "Timer.h"

using namespace std;


struct Material {

    Material(Vec3f color) : diffuse_color(color) {}

    Material() : diffuse_color() {}

    Vec3f diffuse_color;
};

struct Light{
    Light(const Vec3f&pos,const float&i)
    :position(pos),intensity(i){}
    Vec3f position;
    float intensity;
};

struct Sphere {
    Vec3f center;
    float radius;

    Material material;

    Sphere(const Vec3f &c, const float &r, const Material &m) : center(c), radius(r), material(m) {}

    bool ray_intersect(const Vec3f &orig, const Vec3f &dir, float &t0) const {
        Vec3f L = center - orig;
        float tca = L * dir;

        float d2 = L * L - tca * tca;
        if (d2 > radius * radius) return false;
        float tch = sqrtf(radius * radius - d2);
        t0 = tca - tch;
        float t1 = tca + tch;
        if (t0 < 0) t0 = t1;
        if (t0 < 0) return false;
        return true;
    }
};

bool scene_interact(const Vec3f &orig, const Vec3f &dir, const vector<Sphere> spheres,  Material &material,
                     Vec3f&hit, Vec3f&N) {
    float sphere_dist = std::numeric_limits<float>::max();
    for (const auto &sphere: spheres){
        float dist;
        if (sphere.ray_intersect(orig,dir,dist) && dist<sphere_dist){
           sphere_dist=dist;
           material.diffuse_color=sphere.material.diffuse_color;
           hit=orig+dir*dist;
           N=(hit-sphere.center).normalize();
        }
    }
    return sphere_dist<1500;
}

Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir, const vector<Sphere> &spheres,const vector<Light> &lights) {
    Material material;
    Vec3f N;
    Vec3f hit;

    if (scene_interact(orig, dir, spheres, material,hit,N)) {
        float diffuse_light_intensity=0;
        for (const auto&light:lights){
            Vec3f light_dir=(light.position-hit).normalize();
            diffuse_light_intensity+= light.intensity* std::max(0.1f, light_dir*N);
        }
        return material.diffuse_color*diffuse_light_intensity;
    }

    return Vec3f(0.2, 0.7, 1); // background color
}

#define M_PI       3.14159265358979323846   // pi

void render(const vector<Sphere> &spheres,const vector<Light> &lights) {
    const int width = 1024;
    const int height = 768;

    const int fov = M_PI / 2.;
    const Vec3f orig(0, 0, 0);
    std::vector<unsigned char> framebuffer(width * height * 3);
    Timer timer;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float x = (2 * (j + 0.5) / width - 1) * tan(fov / 2.) * width / (float) height;
            float y = -(2 * (i + 0.5) / height - 1) * tan(fov / 2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();

            Vec3f color = cast_ray(orig, dir, spheres,lights);
            framebuffer[3 * (i * width + j) + 0] = color.x * 255.0;
            framebuffer[3 * (i * width + j) + 1] = color.y * 255.0;
            framebuffer[3 * (i * width + j) + 2] = color.z * 255.0;
        }
    }
    auto time = timer.GetElapseTime();

    cout << "use time " << time << " ms" << endl;

    stbi_write_png("./out.png", width, height, 3, framebuffer.data(), 0);
}

int main() {

    Material      ivory(Vec3f(0.4, 0.4, 0.3));
    Material red_rubber(Vec3f(0.3, 0.1, 0.1));

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3f(-3,    0,   -16), 2,      ivory));
    spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2, red_rubber));
    spheres.push_back(Sphere(Vec3f( 1.5, -0.5, -18), 3, red_rubber));
    spheres.push_back(Sphere(Vec3f( 7,    5,   -18), 4,      ivory));


    std::vector<Light> lights;
    lights.push_back(Light(Vec3f(-20, 20, 20), 0.9));


    render(spheres,lights);
    return 0;
}
