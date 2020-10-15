	//【1】数据输入
	Mat srcImage, hsvImage;
	srcImage = imread(imgPath);

	if (!srcImage.data)
	{
		MessageBox(_T("读取图片失败！"));
		return;
	}
	cvtColor(srcImage, hsvImage, CV_BGR2GRAY);

	//【2】定义变量
	MatND dstHist;
	int dims = 1;
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges }; //需要为const类型
	int size = 256;
	int channels = 0;

	//【3】计算图像的直方图
	calcHist(&srcImage, 1, &channels, Mat(), dstHist, dims, &size, ranges);
	int scale = 1;

	Mat dstImage(size * scale, size, CV_8U, Scalar(0));
	//【4】获取最大值和最小值
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);

	int targetValue[256];
	int targetMinPosition = 0;
	int targetMaxValue = 0;
	int targetMaxPosition = 0;
	int targetPosition = 0;

	//【5】绘制出直方图
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);//hist中是float类型,而在OpenCV1.0版中用cvQueryHistValue_1D
		int realValue = saturate_cast<int>(binValue * hpt / maxValue);
		targetValue[i] = realValue;
		if (realValue > targetMaxValue)
		{
			targetMaxValue = realValue;
			targetMaxPosition = i;
		}
	}

	//从右往左第一个波峰
	for (int i = 255; i >= 0; i--)
	{
		if (targetValue[i - 1] < targetValue[i])
		{
			targetMinPosition = i;
			break;
		}
	}

	targetPosition = (targetMaxPosition + targetMinPosition) / 2;

	Mat originimg = srcImage.clone();

	for (int i = 0; i < originimg.rows; i++)
	{
		uchar* p = originimg.ptr(i);//获取Mat某一行的首地址  

		for (int j = 0; j < originimg.cols; j++)
		{
			//顺序是BGR,不是RGB
			int B = *(p + j * 3);
			int G = *(p + j * 3 + 1);
			int R = *(p + j * 3 + 2);

			if (!(R>targetPosition&& G>targetPosition && B>targetPosition))
			{
				*(p + j * 3) = 0;
				*(p + j * 3 + 1) = 0;
				*(p + j * 3 + 2) = 0;
			}
			else {
				*(p + j * 3) = 255;
				*(p + j * 3 + 1) = 255;
				*(p + j * 3 + 2) = 255;
			}
		}
	}


	//---------------------------------------------------------------漫水填充--------------------------------------------------
	Mat ime1 = originimg.clone();
	//漫水填充，随机找个点开始填充，当填充面积大于原图的1/2时退出
	bool bFlood = true;
	srand(GetTickCount());
	int i = ((rand() % 33) * (rand() % 33) * (rand() % 33)) % ime1.cols;
	srand(GetTickCount());
	int j = rand() % ime1.rows;
	Mat floodImg;
	Rect ccomp;
	while (bFlood)
	{
		floodImg = ime1.clone();
		uchar* p = floodImg.ptr(i);
		int R = *(p + j * 3);
		int G = *(p + j * 3 + 1);
		int B = *(p + j * 3 + 2);

		if (R == 0 && G == 0 && B == 0)
		{
			int  area = floodFill(floodImg, Point(i, j), Scalar(10, 10, 10), &ccomp, Scalar(), Scalar());
			if (area > floodImg.rows * floodImg.cols / 4)
			{
				ime1 = floodImg.clone();
				bFlood = false;
			}
		}

		srand(GetTickCount());
		i = ((rand() % 100) * (rand() % 100)) % floodImg.cols;
		srand(GetTickCount());
		j = ((rand() % 66) * (rand() % 66)) % floodImg.rows;
	}

	//非填充的点置成白色
	for (int i = 0; i < ime1.rows; i++)
	{
		uchar* p = ime1.ptr(i);//获取Mat某一行的首地址  

		for (int j = 0; j < ime1.cols; j++)
		{
			int R = *(p + j * 3);
			int G = *(p + j * 3 + 1);
			int B = *(p + j * 3 + 2);

			if (R != 10 && G != 10 && B != 10)
			{
				*(p + j * 3) = 255;
				*(p + j * 3 + 1) = 255;
				*(p + j * 3 + 2) = 255;
			}
			else {
				*(p + j * 3) = 0;
				*(p + j * 3 + 1) = 0;
				*(p + j * 3 + 2) = 0;
			}
		}
	}
	//-------------------------------------------------------------------------------开运算-----------------------------------------
	Mat openImgSrc = ime1.clone();
	Mat openOut;
	sliderNum = 0;
	//MORPH_RECT表示矩形的卷积核，自定义核还可以选择椭圆形的、交叉型的
	Mat element = getStructuringElement(MORPH_RECT, Size(60, 60));
	//高级形态学处理
	/*	MORPH_ERODE ：腐蚀运算
	MORPH_DILATE ：膨胀运算
	MORPH_OPEN： 开运算
	MORPH_CLOSE ：闭运算
	MORPH_GRADIENT： 形态学梯度
	MORPH_TOPHAT：顶帽运算
	MORPH_BLACKHAT： 黑帽运算
	MORPH_HITMISS : 击中击不中运算(只支持CV_8UC1类型的二值图像)*/
	morphologyEx(openImgSrc, openOut, MORPH_OPEN, element);
	Mat cannyOut;
	Canny(openOut, cannyOut, 50, 200, 3);
	//矩形检测
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;
	//矩形检测
	findContours(cannyOut, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	//对每个找到的轮廓创建可倾斜的边界框 
	vector<RotatedRect> minRect(contours.size());
	for (unsigned int i = 0; i < contours.size(); i++)
	{
		minRect[i] = minAreaRect(Mat(contours[i]));
	}

	CvBox2D  End_Rage2D;
	string angleStr;

	for (unsigned int i = 0; i< contours.size(); i++)
	{

		Point2f rect_points[4];
		minRect[i].points(rect_points);

		Rect silidRect;
		//筛选符合条件的标签（矩形）
		float a;
		if (minRect[i].size.width > minRect[i].size.height)
		{
			a = minRect[i].size.width / minRect[i].size.height;
		}
		else {
			a = minRect[i].size.height / minRect[i].size.width;
		}
		if (minRect[i].size.width  * minRect[i].size.height > 40000 &&
			minRect[i].size.width > 100 &&
			minRect[i].size.height > 100)
		{
			End_Rage2D = minAreaRect(Mat(contours[i]));
			float angle = End_Rage2D.angle;

			//顺时针倾斜：End_Rage2D.angle = 实际倾斜度数
			//逆时针倾斜：End_Rage2D.angle = -90 + 实际倾斜度数，Opencv的findContours很奇葩
			if (angle < -45)
			{
				angle += 90;
			}
			//小于0度(包括0)的时候各坐标点要变换才能得出ROI区域silidRect
			if (angle <= 0)
			{
				Point2f p2fTemp = rect_points[3];
				rect_points[3] = rect_points[2];
				rect_points[2] = rect_points[1];
				rect_points[1] = rect_points[0];
				rect_points[0] = p2fTemp;
			}

			sliderNum++;
			//切割1.1倍标签大小，防止倾斜大的标签内容会被截掉
			silidRect.width = (int)sqrt((pow((rect_points[0].x - rect_points[1].x), 2) + pow((rect_points[0].y - rect_points[1].y), 2)));
			silidRect.height = (int)sqrt((pow((rect_points[0].x - rect_points[3].x), 2) + pow((rect_points[0].y - rect_points[3].y), 2)));
			silidRect.x = (int)(rect_points[2].x - silidRect.width*(silieMul - 1) / 2);
			silidRect.y = (int)(rect_points[2].y - silidRect.height*(silieMul - 1) / 2);
			silidRect.width *= silieMul;
			silidRect.height *= silieMul;

			//切割超出边界会导致崩溃
			if (silidRect.x + silidRect.width > srcImage.cols ||
				silidRect.y + silidRect.height > srcImage.rows)
			{
				MessageBox(_T("致命错误：切割出错！！！"));
				return;
			}

			Mat sliderOutImg = srcImage(silidRect);

			memset(timeStrBuf, 0, 128);
			if (abs(angle) > rotateMinAngle)
			{
				imRoTate(sliderOutImg, sliderOutImg, angle);
			}
			else {
				sprintf(timeStrBuf, "标签%2d倾斜:%9f°，未矫正", sliderNum, angle);
			}
			sliderImg[i] = sliderOutImg.clone();
		}

	}
