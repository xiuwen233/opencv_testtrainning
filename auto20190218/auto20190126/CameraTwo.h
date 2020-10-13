#pragma once
#include<QThread>
#include<QImage>
#include<QMutex>
#include<opencv2\core\core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>
//#include <synchapi.h>

using namespace cv;

typedef struct CapturePragmaTwo
{
	int rect_x;
	int rect_y;
	int rect_weight;
	int rect_height;
	int i_threshold;
	int circle_mask_d;
    int circle_center_x ;
	int circle_center_y ;
    int rectangle_mask_image_x;
    int rectangle_mask_image_y;
}Capturepragmatwo;


class CameraTwo
{
public:
	CameraTwo();
	~CameraTwo();
	virtual void run();
    bool SetCaptureIndex(int index);
	void Start_Check_Taping(); //�����
    void Stop_Check_Taping();
	void Start_Check_Chip();   //���оƬ
	void Stop_Check_Chip();
    void Set_Show_draw();  //��ʾ����ɫ��
	void Set_unShow_draw();
	void SetCaptureInit(Capturepragmatwo  capturepragma);  //��ʼ��һЩ����
	Point Get_Pappoint();  //�õ�����ļ������
	Point Get_Chippoint();  //�õ�оƬ�ļ������
	QImage GetFrame();
	void StopRun();   //�ر��߳�
private:
	int CaptureIndex;
	bool b_detect;
	int rect_x;
	int rect_y;
	int rect_weight;
	int rect_height;
	int threashold_two;
	
	Mat circle_mask_image;
	int circle_mask_d;
	Mat rectangle_mask_image;
	int rectangle_mask_image_x;
	int rectangle_mask_image_y;

	Mat frame;
    Mat tempige;
    VideoCapture capture;
    QMutex qmutex;

	Point src_circle_center;
	Point pap_center_pointer;
	Point chip_center_pointer;

	bool b_checkPad;
	bool b_ckeckChip;
	bool b_drawCircle;
private :
	void  Circle_mask_made();
	void  Rectangle_mask_made();
};


