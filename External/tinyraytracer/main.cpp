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

const int width = 1024;
const int height = 576;
int img_width,img_height;
static vector<vec<3,unsigned char >> imgData(img_width*img_height);
struct Material {
    Material(const float &r, const Vec4f &a, const Vec3f &color, const float &spec) : refractive_index(r), albedo(a), diffuse_color(color), specular_exponent(spec) {}
    Material() : refractive_index(1), albedo(1,0,0,0), diffuse_color(), specular_exponent() {}
    float refractive_index;
    Vec4f albedo;
    Vec3f diffuse_color;
    float specular_exponent;
};

struct Light {
    Light(const Vec3f &pos, const float &i)
            : position(pos), intensity(i) {}
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

Vec3f reflect(const Vec3f &I, const Vec3f &N) {
    return I - N * 2 * (I * N);
}


Vec3f refract(const Vec3f &I, const Vec3f &N, const float &refractive_index) { // Snell's law
    float cosi = - std::max(-1.f, std::min(1.f, I*N));
    float etai = 1, etat = refractive_index;
    Vec3f n = N;
    if (cosi < 0) { // if the ray is inside the object, swap the indices and invert the normal to get the correct result
        cosi = -cosi;
        std::swap(etai, etat); n = -N;
    }
    float eta = etai / etat;
    float k = 1 - eta*eta*(1 - cosi*cosi);
    return k < 0 ? Vec3f(0,0,0) : I*eta + n*(eta * cosi - sqrtf(k));
}

bool scene_interact(const Vec3f &orig, const Vec3f &dir, const vector<Sphere> &spheres, Material &material,
                    Vec3f &hit, Vec3f &N) {
    float sphere_dist = std::numeric_limits<float>::max();
    for (const auto &sphere: spheres) {
        float dist;
        if (sphere.ray_intersect(orig, dir, dist) && dist < sphere_dist) {
            sphere_dist = dist;
            material = sphere.material;
            hit = orig + dir * dist;
            N = (hit - sphere.center).normalize();
        }
    }


    float checkerboard_dist = std::numeric_limits<float>::max();
    if (fabs(dir.y)>1e-3)  {
        float d = -(orig.y+4)/dir.y; // the checkerboard plane has equation y = -4
        Vec3f pt = orig + dir*d;
        if (d>0 && fabs(pt.x)<10 && pt.z<-10 && pt.z>-30 && d<sphere_dist) {
            checkerboard_dist = d;
            hit = pt;
            N = Vec3f(0,1,0);
            material.diffuse_color = (int(.5*hit.x) + int(.5*hit.z)) & 1 ? Vec3f(1,1,1) : Vec3f(1, .7, .3);
          material.diffuse_color = material.diffuse_color*.3;
        }
    }
    return std::min(sphere_dist, checkerboard_dist)<1000;
}

Vec3f cast_ray(const Vec3f &orig, const Vec3f &dir, const vector<Sphere> &spheres, const vector<Light> &lights,
               int depth = 0) {
    Material material;
    Vec3f N;
    Vec3f hit;

    if (scene_interact(orig, dir, spheres, material, hit, N) && depth <= 4) {

        Vec3f reflect_dir = reflect(dir, N).normalize();
        Vec3f reflect_orig = reflect_dir * N < 0 ? hit - N * 1e-3 : hit + N * 1e-3;
        Vec3f reflect_color = cast_ray(reflect_orig, reflect_dir, spheres, lights, depth + 1);

        Vec3f refract_dir= refract(dir,N,material.refractive_index).normalize();
        Vec3f refract_orig = refract_dir*N < 0 ? hit - N*1e-3 : hit + N*1e-3;
        Vec3f refract_color = cast_ray(refract_orig, refract_dir, spheres, lights, depth + 1);

        float diffuse_light_intensity = 0;
        float specular_light_intensity = 0;
        for (const auto &light: lights) {
            float light_distance = (light.position - hit).norm();
            Vec3f light_dir = (light.position - hit).normalize();

            Vec3f shadow_orig = light_dir * N < 0 ? hit - N * 1e-3 : hit + N * 1e-3;
            Material temp_material;
            Vec3f shadow_point;
            Vec3f shadow_N;
            if (scene_interact(shadow_orig, light_dir, spheres, temp_material, shadow_point, shadow_N)
                && (shadow_point - shadow_orig).norm() < light_distance
                    ) {
                continue;
            }

            diffuse_light_intensity += light.intensity * std::max(0.f, light_dir * N);
            specular_light_intensity +=
                    powf(std::max(0.f, -reflect(-light_dir, N) * dir), material.specular_exponent) * light.intensity;
        }
        return material.diffuse_color * diffuse_light_intensity * material.albedo[0] +
               Vec3f(1.f, 1.f, 1.f) * specular_light_intensity * material.albedo[1] +
               reflect_color * material.albedo[2] + refract_color*material.albedo[3];;
    }

    return Vec3f(0.2, 0.7, 0.8); // background color
}

#define M_PI       3.14159265358979323846   // pi

static Vec3f bg(0.2, 0.7, 0.8); // background color
void render(const vector<Sphere> &spheres, const vector<Light> &lights) {


    const int fov = M_PI / 2.;
    const Vec3f orig(0, 0, 0);
    std::vector<unsigned char> framebuffer(width * height * 3);
    Timer timer;
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            float x = (2 * (j + 0.5) / width - 1) * tan(fov / 2.) * width / (float) height;
            float y = -(2 * (i + 0.5) / height - 1) * tan(fov / 2.);
            Vec3f dir = Vec3f(x, y, -1).normalize();






            Vec3f color = cast_ray(orig, dir, spheres, lights);
            if (color.x==bg.x && color.y==bg.y&&color.z==bg.z){
                framebuffer[3 * (i * width + j) + 0] = imgData [(i * width + j)].x;
                framebuffer[3 * (i * width + j) + 1] = imgData [(i * width + j)].y;
                framebuffer[3 * (i * width + j) + 2] = imgData [(i * width + j)].z;

            } else
            {
                float maxComp = std::max(color.x, std::max(color.y, color.z));
                if (maxComp > 1.0f) {
                    color = color * (1.0f / maxComp);
                }
                framebuffer[3 * (i * width + j) + 0] = color.x * 255.0;
                framebuffer[3 * (i * width + j) + 1] = color.y * 255.0;
                framebuffer[3 * (i * width + j) + 2] = color.z * 255.0;
            }
        }
    }
    auto time = timer.GetElapseTime();

    cout << "use time " << time << " ms" << endl;

    stbi_write_png("./out.png", width, height, 3, framebuffer.data(), 0);
}

