
//结构定义：TIFF头+标记表

struct TiffHeaderTable
{
	unsigned char	i1;					//字母'I'
	unsigned char	i2;					//字母'I'
	WORD			vers;				//版本号42
	unsigned long	ifd_offs;			//1st IFD偏移量

	WORD			tag_count;			//标记数量

	WORD			N_ImgWidth;			//标记号256=横向像素数
	WORD			Type_ImgWidth;		//数据类型
	unsigned long	Count_ImgWidth;		//数据个数=1
	unsigned long	Intent_ImgWidth;	//横向像素数数据

	WORD			N_ImgLen;			//标记号257=纵向像素数
	WORD			Type_ImgLen;		//数据类型
	unsigned long	Count_ImgLen;		//数据个数=1
	unsigned long	Intent_ImgLen;		//纵向像素数数据

	WORD			N_BitsPSmp;			//标记号258=每采样位数
	WORD			Type_BitsPSmp;		//数据类型
	unsigned long	Count_BitsPSmp;		//数据个数=3
	unsigned long	Addr_BitsPSmp;		//数据存放偏移量！

	WORD			N_Compress;			//标记号259=压缩
	WORD			Type_Compress;		//数据类型		
	unsigned long	Count_Compress;		//数据个数=1
	unsigned long	Intent_Compress;	//压缩类型数据=1，非压缩

	WORD			N_LAB;				//标记号262=光度学解释
	WORD			Type_LAB;			//数据类型
	unsigned long	Count_LAB;			//数据个数=1
	unsigned long	Intent_LAB;			//数据=8(LAB模式)

	WORD			N_StripOffs;		//标记号273=像素数据偏移量
	WORD			Type_StripOffs;		//数据类型
	unsigned long	Count_StripOffs;	//数据个数=1
	unsigned long	Intent_StripOffs;	//偏移量数据	

	WORD			N_SmpPerPixel;		//标记号277=每个像素采样数
	WORD			Type_SmpPerPixel;	//数据类型
	unsigned long	Count_SmpPerPixel;	//数据个数=1
	unsigned long	Intent_SmpPerPixel;	//采样个数=3

	WORD			N_RowsPerStrip;		//标记号278=每条图像行数
	WORD			Type_RowsPerStrip;	//数据类型
	unsigned long	Count_RowsPerStrip;	//数据个数
	unsigned long	Intent_RowsPerStrip;//行数数据

	WORD			N_ByteCounts;		//标记号279=每条图像字节数
	WORD			Type_ByteCounts;	//数据类型
	unsigned long	Count_ByteCounts;	//数据个数
	unsigned long	Intent_ByteCounts;	//像素数据字节数

	WORD			N_XRes;				//标记号282=X方向分辨率
	WORD			Type_XRes;			//数据类型=5
	unsigned long	Count_XRes;			//数据个数=1
	unsigned long	Addr_XRes;			//数据偏移量！

	WORD			N_YRes;				//标记号283=Y方向分辨率
	WORD			Type_YRes;			//数据类型=5
	unsigned long	Count_YRes;			//数据个数=1
	unsigned long	Addr_YRes;  		//数据偏移量！

	WORD			N_ResUnit;			//标记号296=分辨率单位
	WORD			Type_ResUnit;		//数据类型
	unsigned long	Count_ResUnit;		//数据个数
	unsigned long	Intent_ResUnit;		//数据=2(dpi)

	WORD			N_PlanarConfig;		//标记号284=平面配置
	WORD			Type_PlanarConfig;	//数据类型
	unsigned long	Count_PlanarConfig;	//数据个数=1
	unsigned long	Intent_PlanarConfig;//数据


	unsigned long	EndZeros;			//表示结束的4字节0

	WORD			SmpPerPixel1;		//每采样位数=16
	WORD			SmpPerPixel2;		//每采样位数=16
	WORD			SmpPerPixel3;		//每采样位数=16

	unsigned long	XRes_Num;			//X方向分辨率数据：分子
	unsigned long	XRes_Denom;			//X方向分辨率数据：分母

	unsigned long	YRes_Num;			//Y方向分辨率数据：分子
	unsigned long	YRes_Denom;			//Y方向分辨率数据：分母

};


//结构定义：测试图像参数

struct StimulusParams
{
	unsigned char	TestType[2];		//测试类型		
	//"LL"=明度测试;"SS"=饱和度测试;"HH"=色相测试

	unsigned long	ImgPixelH;		//图像横向像素行数
	unsigned long	ImgPixelV;		//图像纵向像素行数
	unsigned long	TestZonePixelH;	//测试区横向像素行数
	unsigned long	TestZonePixelV;	//测试区纵向像素行数
									
	double			Central_L;		//中心亮度L0值
	double			Delta_L;		//亮度L步距
	double			Central_H;		//中心色相H0值
	double			Delta_H;		//色相H步距
	double			Central_C;		//中心饱和度值
	double			Delta_C;		//饱和度步距
	double			Central_A;		//中心A值
	double			Central_B;		//中心B值
	
	unsigned long	PM_Steps;		//由中心值上下增减的次数

	double			Start_L;		//起始亮度L值
	double			End_L;			//终止亮度L值
	double			Start_H;		//起始色相值
	double			End_H;			//终止色相值
	double			Start_C;		//起始饱和度值
	double			End_C;			//终止饱和度值
	double			Start_A;		//起始A值
	double			End_A;			//终止A值
	double			Start_B;		//起始B值	
	double			End_B;			//终止B值

	unsigned char   BKG_Type[3];	//背景色数据类型："LCH"或"LAB"
	double			BKG_L;			//背景色明度值
	double			BKG_C;			//背景色彩度值
	double			BKG_H;			//背景色色调角
	double			BKG_A;			//背景色a*
	double			BKG_B;			//背景色b*

	double			StiFreqMin;		//最小空间频率
	double			StiFreqMax;		//最大空间频率
	double			StiFreqDiff;	//空间频率级差

	double			StiAngle;		//方向角度

	unsigned long	StiImgCount;	//刺激图像数

	unsigned char	StiFilePath[300];	//刺激图像文件存放位置

};

struct DisplayParams
{
	unsigned long	DispPixelH;		//显示器横向像素数
	unsigned long	DispPixelV;		//显示器纵向像素数
	double			DispSizeH;		//显示器横向尺寸
	double			DispSizeV;		//显示器纵向尺寸
};

struct ViewCond
{
	double			AnglePerPixel;	//每一像素的视觉张角
	double			ViewDistance;	//观察距离
};
