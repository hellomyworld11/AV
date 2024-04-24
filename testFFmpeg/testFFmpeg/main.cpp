#include <iostream>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

std::string video = "SampleVideo_720x480_2mb.flv";

int main()
{
    std::cout << "Hello World!\n";
    printf("%s\n", avcodec_configuration());


}