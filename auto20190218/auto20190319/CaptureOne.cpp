#include"CaptureOne.h"

CaptureOne::CaptureOne()
{
	is_detect = true;
	is_drawresult = false;
	is_writematchtemp = false;
	is_calpashcode = false;
	is_setrectrangle = false;
	is_calhamdis = false;
	is_getchippostion = false;
	is_runing = false;
	is_isp = false;
	threshold_value = 180;
	element_type = 0;
	fincout_x = 100;
	findcut_y = 100;
	findcut_size = 10000;
}

CaptureOne::~CaptureOne()
{
	is_detect = false;
}

int  CaptureOne::CaptureIndex(int index)
{
	captureindex = index;
	return 0;
}

bool CaptureOne::iscapture_openOK()
{
	capture.open(captureindex);
	bool isopen = capture.isOpened();
	is_capturerun = isopen;
	return isopen;
}

int  CaptureOne::capturerealse()
{
	if (true == is_runing)
	{
		return -1;
	}
	capture.release();
	return 0;
}

GetPointRect CaptureOne::GetTheRectRangle()
{
	GetPointRect  getpointrect;
	if (corner_point[0].x > corner_point[1].x)
	{
		getpointrect.rect_x = corner_point[1].x;
	}
	else {
		getpointrect.rect_x = corner_point[0].x;
	}

	if (corner_point[1].y > corner_point[2].y)
	{
		getpointrect.rect_y = corner_point[2].y;
	}
	else {
		getpointrect.rect_y = corner_point[1].y;
	}

	if (corner_point[2].x > corner_point[3].x)
	{
		getpointrect.rect_weight = corner_point[2].x - getpointrect.rect_x;
	}
	else
	{
		getpointrect.rect_weight = corner_point[3].x - getpointrect.rect_x;
	}

	if (corner_point[0].y > corner_point[3].y)
	{
		getpointrect.rect_hight = corner_point[0].y - getpointrect.rect_y;
	}
	else {
		getpointrect.rect_hight = corner_point[3].y - getpointrect.rect_y;
	}

	return getpointrect;
}

Mat calPHashCode(Mat image)
{
	Mat floatImage, imageDct;
	resize(image, image, Size(32, 32));
	image.convertTo(floatImage, CV_32FC1);
	dct(floatImage, imageDct);
	Rect roi(0, 0, 8, 8);
	Scalar imageMean = mean(imageDct(roi));
	return (imageDct(roi) > imageMean[0]);
}

int calHammingDistance(Mat modelHashCode, Mat testHashCode)
{
	return countNonZero(modelHashCode != testHashCode);
}

void  CaptureOne::trueRun()
{
	Mat frame, fghher, binner, getrch;
	Mat src, out;
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));

	Mat str1;
	Mat str2;

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	CvBox2D rect;
	CvPoint2D32f Corners[4];

	GetPointRect ResultRect;
	is_detect = true;

	bool is_fileexist;
	bool is_findcouter = false;

	if (_access("matchtemp.jpg", 0) != 0)
	{
		is_fileexist = false;
	}
	else {
		is_fileexist = true;
	}
	is_runing = true;
	printf("ready to start \n");
	while (is_detect)
	{
		capture >> frame;
		if (!frame.data)
		{
			is_capturerun = false;
			is_runing = false;
			break;
		}

		if (is_calpashcode)
		{
			Mat result_temp = imread("matchtemp.jpg", 0);
			str1 = calPHashCode(result_temp);
			is_calpashcode = false;
		}

		if (is_setrectrangle)
		{
			frame = frame(Rect(rect_x, rect_y, rect_weight, rect_hight));
		}
		src = frame.clone();
		cvtColor(frame, fghher, CV_RGB2GRAY);
		out = fghher.clone();
		threshold(fghher, binner, threshold_value, 255, CV_THRESH_BINARY);
		if (is_isp)
		{
			imshow("binner", binner);
		}
		morphologyEx(binner, getrch, element_type, element);
		if (is_isp)
		{
			imshow("getrch", getrch);
		}
		findContours(getrch, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point()); //CV_RETR_EXTERNAL
		for (int i = 0; i < contours.size(); i++)
		{
			rect = minAreaRect((contours[i]));

			cvBoxPoints(rect, Corners);
			if (rect.size.width  * rect.size.height > findcut_size &&
				rect.size.width > fincout_x &&
				rect.size.height > findcut_y) {

				if (is_drawresult)
				{
					line(frame, Point((int)Corners[0].x, (int)Corners[0].y), Point((int)Corners[1].x, (int)Corners[1].y), Scalar(255, 0, 255), 2, 8, 0);
					line(frame, Point((int)Corners[1].x, (int)Corners[1].y), Point((int)Corners[2].x, (int)Corners[2].y), Scalar(255, 0, 255), 2, 8, 0);
					line(frame, Point((int)Corners[2].x, (int)Corners[2].y), Point((int)Corners[3].x, (int)Corners[3].y), Scalar(255, 0, 255), 2, 8, 0);
					line(frame, Point((int)Corners[0].x, (int)Corners[0].y), Point((int)Corners[3].x, (int)Corners[3].y), Scalar(255, 0, 255), 2, 8, 0);
				}

				if (is_getchippostion)
				{
					findchippostion.angel = rect.angle;
					findchippostion.center_x = (int)rect.center.x;
					findchippostion.center_y = (int)rect.center.y;
					is_getchippostion = false;
				}

				for (int j = 0; j < 4; j++)
				{
					corner_point[j].x = (int)Corners[j].x;
					corner_point[j].y = (int)Corners[j].y;
				}

				ResultRect = GetTheRectRangle();

				if (is_writematchtemp)
				{
					imwrite("matchtemp.jpg", src(Rect(ResultRect.rect_x, ResultRect.rect_y, ResultRect.rect_weight, ResultRect.rect_hight)));
					is_writematchtemp = false;
					is_fileexist = true;
				}

				is_findcouter = true;
			}
		}

		if (true == is_findcouter && true == is_calhamdis && is_fileexist == true)
		{
			out = out(Rect(ResultRect.rect_x, ResultRect.rect_y, ResultRect.rect_weight, ResultRect.rect_hight));
			str2 = calPHashCode(out);
			hammingdistance = calHammingDistance(str1, str2);
			is_calhamdis = false;
		}
		is_findcouter = false;

		camutex.lock();
		frameout = frame.clone();
		camutex.unlock();
		if (is_isp)
		{
			imshow("frame", frame);
		}
		
		waitKey(10);
	}

	cv::destroyAllWindows();
	capture.release();
}

