//#define SDL_MAIN_HANDLED
extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}


int main(int argc, char *argv[])
{
	printf("%s ", avcodec_configuration());


	getchar();
	return 0;
}