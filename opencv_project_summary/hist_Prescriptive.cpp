
bool Cal_Hist(Mat Gray_img, MatND &hist) {
	int bins = 256;
	int hist_size[] = { bins };
	float range[] = { 0, 256 };
	const float* ranges[] = { range };

	int channels[] = { 0 };
	//计算直方图    
	calcHist(&Gray_img, 1, channels, Mat(), // do not use mask      
		hist, 1, hist_size, ranges,
		true, // the histogram is uniform      
		false);
	if (hist.data == 0) return false;
	return true;
}

void one_channel_hist_specify(Mat input_img, Mat dst_img, Mat &output_img)//单通道  
{
	int i, j;

	//计算输入，规定图像的直方图  
	MatND input_hist, dst_hist;
	Cal_Hist(input_img, input_hist);
	Cal_Hist(dst_img, dst_hist);

	//计算概率直方图  
	MatND input_p_hist, dst_p_hist;
	input_p_hist = MatND::zeros(input_hist.size[0], input_hist.size[1], CV_32FC1);//原始概率直方图  
	dst_p_hist = MatND::zeros(dst_hist.size[0], dst_hist.size[1], CV_32FC1);//规定概率直方图  
	float input_totalnum = 0;
	float dst_totalnum = 0;
	for (i = 0; i < input_hist.rows; i++)
		input_totalnum += input_hist.at<float>(i);
	for (i = 0; i < dst_hist.rows; i++)
		dst_totalnum += dst_hist.at<float>(i);
	for (i = 0; i < input_hist.rows; i++)
		input_p_hist.at<float>(i) = input_hist.at<float>(i) / input_totalnum;
	for (i = 0; i < dst_hist.rows; i++)
		dst_p_hist.at<float>(i) = dst_hist.at<float>(i) / dst_totalnum;

	//计算累计直方图  
	MatND input_c_hist, dst_c_hist;
	input_c_hist = MatND::zeros(input_hist.size[0], input_hist.size[1], CV_32FC1);//原始累计直方图  
	dst_c_hist = MatND::zeros(dst_hist.size[0], dst_hist.size[1], CV_32FC1);//规定累计直方图  
	float input_accum_p = 0;
	float dst_accum_p = 0;
	for (i = 0; i < input_hist.rows; i++)
	{
		input_accum_p += input_p_hist.at<float>(i);
		input_c_hist.at<float>(i) = input_accum_p;
	}
	for (i = 0; i < dst_hist.rows; i++)
	{
		dst_accum_p += dst_p_hist.at<float>(i);
		dst_c_hist.at<float>(i) = dst_accum_p;
	}

	//计算单映射规则  
	MatND SML = MatND::zeros(input_hist.size[0], input_hist.size[1], CV_32FC1);//SML单映射规则  
	for (i = 0; i < input_c_hist.rows; i++)
	{
		int minind = 0;
		float minval = 1;
		for (j = 0; j < dst_c_hist.rows; j++)
		{
			float abssub = abs(input_c_hist.at<float>(i) - dst_c_hist.at<float>(j));
			if (abssub < minval)
			{
				minval = abssub;
				minind = j;
			}
		}
		SML.at<float>(i) = minind;
	}

	//计算输出图像  
	Mat outimg = Mat::zeros(input_img.size[0], input_img.size[1], CV_8U);
	for (i = 0; i < input_img.rows; i++)
	{
		for (j = 0; j < input_img.cols; j++)
		{
			outimg.at<uchar>(i, j) = SML.at<float>(input_img.at<uchar>(i, j));
		}
	}
	outimg.copyTo(output_img);
}








//归一化调用
Mat src, gray, src2, gray2;
src = imread(imgPath);
cvtColor(src, gray, CV_RGB2GRAY);  //转换成灰度图    
src2 = imread("136.jpg");
cvtColor(src2, gray2, CV_RGB2GRAY);  //转换成灰度图    
Mat output_img;
//three_channel_hist_specify(src, src2, output_img);
one_channel_hist_specify(gray, gray2, output_img);
imwrite("out.jpg", output_img);
