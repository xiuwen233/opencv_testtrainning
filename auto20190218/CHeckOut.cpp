#include "CHeckOut.h"

bool VideoCheck::OpenVideo(int index)
{
	VideoIndex = index;
	capture.open(VideoIndex);
	bool isopen = capture.isOpened();
	return isopen;
}

void VideoCheck::VideoInit(VideoInitParam videoparam)
{
	rect_x = videoparam.rect_x;
	rect_y = videoparam.rect_y;
	rect_weight = videoparam.rect_weight;
	rect_height = videoparam.rect_height;
	i_threshold = videoparam.i_threshold;

	rect_pa_x = videoparam.rect_pa_x;
	rect_pa_y = videoparam.rect_pa_y;
	rect_pa_weight = videoparam.rect_pa_weight;
	rect_pa_height = videoparam.rect_pa_height;

	animal_mask_image_x = videoparam.animal_mask_image_x;
	animal_mask_image_y = videoparam.animal_mask_image_y;
	pallet_threshold = videoparam.pallet_threshold;
}


void VideoCheck::Rectangle_animal_mask_made()
{
	Mat mask;
	mask = Mat(Size(animal_mask_image_x + 2, animal_mask_image_y + 2), CV_8UC1, Scalar::all(0));
	rectangle(mask, Point(0, 0), Point(animal_mask_image_x + 1, animal_mask_image_y + 1), Scalar(255), 1);
	animal_mask_image = mask.clone();
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


int VideoCheck::StartRun()
{
	Mat src;
	Mat binary;
	Mat mthresd;
	Mat luo;
	Mat linary;
	Rectangle_animal_mask_made();
	Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	int true_threa;
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	CvBox2D rect;
	CvPoint2D32f Corners[4];

	while (b_detect)
	{
		capture >> src;
		if (!src.data)
		{
			capture.release();
			destroyAllWindows();
			return -1;
		}

		if(b_CheckCassette == true && b_CheckPallet == false)
		{
			src = src(Rect(rect_x, rect_y, rect_weight, rect_height));

			cvtColor(src, binary, CV_RGB2GRAY);
			true_threa = i_threshold;
			if (true_threa <= 0 || true_threa >= 255)
			{
				true_threa = 180;
			}
			threshold(binary, mthresd, true_threa, 255, CV_THRESH_BINARY);
			morphologyEx(mthresd, luo, MORPH_DILATE, element);

			findContours(luo, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point()); //CV_RETR_EXTERNAL
			for (int i = 0; i < contours.size(); i++)
			{
				rect = minAreaRect((contours[i]));

				cvBoxPoints(rect, Corners);
				if (rect.size.width  * rect.size.height > 10000 &&
					rect.size.width > 100 &&
					rect.size.height > 100) {

					angle = rect.angle;
					center.x = rect.center.x;
					center.y = rect.center.y;

					line(src, Point((int)Corners[0].x, (int)Corners[0].y), Point((int)Corners[1].x, (int)Corners[1].y), Scalar(255, 0, 255), 2, 8, 0);
					line(src, Point((int)Corners[1].x, (int)Corners[1].y), Point((int)Corners[2].x, (int)Corners[2].y), Scalar(255, 0, 255), 2, 8, 0);
					line(src, Point((int)Corners[2].x, (int)Corners[2].y), Point((int)Corners[3].x, (int)Corners[3].y), Scalar(255, 0, 255), 2, 8, 0);
					line(src, Point((int)Corners[0].x, (int)Corners[0].y), Point((int)Corners[3].x, (int)Corners[3].y), Scalar(255, 0, 255), 2, 8, 0);
				}
			}
		}
	   
		if (b_CheckPallet == true && b_CheckCassette == false)
		{    
			src = src(Rect(rect_pa_x, rect_pa_y, rect_pa_weight, rect_pa_height));
			cvtColor(src, linary, CV_RGB2GRAY);
			//imshow("image", linary);
			threshold(linary, mthresd, pallet_threshold, 255, CV_THRESH_BINARY);
			palletcenter = MatchRectangleTemplate(mthresd, animal_mask_image);
			rectangle(src, center, Point(center.x + animal_mask_image_x, center.y + animal_mask_image_y), Scalar(255, 0, 255), 1);
		}
		
		imshow("src", src);
		waitKey(10);
	}
	destroyAllWindows();
	return 0;
}

void  VideoCheck::StartCheckCassette()
{
	b_CheckCassette = true;
}

void  VideoCheck::StopCheckCassette()
{
	b_CheckCassette = false;
}

void  VideoCheck::StartCheckPallet()
{
	b_CheckPallet = true;
}

void  VideoCheck::StopCheckPallet()
{
	b_CheckPallet = false;
}

int VideoCheck::GetResult(ResultParam * result)
{
	result->angle = angle;
	result->center_x = center.x;
	result->center_y = center.y;
}

Point  VideoCheck::GetPalletPoint()
{
	return palletcenter;
}

void  VideoCheck::StopRun()
{
	b_detect = false;
}

VideoCheck::VideoCheck()
{
	b_detect = true;
	b_CheckCassette = false;
	b_CheckPallet = false;
}

VideoCheck::~VideoCheck()
{
	capture.release();
}
