/**
 * 最简单的基于FFmpeg的解码器
 * Simplest FFmpeg Decoder
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序实现了视频文件的解码(支持HEVC，H.264，MPEG2等)。
 * 是最简单的FFmpeg视频解码方面的教程。
 * 通过学习本例子可以了解FFmpeg的解码流程。
 * This software is a simplest video decoder based on FFmpeg.
 * Suitable for beginner of FFmpeg.
 *
 */



#include <stdio.h>

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
};


int main(int argc, char* argv[])
{
	AVFormatContext	*pFormatCtx;
	int				i, videoindex;

	AVCodecParameters* pCodecParameters;
	
	AVFrame	*pFrame,*pFrameYUV;
	uint8_t *out_buffer;
	AVPacket *packet;
	int y_size;
	int ret, got_picture;
	struct SwsContext *img_convert_ctx;
	//输入文件路径
	char filepath[]="../res/Titanic.ts";

	int frame_cnt;

	avformat_network_init();
	pFormatCtx = avformat_alloc_context();

	if(avformat_open_input(&pFormatCtx,filepath,NULL,NULL)!=0){
		printf("Couldn't open input stream.\n");
		return -1;
	}
	if(avformat_find_stream_info(pFormatCtx,NULL)<0){
		printf("Couldn't find stream information.\n");
		return -1;
	}
	videoindex=-1;
	for(i=0; i<pFormatCtx->nb_streams; i++) 
		if(pFormatCtx->streams[i]->codecpar->codec_type==AVMEDIA_TYPE_VIDEO){
			videoindex=i;
			break;
		}
	if(videoindex==-1){
		printf("Didn't find a video stream.\n");
		return -1;
	}


	pCodecParameters = pFormatCtx->streams[videoindex]->codecpar;
	//pCodecCtx=pFormatCtx->streams[videoindex]->codec;
	AVCodecContext* pCodecCtx = avcodec_alloc_context3(NULL);
	avcodec_parameters_to_context(pCodecCtx, pCodecParameters);

	
	const AVCodec* pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
	if(pCodec==NULL){
		printf("Codec not found.\n");
		return -1;
	}


	if(avcodec_open2(pCodecCtx, pCodec,NULL)<0){
		printf("Could not open codec.\n");
		return -1;
	}


	/*
	 * 在此处添加输出视频信息的代码
	 * 取自于pFormatCtx，使用fprintf()
	 */
	pFrame=av_frame_alloc();
	pFrameYUV=av_frame_alloc();
	out_buffer=(uint8_t *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
	av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);
	packet=(AVPacket *)av_malloc(sizeof(AVPacket));
	//Output Info-----------------------------
	printf("--------------- File Information ----------------\n");
	av_dump_format(pFormatCtx,0,filepath,0);
	printf("-------------------------------------------------\n");
	img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, 
		pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);

	FILE* fileyuv = nullptr;
	fopen_s(&fileyuv, "../res/1.yuv", "wb+");
	FILE* fileh264 = nullptr;
	fopen_s(&fileh264, "../res/1.h264", "wb+");

	frame_cnt=0;
	while(av_read_frame(pFormatCtx, packet)>=0){
		if(packet->stream_index==videoindex){
			/*
			* 在此处添加输出H264码流的代码
			* 取自于packet，使用fwrite()
			*/
			fwrite(packet->data, 1, packet->size, fileh264);

			ret = avcodec_send_packet(pCodecCtx, packet);

			ret = avcodec_receive_frame(pCodecCtx, pFrame);
//			ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
			if(ret < 0){
				printf("Decode Error.\n");
	//			return -1;
				continue;
			}
			//if(got_picture){
				sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, 
					pFrameYUV->data, pFrameYUV->linesize);
				printf("Decoded frame index: %d\n",frame_cnt);

				/*
				 * 在此处添加输出YUV的代码
				 * 取自于pFrameYUV，使用fwrite()
				 */
				//y 数据
				fwrite(pFrame->data[0], 1, pCodecCtx->width * pCodecCtx->height, fileyuv);
				//u 数据 
				fwrite(pFrame->data[1], 1, pCodecCtx->width * pCodecCtx->height /4, fileyuv);
				//v 数据
				fwrite(pFrame->data[2], 1, pCodecCtx->width * pCodecCtx->height /4, fileyuv);


				fflush(fileyuv);


				frame_cnt++;

		//	}
		}
		av_packet_unref(packet);
//		av_free_packet(packet);
	}

	fclose(fileyuv);
	fclose(fileh264);

	sws_freeContext(img_convert_ctx);

	av_frame_free(&pFrameYUV);
	av_frame_free(&pFrame);
//	avcodec_close(pCodecCtx);
	avformat_close_input(&pFormatCtx);

	return 0;
}

