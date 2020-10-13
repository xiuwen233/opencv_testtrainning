#pragma once

#include<opencv2\core\core.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<opencv2\highgui\highgui.hpp>

using namespace cv;
using namespace std;


typedef struct ResultParam
{
	float  center_x;
	float  center_y;
	float angle;
}ResultParam;

typedef struct VideoInitParam
{
	int rect_x;
	int rect_y;
	int rect_weight;
	int rect_height;

	int rect_pa_x;
	int rect_pa_y;
	int rect_pa_weight;
	int rect_pa_height;

	int animal_mask_image_x;
	int animal_mask_image_y;

	int i_threshold;
	int pallet_threshold;
}VideoInitParam;

class VideoCheck
{
public:
	VideoCheck();
	~VideoCheck();
	bool OpenVideo(int index);
	int StartRun(); //返回 -1 摄像头不对 返回stop  需要开启一个线程
	void StopRun(); //停止检测2
	void VideoInit(VideoInitParam videoparam);
	void  StartCheckCassette();
	void  StopCheckCassette();
	void  StartCheckPallet();
	void  StopCheckPallet();

	int GetResult(ResultParam *result);
	Point  GetPalletPoint();

private:
	int VideoIndex;
	bool b_detect;
	bool b_CheckCassette;
	bool b_CheckPallet;

	int rect_x;
	int rect_y;
	int rect_weight;
	int rect_height;

	int pallet_threshold;
	int i_threshold;

	int rect_pa_x;
	int rect_pa_y;
	int rect_pa_weight;
	int rect_pa_height;

	Mat animal_mask_image;
	int animal_mask_image_x ;
	int animal_mask_image_y;


	float angle;
	Point center;
	Point palletcenter;
	VideoCapture capture;

private:
	void  Rectangle_animal_mask_made();
};
