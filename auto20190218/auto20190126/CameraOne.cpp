
#include"CameraOne.h"

bool CameraOne::SetCaptureIndex(int index)
{
	CaptureIndex = index;
    capture.open(CaptureIndex);
    bool isopenc = capture.isOpened();
    return isopenc;
}

void CameraOne::SetCaptureInit(Capturepragma  capturepragma) {
	rect_x = capturepragma.rect_x;
	rect_y = capturepragma.rect_y;
	rect_weight = capturepragma.rect_weight;
	rect_height = capturepragma.rect_height;
	threashold_one = capturepragma.i_threshold;
	out_mask_img_x = capturepragma.out_mask_img_x;
	out_mask_img_y = capturepragma.out_mask_img_y;
	back_small_x = capturepragma.back_small_x;
	back_samll_y = capturepragma.back_small_y;
}


//模板匹配 rectangle 矩形
Point MatchRectangleTemplateOne(Mat bitwise_out, Mat templ)
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


//创建一个最大外矩形
void CameraOne::Max_Rectrectangle_mask()
{
	Mat mask;
	mask = Mat(Size(out_mask_img_x + 2, out_mask_img_y + 2), CV_8UC1, Scalar::all(0));
	rectangle(mask, Point(1, 1), Point(out_mask_img_x + 1, out_mask_img_y + 1), Scalar(255), 1);
	out_mask_img = mask.clone();
}

//创建一个小黑的矩形
void CameraOne::Back_mask()
{
	Mat mask;
	mask = Mat(Size(back_small_x + 2, back_samll_y + 2), CV_8UC1, Scalar::all(0));
	rectangle(mask, Point(0, 0), Point(back_small_x + 1, back_samll_y + 1), Scalar(255), 1);
	back_small = mask.clone();
}


void CameraOne::run()
{
	b_detect = true;
	Mat src,out;
	Mat blue, binaryimage;
	Mat threImage, resultImage;
	Mat canny_image;
	int true_threshold;
	Point center ;
	Point src_location;
	Point point_src;
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));

	while (b_detect)
	{ 
		capture >> src;
		if (!src.data)
		{  
			//如何报警没有摄像头
			break;
		}
		src = src(Rect(rect_x, rect_y, rect_weight, rect_height));
		cvtColor(src, binaryimage, CV_RGB2GRAY);
		true_threshold = threashold_one;
		if (true_threshold <= 0 || true_threshold >= 255)
		{
			true_threshold = 180;
		}
		threshold(binaryimage, threImage, true_threshold, 255, CV_THRESH_BINARY);
		resultImage = threImage.clone();
		morphologyEx(threImage, out, MORPH_ERODE, element);
		//imshow("threshold", out);
		Canny(out, canny_image, 50, 100, 3);
		//imshow("src_xytr", canny_image);
		center = MatchRectangleTemplateOne(canny_image, out_mask_img);
		//rectangle(VideoImage, center, Point(center.x + i_img_mask_max_rect_x, center.y + i_img_mask_max_rect_y), Scalar(255, 0, 255), 1);
		resultImage = resultImage(Rect(center.x, center.y, out_mask_img_x, out_mask_img_y));
		src_location = MatchRectangleTemplateOne(resultImage, back_small);
		point_src.x = src_location.x + center.x;
		point_src.y = src_location.y + center.y;
		center_point = point_src;

		if (b_draw)
		{
			rectangle(src, point_src, Point(point_src.x + back_small_x, point_src.y + back_samll_y), Scalar(255, 0, 255), 1);
		}
        qmutex.lock();
		frame = src.clone();
        qmutex.unlock();
        waitKey(10);
		//circle(out, Point(center.x, center.y), third_mask_circle_d / 2, Scalar(255, 0, 255), 1);
	}
}


QImage CameraOne::GetFrame()
{  
    qmutex.lock();
    tempige = frame.clone();
    qmutex.unlock();

    cvtColor(tempige, tempige, CV_BGR2RGB);
    QImage image((const uchar*)tempige.data, tempige.cols, tempige.rows, QImage::Format_RGB888);
	return image;
}


void CameraOne::Set_Show_Draw()
{
	b_draw = true;
}

void CameraOne::Set_unShow_Draw()
{
	b_draw = false;
}

Point CameraOne::GetCenterPointer()
{
	//Sleep(100);
	return center_point;
}

void CameraOne::StopRun()
{
	b_detect = false;
}

CameraOne::CameraOne()
{
	b_draw = false;
}

CameraOne::~CameraOne()
{
    capture.release();
}
