

Mat black_recentangle_mask;
int black_recentangle_mask_x = 44;
int black_recentangle_mask_y = 9;
//创建第三个摄像头的掩码
void Third_rentangle_mask_made()
{
	Mat mask;
	mask = Mat(Size(black_recentangle_mask_x + 4, black_recentangle_mask_y + 4), CV_8UC1, Scalar::all(0));
	rectangle(mask, Point(0,0 ), Point(black_recentangle_mask_x + 3, black_recentangle_mask_y + 3), Scalar(255), 1);
	rectangle(mask, Point(1,1 ), Point(black_recentangle_mask_x + 2, black_recentangle_mask_y + 2), Scalar(255), 1);
	black_recentangle_mask = mask.clone();
	imwrite("rtgffFFF.jpg", black_recentangle_mask);
}


int main()
{
	Mat videoimage;
	Mat videoimage2;
	Mat filter, canny, wavefilter;
	Mat blue, out;
	Point center;
	InitControlBoard();
	int true_threshold = 0;
	VideoCapture  capture2(2);
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	CvBox2D rect;
	CvPoint2D32f Corners[4];
	vector<Vec4i> lines;
	Third_rentangle_mask_made();
	while(1)
	{
		capture2 >> videoimage2;
		videoimage2 = videoimage2(Rect(110, 50, 360, 360));
		out = videoimage2.clone();
		cvtColor(videoimage2, videoimage2, CV_RGB2GRAY);
		imshow("videoimage2", videoimage2);

		true_threshold = i_threshold;
		if (true_threshold <= 0 || true_threshold >= 255)
		{
			true_threshold = 180;
		}
		threshold(videoimage2, blue, true_threshold, 255, CV_THRESH_BINARY);
		imshow("blue",blue);
		morphologyEx(blue, wavefilter, MORPH_DILATE, element);
		imshow("wavefilter", wavefilter);
		center = MatchSmallRectangleTemplate(wavefilter, black_recentangle_mask);
		if (abs(center.x - 226) < 6  && abs(center.y - 87) < 6 )
		{
			rectangle(out, center, Point(center.x + black_recentangle_mask_x, center.y + black_recentangle_mask_y), Scalar(255, 0, 255), 1);
			printf(" OK    \n");
		}
		else if (abs(center.y - 87) < 6)
		{  
			rectangle(out, center, Point(center.x + black_recentangle_mask_x, center.y + black_recentangle_mask_y), Scalar(255, 0, 255), 1);
			printf(" we nend move  %d   \n", center.x - 223);
		}
		else
		{
			printf(" error    put out    \n");
		}
		
		
		imshow("out",out);
		waitKey(10);
	}
	return 0;
}


//-------------------------------------

//opencv控制面板响应函数
static void ContrastAndBright(int, void *)
{
	Mat m = Mat(800, 600, 0);
	Max_Rectrectangle_mask();
	imshow("【控制面板】", m);
}

//创建一个控制面板，用来拖动控制响应值
void InitControlBoard()
{
	namedWindow("【控制面板】", 0);
	createTrackbar("二值化阈值：", "【控制面板】", &i_threshold, 256, ContrastAndBright);
	createTrackbar("初始点x：", "【控制面板】", &i_rect_max_x, 300, ContrastAndBright);
	createTrackbar("初始点y：", "【控制面板】", &i_rect_max_y, 300, ContrastAndBright);
	createTrackbar("矩形宽度：", "【控制面板】", &i_img_mask_max_rect_x, 256, ContrastAndBright);
	createTrackbar("矩形高度：", "【控制面板】", &i_img_mask_max_rect_y, 256, ContrastAndBright);
	createTrackbar("第二张二值化阈值：", "【控制面板】", &i_second_threshold, 256, ContrastAndBright);
	createTrackbar("thirdvideo_xx：", "【控制面板】", &thirdvideo_xx, 256, ContrastAndBright);
	createTrackbar("thirdvideo_yy：", "【控制面板】", &thirdvideo_yy, 256, ContrastAndBright);
	createTrackbar("thirdvideo_ww：", "【控制面板】", &thirdvideo_ww, 500, ContrastAndBright);
	createTrackbar("thirdvideo_hh：", "【控制面板】", &thirdvideo_hh,500, ContrastAndBright);
}