int main() {


    int channel;
    unsigned char *background= stbi_load("./env1.jpg",&img_width,&img_height,&channel,0);
    if (background== nullptr){
        std::cout<<"error load img!";
        return -1;
    }

    imgData.resize(img_height*img_width);

    for (int i = 0; i < img_height; ++i) {
        for (int j = 0; j < img_width; ++j) {
            imgData[(i * img_width + j)] = { background[3 * (i * img_width + j) + 0],background[3 * (i * img_width + j) + 1], background[3 * (i * img_width + j) + 2]}         ;
       }
    }

    stbi_image_free(background);

    Material      ivory(1.0, Vec4f(0.6,  0.3, 0.1, 0.0), Vec3f(0.4, 0.4, 0.3),   50.);
    Material      glass(1.5, Vec4f(0.0,  0.5, 0.1, 0.8), Vec3f(0.6, 0.7, 0.8),  125.);
    Material red_rubber(1.0, Vec4f(0.9,  0.1, 0.0, 0.0), Vec3f(0.3, 0.1, 0.1),   10.);
    Material     mirror(1.0, Vec4f(0.0, 10.0, 0.8, 0.0), Vec3f(1.0, 1.0, 1.0), 1425.);

    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vec3f(-3,    0,   -16), 2,      ivory));
    spheres.push_back(Sphere(Vec3f(-1.0, -1.5, -12), 2,      glass));
    spheres.push_back(Sphere(Vec3f( 1.5, -0.5, -18), 3, red_rubber));
    spheres.push_back(Sphere(Vec3f( 7,    5,   -18), 4,     mirror));

    std::vector<Light>  lights;
    lights.push_back(Light(Vec3f(-20, 20,  20), 1.5));
    lights.push_back(Light(Vec3f( 30, 50, -25), 1.8));
    lights.push_back(Light(Vec3f( 30, 20,  30), 1.7));


    render(spheres, lights);
    return 0;
}
