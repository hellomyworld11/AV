#include <iostream>

#define SDL_MAIN_HANDLED
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

#include "vPlayer_sdl2.h"

std::string video = "F:\\mycode\\sourceCC++\\testFFmpeg\\res\\SampleVideo_720x480_2mb.flv";

int main()
{
    std::cout << "Hello World!\n";
    printf("%s\n", avcodec_configuration());

    vPlayer_sdl2((char*)video.c_str());
    return 0;
}