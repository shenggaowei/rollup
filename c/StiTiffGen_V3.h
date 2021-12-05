
//�ṹ���壺TIFFͷ+��Ǳ�

struct TiffHeaderTable
{
	unsigned char	i1;					//��ĸ'I'
	unsigned char	i2;					//��ĸ'I'
	WORD			vers;				//�汾��42
	unsigned long	ifd_offs;			//1st IFDƫ����

	WORD			tag_count;			//�������

	WORD			N_ImgWidth;			//��Ǻ�256=����������
	WORD			Type_ImgWidth;		//��������
	unsigned long	Count_ImgWidth;		//���ݸ���=1
	unsigned long	Intent_ImgWidth;	//��������������

	WORD			N_ImgLen;			//��Ǻ�257=����������
	WORD			Type_ImgLen;		//��������
	unsigned long	Count_ImgLen;		//���ݸ���=1
	unsigned long	Intent_ImgLen;		//��������������

	WORD			N_BitsPSmp;			//��Ǻ�258=ÿ����λ��
	WORD			Type_BitsPSmp;		//��������
	unsigned long	Count_BitsPSmp;		//���ݸ���=3
	unsigned long	Addr_BitsPSmp;		//���ݴ��ƫ������

	WORD			N_Compress;			//��Ǻ�259=ѹ��
	WORD			Type_Compress;		//��������		
	unsigned long	Count_Compress;		//���ݸ���=1
	unsigned long	Intent_Compress;	//ѹ����������=1����ѹ��

	WORD			N_LAB;				//��Ǻ�262=���ѧ����
	WORD			Type_LAB;			//��������
	unsigned long	Count_LAB;			//���ݸ���=1
	unsigned long	Intent_LAB;			//����=8(LABģʽ)

	WORD			N_StripOffs;		//��Ǻ�273=��������ƫ����
	WORD			Type_StripOffs;		//��������
	unsigned long	Count_StripOffs;	//���ݸ���=1
	unsigned long	Intent_StripOffs;	//ƫ��������	

	WORD			N_SmpPerPixel;		//��Ǻ�277=ÿ�����ز�����
	WORD			Type_SmpPerPixel;	//��������
	unsigned long	Count_SmpPerPixel;	//���ݸ���=1
	unsigned long	Intent_SmpPerPixel;	//��������=3

	WORD			N_RowsPerStrip;		//��Ǻ�278=ÿ��ͼ������
	WORD			Type_RowsPerStrip;	//��������
	unsigned long	Count_RowsPerStrip;	//���ݸ���
	unsigned long	Intent_RowsPerStrip;//��������

	WORD			N_ByteCounts;		//��Ǻ�279=ÿ��ͼ���ֽ���
	WORD			Type_ByteCounts;	//��������
	unsigned long	Count_ByteCounts;	//���ݸ���
	unsigned long	Intent_ByteCounts;	//���������ֽ���

	WORD			N_XRes;				//��Ǻ�282=X����ֱ���
	WORD			Type_XRes;			//��������=5
	unsigned long	Count_XRes;			//���ݸ���=1
	unsigned long	Addr_XRes;			//����ƫ������

	WORD			N_YRes;				//��Ǻ�283=Y����ֱ���
	WORD			Type_YRes;			//��������=5
	unsigned long	Count_YRes;			//���ݸ���=1
	unsigned long	Addr_YRes;  		//����ƫ������

	WORD			N_ResUnit;			//��Ǻ�296=�ֱ��ʵ�λ
	WORD			Type_ResUnit;		//��������
	unsigned long	Count_ResUnit;		//���ݸ���
	unsigned long	Intent_ResUnit;		//����=2(dpi)

	WORD			N_PlanarConfig;		//��Ǻ�284=ƽ������
	WORD			Type_PlanarConfig;	//��������
	unsigned long	Count_PlanarConfig;	//���ݸ���=1
	unsigned long	Intent_PlanarConfig;//����


	unsigned long	EndZeros;			//��ʾ������4�ֽ�0

	WORD			SmpPerPixel1;		//ÿ����λ��=16
	WORD			SmpPerPixel2;		//ÿ����λ��=16
	WORD			SmpPerPixel3;		//ÿ����λ��=16

	unsigned long	XRes_Num;			//X����ֱ������ݣ�����
	unsigned long	XRes_Denom;			//X����ֱ������ݣ���ĸ

	unsigned long	YRes_Num;			//Y����ֱ������ݣ�����
	unsigned long	YRes_Denom;			//Y����ֱ������ݣ���ĸ

};


//�ṹ���壺����ͼ�����

struct StimulusParams
{
	unsigned char	TestType[2];		//��������		
	//"LL"=���Ȳ���;"SS"=���ͶȲ���;"HH"=ɫ�����

	unsigned long	ImgPixelH;		//ͼ�������������
	unsigned long	ImgPixelV;		//ͼ��������������
	unsigned long	TestZonePixelH;	//������������������
	unsigned long	TestZonePixelV;	//������������������
									
	double			Central_L;		//��������L0ֵ
	double			Delta_L;		//����L����
	double			Central_H;		//����ɫ��H0ֵ
	double			Delta_H;		//ɫ��H����
	double			Central_C;		//���ı��Ͷ�ֵ
	double			Delta_C;		//���ͶȲ���
	double			Central_A;		//����Aֵ
	double			Central_B;		//����Bֵ
	
	unsigned long	PM_Steps;		//������ֵ���������Ĵ���

	double			Start_L;		//��ʼ����Lֵ
	double			End_L;			//��ֹ����Lֵ
	double			Start_H;		//��ʼɫ��ֵ
	double			End_H;			//��ֹɫ��ֵ
	double			Start_C;		//��ʼ���Ͷ�ֵ
	double			End_C;			//��ֹ���Ͷ�ֵ
	double			Start_A;		//��ʼAֵ
	double			End_A;			//��ֹAֵ
	double			Start_B;		//��ʼBֵ	
	double			End_B;			//��ֹBֵ

	unsigned char   BKG_Type[3];	//����ɫ�������ͣ�"LCH"��"LAB"
	double			BKG_L;			//����ɫ����ֵ
	double			BKG_C;			//����ɫ�ʶ�ֵ
	double			BKG_H;			//����ɫɫ����
	double			BKG_A;			//����ɫa*
	double			BKG_B;			//����ɫb*

	double			StiFreqMin;		//��С�ռ�Ƶ��
	double			StiFreqMax;		//���ռ�Ƶ��
	double			StiFreqDiff;	//�ռ�Ƶ�ʼ���

	double			StiAngle;		//����Ƕ�

	unsigned long	StiImgCount;	//�̼�ͼ����

	unsigned char	StiFilePath[300];	//�̼�ͼ���ļ����λ��

};

struct DisplayParams
{
	unsigned long	DispPixelH;		//��ʾ������������
	unsigned long	DispPixelV;		//��ʾ������������
	double			DispSizeH;		//��ʾ������ߴ�
	double			DispSizeV;		//��ʾ������ߴ�
};

struct ViewCond
{
	double			AnglePerPixel;	//ÿһ���ص��Ӿ��Ž�
	double			ViewDistance;	//�۲����
};
