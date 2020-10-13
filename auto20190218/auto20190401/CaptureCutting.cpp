#include"CaptureCutting.h"

CaptureCutting::CaptureCutting()
{  
	runstatue = false;
	bSetRect = false;
	readmatchtemp = false;
	getmatchpoint = false;
	debugshow = false;
	writeimage = false;

	match_type = 3;
}

CaptureCutting::~CaptureCutting()
{
}

void CaptureCutting::SetCaptureIndex(int index)
{
	captureindex = index;
}

bool CaptureCutting::CheckCaptureAble()
{
	capture.open(captureindex);
	bool isopen = capture.isOpened();
	return isopen;
}

int CaptureCutting::RealeaseCapture()
{
	if (runstatue )
	{
		return -1;
	}
	capture.release();
	return 0;
}


cv::Point  MatchTempleImage(cv::Mat src, cv::Mat temple, int matchtype)
{
	cv::Mat result;
	int result_rows = src.rows - temple.rows + 1;
	int result_cols = src.cols - temple.cols + 1;
	result.create(result_cols, result_rows, CV_32FC1);
	matchTemplate(src, temple, result, matchtype);
	normalize(result, result, 0, 1, cv::NORM_MINMAX, -1, cv::Mat());
	double minVal = -1;
	double maxVal;
	cv::Point minLoc;
	cv::Point maxLoc;
	cv::Point matchLoc;
	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat());

	if (matchtype == cv::TM_SQDIFF || matchtype == cv::TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	//rectangle(src, matchLoc, cv::Point(matchLoc.x + temple.cols, matchLoc.y + temple.rows), Scalar(0, 255, 0), 2, 8, 0);
	//imshow("src", src);
	//return src;
	return matchLoc;
}


void CaptureCutting::TrueRun()
{ 
	cv::Mat frame;
	cv::Mat binnary;
	cv::Mat matchtemple;
	cv::Point matchpoint;
	runstatue = true;
	while (runstatue)
	{
		capture >> frame;

		if (!frame.data)
		{
			runstatue = false;
			break;
		}

		if (bSetRect)
		{
			frame = frame(cv::Rect(rect_x, rect_y, rect_width, rect_height));
		}

		if (readmatchtemp)
		{
			matchtemple = cv::imread("matchtemple.jpg");
			readmatchtemp = false;
		}

		if (getmatchpoint )
		{
			matchpoint = MatchTempleImage(frame, matchtemple, match_type);
			rectangle(frame, matchpoint, cv::Point(matchpoint.x + matchtemple.cols, matchpoint.y + matchtemple.rows), cv::Scalar(0, 255, 0), 2, 8, 0);
			getmatchpoint = false;
		}
		
		cutmutex.lock();
		frameout = frame.clone();
		cutmutex.unlock();

		if (writeimage)
		{
			cv::imwrite("src.jpg", frame);
			writeimage = false;
		}

		if (debugshow)
		{
			cv::imshow("frame",frame);
		}

		cv::waitKey(10);
	}

	cv::destroyAllWindows();
	capture.release();
}


void CaptureCutting::StartRun()
{
	std::thread t(&CaptureCutting::TrueRun, this);
	t.detach();
}

int CaptureCutting::ReadMatchTemple()
{
	if (_access("matchtemple.jpg", 0) != 0)
	{
		//printf("file is not exits \n");
		return -1;
	}
	else
	{
		//printf("file is exits \n");
		readmatchtemp = true;
		return 0;
	}
}

int CaptureCutting::SetRectROI(RectangleParam &rectroi)
{  
	if ((rectroi.rect_x + rectroi.rect_width) > CPATURE_WIDTH || (rectroi.rect_y + rectroi.rect_height) > CPATURE_HEIGHT)
	{
		return -1;
	}
	rect_x = rectroi.rect_x;
	rect_y = rectroi.rect_y;
	rect_width = rectroi.rect_width;
	rect_height = rectroi.rect_height;
	bSetRect = true;
	return 0;
}


int CaptureCutting::SetMatchType(int matchtype)
{
	if (0<matchtype || matchtype >5)
	{
		return -1;
	}
	match_type = matchtype;
	return 0;
}


int CaptureCutting::GetMatchPoint(cv::Point &matchpoint)
{
	getmatchpoint = true;
	for (int i = 0; i++; i<10)
	{
		if (false == getmatchpoint)
		{
			matchpoint.x = cmatchpoint.x;
			matchpoint.y = cmatchpoint.y;
			return 0;
		}
		Sleep(20);
	}
	return -1;
}

void CaptureCutting::StopRun()
{
	runstatue = false;
}

void CaptureCutting::StratDeBugShow()
{
	debugshow = true;
}

void CaptureCutting::StopDeBugShow()
{
	debugshow = false;
}

int CaptureCutting::WriteImageCurrent()
{
	writeimage = true;
	for (int i = 0; i++; i<10)
	{
		if (false == writeimage)
		{
			return 0;
		}
		Sleep(10);
	}
	return -1;
}


cv::Mat CaptureCutting::GetFrame()
{
	return frameout;
}


void CaptureCutting::UnSetRectROI()
{
	bSetRect = false;
}