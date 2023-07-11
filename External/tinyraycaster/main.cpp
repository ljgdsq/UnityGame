#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cassert>

#include "map.h"
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

void draw_rectangle(std::vector<uint32_t>&img,const int img_w,const int img_h,const int x,const int y,
                    const int w,const int h,const uint32_t color){
    assert(img.size()==img_w*img_h);
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            int cx=x+i;
            int cy=y+j;
            assert(cx<img_w&&cy<img_h);
            img[cx+cy*img_w]=color;
        }
    }
}

int main() {

    const int w=512;
    const int h=512;
    std::vector<uint32_t> framebuffer(w*h,255);

    const int rect_w=w/map_w;
    const int rect_h=h/map_h;


    for (int j = 0; j < h; ++j) {
        for (int i = 0; i < w; ++i) {
            uint8_t r=255*j/float(h);
            uint8_t g=255*i/float(w);
            uint8_t b=0;
            framebuffer[j*w+i]= pack_color(r,g,b);
        }
    }


    for (int j = 0; j < map_h; ++j) {
        for (int i = 0; i < map_w; ++i) {
            if (map[i+j*map_w] == ' ')continue;
            int rect_x=i*rect_w;
            int rect_y=j*rect_h;

            draw_rectangle(framebuffer,w,h,rect_x,rect_y,rect_w,rect_h, pack_color(0,255,255));
        }
    }

    save_ppm_image("./out.ppm",framebuffer,w,h);

    return 0;
}