Mat third_mask_circle;
int  third_mask_circle_d = 30;

//创建第三个摄像头的掩码
void Third_mask_made()
{
	Mat mask;
	mask = Mat(Size(third_mask_circle_d + 2, third_mask_circle_d + 2), CV_8UC1, Scalar::all(0));
	circle(mask, Point(third_mask_circle_d / 2, third_mask_circle_d / 2), third_mask_circle_d / 2, Scalar(255), 1);
	third_mask_circle = mask.clone();
	//imwrite("third.jpg", third_mask_circle);
}



//计算并返回一幅图像的亮度
void GetBrightness(Mat InputImg, float& cast, float& f_dark_quantity)
{
	Mat GRAYimg;
	cvtColor(InputImg, GRAYimg, CV_BGR2GRAY);
	float a = 0;
	int Hist[256];
	for (int i = 0; i<256; i++)
		Hist[i] = 0;
	for (int i = 0; i<GRAYimg.rows; i++)
	{
		for (int j = 0; j<GRAYimg.cols; j++)
		{
			a += float(GRAYimg.at<uchar>(i, j) - 128);//在计算过程中，考虑128为亮度均值点  
			int x = GRAYimg.at<uchar>(i, j);
			Hist[x]++;
		}
	}
	f_dark_quantity = a / float(GRAYimg.rows*InputImg.cols);
	float D = abs(f_dark_quantity);
	float Ma = 0;
	for (int i = 0; i<256; i++)
	{
		Ma += abs(i - 128 - f_dark_quantity)*Hist[i];
	}
	Ma /= float((GRAYimg.rows*GRAYimg.cols));
	float M = abs(Ma);
	float K = D / M;
	cast = K;
	return;
}


int main()
{
	Mat videoimage;
	Mat videoimage2;
	Mat filter, canny, wavefilter;
	Mat blue, out;
	Point center;

	InitControlBoard();
	VideoCapture  capture2(2);
	vector<Vec4i> lines;

	Third_mask_made();

	int true_threshold = 0;
	Mat element = getStructuringElement(MORPH_RECT, Size(3, 3));
	while (1)
	{

		capture2 >> videoimage2;
		
		if (!videoimage2.data)
		{
			printf("error \n");
			break;
		}
		videoimage2 = videoimage2(Rect(110,50,360,360));
		out = videoimage2.clone();
		cvtColor(videoimage2, videoimage2, CV_RGB2GRAY);
		medianBlur(videoimage2, videoimage2, 5);
		imshow("videoimage2", videoimage2);

		adaptiveThreshold(videoimage2, blue, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY_INV, 3, 5);
		imshow("blue", blue);
	    
		Canny(blue, filter, 50, 200, 5);
		imshow("filter", filter);

	    center = MatchCircleTemplate(blue, third_mask_circle);
		circle(out, Point(center.x, center.y), third_mask_circle_d / 2, Scalar(255,0,255), 1);
		if ( abs(center.x - 180)<5 && abs(center.y - 180)<5)
		{
			printf("OK \n");
		}
		else if (abs(center.y - 179)<10)
		{
			printf("we need move %d \n", center.x - 153);
		}
		else {
			printf("error \n");
		}
		
		imshow("out", out);
	
		waitKey(10);
	}
	getchar();
	return 0;
}

//----------------------------------------------------------------------------------------------------------------------

int i_rect_max_x = 204;
int i_rect_max_y = 76;
int i_rect_max_w = 260;
int i_rect_max_h = 260;

Mat out_mask_img;
int i_img_mask_max_rect_x = 175;
int i_img_mask_max_rect_y = 181;

