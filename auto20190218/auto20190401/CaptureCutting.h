#pragma once
#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<io.h>
#include<Windows.h>
#include<mutex>
#include<thread>

#define   CPATURE_WIDTH              640
#define   CPATURE_HEIGHT            480


typedef struct  RectangleParam {
	int rect_x;
	int rect_y;
	int rect_width;
	int rect_height;
}RectangleParam;


class CaptureCutting
{
public:
	CaptureCutting();
	~CaptureCutting();

	void SetCaptureIndex(int index);
	bool CheckCaptureAble();
	int RealeaseCapture();

	void StartRun();
	int ReadMatchTemple();

	int SetRectROI(RectangleParam &rectroi);
	void UnSetRectROI();
	int SetMatchType(int matchtype);

	int GetMatchPoint(cv::Point &matchpoint);
	cv::Mat GetFrame();
	void StopRun();

	void StratDeBugShow();
	void StopDeBugShow();
	
	int WriteImageCurrent();

private:
	int captureindex;
	bool runstatue;
	bool debugshow;
	bool writeimage;
	std::mutex cutmutex;
	void TrueRun();

private:
	cv::VideoCapture capture;
	cv::Mat frameout;
	bool bSetRect;
	int rect_x;
	int rect_y;
	int rect_width;
	int rect_height;
	int match_type;

	bool readmatchtemp;
	bool getmatchpoint;

	cv::Point cmatchpoint;
};

