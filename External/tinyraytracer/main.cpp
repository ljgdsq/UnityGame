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

struct Sphere {
    Vec3f center;
    float radius;

    Sphere(const Vec3f &c, const float &r) : center(c), radius(r) {}

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

Vec3f cast_ray(const Vec3f&orig,const Vec3f&dir,const Sphere&sphere){
    float sphere_dist=std::numeric_limits<float>::max();
    if (sphere.ray_intersect(orig,dir,sphere_dist)){
        return Vec3f(0.4, 0.4, 0.3);
    }
    return Vec3f(0.2, 0.7, 0.8); // background color
}

#define M_PI       3.14159265358979323846   // pi

void render(const Sphere&sphere) {
    const int width = 1024;
    const int height = 768;

    const int fov      = M_PI/2.;
    const Vec3f orig(0,0,0);
    std::vector<unsigned char> framebuffer(width * height * 3);
    Timer timer;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float x= (2*(j+0.5)/width -1)* tan(fov/2.)*width/(float)height;
            float y= (2*(i+0.5)/height -1)* tan(fov/2.);
            Vec3f dir=Vec3f(x,y,-1).normalize();
            Vec3f color=cast_ray(orig,dir,sphere);
            framebuffer[3 * (i * width + j) + 0] = color.x * 255.0;
            framebuffer[3 * (i * width + j) + 1] =color.y * 255.0;
            framebuffer[3 * (i * width + j) + 2] = color.z * 255.0;
        }
    }
    auto time=timer.GetElapseTime();

    cout<<"use time "<<time<<" ms"<<endl;

    stbi_write_png("./out.png", width, height, 3, framebuffer.data(), 0);
}

int main() {

    Sphere sphere(Vec3f(-3, 0, -16), 2);
    render(sphere);
    return 0;
}