//创建一个最大外矩形
void Max_Rectrectangle_mask()
{
	Mat mask;
	mask = Mat(Size(i_img_mask_max_rect_x + 2, i_img_mask_max_rect_y + 2), CV_8UC1, Scalar::all(0));
	rectangle(mask, Point(1, 1), Point(i_img_mask_max_rect_x+1 , i_img_mask_max_rect_y+1 ), Scalar(255), 1);
	out_mask_img = mask.clone();
}

Mat back_small;
int back_small_x = 35;
int back_samll_y = 8;

//创建一个小黑的矩形
void Back_mask()
{
	Mat mask;
	mask = Mat(Size(back_small_x + 2, back_samll_y + 2), CV_8UC1, Scalar::all(0));
	rectangle(mask, Point(0, 0), Point(back_small_x+1 , back_samll_y+1 ), Scalar(255), 1);
	back_small = mask.clone();
}


int main()
{
	 Mat  VideoImage, binaryimage, threImage, out, canny_image;
	 Mat  resultImage, restIm_binary, resuIm_threshold;
	 Point  center, src_location;
	 int true_threshold = 0;
	 VideoCapture capture(1);

	 InitControlBoard();
	 Mat element = getStructuringElement(MORPH_RECT, Size(5, 5));
	 Max_Rectrectangle_mask();
	 Back_mask();

	 while (1)
	 {
		 capture >> VideoImage;
		 VideoImage = VideoImage(Rect(i_rect_max_x, i_rect_max_y, i_rect_max_w, i_rect_max_h));
		 resultImage = VideoImage.clone();
		 cvtColor(VideoImage, binaryimage, CV_RGB2GRAY);	
		 true_threshold = i_threshold;
		 if (true_threshold <= 0 || true_threshold >= 255)
		 {
			 true_threshold = 180;
		 }
		 threshold(binaryimage, threImage, true_threshold, 255, CV_THRESH_BINARY);
		 morphologyEx(threImage, out, MORPH_ERODE, element);
		 imshow("threshold", threImage);
		 Canny(out, canny_image, 50, 100, 3);
		 imshow("src_xytr", canny_image);
		 center = MatchMaxRectangleTemplate(canny_image, out_mask_img); 																
		 rectangle(VideoImage, center, Point(center.x + i_img_mask_max_rect_x, center.y + i_img_mask_max_rect_y), Scalar(255, 0, 255), 1);
		 imshow("src", VideoImage);

		 resultImage = resultImage(Rect(center.x, center.y, i_img_mask_max_rect_x, i_img_mask_max_rect_y));
		 cvtColor(resultImage, restIm_binary, CV_RGB2GRAY);
		 threshold(restIm_binary, resuIm_threshold, 180, 255, CV_THRESH_BINARY);
		 imshow("resultImage", resuIm_threshold);
		 src_location = MatchMaxRectangleTemplate(resuIm_threshold, back_small);
		 rectangle(resultImage, src_location, Point(src_location.x + back_small_x, src_location.y + back_samll_y), Scalar(255, 0, 255), 1);
		// printf("src_location.x = %d，src_location.y = %d  center.x =%d   center,y = %d\n", src_location.x, src_location.y, center.x, center.y);

		 if (abs(src_location.x - 176)> 10 )
		 {
			 printf("X轴相差 %d \n", (src_location.x - 176)/10);
		 }
		 if (abs(src_location.y - 85 )>10)
		 {
			 printf("Y轴相差 %d \n", (src_location.y - 85) / 10);
		 }
	/*	 if (src_location.y - src_location.x  >140 && src_location.y - src_location.x <160)
		 {
			 printf("左下角  \n");
		 }
		 else if (src_location.x - src_location.y > 90 && src_location.y < 40)
		 {
			 printf("右上角  \n");
		 }
		 else {
			 printf("检测错误  \n");
		 }*/

		 imshow("back", resultImage);
		 waitKey(20);
	 }
	 return 0;
}
