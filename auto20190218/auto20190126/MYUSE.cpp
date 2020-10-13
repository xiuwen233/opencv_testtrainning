

 这是之前判断的芯片位置偏差
//检测芯片位置的误差
void Caclulation_error(int difference_x, int difference_y, int src_x, int src_y)
{
	char str[32];
	if (difference_x > 10)
	{
		if (difference_y  >10)
		{
			//	sprintf(str, "EBB501%02XBE", -err_trans_x);
			sprintf(str, "414A00%02X00%02X", difference_x, (int)(difference_y * 0.8));
		}
		else if (difference_y < -10)
		{
			sprintf(str, "414A00%02X01%02X", difference_x, -((int)(difference_y * 0.8)));
		}
		else {
			sprintf(str, "414A00%02X0000", difference_x);
		}
	}
	else if (difference_x < -10)
	{
		if (difference_y  >10)
		{
			//	sprintf(str, "EBB501%02XBE", -err_trans_x);
			sprintf(str, "414A01%02X00%02X", -(difference_x), (int)(difference_y * 0.8));
		}
		else if (difference_y < -10)
		{
			sprintf(str, "414A01%02X01%02X", -(difference_x), -((int)(difference_y * 0.8)));
		}
		else {
			sprintf(str, "414A01%02X0000", -(difference_x));
		}
	}
	else
	{
		if (difference_y >10)
		{
			//	sprintf(str, "EBB501%02XBE", -err_trans_x);
			sprintf(str, "414A000000%02X", (int)(difference_y * 0.8));
		}
		else if (difference_y < -10)
		{
			sprintf(str, "414A000001%02X", -((int)(difference_y * 0.8)));
		}
		else {
			sprintf(str, "414A00000000");
		}
	}
	ForceSendCmd(str);
}


//检测编带是否弯曲
int  circle_center_x = 180;
int circle_center_y = 180;

if (abs(center.x - circle_center_x) < 8     &&   abs(center.y - circle_center_y) < 15)
			{ 

				printf("it is 0k \n");
				theApp.GetMainWnd()->GetDlgItem(IDC_STATIC_TXT)->SetWindowText("编带位置检测正确");

				
//检测编带的位置是否放歪

int  small_third_x = 228;
int  small_third_y = 84;
				
if (abs(center.x - small_third_x) < 15 && abs(center.y - small_third_y) < 15)
			{  			
				theApp.GetMainWnd()->GetDlgItem(IDC_STATIC_TXT)->SetWindowText("检测放置正确 继续");
				
				
两个不同的初始化参数：

Capturepragma parsmm = {
	 .rect_x = 170,
	.rect_y = 100,
	.rect_weight = 256,
	.rect_height = 256,
	.i_threshold = 180,
    .out_mask_img_x = 180,
	.out_mask_img_y = 185,
    .back_small_x = 36,
	.back_small_y =9
	
}


//class two
Capturepragmatwo parrsajnj = {
	
	.rect_x = 110,
	.rect_y = 50,
	.rect_weight = 360,
	.rect_height = 360,
	.i_threshold = 200,
	.circle_mask_d = 30,
    .circle_center_x  = 180,
	.circle_center_y = 180;
	.rectangle_mask_image_x = 44,
    .rectangle_mask_image_y = 9;
}


// 开始先初始化
void SetCaptureInit(Capturepragmatwo  capturepragma);
bool SetCaptureIndex(int index);
start();
StopRun
