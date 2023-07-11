#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>


#define SDL_MAIN_HANDLED
#include "SDL.h"

uint32_t pack_color(const uint8_t r,const uint8_t g,const uint8_t b,const uint8_t a=255){
    return (a<<24) + (b<<16) + (g<<8) + r;
}
void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
    a= (color>>24)&255;
    b= (color>>16)&255;
    g= (color>>8)&255;
    r= (color>>0)&255;
}

void save_ppm_image(const std::string filename,std::vector<uint32_t> &image,const int w,const int h){
    assert(image.size()==w*h);

    std::ofstream ofs(filename,std::ios::binary);
    ofs << "P6\n";
    ofs<<w<<" "<<h<<"\n255\n";
    for (int i = 0; i < image.size(); ++i) {
        uint8_t r,g,b,a;
        unpack_color(image[i],r,g,b,a);
        ofs<<static_cast<unsigned char>(r)<<static_cast<unsigned char>(g)<<static_cast<unsigned char>(b);
    }
    ofs.close();
}

int main() {

//    if (SDL_Init(SDL_INIT_VIDEO)) {
//        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
//        return -1;
//    }

    const int w=512;
    const int h=512;

    std::vector<uint32_t> framebuffer(w*h,255);
    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            uint8_t r=255*j/float(h);
            uint8_t g=255*i/float(w);
            uint8_t b=0;
            framebuffer[j*w+i]= pack_color(r,g,b);
        }
    }


    save_ppm_image("./out.ppm",framebuffer,w,h);

    return 0;
}
