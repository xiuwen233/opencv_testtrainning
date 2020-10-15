void Test33()
{
	Mat srcImage;
	Mat slideImeg[100];
	Mat colourImeg[100];
	Mat imme;
	srcImage = imread("E://photo_opencv/pic_temp/678.jpg");
	//Mat binImage = imread("E://photo_opencv/pic_temp/456.jpg", 0);
	imme = srcImage.clone();
	if (!srcImage.data)
	{
		//MessageBox(_T("读取图片失败！"));
		return;
	}
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
	Mat afterImage(size * scale, size, CV_8U, Scalar(0));
	//【4】获取最大值和最小值
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);

	int targetMinPosition = 0;
	int targetMaxValue = 0;
	int targetMaxPosition = 0;
	int targetPosition = 0;

	//【5】绘制出直方图
	int hpt = saturate_cast<int>(0.9 * size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);//hist中是float类型,而在OpenCV1.0版中用cvQueryHistValue_1D
		int realValue1 = saturate_cast<int>(binValue * hpt / maxValue);
		rectangle(dstImage, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue1), Scalar(255));
	}
	   

	int realValue_tmp1;
	int realValue_tmp2;
	int realValue_tmp3;
	int realValue[256];

	for (int i = 0; i < 256; i++)
	{
		realValue[i] = saturate_cast<int>(dstHist.at<float>(i) * hpt / maxValue);
	}


	//直方图滤波（简单的中值滤波）  滤波10次
	for (int k = 0; k < 10; k++)
	{
		realValue[0] = (realValue[0] + realValue[0] + realValue[1]) / 3;

		for (int i = 1; i < 255; i++)
		{
			realValue_tmp1 = realValue[i - 1];
			realValue_tmp2 = realValue[i];
			realValue_tmp3 = realValue[i + 1];
			realValue[i] = (realValue_tmp1 + realValue_tmp2 + realValue_tmp3) / 3;
		}
		realValue[255] = (realValue[254] + realValue[255] + realValue[255]) / 3;
	}


	for (int i = 0; i < 256; i++)
	{
		rectangle(afterImage, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue[i]), Scalar(255));
	}



	// 滤波后最大值
	for (int i = 255; i >= 0; i--)
	{
		if (realValue[i]> targetMaxValue)
		{
			targetMaxValue = realValue[i];
			targetMaxPosition = i;
		}
	}

	//从右往左第一个波峰
	for (int i = 255; i >= 0; i--)
	{
		if (realValue[i - 1] < realValue[i])
		{
			if (realValue[i - 2] < realValue[i - 1])
			{
				if (realValue[i - 3] < realValue[i - 2])
				{   
					if (realValue[i - 4] < realValue[i - 3])
					{
						targetMinPosition = i;
						break;
					}
				}
			}
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
			float Gray = R*0.299 + G*0.587 + B*0.114;
			if (!( Gray> targetPosition))
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
	//Mat ime1 = hhime.clone();
	////漫水填充，随机找个点开始填充，当填充面积大于原图的1/2时退出
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
			int  area = floodFill(floodImg, Point(i, j), Scalar(10, 10, 10), &ccomp, Scalar(0,0,0), Scalar(0,0,0),4);
			if (area > floodImg.rows * floodImg.cols / 4)
			{
				ime1 = floodImg.clone();
				bFlood = false;
			}
		}

		srand(GetTickCount());
		i = ((rand() % 100) * (rand() % 100)) % floodImg.cols;
		srand(GetTickCount());
		j = ((rand() % 88) * (rand() % 90)) % floodImg.rows;
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
	//Mat openImgSrc = hhime.clone();
	Mat openOut;
	 int sliderNum = 0;
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
	imwrite("kaiyunsuan.jpg", openOut);
	
	Mat cannyOut;
	//Canny(openOut, cannyOut, 50, 200, 3);
	Canny(openOut, cannyOut, 50, 200, 3);
	
	imwrite("canny2222.jpg", cannyOut);
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

			
			float silieMul = 1.10;
			//sliderNum++;
			//切割倍标签大小，防止倾斜大的标签内容会被截掉
			silidRect.width = (int)sqrt((pow((rect_points[0].x - rect_points[1].x), 2) + pow((rect_points[0].y - rect_points[1].y), 2)));
			silidRect.height = (int)sqrt((pow((rect_points[0].x - rect_points[3].x), 2) + pow((rect_points[0].y - rect_points[3].y), 2)));
			silidRect.x = (int)(rect_points[2].x - silidRect.width*(silieMul - 1) / 2);
			silidRect.y = (int)(rect_points[2].y - silidRect.height*(silieMul - 1) / 2);
			silidRect.width *= silieMul;
			silidRect.height *= silieMul;

			rectangle(imme, silidRect, 255, 1);
			//切割超出边界会导致崩溃
			
			if (silidRect.x + silidRect.width > srcImage.cols ||
				silidRect.y + silidRect.height > srcImage.rows)
			{
				//MessageBox(_T("致命错误：切割出错！！！"));
				cout << "超出的值  x =     "  << silidRect.x + silidRect.width <<"   silidRect.x=    "<< silidRect.x <<endl;
				cout << "超出的值  y=      " << silidRect.y + silidRect.height <<"   silidRect.y=    "<< silidRect.y <<endl;
				return;
			}

			Mat sliderOutImg = srcImage(silidRect);
			colourImeg[sliderNum] = sliderOutImg.clone();
			slideImeg[sliderNum++] = cannyOut(silidRect);
			
			//memset(timeStrBuf, 0, 128);
		
			imwrite("hah_ "+to_string(sliderNum) + ".jpg", sliderOutImg);
			
			//sliderImg[i] = sliderOutImg.clone();
		}

	}
	
	
	for (int mm = 0; mm < sliderNum; mm++)
	{
		Mat ime1, ime2, ime3, ime4, ime5;
		Mat inimg, ime;
		ime1 = slideImeg[mm].clone();
		inimg = colourImeg[mm].clone();
		ime = inimg.clone();

		vector<Vec4i> lines;
        
		HoughLinesP(ime1, lines, 1, CV_PI / 360, 125, 50, 20);
		//   获取的直线赋值给自定义的结果体，并且写上一些数据以供直线排序
		Myline L[100];//直线  lines.size()
		for (int i = 0; i < lines.size(); i++)
		{
			line(ime, Point(lines[i][0], lines[i][1]), Point(lines[i][2], lines[i][3]), Scalar(0, 255, 255), 2, 8, 0);
			imwrite("ime_" + to_string(mm) + ".jpg", ime);
			L[i].start.x = lines[i][0];
			L[i].start.y = lines[i][1];
			L[i].end.x = lines[i][2];
			L[i].end.y = lines[i][3];
			L[i].linelen = (lines[i][0] - lines[i][2])* (lines[i][0] - lines[i][2]) + (lines[i][1] - lines[i][3])*(lines[i][1] - lines[i][3]);

			float k;
			if ((L[i].start.x - L[i].end.x) != 0)//计算斜率
			{
				k = abs(((float)L[i].start.y - L[i].end.y) / (L[i].start.x - L[i].end.x));
			}
			else {
				k = 20;
			}

			if (k > 1)	//判断直线横或竖
			{
				L[i].direction = true;
			}
			else {
				L[i].direction = false;
			}

		}

	

		//  分成长和框  两个数组
		Myline  vertical[50], across[50];
		Myline releseout[4];
		int numver = 0, numacr = 0;
		for (int j = 0; j < lines.size(); j++)
		{
			if (L[j].direction == true)
			{
				vertical[numver++] = L[j];
			}
			else {
				across[numacr++] = L[j];
			}
		}

		int maxmidx = 0;
		int maxmidy = 0;
		int maxy = 0;
		int maxx = 0;
		for (int i = 0; i <(numver - 1); i++)
		{
			for (int j = i, k = i; j < (numver - 1); j++)
			{
				int mid = abs(((vertical[k].start.x + vertical[k].end.x) / 2 - (vertical[j + 1].start.x + vertical[j + 1].end.x) / 2) / 2);
				if (mid > maxmidx) {
					maxmidx = mid;
				}

				if ((vertical[k].start.x > vertical[j + 1].start.x))
				{
					if (vertical[k].start.x > maxx)
					{
						maxx = vertical[k].start.x;
					}
				}
				else
				{
					if (vertical[j + 1].start.x > maxx)
					{
						maxx = vertical[j + 1].start.x;
					}
				}

			}
		}

		for (int i = 0; i < (numacr - 1); i++)
		{
			for (int j = i, k = i; j < (numacr - 1); j++)
			{
				int mid = abs(((across[k].start.y + across[k].end.y) / 2 - (across[j + 1].start.y + across[j + 1].end.y) / 2) / 2);
				if (mid > maxmidy) {
					maxmidy = mid;
				}
				if (across[k].start.y > across[j + 1].start.y)
				{
					if (across[k].start.y > maxy)
					{
						maxy = across[k].start.y;
					}
				}
				else
				{
					if (across[j + 1].start.y > maxy)
					{
						maxy = across[j + 1].start.y;
					}
				}

			}
		}

		Myline  verticalRight[50], verticalLeft[50];
		Myline  acrossUp[50], acrossDown[50];

		int vRi = 0, vLi = 0;
		for (int i = 0; i < numver; i++)
		{
			if ((vertical[i].start.x + vertical[i].end.x) / 2 >(maxx - maxmidx))
			{
				verticalRight[vRi++] = vertical[i];
			}
			else {
				verticalLeft[vLi++] = vertical[i];
			}
		}

		int aRi = 0, aLi = 0;
		for (int i = 0; i < numacr; i++)
		{
			if ((across[i].start.y + across[i].end.y) / 2 >(maxy - maxmidy))
			{
				acrossDown[aRi++] = across[i];
			}
			else {
				acrossUp[aLi++] = across[i];
			}
		}

		if (vRi == 1) {
			releseout[1] = verticalRight[0];
		}
		else {
			for (int i = 0; i < vRi - 1; i++)
			{
				if (verticalRight[i].linelen > verticalRight[i + 1].linelen)
				{
					releseout[1] = verticalRight[i];
				}
				else {
					releseout[1] = verticalRight[i + 1];
				}
			}
		}

		if (vLi == 1) {
			releseout[0] = verticalLeft[0];
		}
		else {
			for (int i = 0; i < vLi - 1; i++)
			{
				if (verticalLeft[i].linelen > verticalLeft[i + 1].linelen)
				{
					releseout[0] = verticalLeft[i];
				}
				else {
					releseout[0] = verticalLeft[i + 1];
				}
			}
		}

		if (aRi == 1) {
			releseout[3] = acrossDown[0];
		}
		else {
			for (int i = 0; i < (aRi - 1); i++)
			{
				if (acrossDown[i].linelen > acrossDown[i + 1].linelen)
				{
					releseout[3] = acrossDown[i];
				}
				else {
					releseout[3] = acrossDown[i + 1];
				}
			}
		}

		if (aLi == 1) {
			releseout[2] = acrossUp[0];
		}
		else {
			for (int i = 0; i < (aLi - 1); i++)
			{
				if (acrossUp[i].linelen > acrossUp[i + 1].linelen)
				{
					releseout[2] = acrossUp[i];
				}
				else {
					releseout[2] = acrossUp[i + 1];
				}
			}
		}
		
		vector<Point> not_a_rect_shape;
		Point outp;

		outp = getCross(releseout[0], releseout[2]);
		not_a_rect_shape.push_back(outp);
		

		outp = getCross(releseout[1], releseout[2]);
		not_a_rect_shape.push_back(outp);
		

		outp = getCross(releseout[0], releseout[3]);
		not_a_rect_shape.push_back(outp);
		

		outp = getCross(releseout[1], releseout[3]);
		not_a_rect_shape.push_back(outp);
		

		/*const Point* point = &not_a_rect_shape[0];
		int n = (int)not_a_rect_shape.size();*/
	/*	Mat draw = inimg.clone();
		polylines(draw, &point, &n, 1, true, Scalar(0, 255, 0), 3, CV_AA);*/
		//imshow(to_string(mm) + "    44", draw);

		Point2f src_vertices[4];
		src_vertices[0] = not_a_rect_shape[0];
		src_vertices[1] = not_a_rect_shape[1];
		src_vertices[2] = not_a_rect_shape[2];
		src_vertices[3] = not_a_rect_shape[3];

		Point2f dst_vertices[4];
		dst_vertices[0] = Point(0, 0);
		dst_vertices[1] = Point(ime1.cols*0.9, 0);
		dst_vertices[2] = Point(0, ime1.rows*0.9);
		dst_vertices[3] = Point(ime1.cols*0.9, ime1.rows*0.9);

		Mat warpMatrix = getPerspectiveTransform(src_vertices, dst_vertices);

		Mat rotated;

		warpPerspective(inimg, rotated, warpMatrix, rotated.size(), INTER_LINEAR, BORDER_CONSTANT);
		//imshow(to_string(mm) + "    ot", rotated);
		imwrite(to_string(mm) + "rotated.jpg", rotated);
	
	}
	waitKey(0);
	
}
