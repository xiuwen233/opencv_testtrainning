#pragma once
#include<QThread>
#include<QImage>
#include<QMutex>
#include<opencv2\core\core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>


using namespace cv;

typedef struct CapturePragma
{
	int rect_x;
	int rect_y;
	int rect_weight;
	int rect_height;
	int i_threshold ;
	int out_mask_img_x;
	int out_mask_img_y;
	int back_small_x;
	int back_small_y;
}Capturepragma;

class CameraOne : public QThread
{
public:
	CameraOne();
	~CameraOne();
	virtual void run();
    bool SetCaptureIndex( int index);
	void Set_Show_Draw();  //画出匹配的框
	void Set_unShow_Draw();
	void SetCaptureInit(Capturepragma  capturepragma);  //初始化一些参数
	Point GetCenterPointer();  //得到匹配的坐标
	QImage GetFrame();
	void StopRun();   //关闭线程

private:
	int CaptureIndex;
	bool b_detect;
	int rect_x;
	int rect_y;
	int rect_weight;
	int rect_height;
	int threashold_one;
	Mat out_mask_img;
	int out_mask_img_x;
	int out_mask_img_y;
	Mat back_small;
	int back_small_x ;
	int back_samll_y ;
	Mat frame; 
    Mat tempige;
    VideoCapture capture;
	bool b_draw;
	Point center_point;
private:
	void Max_Rectrectangle_mask();
	void Back_mask();
};