void  CaptureOne::startrun()
{  
	printf("start thread \n");
	thread t(&CaptureOne::trueRun,this);
	t.detach();
}

bool CaptureOne::capturestatus()
{
	return is_capturerun;
}

void  CaptureOne::stopCapture()
{
	is_detect = false;
}

void CaptureOne::StartIsp()
{
	is_isp = true;
}

void CaptureOne::StopIsp()
{
	is_isp = false;
}

void CaptureOne::SetRectRangle(GetPointRect rectrangle)
{
	rect_x = rectrangle.rect_x;
	rect_y = rectrangle.rect_y;
	rect_weight = rectrangle.rect_weight;
	rect_hight = rectrangle.rect_hight;
	is_setrectrangle = true;
}

void CaptureOne::SetSrcImage()
{
	is_setrectrangle = false;
}

int  CaptureOne::SetPhologyType(int type)
{
	if (type<0 || type >5)
	{
		return -1;
	}
	element_type = type;
	return 0;
}

int CaptureOne::SetThreshold(int mythreshold)
{
	if (mythreshold<1 || mythreshold > 254)
	{
		return -1;
	}
	threshold_value = mythreshold;
	return 0;
}

void CaptureOne::SetDrawResult()
{
	is_drawresult = true;
}

void CaptureOne::StopDrawResult()
{
	is_drawresult = false;
}

void  CaptureOne::SetFindCouter(FindCouterRangle findcouterrangle)
{
	fincout_x = findcouterrangle.fincout_x;
	findcut_y = findcouterrangle.fincout_y;
	findcut_size = findcouterrangle.findcut_size;
}

int CaptureOne::CalSrc_PHashCode()
{
	if (_access("matchtemp.jpg", 0) != 0)
	{
		return -1;
	}
	is_calpashcode = true;
	return 0;
}

int  CaptureOne::WriteMatchTemp()
{
	is_writematchtemp = true;
	for (int i = 0; i < 10; i++)
	{
		if (false == is_writematchtemp)
		{
			return 0;
		}
		Sleep(30);
	}
	return -1;
}

int CaptureOne::getHammingDistance(int * d)
{
	is_calhamdis = true;
	for (int i = 0; i < 10; i++)
	{
		if (false == is_calhamdis)
		{
			*d = hammingdistance;
			return 0;
		}
		Sleep(50);
	}
	return -1;
}

int CaptureOne::getSingleRectangle(FindChipPostion * getpostion)
{
	is_getchippostion = true;
	for (int i = 0; i < 10; i++)
	{
		if (false == is_getchippostion)
		{
			getpostion->angel = findchippostion.angel;
			getpostion->center_x = findchippostion.center_x;
			getpostion->center_y = findchippostion.center_y;
			return 0;
		}
		Sleep(30);
	}
	return -1;
}

Mat   CaptureOne::getFrameOut()
{
	camutex.lock();
	frameouttemp = frameout.clone();
	camutex.unlock();
	return frameouttemp;
}
