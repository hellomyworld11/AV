#pragma once


#include <stdio.h>
#include <iostream>
using namespace std;

#define __STDC_CONSTANT_MACROS
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
#include <SDL.h>
}

static int g_frame_rate = 1;
static int g_sfp_refresh_thread_exit = 0;
static int g_sfp_refresh_thread_pause = 0;

#define SFM_REFRESH_EVENT (SDL_USEREVENT+1)
#define SFM_BREAK_EVENT (SDL_USEREVENT+2)

typedef struct FFmpeg_V_Param_T
{
    AVFormatContext* pFormatCtx;
    AVCodecContext* pCodecCtx;
    SwsContext* pSwsCtx;
    int video_index;
}FFmpeg_V_Param;

typedef struct SDL_Param_T
{
    SDL_Window* p_sdl_window;
    SDL_Renderer* p_sdl_renderer;
    SDL_Texture* p_sdl_texture;
    SDL_Rect sdl_rect;
    SDL_Thread* p_sdl_thread;
}SDL_Param;


/*
  return value:zero(success) non-zero(failure)
*/
int init_ffmpeg(FFmpeg_V_Param* p_ffmpeg_param, char* filePath);
/*
  return value:zero(success) non-zero(failure)
*/
int release_ffmpeg(FFmpeg_V_Param* p_ffmpeg_param);

int sfp_refresh_thread(void* opaque);

int init_sdl2(SDL_Param_T* p_sdl_param, int screen_w, int screen_h);

int release_sdl2(SDL_Param_T* p_sdl_param);

int vPlayer_sdl2(char* filePath);