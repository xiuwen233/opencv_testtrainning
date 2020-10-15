//【5】绘制出直方图
		int hpt = saturate_cast<int>(0.9 * size);
		int realValue_tmp1;
		int realValue_tmp2;
		int realValue_tmp3;
		int realValue[256];

		int targetValue[256];
		int targetMinPosition = 0;
		int targetMaxValue = 0;
		int targetMaxPosition = 0;
		int targetPosition = 0;

		for (int i = 0; i < 256; i++)
		{
			realValue[i] = realValue[0] = saturate_cast<int>(dstHist.at<float>(i) * hpt / maxValue);
			rectangle(dstImage, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue[0]), Scalar(255), 1, 8, 0);
		}

		imshow("滤波前", dstImage);
		//直方图滤波（简单的中值滤波）
		for (int k = 0; k < slider1; k++)
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
