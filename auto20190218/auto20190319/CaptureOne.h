#pragma once
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<thread>
#include<mutex>
#include<io.h>
#include<Windows.h>

using namespace cv;
using namespace std;

typedef struct GetPointRect
{
	int rect_x;
	int rect_y;
	int rect_weight;
	int rect_hight;
}GetPointRect;

typedef struct FindCouterRangle
{
	int  fincout_x;
	int  fincout_y;
	int  findcut_size;
}FindCouterRangle;

typedef struct FindChipPostion
{
	float angel;
	int center_x;
	int center_y;
}FindChipPostion;

class CaptureOne
{
public:
	CaptureOne();
	~CaptureOne();
	int     CaptureIndex(int index);
	bool  iscapture_openOK();
	int     capturerealse();
	void  startrun();
	bool  capturestatus();
	void  stopCapture();

	void  StartIsp();
	void  StopIsp();

	void  SetRectRangle(GetPointRect rectrangle);
	void  SetSrcImage();
	int     SetPhologyType(int type);
	int     SetThreshold(int mythreshold);
	void  SetDrawResult();
	void  StopDrawResult();
	void  SetFindCouter(FindCouterRangle findcouterrangle);
	
	int     CalSrc_PHashCode();
	int     WriteMatchTemp();
	int     getHammingDistance(int * d);

	int     getSingleRectangle(FindChipPostion * findchippostion);
	Mat   getFrameOut();
private:
	GetPointRect GetTheRectRangle();
	void  trueRun();

private:
	Mat frameout;
	Mat frameouttemp;
	FindChipPostion findchippostion;
	Point  corner_point[4];
	mutex camutex;
	int     captureindex;
	int     hammingdistance;
	bool  is_detect;
	bool  is_capturerun;
	bool  is_drawresult;
	bool  is_writematchtemp;
	bool  is_calpashcode;
	bool  is_setrectrangle;
	bool  is_getchippostion;
	bool  is_calhamdis;
	bool  is_runing;
	bool  is_isp;

private:
	VideoCapture capture;
	int  threshold_value;
	int  element_type;
	int  rect_x;
	int  rect_y;
	int  rect_weight;
	int  rect_hight;
	int  fincout_x;
	int  findcut_y;
	int  findcut_size;
};


