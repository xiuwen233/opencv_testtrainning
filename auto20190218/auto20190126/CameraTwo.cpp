#include"CameraTwo.h"

bool CameraTwo::SetCaptureIndex(int index)
{
	CaptureIndex = index;
    capture.open(CaptureIndex);
    bool isopenc = capture.isOpened();
    return isopenc;
}

void CameraTwo::SetCaptureInit(Capturepragmatwo  capturepragma)
{
	rect_x = capturepragma.rect_x;
	rect_y = capturepragma.rect_y;
	rect_weight = capturepragma.rect_weight;
	rect_height = capturepragma.rect_height;
	circle_mask_d = capturepragma.circle_mask_d;
	threashold_two = capturepragma.i_threshold;
	src_circle_center.x = capturepragma.circle_center_x;
	src_circle_center.y = capturepragma.circle_center_y;
    rectangle_mask_image_x = capturepragma.rectangle_mask_image_x;
    rectangle_mask_image_y = capturepragma.rectangle_mask_image_y;
}

//模版匹配 圆
Point MatchCircleTemplate(Mat bitwise_out, Mat templ, int circle_d)
{
	Mat g_resultImage;
	Mat srcImage;
	bitwise_out.copyTo(srcImage);
	int resultImage_cols = bitwise_out.cols - templ.cols + 1;
	int resultImage_rows = bitwise_out.rows - templ.rows + 1;
	g_resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);

	//进行匹配
	matchTemplate(bitwise_out, templ, g_resultImage, 4);

	//标准化
	normalize(g_resultImage, g_resultImage, 0, 2, NORM_MINMAX, -1, Mat());
	double minValue, maxValue;
	Point minLocation, maxLocation, matchLocation;

	//定位最匹配的位置
	minMaxLoc(g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation);
	if (4 == TM_SQDIFF || 4 == CV_TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	Point p_re;
	p_re.x = matchLocation.x + circle_d / 2;
	p_re.y = matchLocation.y + circle_d / 2;
	return p_re;
}

//创建第三个摄像头的掩码
void  CameraTwo::Circle_mask_made()
{
	Mat mask;
	mask = Mat(Size(circle_mask_d + 2, circle_mask_d + 2), CV_8UC1, Scalar::all(0));
	circle(mask, Point(circle_mask_d / 2, circle_mask_d / 2), circle_mask_d / 2, Scalar(255), 1);
	circle_mask_image = mask.clone();
	//imwrite("third.jpg", third_mask_circle);
}

//创建第三个摄像头的掩码
void CameraTwo::Rectangle_mask_made()
{
	Mat mask;
	mask = Mat(Size(rectangle_mask_image_x + 2, rectangle_mask_image_y + 2), CV_8UC1, Scalar::all(0));
	rectangle(mask, Point(0, 0), Point(rectangle_mask_image_x + 1, rectangle_mask_image_y + 1), Scalar(255), 1);
	rectangle_mask_image = mask.clone();
}


//模板匹配 rectangle 矩形
Point MatchRectangleTemplate(Mat bitwise_out, Mat templ)
{
	Mat g_resultImage;
	Mat srcImage;
	bitwise_out.copyTo(srcImage);
	int resultImage_cols = bitwise_out.cols - templ.cols + 1;
	int resultImage_rows = bitwise_out.rows - templ.rows + 1;
	g_resultImage.create(resultImage_cols, resultImage_rows, CV_32FC1);

	//进行匹配
	matchTemplate(bitwise_out, templ, g_resultImage, 4);

	//标准化
	normalize(g_resultImage, g_resultImage, 0, 2, NORM_MINMAX, -1, Mat());
	double minValue, maxValue;
	Point minLocation, maxLocation, matchLocation;

	//定位最匹配的位置
	minMaxLoc(g_resultImage, &minValue, &maxValue, &minLocation, &maxLocation);
	if (4 == TM_SQDIFF || 4 == CV_TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else
	{
		matchLocation = maxLocation;
	}

	Point p_re;
	p_re.x = matchLocation.x;
	p_re.y = matchLocation.y;
	return p_re;
}

void CameraTwo::run()
{
	b_detect = true;
	Mat src, out;
	Mat blue, wavefilter;
	Point center;
	int true_threshold;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	Circle_mask_made();
	Rectangle_mask_made();
	while (b_detect)
	{
		capture >> src;
		if (!src.data)
		{
			//图像获取错误
			break;
		}
		src = src(Rect(rect_x, rect_y, rect_weight,rect_height));
		out = src.clone();
		cvtColor(src, src, CV_RGB2GRAY);

		if (b_drawCircle)
		{
			circle(out, src_circle_center, circle_mask_d / 2, Scalar(0, 255, 255), 2);
		}
		
		if (b_checkPad)
		{
			medianBlur(src, src, 5);
			//imshow("videoimage2", videoimage2);
			adaptiveThreshold(src, blue, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 5);
			//  imshow("blue", blue);
			center = MatchCircleTemplate(blue, circle_mask_image, circle_mask_d);
			pap_center_pointer = center;
			circle(out, Point(center.x, center.y), circle_mask_d / 2, Scalar(255, 0, 255), 1);

		}
	
		if (b_ckeckChip)
		{
			true_threshold = threashold_two;
			if (true_threshold <= 0 || true_threshold >= 255)
			{
				true_threshold = 180;
			}
			threshold(src, blue, true_threshold, 255, CV_THRESH_BINARY);
			//imshow("blue", blue);
			morphologyEx(blue, wavefilter, MORPH_DILATE, element);
			//imshow("wavefilter", wavefilter);
			center = MatchRectangleTemplate(wavefilter, rectangle_mask_image);
			chip_center_pointer = center;
			rectangle(out, center, Point(center.x + rectangle_mask_image_x, center.y + rectangle_mask_image_y), Scalar(255, 0, 255), 1);
		}
        qmutex.lock();
		frame = out.clone();
        qmutex.unlock();
		waitKey(10);	
	}
}


QImage CameraTwo::GetFrame()
{
    qmutex.lock();
    tempige = frame.clone();
    qmutex.unlock();
    cvtColor(tempige, tempige, CV_BGR2RGB);
    QImage image((const uchar*)tempige.data, tempige.cols, tempige.rows, QImage::Format_RGB888);
	return image;
}

void CameraTwo::StopRun()
{
	b_detect = false;
}

void CameraTwo::Start_Check_Taping()
{
	b_checkPad = true;
}

void CameraTwo::Stop_Check_Taping()
{
	b_checkPad = false;
}

void CameraTwo::Start_Check_Chip()
{
	b_ckeckChip = true;
}

void CameraTwo::Stop_Check_Chip()
{
	b_ckeckChip = false;
}

void CameraTwo::Set_Show_draw()
{
	b_drawCircle = true;
}

void CameraTwo::Set_unShow_draw()
{
	b_drawCircle = false;
}

Point CameraTwo::Get_Pappoint()
{
	// Sleep(100);
	return pap_center_pointer;
}

Point CameraTwo::Get_Chippoint()
{  
	//Sleep(100);
	return chip_center_pointer;
}

CameraTwo::CameraTwo()
{
	b_checkPad = false;
	b_ckeckChip = false;
	b_drawCircle = false;
}

CameraTwo::~CameraTwo()
{
    capture.release();
}
