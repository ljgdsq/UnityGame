#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>
#include <iomanip>

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "map.h"
#define M_PI    3.14159265358979323846264338327950288   /**< pi */

uint32_t pack_color(const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 255) {
    return (a << 24) + (b << 16) + (g << 8) + r;
}

void unpack_color(const uint32_t &color, uint8_t &r, uint8_t &g, uint8_t &b, uint8_t &a) {
    a = (color >> 24) & 255;
    b = (color >> 16) & 255;
    g = (color >> 8) & 255;
    r = (color >> 0) & 255;
}

bool load_texture(const std::string filename, std::vector<uint32_t> &texture, int &text_size, int &text_cnt) {

    int nchannels = -1, w, h;
    unsigned char *pixmap = stbi_load(filename.c_str(), &w, &h, &nchannels, 0);
    if (!pixmap) {
        std::cerr << "Error: can not load the textures" << std::endl;
        return false;
    }

    if (4!=nchannels) {
        std::cerr << "Error: the texture must be a 32 bit image" << std::endl;
        stbi_image_free(pixmap);
        return false;
    }
    text_cnt = w/h;
    text_size = w/text_cnt;
    if (w!=h*int(text_cnt)) {
        std::cerr << "Error: the texture file must contain N square textures packed horizontally" << std::endl;
        stbi_image_free(pixmap);
        return false;
    }

    texture = std::vector<uint32_t>(w*h);
    for (int j=0; j<h; j++) {
        for (int i=0; i<w; i++) {
            uint8_t r = pixmap[(i+j*w)*4+0];
            uint8_t g = pixmap[(i+j*w)*4+1];
            uint8_t b = pixmap[(i+j*w)*4+2];
            uint8_t a = pixmap[(i+j*w)*4+3];
            texture[i+j*w] = pack_color(r, g, b, a);
        }
    }
    stbi_image_free(pixmap);
    return true;


}

void save_ppm_image(const std::string filename, std::vector<uint32_t> &image, const int w, const int h) {
    assert(image.size() == w * h);

    std::ofstream ofs(filename, std::ios::binary);
    ofs << "P6\n";
    ofs << w << " " << h << "\n255\n";
    for (int i = 0; i < image.size(); ++i) {
        uint8_t r, g, b, a;
        unpack_color(image[i], r, g, b, a);
        ofs << static_cast<unsigned char>(r) << static_cast<unsigned char>(g) << static_cast<unsigned char>(b);
    }
    ofs.close();
}

void draw_rectangle(std::vector<uint32_t> &img, const int img_w, const int img_h, const int x, const int y,
                    const int w, const int h, const uint32_t color) {
    assert(img.size() == img_w * img_h);
    for (int i = 0; i < w; ++i) {
        for (int j = 0; j < h; ++j) {
            int cx = x + i;
            int cy = y + j;
//            assert(cx<img_w&&cy<img_h);
            if (cx >= img_w || cy >= img_h) {
                continue; // no need to check negative values, (unsigned variables)
            }
            img[cx + cy * img_w] = color;
        }
    }
}

int main() {

    const int frame_count=1;
    const int w = 1024;
    const int h = 512;
    std::vector<uint32_t> framebuffer(w * h, pack_color(255, 255, 255));


    std::vector<uint32_t> walltext; // textures for the walls
    int walltext_size;  // texture dimensions (it is a square)
    int walltext_cnt;   // number of different textures in the image
    if (!load_texture("../walltext.png", walltext, walltext_size, walltext_cnt)) {
        std::cerr << "Failed to load wall textures" << std::endl;
        return -1;
    }

    const int rect_w = w / (map_w * 2);
    const int rect_h = h / map_h;


    float player_x = 3.456;
    float player_y = 2.345;
    float player_a = 1.523;// player view direction

    const float fov = M_PI / 3;

    const int ncolors = 10;
    std::vector<uint32_t> colors(ncolors);
    for (int i = 0; i < ncolors; ++i) {
        colors[i] = pack_color(rand() % 255, rand() % 255, rand() % 255);
    }





    for (int frame = 0; frame < frame_count; ++frame) {
        std::cout<<"begin "<<frame<<std::endl;
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(5) << frame << ".ppm";
        player_a += 2 * M_PI / 360;
        framebuffer = std::vector<uint32_t>(w * h, pack_color(255, 255, 255)); // clear the screen


        for (int j = 0; j < map_h; ++j) { //draw the map
            for (int i = 0; i < map_w; ++i) {
                if (map[i + j * map_w] == ' ')continue;
                int rect_x = i * rect_w;
                int rect_y = j * rect_h;
                int icolor = map[i + j * map_w] - '0';
                assert(icolor < ncolors);
                draw_rectangle(framebuffer, w, h, rect_x, rect_y, rect_w, rect_h, colors[icolor]);
            }
        }

        for (int i = 0; i < w / 2; ++i) { //draw the visibility cone adn "3d" view
            float angle = player_a - fov / 2 + fov * i / float(w / 2);
            for (float t = 0; t < 20; t += 0.01) {
                float cx = player_x + t * cos(angle);
                float cy = player_y + t * sin(angle);

                size_t pix_x = cx * rect_w;
                size_t pix_y = cy * rect_h;

                framebuffer[pix_x + pix_y * w] = pack_color(160, 160, 160);  // this draws the visibility cone

                if (map[int(cx) + int(cy) * map_w] != ' ') {
                    int icolor = map[int(cx) + int(cy) * map_w] - '0';
                    assert(icolor < ncolors);

                    int col_height = h / (t* cos(angle-player_a));
                    draw_rectangle(framebuffer, w, h, w / 2 + i, h / 2 - col_height / 2, 1, col_height,
                                   colors[icolor]);
                    break;

                }
            }
        }

        const int texid=4;
        for (int i = 0; i < walltext_size; ++i) {
            for (int j = 0; j < walltext_size; ++j) {
                framebuffer[i+j*w] = walltext[i + texid*walltext_size + j*walltext_size*walltext_cnt];
            }
        }



        save_ppm_image(ss.str(), framebuffer, w, h);

    }


//    draw_rectangle(framebuffer, w, h, player_x * rect_w, player_y * rect_h, 5, 5, pack_color(255, 0, 0));


    return 0;
}
