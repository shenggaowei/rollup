 // Windows 程序

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "resource.h"
#include "StiTiffGen_V3.h"

/*********************************************************************************************/
//函数声明

//Windows窗口处理函数
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
//对话框函数：刺激图像设置及生成	
BOOL CALLBACK DlgProc1(HWND,UINT,WPARAM,LPARAM);
//对话框函数：显示器和像素张角设置
BOOL CALLBACK DlgProc2(HWND,UINT,WPARAM,LPARAM);
//对话框函数3：视觉观察距离计算
BOOL CALLBACK DlgProc3(HWND,UINT,WPARAM,LPARAM);
//对话框函数4：存储盘设置
BOOL CALLBACK DlgProc4(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam);

//刺激图像参数设置函数
void StiImgParamSetting(HWND hdwnd,struct StimulusParams *StiTestParam);
//刺激图像参数设置分析函数
int	StiImgParamAnalyse(struct StimulusParams *StiTestParam);
//显示及观察条件参数分析函数
int	DispViewParamAnalyse(struct DisplayParams DispParam,struct ViewCond ViewParam);
//刺激图像参数错误警告显示
void ErrWarning_StiImg(HWND hdwnd,int Err_num);
//色度LAB->LAB转换
void StiParamLchToLab(struct StimulusParams *StiTestParam);
void Lch2Lab(double *Lch_C, double *Lch_H, double *Lab_A, double *Lab_B);
//计算：按中心值增减多个步距的最大值、最小值、刺激图像个数
void StiImgCount(struct StimulusParams *StiTestParam);
//TIFF文件头及图像文件目录数据设置
void TiffHeaderTableSet(struct TiffHeaderTable *TiffHdTable,struct StimulusParams *StiTestParam);
//TIFF文件头及图像文件目录数据写入文件的函数
int TiffHeaderTableWrite(unsigned char *TiffFileName,struct TiffHeaderTable *TiffHdTable);
//TIFF图像文件波形生成函数
int TiffWaveImgFileGen(struct TiffHeaderTable *TiffHdTable);
//Tiff图像文件生成错误警告显示
void ErrWarning_TiffImgFileGen(HWND hdwnd,int Err_num);
//生成时间字符串
void GenTimeString(unsigned char *TimeString);

/*********************************************************************************************/
//全局变量声明

//窗口相关变量
char					*szWinName="WinMain";	//窗口类名
HINSTANCE				hInst;					//实例句柄

//测试参数结构变量
struct	StimulusParams	StiTestParam;			//测试刺激图像参数的结构变量

//显示器参数结构变量
struct	DisplayParams	DispParam;

//视觉观察参数结构变量
struct	ViewCond		ViewParam;

//TIFF文件头和标记表结构：指针变量
struct	TiffHeaderTable	TiffHdTable;

//时间结构
SYSTEMTIME	StiGenTime;

//其他变量
unsigned char			tempstr[256];			//存放字符串的临时变量
int						Err_num;				//存放错误号码


/*****************************************************************************************/
//Windows主程序

int WINAPI WinMain(HINSTANCE hThisInst,HINSTANCE hPrevInst,PSTR pszCmdline,int nCmdShow)

{
   HWND     hwnd;  // 窗口句柄；
   MSG      uMsg;  // 消息结构；
   WNDCLASS wndclass; //定义窗口类； 

   //设置窗口类的各参数；
   wndclass.style=0;  //使用缺省类型；
   wndclass.lpfnWndProc=WndProc;  //窗口函数名；
   wndclass.cbClsExtra=0;  //无类扩展；
   wndclass.cbWndExtra=0;  //无窗口扩展；
   wndclass.hInstance=hThisInst;  //窗口类实例句柄（使用前面所定义的）；
   wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);  //最小化时的图标；
   wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);  //光标类型定义；
   wndclass.hbrBackground=GetStockObject(WHITE_BRUSH);  //窗口背景色为：白色
   wndclass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1); //菜单；
   wndclass.lpszClassName=szWinName;  // 窗口类的名称（使用前面定义的）

   //注册窗口类
   RegisterClass(&wndclass);

   //创建窗口
   hwnd=CreateWindow(szWinName,"视觉刺激波形图像生成器",
	                 WS_OVERLAPPEDWINDOW|WS_HSCROLL|WS_VSCROLL,
					 CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
					 CW_USEDEFAULT,NULL,NULL,hThisInst,NULL);

   //显示窗口

   ShowWindow(hwnd,nCmdShow);
   UpdateWindow(hwnd);

   //消息循环处理

   while(GetMessage(&uMsg,NULL,0,0))
   {
	   TranslateMessage(&uMsg);
	   DispatchMessage(&uMsg);
     
   }

   return uMsg.wParam;

}

/*******************************************************************************************/
//程序主窗口的处理函数：处理各种消息，由Windows调用

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)

{

      switch(message)
	  {
	    case WM_COMMAND:
			switch(LOWORD(wParam))
			{

				case IDM_PROJ_NEW:
					MessageBox(hwnd,"新建项目","新建",MB_OK);
					break;

				case IDM_PROJ_OPEN:
					MessageBox(hwnd,"打开项目","设置",MB_OK);
					break;

				case IDM_PROJ_SAVE:
					MessageBox(hwnd,"保存项目","设置",MB_OK);
					break;

				case IDM_PROJ_INFO:
					MessageBox(hwnd,"显示项目信息","设置",MB_OK);
					break;

				case IDM_EXIT:
				//	MessageBox(hwnd,"退出程序","退出",MB_OK);
					PostQuitMessage(0);
					break;

				case IDM_SET_TIFFGEN:
									
					//调用对话框函数：刺激图像设置及生成
					DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG1),hwnd,DlgProc1);
					break;

				case IDM_SET_DISPVIEW:

					//调用对话框函数：设置显示器参数、每像素视角
					DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG2),hwnd,DlgProc2);

					break;

				case IDM_FILEPATH:

					//调用对话框函数：设置文件存储盘
					DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG4),hwnd,DlgProc4);

					break;
				case IDM_ABOUT:
					MessageBox(hwnd,"视觉刺激图像生成器V3.01\n\n 2009.11.08\n\n 2014.04.26 略增\n\nJinYang","关于",MB_OK);
					break;

			}

			break;  
			
	    case WM_DESTROY:
		     
			   PostQuitMessage(0);

		default:
		     return DefWindowProc(hwnd,message,wParam,lParam);
	  }
     return 0;
}

/*******************************************************************************************/
//对话框函数1：刺激图像参数设置

BOOL CALLBACK DlgProc1(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	int				i;
	unsigned char	tmp_str[100];
	double			val1;

	switch(message)
	{
	case WM_INITDIALOG:
		
		//对话框初始化

		//设置刺激测试类型字符串为空，表示尚未设置
		strcpy(StiTestParam.TestType,"");

		//设置刺激图像LCH数据编辑框为“只读”状态
		for(i=0;i<12;i++)
		{
			SendDlgItemMessage(hdwnd,IDC_EDIT5+i,EM_SETREADONLY,1,0);
		}

		//设置背景色数据编辑框为“只读”状态
		SendDlgItemMessage(hdwnd,IDC_EDIT21,EM_SETREADONLY,1,0);
		SendDlgItemMessage(hdwnd,IDC_EDIT22,EM_SETREADONLY,1,0);
		SendDlgItemMessage(hdwnd,IDC_EDIT23,EM_SETREADONLY,1,0);
		
		//设置图像像素数缺省值
		strcpy(tmp_str,"1280");
		SetDlgItemText(hdwnd,IDC_EDIT1,tmp_str);
		strcpy(tmp_str,"1024");
		SetDlgItemText(hdwnd,IDC_EDIT2,tmp_str);
		strcpy(tmp_str,"256");
		SetDlgItemText(hdwnd,IDC_EDIT3,tmp_str);
		strcpy(tmp_str,"256");
		SetDlgItemText(hdwnd,IDC_EDIT4,tmp_str);

		//设置正弦波个数和波形方向角缺省值
		strcpy(tmp_str,"5");
		SetDlgItemText(hdwnd,IDC_EDIT17,tmp_str);
		strcpy(tmp_str,"0");
		SetDlgItemText(hdwnd,IDC_EDIT20,tmp_str);

		//设置图像个数缺省值("5"代表：由中心值起，按步距增加和减少各5幅图像，共10幅)
		strcpy(tmp_str,"5");
		SetDlgItemText(hdwnd,IDC_EDIT18,tmp_str);

		
		return FALSE;
			
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{

		case IDC_RADIO1_TEST_L:	//点击“L变化，C和H固定”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO1_TEST_L,BM_GETCHECK,0,0))
			{
				//将“L变化，C和H固定”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO1_TEST_L,BM_GETCHECK,1,0);
				//不选其他两个单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO2_TEST_C,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO3_TEST_H,BM_GETCHECK,0,0);
			}

			for(i=0;i<4;i++)
			{
				//解除L数据编辑框的“只读”状态
				SendDlgItemMessage(hdwnd,IDC_EDIT5+i,EM_SETREADONLY,0,0);			
			}

			for(i=0;i<8;i++)
			{
				//设置C和H数据编辑框为“只读”状态，清空其他编辑框的字符
				SendDlgItemMessage(hdwnd,IDC_EDIT9+i,EM_SETREADONLY,1,0);
				SetDlgItemText(hdwnd,IDC_EDIT9+i,"");
			}

			//设置测试类型为：亮度测试
			strcpy(StiTestParam.TestType,"LL");

			return 1;

		case IDC_RADIO2_TEST_C:	//点击“C变化，L和H固定”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO2_TEST_C,BM_GETCHECK,0,0))
			{
				//将“C变化，L和H固定”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO2_TEST_C,BM_GETCHECK,1,0);
				//不选其他两个单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO1_TEST_L,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO3_TEST_H,BM_GETCHECK,0,0);
			}

			for(i=0;i<4;i++)
			{
				//解除C数据编辑框的“只读”状态
				SendDlgItemMessage(hdwnd,IDC_EDIT9+i,EM_SETREADONLY,0,0);			
			}

			for(i=0;i<4;i++)
			{
				//设置L和H数据编辑框为“只读”状态，清空其他编辑框的字符
				SendDlgItemMessage(hdwnd,IDC_EDIT5+i,EM_SETREADONLY,1,0);
				SendDlgItemMessage(hdwnd,IDC_EDIT13+i,EM_SETREADONLY,1,0);
				SetDlgItemText(hdwnd,IDC_EDIT5+i,"");
				SetDlgItemText(hdwnd,IDC_EDIT13+i,"");
			}

			//设置测试类型为：彩度测试
			strcpy(StiTestParam.TestType,"CC");

			return 1;

		case IDC_RADIO3_TEST_H:	//点击“H变化，L和C固定”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO3_TEST_H,BM_GETCHECK,0,0))
			{
				//将“H变化，L和C固定”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO3_TEST_H,BM_GETCHECK,1,0);
				//不选其他两个单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO1_TEST_L,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO2_TEST_C,BM_GETCHECK,0,0);
			}
	
			for(i=0;i<4;i++)
			{
				//解除H数据编辑框的“只读”状态
				SendDlgItemMessage(hdwnd,IDC_EDIT13+i,EM_SETREADONLY,0,0);			
			}

			for(i=0;i<8;i++)
			{
				//设置L和C数据编辑框为“只读”状态，清空其他编辑框的字符
				SendDlgItemMessage(hdwnd,IDC_EDIT5+i,EM_SETREADONLY,1,0);
				SetDlgItemText(hdwnd,IDC_EDIT5+i,"");
			}

			//设置测试类型为：色调角测试
			strcpy(StiTestParam.TestType,"HH");

			return 1;

		case IDC_RADIO4_AVR_L:	//背景色"取L0"单选钮
			
			//如果尚未设置测试类型，则警告
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"应先选择测试类型。","顺序出错",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO4_AVR_L,BM_GETCHECK,0,0);
				
				return 0;
			}
			//如果测试类型不是"LL"，则警告
			if(strcmp(StiTestParam.TestType,"LL")!=0)
			{
				MessageBox(hdwnd,"测试类型：彩度和色调角，不能按此方式选择背景色。",
							"方式出错",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO4_AVR_L,BM_GETCHECK,0,0);
				
				return 0;
			}

			SendDlgItemMessage(hdwnd,IDC_RADIO4_AVR_L,BM_GETCHECK,1,0);

			//明度中心值，作为背景色L
			GetDlgItemText(hdwnd,IDC_EDIT5,tmp_str,20);//获取控件的文本，即长度为20的字符串tmp_str 
			val1 = atof(tmp_str);//将tmp_str字符串转换为浮点数 
			sprintf(tmp_str,"%5.2f",val1);//变参函数，将val1 结果输出给tmp_str 
			SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);

			//设置背景色数据类型为LCH
			strcpy(StiTestParam.BKG_Type,"LCH");
			
			//传递色调角值到背景色编辑框
			GetDlgItemText(hdwnd,IDC_EDIT7,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"尚未输入刺激波形的H值","缺少色调角值",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT23,tmp_str);

			//传递彩度值到背景色编辑框
			GetDlgItemText(hdwnd,IDC_EDIT8,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"尚未输入刺激波形的C值","缺少彩度值",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT22,tmp_str);

			return 1;

		case IDC_RADIO5_AVR_C:	//背景色"取C0"单选钮
			
			//如果尚未设置测试类型，则警告
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"应先选择测试类型。","顺序出错",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO5_AVR_C,BM_GETCHECK,0,0);
				
				return 0;
			}
			
			//如果测试类型不是"CC"，则警告
			if(strcmp(StiTestParam.TestType,"CC")!=0)
			{
				MessageBox(hdwnd,"明度和色调角测试类型不能按此方式选择背景色。",
							"方式出错",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO5_AVR_C,BM_GETCHECK,0,0);
				
				return 0;
			}

			SendDlgItemMessage(hdwnd,IDC_RADIO5_AVR_C,BM_GETCHECK,1,0);

			//计算彩度C0值，作为背景色
			GetDlgItemText(hdwnd,IDC_EDIT9,tmp_str,20);
			val1 = atof(tmp_str);
			sprintf(tmp_str,"%5.2f",val1);
			SetDlgItemText(hdwnd,IDC_EDIT22,tmp_str);
			
			//传递色调角值到背景色编辑框
			GetDlgItemText(hdwnd,IDC_EDIT11,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"尚未输入刺激波形的H值","缺少色调角值",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT23,tmp_str);

			//传递明度值到背景色编辑框
			GetDlgItemText(hdwnd,IDC_EDIT12,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"尚未输入刺激波形的L值","缺少明度值",MB_OK|MB_ICONWARNING);
			}

			SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);

			//设置背景色数据类型为LCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			return 1;

		case IDC_RADIO6_AVR_H:	//背景色"取H0"单选钮
			
			//如果尚未设置测试类型，则警告
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"应先选择测试类型。","顺序出错",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO6_AVR_H,BM_GETCHECK,0,0);
				
				return 0;
			}
			//如果测试类型不是"HH"，则警告
			if(strcmp(StiTestParam.TestType,"HH")!=0)
			{
				MessageBox(hdwnd,"明度和彩度测试类型不能按此方式选择背景色。",
							"方式出错",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO6_AVR_H,BM_GETCHECK,0,0);
				
				return 0;
			}

			SendDlgItemMessage(hdwnd,IDC_RADIO6_AVR_H,BM_GETCHECK,1,0);

			//计算色调角平均值，作为背景色
			GetDlgItemText(hdwnd,IDC_EDIT13,tmp_str,20);
			val1 = atof(tmp_str);
			sprintf(tmp_str,"%5.2f",val1);
			SetDlgItemText(hdwnd,IDC_EDIT23,tmp_str);
			
			//传递明度值到背景色编辑框
			GetDlgItemText(hdwnd,IDC_EDIT15,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"尚未输入刺激波形的L值","缺少明度值",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);

			//传递彩度值到背景色编辑框
			GetDlgItemText(hdwnd,IDC_EDIT16,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"尚未输入刺激波形的C值","缺少彩度值",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT22,tmp_str);

			//设置背景色数据类型为LCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			return 1;

		case IDC_RADIO7_NEUTRAL:	//背景色"中性灰(取L0)"单选钮	

			//如果尚未设置测试类型，则警告
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"应先选择测试类型。","顺序出错",MB_OK|MB_ICONWARNING);

				//取消选择此按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO7_NEUTRAL,BM_GETCHECK,0,0);

				return 0;
			}

			//取亮度值L0用于中性灰背景色
			if(strcmp(StiTestParam.TestType,"LL")==0)
			{
				//取明度值L0，作为背景色
				GetDlgItemText(hdwnd,IDC_EDIT5,tmp_str,20);
				val1 = atof(tmp_str);
				sprintf(tmp_str,"%5.2f",val1);
				SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);
				//背景色的彩色分量置为"0.0"
				SetDlgItemText(hdwnd,IDC_EDIT22,"0.0");
				SetDlgItemText(hdwnd,IDC_EDIT23,"0.0");
			}

			//CC模式：传递明度值到背景色明度值编辑框
			if(strcmp(StiTestParam.TestType,"CC")==0)
			{
				//获取明度值，作为背景色
				GetDlgItemText(hdwnd,IDC_EDIT12,tmp_str,20);
				//传递字符串到背景色明度值编辑框
				SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);
				//背景色的彩色分量置为"0.0"
				SetDlgItemText(hdwnd,IDC_EDIT22,"0.0");
				SetDlgItemText(hdwnd,IDC_EDIT23,"0.0");
			}

			//HH模式：传递明度值到背景色明度值编辑框
			if(strcmp(StiTestParam.TestType,"HH")==0)
			{
				//获取明度平均值，作为背景色
				GetDlgItemText(hdwnd,IDC_EDIT15,tmp_str,20);
				//传递字符串到背景色明度值编辑框
				SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);
				//背景色的彩色分量置为"0.0"
				SetDlgItemText(hdwnd,IDC_EDIT22,"0.0");
				SetDlgItemText(hdwnd,IDC_EDIT23,"0.0");
			}

			//设置背景色数据类型为LCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			//选择此按钮
			SendDlgItemMessage(hdwnd,IDC_RADIO7_NEUTRAL,BM_GETCHECK,1,0);

			return 1;

		case IDC_RADIO8_NEUTRAL2:	//背景色"中性灰(自定L)"单选钮

			//提示输入背景色明度值
			MessageBox(hdwnd,"请输入背景色明度值","要求输入数据",MB_OK);
				
			//解除背景色L编辑框的“只读”状态
			SendDlgItemMessage(hdwnd,IDC_EDIT21,EM_SETREADONLY,0,0);			
			//背景色CH编辑框“只读”
			SendDlgItemMessage(hdwnd,IDC_EDIT22,EM_SETREADONLY,1,0);			
			SendDlgItemMessage(hdwnd,IDC_EDIT23,EM_SETREADONLY,1,0);			
			//背景色CH编辑框为0
			SetDlgItemText(hdwnd,IDC_EDIT22,"0.0");
			SetDlgItemText(hdwnd,IDC_EDIT23,"0.0");

			//设置背景色数据类型为LCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			//选择此按钮
			SendDlgItemMessage(hdwnd,IDC_RADIO8_NEUTRAL2,BM_GETCHECK,1,0);

			return 1;

		case IDC_RADIO9_LCH_SELF:	//背景色"自定LCH"单选钮

			//选择此按钮
			SendDlgItemMessage(hdwnd,IDC_RADIO9_LCH_SELF,BM_GETCHECK,1,0);

			//设置背景色数据编辑框为可输入状态
			SendDlgItemMessage(hdwnd,IDC_EDIT21,EM_SETREADONLY,0,0);
			SendDlgItemMessage(hdwnd,IDC_EDIT22,EM_SETREADONLY,0,0);
			SendDlgItemMessage(hdwnd,IDC_EDIT23,EM_SETREADONLY,0,0);

			//提示输入背景色明度值
			MessageBox(hdwnd,"请输入背景色明度L、彩度C、色调角H值","要求输入背景色数据",MB_OK);

			//设置背景色数据类型为LCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			return 1;

		case IDC_RADIO10_LAB_SELF:		//背景色"自定LAB"单选钮

			//选择此按钮
			SendDlgItemMessage(hdwnd,IDC_RADIO10_LAB_SELF,BM_GETCHECK,1,0);

			//设置背景色数据编辑框为可输入状态
			SendDlgItemMessage(hdwnd,IDC_EDIT21,EM_SETREADONLY,0,0);
			SendDlgItemMessage(hdwnd,IDC_EDIT22,EM_SETREADONLY,0,0);
			SendDlgItemMessage(hdwnd,IDC_EDIT23,EM_SETREADONLY,0,0);			
			
			//提示输入背景色明度值
			MessageBox(hdwnd,"请输入背景色LAB值","要求输入背景色数据",MB_OK);

			//设置背景色数据类型为LAB
			strcpy(StiTestParam.BKG_Type,"LAB");

			return 1;

		case IDC_DISPLAY_SET:

			//调用对话框函数：设置显示器参数、每像素视角
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG2),hdwnd,DlgProc2);

			return 1;

		case IDC_VIEWDISTANCE:

			//调用对话框函数：计算视距
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG3),hdwnd,DlgProc3);

			return 1;

		case IDC_TIFFSAVEPATH:

			//调用对话框函数：设置文件存储盘
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG4),hdwnd,DlgProc4);

			return 1;

		case IDC_TIFFGEN:

			//如果尚未设置测试类型，则警告
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"应先选择测试类型。","测试类型未设置",MB_OK|MB_ICONWARNING);

				return 0;
			}

			//如果尚未设置背景色类型，则警告
			if(strcmp(StiTestParam.BKG_Type,"")==0)
			{
				MessageBox(hdwnd,"背景色类型未选择。","背景色设置出错",MB_OK|MB_ICONWARNING);

				return 0;
			}

			//设置刺激图像参数：使其进入结构变量
			StiImgParamSetting(hdwnd,&StiTestParam);
			
			//检验输入的参数，得出错误号码
			Err_num = StiImgParamAnalyse(&StiTestParam);
			//按照号码出错警告
			ErrWarning_StiImg(hdwnd,Err_num);

			//无错，即进行刺激图像生成作业
			if(Err_num==0)
			{
				//计算：LCH-->LAB
				StiParamLchToLab(&StiTestParam);
				//计算图像个数：按最小/最大值和步距
				StiImgCount(&StiTestParam);
/*
				//输出lab值
				sprintf(tempstr,"L= %f, a=%f, b=%f",
					StiTestParam.End_L,StiTestParam.End_A,StiTestParam.End_B);

				MessageBox(hdwnd,tempstr,"Lab值",MB_OK|MB_ICONWARNING);
*/
				//TIFF刺激图像生成作业
				Err_num = TiffWaveImgFileGen(&TiffHdTable);	
				//出错警告
				ErrWarning_TiffImgFileGen(hdwnd,Err_num);

				//生成完毕，退出对话框
				EndDialog(hdwnd,0);
			}

			return 1;

		case IDCANCEL:
			EndDialog(hdwnd,0);
			return 1;
		}
	}
	return 0;
}

/*******************************************************************************************/
//对话框函数2：显示器参数及观察条件设置

BOOL CALLBACK DlgProc2(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	unsigned char	tmp_str[100];

	switch(message)
	{
	case WM_INITDIALOG:
		
		//对话框初始化
		
		//设置图像像素数缺省值
		strcpy(tmp_str,"1280");
		SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);
		strcpy(tmp_str,"1024");
		SetDlgItemText(hdwnd,IDC_EDIT22,tmp_str);
		strcpy(tmp_str,"340");
		SetDlgItemText(hdwnd,IDC_EDIT23,tmp_str);
		strcpy(tmp_str,"270");
		SetDlgItemText(hdwnd,IDC_EDIT24,tmp_str);

		ViewParam.AnglePerPixel = -10000;
		
		return FALSE;
			
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{

		case IDC_RADIO1_ANGLE05:	//点击“每像素视角0.5分”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,0,0))
			{
				//将“每像素视角0.5分”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,1,0);
				//不选“每像素视角1.0分”单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,0,0);
			}

			//设置观察条件参数：设置为错误值，用于判断用户是否设置过
			ViewParam.AnglePerPixel = 0.5;

			return 1;

		case IDC_RADIO2_ANGLE10:	//点击“每像素视角1.0分”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,0,0))
			{
				//将“每像素视角1.0分”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,1,0);
				//不选“每像素视角0.5分”单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,0,0);
			}

			//设置观察条件参数：每像素视角1.0分
			ViewParam.AnglePerPixel =1.0;

			return 1;

		case IDOK:

			//从编辑框获取显示器像素数、显示器尺寸,出错警告
			GetDlgItemText(hdwnd,IDC_EDIT21,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"未设置显示器横向像素数","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispParam.DispPixelH  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT22,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"未设置显示器纵向像素数","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispParam.DispPixelV  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT23,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"未设置显示器横向尺寸","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispParam.DispSizeH  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT24,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"未设置显示器纵向尺寸","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispParam.DispSizeV  = abs(atoi(tmp_str));}

			if(ViewParam.AnglePerPixel == -10000) 
			{
				MessageBox(hdwnd,"未选择每像素视角","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else 
			{
				//结束对话框
				EndDialog(hdwnd,0);
			}	
			
			return 1;

		case IDCANCEL:
			EndDialog(hdwnd,0);
			return 1;
		}
	}
	return 0;
}

/*******************************************************************************************/
//对话框函数3：视觉观察距离计算

BOOL CALLBACK DlgProc3(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	unsigned char	tmp_str[100];

	static	DWORD	DispPixH,DispPixV;
	static	double	DispH,DispV, PixelSize, AnglePerPixel,AnglePerPixelRad, ViewDistance;

	switch(message)
	{
	case WM_INITDIALOG:
		
		//对话框初始化
		
		//由刺激图像参数设置图像像素数缺省值
		itoa(DispParam.DispPixelH, tmp_str, 10);
		SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);
		itoa(DispParam.DispPixelV, tmp_str, 10);
		SetDlgItemText(hdwnd,IDC_EDIT22,tmp_str);

		sprintf(tmp_str,"%5.2f",DispParam.DispSizeH);
		SetDlgItemText(hdwnd,IDC_EDIT23,tmp_str);
		sprintf(tmp_str,"%5.2f",DispParam.DispSizeV);
		SetDlgItemText(hdwnd,IDC_EDIT24,tmp_str);

		AnglePerPixel = -10000;

		return FALSE;
			
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{

		case IDC_RADIO1_ANGLE05:	//点击“每像素视角0.5分”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,0,0))
			{
				//将“每像素视角0.5分”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,1,0);
				//不选“每像素视角1.0分”单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,0,0);
			}

			//设置观察条件参数：每像素视角0.5分
			AnglePerPixel = 0.5;

			return 1;

		case IDC_RADIO2_ANGLE10:	//点击“每像素视角1.0分”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,0,0))
			{
				//将“每像素视角1.0分”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,1,0);
				//不选“每像素视角0.5分”单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,0,0);
			}

			//设置观察条件参数：每像素视角1.0分
			AnglePerPixel = 1.0;

			return 1;

		case IDOK:

			//从编辑框获取显示器像素数、显示器尺寸,出错警告
			GetDlgItemText(hdwnd,IDC_EDIT21,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"未设置显示器横向像素数","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispPixH  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT22,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"未设置显示器纵向像素数","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispPixV  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT23,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"未设置显示器横向尺寸","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispH  = fabs(atof(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT24,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"未设置显示器纵向尺寸","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispV  = fabs(atof(tmp_str));}

			if(AnglePerPixel == -10000) 
			{
				MessageBox(hdwnd,"未选择每像素视角","参数缺失",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else 
			{
				//计算视距
				PixelSize = DispH / (double)DispPixH;//每个像素尺寸 =显示器纵向尺寸 / 纵向像素个数 
				AnglePerPixelRad = 3.1415926 * (AnglePerPixel/ (2.0 * 60.0)) / 180.0;//每像素视角转为弧度 ，1°=60′ 

				ViewDistance = PixelSize / (2 * tan(AnglePerPixelRad));

				//显示视距到编辑框
				sprintf(tmp_str, "%8.3f", ViewDistance);
				SetDlgItemText(hdwnd,IDC_EDIT1_VIEWDIST,tmp_str);

			}	
			
			return 1;

		case IDCANCEL:
			EndDialog(hdwnd,0);
			return 1;
		}
	}
	return 0;
}


/*******************************************************************************************/
//对话框函数4：存储盘设置

BOOL CALLBACK DlgProc4(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	switch(message)
	{
			
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{

		case IDC_RADIO_DISK1:	//点击“C:\”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0))
			{
				//将“C:\”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,1,0);
				//不选其他单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//设置为C盘
			strcpy(StiTestParam.StiFilePath,"c:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);

			return 1;

		case IDC_RADIO_DISK2:	//点击“D:\”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0))
			{
				//将“D:\”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,1,0);
				//不选其他单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//设置为D盘
			strcpy(StiTestParam.StiFilePath,"d:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);
			return 1;

		case IDC_RADIO_DISK3:	//点击“E:\”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0))
			{
				//将“E:\”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,1,0);
				//不选其他单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//设置为E盘
			strcpy(StiTestParam.StiFilePath,"e:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);

			return 1;

		case IDC_RADIO_DISK4:	//点击“F:\”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0))
			{
				//将“F:\”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,1,0);
				//不选其他单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//设置为F盘
			strcpy(StiTestParam.StiFilePath,"f:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);
			
			return 1;

		case IDC_RADIO_DISK5:	//点击“G:\”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0))
			{
				//将“G:\”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,1,0);
				//不选其他单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//设置为G盘
			strcpy(StiTestParam.StiFilePath,"g:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);

			return 1;

		case IDC_RADIO_DISK6:	//点击“H:\”单选按钮
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0))
			{
				//将“H:\”单选按钮选中
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,1,0);
				//不选其他单选按钮
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);

			}

			//设置为H盘
			strcpy(StiTestParam.StiFilePath,"h:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);

			return 1;
		
		case IDOK:
			EndDialog(hdwnd,0);
			return 1;
		}
	}
	return 0;
}

/*******************************************************************************************/
//刺激图像参数设置函数

void StiImgParamSetting(HWND hdwnd,struct StimulusParams *StiTestParam)
{
	unsigned char tempstr[256];

	//获取增减变化次数
	GetDlgItemText(hdwnd,IDC_EDIT18,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->PM_Steps = -80000;}
	else {StiTestParam->PM_Steps = abs(atol(tempstr));}

	//如果增减次数为0，置为1
	if(StiTestParam->PM_Steps == 0) {StiTestParam->PM_Steps =1;}
	
	if(strcmp(StiTestParam->TestType,"LL")==0)	//测试亮度频响
	{	
		//获取各编辑框中的字串，转换成小数
		//未输入，则数据置为-10000;

		//L0
		GetDlgItemText(hdwnd,IDC_EDIT5,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_L = -10000;}
		else {StiTestParam->Central_L  = fabs(atof(tempstr));}
		//dL
		GetDlgItemText(hdwnd,IDC_EDIT6,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Delta_L = -10000;}
		else {StiTestParam->Delta_L = fabs(atof(tempstr));}
		//H：
		GetDlgItemText(hdwnd,IDC_EDIT7,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_H = -10000;}
		else 
		{
			StiTestParam->Central_H = fabs(atof(tempstr));
		}
		//C：
		GetDlgItemText(hdwnd,IDC_EDIT8,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_C = -10000;}
		else 
		{
			StiTestParam->Central_C = fabs(atof(tempstr));	
		}
		
	}
	
	if(strcmp(StiTestParam->TestType,"CC")==0)	//测试彩度频响
	{	
		//获取各编辑框中的字串，转换成小数
		//未输入，则数据置为-20000;

		//C0
		GetDlgItemText(hdwnd,IDC_EDIT9,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_C = -20000;}
		else {StiTestParam->Central_C = fabs(atof(tempstr));}
		//dC
		GetDlgItemText(hdwnd,IDC_EDIT10,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Delta_C = -20000;}
		else {StiTestParam->Delta_C = fabs(atof(tempstr));}
		//H：
		GetDlgItemText(hdwnd,IDC_EDIT11,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_H  = -20000;}
		else 
		{
			StiTestParam->Central_H  = fabs(atof(tempstr));
		}
		//L：
		GetDlgItemText(hdwnd,IDC_EDIT12,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_L  = -20000;}
		else
		{
			StiTestParam->Central_L = fabs(atof(tempstr));
		}

	}
	
	if(strcmp(StiTestParam->TestType,"HH")==0)	//测试色调角频响
	{	
		//获取各编辑框中的字串，转换成小数
		//未输入，则数据置为-30000;

		//H0
		GetDlgItemText(hdwnd,IDC_EDIT13,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_H = -30000;}
		else {StiTestParam->Central_H = fabs(atof(tempstr));}		
		//dH
		GetDlgItemText(hdwnd,IDC_EDIT14,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Delta_H = -30000;}
		else {StiTestParam->Delta_H= fabs(atof(tempstr));}		
		//L
		GetDlgItemText(hdwnd,IDC_EDIT15,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_L = -30000;}
		else 
		{
			StiTestParam->Central_L = fabs(atof(tempstr));		
		}
		//C
		GetDlgItemText(hdwnd,IDC_EDIT16,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_C = -30000;}
		else
		{
			StiTestParam->Central_C  = fabs(atof(tempstr));		
		}
	}
	
	//背景色设置
	//数据为LCH类型
	if(strcmp(StiTestParam->BKG_Type,"LCH")==0)
	{
		//未输入数据(字符串为空)，将数据置为-40000
		//接收LCH数据
		GetDlgItemText(hdwnd,IDC_EDIT21,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->BKG_L = -40000;}
		else {StiTestParam->BKG_L = fabs(atol(tempstr));}
		
		GetDlgItemText(hdwnd,IDC_EDIT22,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->BKG_C = -40000;}
		else {StiTestParam->BKG_C = fabs(atol(tempstr));}

		GetDlgItemText(hdwnd,IDC_EDIT23,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->BKG_H = -40000;}
		else {StiTestParam->BKG_H = fabs(atol(tempstr));}

	}
	
	//数据为LAB类型
	if(strcmp(StiTestParam->BKG_Type,"LAB")==0)
	{
		//未输入数据(字符串为空)，将数据置为-40000
		//接收Lab数据：a,b不取绝对值
		GetDlgItemText(hdwnd,IDC_EDIT21,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->BKG_L = -40000;}
		else {StiTestParam->BKG_L = fabs(atol(tempstr));}
		
		GetDlgItemText(hdwnd,IDC_EDIT22,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->BKG_A = -40000;}
		else {StiTestParam->BKG_A = atol(tempstr);}

		GetDlgItemText(hdwnd,IDC_EDIT23,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->BKG_B = -40000;}
		else {StiTestParam->BKG_B = atol(tempstr);}
	}
		
	//图像公有参数
	//获取各编辑框中的字串，转换成小数
	//未输入，则数据置为-50000;

	//ImagePixHor
	GetDlgItemText(hdwnd,IDC_EDIT1,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->ImgPixelH = -50000;}
	else {StiTestParam->ImgPixelH = abs(atol(tempstr));}
	//ImagePixVer
	GetDlgItemText(hdwnd,IDC_EDIT2,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->ImgPixelV = -50000;}
	else {StiTestParam->ImgPixelV = abs(atol(tempstr));}
	//TestZonePixHor
	GetDlgItemText(hdwnd,IDC_EDIT3,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->TestZonePixelH = -50000;}
	else {StiTestParam->TestZonePixelH = abs(atol(tempstr));}
	//TestZonePixVer
	GetDlgItemText(hdwnd,IDC_EDIT4,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->TestZonePixelV = -50000;}
	else {StiTestParam->TestZonePixelV = abs(atol(tempstr));}

	//获取各编辑框中的字串，转换成小数
	//未输入，则数据置为-60000;

	//F(恒定频率，最大/最小值相等)
	GetDlgItemText(hdwnd,IDC_EDIT17,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->StiFreqMin = -60000;}
	else 
	{
		StiTestParam->StiFreqMin = fabs(atof(tempstr));//正弦波个数=周期数，计算绝对值，数据类型：浮点数 
		StiTestParam->StiFreqMax  = StiTestParam->StiFreqMin;
	}

	//Angle
	GetDlgItemText(hdwnd,IDC_EDIT20,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->StiAngle = -60000;}
	else {StiTestParam->StiAngle = fabs(atof(tempstr));}

	//图像存储路径
	//strcpy(StiTestParam->StiFilePath,"e:\\StiTest\\");
	//CreateDirectory(StiTestParam->StiFilePath,NULL);

	//生成带时间的子文件夹字符串
	GenTimeString(tempstr);
	strcat(StiTestParam->StiFilePath,tempstr);
	strcat(StiTestParam->StiFilePath,"\\");

}

/*******************************************************************************************/
//刺激图像参数分析函数

int	StiImgParamAnalyse(struct StimulusParams *StiTestParam)
{
	unsigned long m,n;

	//增减变化次数未输入，返回-80
	if(StiTestParam->PM_Steps ==-80000) return -80;

	//测试亮度频响
	if(strcmp(StiTestParam->TestType,"LL")==0)
	{
		m = (unsigned long)(StiTestParam->Central_L / (StiTestParam->Delta_L/2.0));
		n = (unsigned long)((100.0 - StiTestParam->Central_L)/(StiTestParam->Delta_L/2.0));


		//有数据未输入，返回-10
		if((StiTestParam->Central_L ==-10000)||(StiTestParam->Delta_L==-10000)||
			(StiTestParam->Central_H==-10000)||(StiTestParam->Central_C==-10000)) 
			return -10;

		//中心明度值小于0或大于100，返回-11
		if((StiTestParam->Central_L >100)||(StiTestParam->Central_L <0)) return -11;
		//L步距=0，返回-12
		if((StiTestParam->Delta_L) == 0) return -12;
		
		if(m <StiTestParam->PM_Steps) return -13;
		if(n <StiTestParam->PM_Steps) return -14;		
		
		//色调角大于360度或小于0度，返回-15
		if((StiTestParam->Central_H >360)||(StiTestParam->Central_H <0)) return -15;
		//彩度值小于0，返回-16
		if(StiTestParam->Central_C <0) return -16;
		//中心明度+多倍步距大于100，返回-17
		if((StiTestParam->Central_L - (StiTestParam->PM_Steps * StiTestParam->Delta_L/2.0)) < 0) 
			return -17;
		//中心明度+多倍步距大于100，返回-18
		if((StiTestParam->Central_L + (StiTestParam->PM_Steps * StiTestParam->Delta_L/2.0))>100.0)
		return -18;
		
		
	//	if((StiTestParam->Central_L + (StiTestParam->PM_Steps * StiTestParam->Delta_L/2.0)) >100 0) 
	//		return -18;
		
	}
	
	//测试彩度频响
	if(strcmp(StiTestParam->TestType,"CC")==0)
	{
		m = (unsigned long)(StiTestParam->Central_C / (StiTestParam->Delta_C/2.0));
		n = (unsigned long)((180.3136 - StiTestParam->Central_C)/(StiTestParam->Delta_C/2.0));

		//有数据未输入，返回-20
		if((StiTestParam->Central_C==-20000)||(StiTestParam->Delta_C==-20000)||
			(StiTestParam->Central_L==-20000)||(StiTestParam->Central_H==-20000)) 
			return -20;

		//中心彩度值小于0或大于180.3136，返回-21
		if((StiTestParam->Central_C >180.3136)||(StiTestParam->Central_C <0)) return -21;
		//C步距=0，返回-22
		if((StiTestParam->Delta_C) == 0) return -22;

		if(m <StiTestParam->PM_Steps) return -23;
		if(n <StiTestParam->PM_Steps) return -24;		
		
		//中心明度值小于0或大于100，返回-11
		if((StiTestParam->Central_L >100)||(StiTestParam->Central_L <0)) return -25;
		//色调角大于360度或小于0度，返回-26
		if((StiTestParam->Central_H >360)||(StiTestParam->Central_H <0)) return -26;
		//中心彩度-多倍步距小于0，返回-27
		if((StiTestParam->Central_C - (StiTestParam->PM_Steps * StiTestParam->Delta_C/2.0)) < 0) return -27;
		//中心彩度+多倍步距大于100，返回-28
		if((StiTestParam->Central_C + (StiTestParam->PM_Steps * StiTestParam->Delta_C/2.0)) >180.3136) return -28;

	}

	//测试色调角频响
	if(strcmp(StiTestParam->TestType,"HH")==0)
	{
		m = (unsigned long)(StiTestParam->Central_H / (StiTestParam->Delta_H/2.0));
		n = (unsigned long)((360 - StiTestParam->Central_H)/(StiTestParam->Delta_H/2.0));

		//有数据未输入，返回-30
		if((StiTestParam->Central_H==-30000)||(StiTestParam->Delta_H==-30000)||
			(StiTestParam->Central_L==-30000)||(StiTestParam->Central_C==-30000)) 
			return -30;

		//中心色调角小于0或大于360，返回-31
		if((StiTestParam->Central_H >360)||(StiTestParam->Central_H <0)) return -31;
		//H步距=0，返回-32
		if((StiTestParam->Delta_H) == 0) return -32;

		if(m <StiTestParam->PM_Steps) return -23;
		if(n <StiTestParam->PM_Steps) return -24;		
		
		//中心明度值小于0或大于100，返回-35
		if((StiTestParam->Central_L >100)||(StiTestParam->Central_L <0)) return -35;
		//中心彩度大于180.3136或小于0，返回-36
		if((StiTestParam->Central_C >180.3136)||(StiTestParam->Central_C <0)) return -36;
		//中心色调角-多倍步距小于0，返回-37
		if((StiTestParam->Central_H - (StiTestParam->PM_Steps * StiTestParam->Delta_H/2.0)) < 0) return -37;
		//中心色调角+多倍步距大于100，返回-38
		if((StiTestParam->Central_H + (StiTestParam->PM_Steps * StiTestParam->Delta_H/2.0)) >360) return -38;

	}

	//背景色
	//有背景色数据未输入，返回-41
	if((StiTestParam->BKG_L==-40000)||(StiTestParam->BKG_C==-40000)||
	   (StiTestParam->BKG_H==-40000)||(StiTestParam->BKG_A==-40000)||
	   (StiTestParam->BKG_B==-40000)) return -41;

	//明度超范围，返回-42
	if((StiTestParam->BKG_L>100)||(StiTestParam->BKG_L<0)) return -42;
	//彩度超范围，返回-43
	if((StiTestParam->BKG_C>180.3136156)||(StiTestParam->BKG_C<0)) return -43;
	//色调角超范围，返回-44
	if((StiTestParam->BKG_H>360)||(StiTestParam->BKG_H<0)) return -44;
	//a*超范围，返回-45
	if((StiTestParam->BKG_A>127)||(StiTestParam->BKG_A<-128)) return -45;
	//b*超范围，返回-46
	if((StiTestParam->BKG_B>127)||(StiTestParam->BKG_B<-128)) return -46;

	//有图像像素参数未输入
	if((StiTestParam->TestZonePixelH ==-50000)||(StiTestParam->TestZonePixelV==-50000)||
	   (StiTestParam->ImgPixelH==-50000)||(StiTestParam->ImgPixelV==-50000)) 
	   return -50;

	//图像中心测试区横向像素数>图像横向像素数，返回-51
	if(StiTestParam->TestZonePixelH > StiTestParam->ImgPixelH) return -51;
	//图像中心测试区纵向像素数>图像纵向像素数，返回-52
	if(StiTestParam->TestZonePixelV > StiTestParam->ImgPixelV) return -52;

	//频率设置有数据未输入，返回-60
	if((StiTestParam->StiFreqMin==-60000)||(StiTestParam->StiAngle==-60000)) 
		return -60;

	//生成图像文件夹，出错返回-71
	if(CreateDirectory(StiTestParam->StiFilePath,NULL)==0)	return -71;
	
	//全部正确返回0
	return 0;
}

/*******************************************************************************************/
//显示及观察条件参数分析函数

int	DispViewParamAnalyse(struct DisplayParams DispParam,struct ViewCond ViewParam)
{

	return 0;
}

//错误警告显示函数
void ErrWarning_StiImg(HWND hdwnd,int Err_num)
{
	switch(Err_num)
	{
	case -10:
		MessageBox(hdwnd,"明度正弦波测试：有参数未输入","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -11:
		MessageBox(hdwnd,"中心明度值超出[0~100]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -12:
		MessageBox(hdwnd,"明度步距为0","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -13:
		MessageBox(hdwnd,"中心明度值递减多倍步距后<0，减小次数或步距","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -14:
		MessageBox(hdwnd,"中心明度值递增多倍步距后>100，减小次数或步距","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -15:
		MessageBox(hdwnd,"波形色调角超出[0~360]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -16:
		MessageBox(hdwnd,"波形彩度超范围(小于0等)","参数错误",MB_ICONWARNING|MB_OK);
		break;

	case -20:
		MessageBox(hdwnd,"彩度正弦波测试：有参数未输入","参数错误",MB_ICONWARNING|MB_OK);
	case -21:
		MessageBox(hdwnd,"中心彩度值小于0或大于180.31","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -22:
		MessageBox(hdwnd,"彩度值步距为0","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -23:
		MessageBox(hdwnd,"中心彩度值递减多倍步距后<0，减小次数或步距","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -24:
		MessageBox(hdwnd,"中心彩度值递减多倍步距后>180，减小次数或步距","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -25:
		MessageBox(hdwnd,"波形明度值超出[0~100]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -26:
		MessageBox(hdwnd,"波形色调角超出[0~360]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;

	case -30:
		MessageBox(hdwnd,"色调角正弦波测试：有参数未输入","参数错误",MB_ICONWARNING|MB_OK);
	case -31:
		MessageBox(hdwnd,"中心色调角超出[0~360]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -32:
		MessageBox(hdwnd,"色调角步距为0","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -33:
		MessageBox(hdwnd,"中心色调角递减多倍步距后<0，减小次数或步距","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -34:
		MessageBox(hdwnd,"中心彩度值递增多倍步距后>360，减小次数或步距","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -35:
		MessageBox(hdwnd,"波形明度值超出[0~100]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -36:
		MessageBox(hdwnd,"波形彩度值超范围(小于0等)","参数错误",MB_ICONWARNING|MB_OK);
		break;

	case -41:
		MessageBox(hdwnd,"有背景色数据未输入","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -42:
		MessageBox(hdwnd,"背景色明度值超出[0~100]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -43:
		MessageBox(hdwnd,"背景色彩度值超[0~180.3136516]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -44:
		MessageBox(hdwnd,"背景色色调角超出[0~360]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -45:
		MessageBox(hdwnd,"背景色a*超出[-128~+127]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -46:
		MessageBox(hdwnd,"背景色b*超出[-128~+127]范围","参数错误",MB_ICONWARNING|MB_OK);
		break;

	case -50:
		MessageBox(hdwnd,"图像像素行列数：有数据未输入","参数错误",MB_ICONWARNING|MB_OK);
		break;
	case -51:
		MessageBox(hdwnd,"测试区横向像素数大于图像横向像素数","参数错误",MB_ICONWARNING|MB_OK);
		break;

	case -60:
		MessageBox(hdwnd,"正弦波频率、方向角：有数据未输入。","参数错误",MB_ICONWARNING|MB_OK);
		break;

	case -71:
		MessageBox(hdwnd,"所要求的图像文件夹不能生成","参数错误",MB_ICONWARNING|MB_OK);
		break;

	case -80:
		MessageBox(hdwnd,"未输入增减变化的次数","参数错误",MB_ICONWARNING|MB_OK);
		break;


	default:
		break;
	}

}

/*******************************************************************************************/
//色度LCH->LAB转换函数
void StiParamLchToLab(struct StimulusParams *StiTestParam)
{

	//将最小最大LCH转换成Lab
	Lch2Lab(&StiTestParam->End_C, &StiTestParam->End_H, 
		&StiTestParam->End_A, &StiTestParam->End_B);
	
	Lch2Lab(&StiTestParam->Start_C, &StiTestParam->Start_H, 
		&StiTestParam->Start_A, &StiTestParam->Start_B);


	//背景色转换LCH转换成Lab
	Lch2Lab(&StiTestParam->BKG_C, &StiTestParam->BKG_H, 
		&StiTestParam->BKG_A, &StiTestParam->BKG_B);

}

/*******************************************************************************************/
//计算：按中心值增减多个步距的最大值、最小值、刺激图像个数
void StiImgCount(struct StimulusParams *StiTestParam)
{
	//计算最大最小值：按不同测试方式
	if(strcmp(StiTestParam->TestType,"LL")==0)
	{
		StiTestParam->Start_L = StiTestParam->Central_L 
									- StiTestParam->PM_Steps * StiTestParam->Delta_L /2.0;
	
		StiTestParam->End_L  = StiTestParam->Central_L 
									+ StiTestParam->PM_Steps * StiTestParam->Delta_L /2.0;

		StiTestParam->Start_C = StiTestParam->Central_C;
		StiTestParam->End_C = StiTestParam->Central_C;

		StiTestParam->Start_H = StiTestParam->Central_H;
		StiTestParam->End_H = StiTestParam->Central_H;		
	}

	if(strcmp(StiTestParam->TestType,"CC")==0)
	{
		StiTestParam->Start_C = StiTestParam->Central_C 
									- StiTestParam->PM_Steps * StiTestParam->Delta_C/2.0;
	
		StiTestParam->End_C  = StiTestParam->Central_C 
									+ StiTestParam->PM_Steps * StiTestParam->Delta_C/2.0;

		StiTestParam->Start_L = StiTestParam->Central_L;
		StiTestParam->End_L = StiTestParam->Central_L;

		StiTestParam->Start_H = StiTestParam->Central_H;
		StiTestParam->End_H = StiTestParam->Central_H;		
	}
	
	if(strcmp(StiTestParam->TestType,"HH")==0)
	{
		StiTestParam->Start_H = StiTestParam->Central_H 
									- StiTestParam->PM_Steps * StiTestParam->Delta_H/2.0;
	
		StiTestParam->End_H  = StiTestParam->Central_H 
									+ StiTestParam->PM_Steps * StiTestParam->Delta_H/2.0;

		StiTestParam->Start_L = StiTestParam->Central_L;
		StiTestParam->End_L = StiTestParam->Central_L;

		StiTestParam->Start_C = StiTestParam->Central_C;
		StiTestParam->End_C = StiTestParam->Central_C;		
	}

	//图像个数= 增减次数
	StiTestParam->StiImgCount = StiTestParam->PM_Steps ;

}

/*******************************************************************************************/

//TIFF文件头及图像文件目录数据设置函数

void TiffHeaderTableSet(struct TiffHeaderTable *TiffHdTable,struct StimulusParams *StiTestParam)
{
	
	//Tiff文件头结构赋值
	TiffHdTable->i1 = 'I';
	TiffHdTable->i2 = 'I';
	TiffHdTable->vers = 42;
	TiffHdTable->ifd_offs = 8;
	TiffHdTable->tag_count = 13;
	
	//Tiff文件目录信息结构赋值
	TiffHdTable->N_ImgWidth = 256;
	TiffHdTable->Type_ImgWidth =4;
	TiffHdTable->Count_ImgWidth =1;
	TiffHdTable->Intent_ImgWidth =StiTestParam->ImgPixelH;
	
	TiffHdTable->N_ImgLen = 257;
	TiffHdTable->Type_ImgLen =4;
	TiffHdTable->Count_ImgLen =1;
	TiffHdTable->Intent_ImgLen =StiTestParam->ImgPixelV;
	
	TiffHdTable->N_BitsPSmp = 258;
	TiffHdTable->Type_BitsPSmp = 3;
	TiffHdTable->Count_BitsPSmp = 3;
	TiffHdTable->Addr_BitsPSmp = 170;
	
	TiffHdTable->N_Compress = 259;
	TiffHdTable->Type_Compress = 3;
	TiffHdTable->Count_Compress = 1;
	TiffHdTable->Intent_Compress = 1;
	
	TiffHdTable->N_LAB = 262;
	TiffHdTable->Type_LAB = 3;
	TiffHdTable->Count_LAB = 1;
	TiffHdTable->Intent_LAB = 8;
	
	TiffHdTable->N_StripOffs = 273;
	TiffHdTable->Type_StripOffs = 4;
	TiffHdTable->Count_StripOffs = 1;
	TiffHdTable->Intent_StripOffs = 192;
	
	TiffHdTable->N_SmpPerPixel = 277;
	TiffHdTable->Type_SmpPerPixel = 4;
	TiffHdTable->Count_SmpPerPixel = 1;
	TiffHdTable->Intent_SmpPerPixel = 3;
	
	TiffHdTable->N_RowsPerStrip = 278;
	TiffHdTable->Type_RowsPerStrip = 4;
	TiffHdTable->Count_RowsPerStrip = 1;
	TiffHdTable->Intent_RowsPerStrip =TiffHdTable->Intent_ImgLen;
	
	TiffHdTable->N_ByteCounts = 279;
	TiffHdTable->Type_ByteCounts = 4;
	TiffHdTable->Count_ByteCounts = 1;
	TiffHdTable->Intent_ByteCounts =6 * TiffHdTable->Intent_ImgWidth*TiffHdTable->Intent_ImgLen;
	
	TiffHdTable->N_XRes = 282;
	TiffHdTable->Type_XRes = 5;
	TiffHdTable->Count_XRes = 1;
	TiffHdTable->Addr_XRes = 176;
	
	TiffHdTable->N_YRes = 283;
	TiffHdTable->Type_YRes = 5;
	TiffHdTable->Count_YRes = 1;
	TiffHdTable->Addr_YRes = 184;
	
	TiffHdTable->N_PlanarConfig = 284;
	TiffHdTable->Type_PlanarConfig = 3;
	TiffHdTable->Count_PlanarConfig = 1;
	TiffHdTable->Intent_PlanarConfig = 1;
	
	TiffHdTable->N_ResUnit = 296;
	TiffHdTable->Type_ResUnit = 3;
	TiffHdTable->Count_ResUnit = 1;
	TiffHdTable->Intent_ResUnit = 2;
	
	TiffHdTable->EndZeros = 0;
	
	TiffHdTable->SmpPerPixel1 = 16;
	TiffHdTable->SmpPerPixel2 = 16;
	TiffHdTable->SmpPerPixel3 = 16;
	
	TiffHdTable->XRes_Num = 864680;
	TiffHdTable->XRes_Denom = 10000;
	
	TiffHdTable->YRes_Num = 864680;
	TiffHdTable->YRes_Denom = 10000;

}

/*******************************************************************************************/
//TIFF文件头及图像文件目录数据写入文件的函数
int TiffHeaderTableWrite(unsigned char *TiffFileName,struct TiffHeaderTable *TiffHdTable)
{
	
	//声明文件指针
	FILE *fp;

	//打开文件用于“二进制写”
	fp = fopen(TiffFileName,"wb");

	//出错返回-1
	if(fp==NULL) {return -1;}
	
	//将文件头和标记表写入
	fwrite(&(TiffHdTable->i1),sizeof(unsigned char),1,fp);
	fwrite(&(TiffHdTable->i2),sizeof(unsigned char),1,fp);
	fwrite(&(TiffHdTable->vers),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->ifd_offs),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->tag_count),sizeof(unsigned short),1,fp);
	
	fwrite(&(TiffHdTable->N_ImgWidth),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_ImgWidth),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_ImgWidth),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_ImgWidth),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_ImgLen),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_ImgLen),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_ImgLen),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_ImgLen),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_BitsPSmp),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_BitsPSmp),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_BitsPSmp),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Addr_BitsPSmp),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_Compress),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_Compress),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_Compress),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_Compress),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_LAB),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_LAB),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_LAB),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_LAB),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_StripOffs),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_StripOffs),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_StripOffs),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_StripOffs),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_SmpPerPixel),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_SmpPerPixel),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_SmpPerPixel),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_SmpPerPixel),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_RowsPerStrip),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_RowsPerStrip),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_RowsPerStrip),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_RowsPerStrip),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_ByteCounts),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_ByteCounts),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_ByteCounts),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_ByteCounts),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_XRes),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_XRes),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_XRes),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Addr_XRes),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_YRes),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_YRes),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_YRes),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Addr_YRes),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_ResUnit),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_ResUnit),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_ResUnit),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_ResUnit),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->N_PlanarConfig),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Type_PlanarConfig),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->Count_PlanarConfig),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->Intent_PlanarConfig),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->EndZeros),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->SmpPerPixel1),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->SmpPerPixel2),sizeof(unsigned short),1,fp);
	fwrite(&(TiffHdTable->SmpPerPixel3),sizeof(unsigned short),1,fp);
	
	fwrite(&(TiffHdTable->XRes_Num),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->XRes_Denom),sizeof(unsigned long),1,fp);
	
	fwrite(&(TiffHdTable->YRes_Num),sizeof(unsigned long),1,fp);
	fwrite(&(TiffHdTable->YRes_Denom),sizeof(unsigned long),1,fp);
	
	//关闭文件
	fclose(fp);

	return 0;

}
/*******************************************************************************************/
//TIFF图像文件波形生成函数
int TiffWaveImgFileGen(struct TiffHeaderTable *TiffHdTable)
{
	
	//循环变量
	DWORD			i,j,k;

	//文件指针
	FILE			*fp;
	
	//Tiff基本文件名和Tiff最终文件名字符串变量
	unsigned char	TiffBaseName[200],TiffFileName[300];
	
	//临时字符串变量
	unsigned char	tmpstr[256];
	
	//TIFF图像空间频率
	double	ImgFreq;

	//当前TIFF图像LCH和A、B的初值和终止值
	double L0, A0, B0, C0, H0;
	double L1, A1, B1, C1, H1;


	//坐标变量
	double	x,u,v;

	//函数值变量
	double	y;

	//色度值中间变量
	double	Ls,As,Bs;

	//方向角弧度值中间变量
	double	StiAngle_R;

	//圆周率值
	double	pi=3.1415926;

	//图像内存指针变量
	WORD	*StiImg,*WaveZoneImg;	//StiImg用于整个刺激图像
									//WaveZoneImg测试区波形图像
									//16bit图像用双字节的WORD类型(无符号整数)
									
	//全图像及测试区波形图像的横纵像素数
	DWORD	StiImgH,StiImgV,WaveImgH,WaveImgV;

	//测试区波形图像左上角起点坐标
	DWORD	LeftTopHor,LeftTopVer;

	//测试区在内存中的偏移量
	DWORD	WaveZoneOffs;

	//*************** Tiff图像生成 ********************


	//循环生成多个TIFF图像：空间频率不同
	for(k=1;k<=StiTestParam.PM_Steps;k++)
	{
		//计算当前TIFF文件的LCH和AB起始值/终止值
		if((strcmp(StiTestParam.TestType,"LL"))==0) 
		{
			L0 = StiTestParam.Central_L - k * StiTestParam.Delta_L/2.0 ;
			L1 = StiTestParam.Central_L + k * StiTestParam.Delta_L/2.0 ;

			C0 = StiTestParam.Central_C;
			C1 = StiTestParam.Central_C;
			H0 = StiTestParam.Central_H;
			H1 = StiTestParam.Central_H;

			Lch2Lab(&C0,&H0,&A0,&B0);
			Lch2Lab(&C1,&H1,&A1,&B1);			
		}

		if((strcmp(StiTestParam.TestType,"CC"))==0) 
		{
			C0 = StiTestParam.Central_C - k * StiTestParam.Delta_C/2.0 ;
			C1 = StiTestParam.Central_C + k * StiTestParam.Delta_C/2.0 ;

			L0 = StiTestParam.Central_L;
			L1 = StiTestParam.Central_L;
			H0 = StiTestParam.Central_H;
			H1 = StiTestParam.Central_H;

			Lch2Lab(&C0,&H0,&A0,&B0);
			Lch2Lab(&C1,&H1,&A1,&B1);			
			
		}
		if((strcmp(StiTestParam.TestType,"HH"))==0) 
		{
			H0 = StiTestParam.Central_H - k * StiTestParam.Delta_H/2.0 ;
			H1 = StiTestParam.Central_H + k * StiTestParam.Delta_H/2.0 ;

			C0 = StiTestParam.Central_C;
			C1 = StiTestParam.Central_C;
			L0 = StiTestParam.Central_L;
			L1 = StiTestParam.Central_L;

			Lch2Lab(&C0,&H0,&A0,&B0);
			Lch2Lab(&C1,&H1,&A1,&B1);			
			
		}

		//Tiff文件名清空
		strcpy(TiffFileName,"");

		//基本文件名清空
		strcpy(TiffBaseName,"");

		//按照测试类型，文件基本名："CSF_类型双字母LL_F频率值_L最小_最大_C值_H值.tif"
		//							"CSF_类型双字母CC_F频率值_L值_C最小_最大_H值.tif"
		//							"CSF_类型双字母HH_F频率值_L值_C值_H最小_最大.tif"

		//文件基本名：测试类型
		if((strcmp(StiTestParam.TestType,"LL"))==0) {strcpy(TiffBaseName,"CSF_LL_F");}
		if((strcmp(StiTestParam.TestType,"CC"))==0) {strcpy(TiffBaseName,"CSF_CC_F");}
		if((strcmp(StiTestParam.TestType,"HH"))==0) {strcpy(TiffBaseName,"CSF_HH_F");}

		//图像空间频率值(固定不变)
		ImgFreq = StiTestParam.StiFreqMin;

		//生成频率字符串
		sprintf(tmpstr,"%5.2f",ImgFreq);

		//频率字符串组合入基本文件名
		strcat(TiffBaseName,tmpstr);
		strcat(TiffBaseName,"_L");

		//LCH值-->文件基本名
		if((strcmp(StiTestParam.TestType,"LL"))==0) 
		{
			//最小明度字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",L0);
			strcat(TiffBaseName,tmpstr);
			//间隔符
			strcat(TiffBaseName,"_");
			//最大明度字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",L1);
			strcat(TiffBaseName,tmpstr);
			//间隔符+彩度提示符C
			strcat(TiffBaseName,"_C");
			//彩度字符串-->基本名文件字符串
			sprintf(tmpstr,"%5.2f",C0);
			strcat(TiffBaseName,tmpstr);
			//间隔符+色调角提示符H
			strcat(TiffBaseName,"_H");
			//色调角字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",H0);
			strcat(TiffBaseName,tmpstr);
			//加入方向角
			strcat(TiffBaseName,"_ANG");
			//方向角-->基本文件名字符串
			sprintf(tmpstr,"%4.1f",StiTestParam.StiAngle);
			strcat(TiffBaseName,tmpstr);
			//加入".tif"
			strcat(TiffBaseName,".tif");
		}

		if((strcmp(StiTestParam.TestType,"CC"))==0) 
		{
			//明度字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",L0);
			strcat(TiffBaseName,tmpstr);
			//间隔符+彩度提示符C
			strcat(TiffBaseName,"_C");
			//最小彩度字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",C0);
			strcat(TiffBaseName,tmpstr);
			//间隔符
			strcat(TiffBaseName,"_");
			//最大彩度字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",C1);
			strcat(TiffBaseName,tmpstr);
			//间隔符+色调角提示符H
			strcat(TiffBaseName,"_H");
			//色调角字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",H0);
			strcat(TiffBaseName,tmpstr);
			//加入".tif"
			strcat(TiffBaseName,".tif");
		}
		
		if((strcmp(StiTestParam.TestType,"HH"))==0) 
		{
			//明度字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",L0);
			strcat(TiffBaseName,tmpstr);
			//间隔符+彩度提示符C
			strcat(TiffBaseName,"_C");
			//彩度字符串-->基本名文件字符串
			sprintf(tmpstr,"%5.2f",C0);
			strcat(TiffBaseName,tmpstr);
			//间隔符+色调角提示符H
			strcat(TiffBaseName,"_H");
			//最小色调角字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",H0);
			strcat(TiffBaseName,tmpstr);
			//间隔符
			strcat(TiffBaseName,"_");
			//最大色调角字符串-->基本文件名字符串
			sprintf(tmpstr,"%5.2f",H1);
			strcat(TiffBaseName,tmpstr);
			//加入".tif"
			strcat(TiffBaseName,".tif");
		}

		//最终Tiff文件名：Tiff路径 + Tiff基本名
		strcat(TiffFileName,StiTestParam.StiFilePath);
		strcat(TiffFileName,TiffBaseName);
		
		//为TIFF文件头和标记表填充数据：所有TIFF文件头和标记表相同
		TiffHeaderTableSet(TiffHdTable,	&StiTestParam);
		
		//Tiff文件头和标记表写入文件
		TiffHeaderTableWrite(TiffFileName,TiffHdTable);

		//图像像素行列数
		StiImgH = StiTestParam.ImgPixelH;
		StiImgV = StiTestParam.ImgPixelV;
		WaveImgH = StiTestParam.TestZonePixelH;
		WaveImgV = StiTestParam.TestZonePixelV;

		//波形方向角弧度值计算：逆时针转角，角度为负
		StiAngle_R =  (-1.0) * StiTestParam.StiAngle * pi /180.0;		
		
		//为LAB完整图像分配内存，出错返回-1
		StiImg = (WORD *)calloc(3 * StiImgH * StiImgV,sizeof(WORD));
		if(StiImg == NULL) return -1;
		
		//计算：波形图像左上角起点坐标
		LeftTopHor = (DWORD)((double)(StiImgH - WaveImgH) / 2.0 +.5);
		LeftTopVer = (DWORD)((double)(StiImgV - WaveImgV) / 2.0 +.5);
		
		//为LAB正弦波图像分配内存
		WaveZoneImg = (WORD *)calloc(3 * WaveImgH * WaveImgV,sizeof(WORD));
		if(WaveZoneImg == NULL) return -2;
		
		//全图像背景色生成
		for(i=0;i<StiImgV;i++)
		{
			for(j=0;j<StiImgH;j++)
			{
				*(StiImg + 3*(i * StiImgH + j))     = (WORD) (655.35 * (WORD)StiTestParam.BKG_L);
				*(StiImg + 3*(i * StiImgH + j) + 1) = (WORD) (256.0  * (WORD)StiTestParam.BKG_A);
				*(StiImg + 3*(i * StiImgH + j) + 2) = (WORD) (256.0  * (WORD)StiTestParam.BKG_B);	
			}
		}
		
		//明度测试类型
		if((strcmp(StiTestParam.TestType,"LL"))==0)
		{
			
			//仅明度变化，a*b*不变，计算一遍即可
			As = 256.0 * A0;
			Bs = 256.0 * B0;
			
			//测试区的波形生成
			for(i=0;i<WaveImgV;i++)
			{
				for(j=0;j<WaveImgH;j++)
				{
					//波形方向角等于0度
					if(StiTestParam.StiAngle == 0)
					{
						//沿横向计算
						x = (double)j/(double)(WaveImgH - 1);				
					}
					else	//波形方向角不等于0度
					{
						v = (double)i/(double)StiTestParam.TestZonePixelV;
						u = (double) j/(double)StiTestParam.TestZonePixelH;						
						
						//转角计算
						x = u * cos(StiAngle_R) + v * sin(StiAngle_R);							
					}
					
					//正弦波的函数值
					y = 0.5 * (L1 - L0) 
						* (sin(6.2831852 * ImgFreq * x) + 1.0) 
						+ L0;
					
					//波形区域内的偏移量
					WaveZoneOffs =  3 * (i * WaveImgH + j);
					
					//明度值L的编码
					Ls = 655.35 * y;
					
					//Lab编码值取整，放入测试图像区域
					*(WaveZoneImg + WaveZoneOffs) = (WORD)Ls;
					*(WaveZoneImg + WaveZoneOffs + 1) = (WORD) As;
					*(WaveZoneImg + WaveZoneOffs + 2) = (WORD) Bs;
					
				}
			}			
		}	
		
		//彩度测试类型
		if((strcmp(StiTestParam.TestType,"CC"))==0)
		{
			
			//测试区的波形生成
			
			//明度不变，编码值计算一遍即可
			Ls = 655.35 * L0;
			
			for(i=0;i<WaveImgV;i++)
			{
				for(j=0;j<WaveImgH;j++)
				{
					//波形方向角等于0度
					if(StiTestParam.StiAngle == 0)
					{
						//沿横向计算
						x = (double)j/(double)(WaveImgH - 1);				
					}
					else	//波形方向角不等于0度
					{
						v = (double)i/(double)StiTestParam.TestZonePixelV;
						u = (double) j/(double)StiTestParam.TestZonePixelH;						
						
						//转角计算
						x = u * cos(StiAngle_R) + v * sin(StiAngle_R);							
					}
					
					//正弦波的函数值
					y = 0.5 * (C1 - C0) 
						* (sin(6.2831852 * ImgFreq * x) + 1.0) 
						+ C0;

					Lch2Lab(&y, &H0, &As, &Bs);

					//计算A,B的编码值，仍为浮点数
					As = 256.0 * As;
					Bs = 256.0 * Bs;
					
					//波形区域内的偏移量
					WaveZoneOffs =  3 * (i * WaveImgH + j);
					
					//Lab编码值取整，放入测试图像区域
					*(WaveZoneImg + WaveZoneOffs) = (WORD)Ls;
					*(WaveZoneImg + WaveZoneOffs + 1) = (WORD) As;
					*(WaveZoneImg + WaveZoneOffs + 2) = (WORD) Bs;				
				}
			}
		}
		
		//色调角测试类型
		if((strcmp(StiTestParam.TestType,"HH"))==0)
		{
			
			//测试区的波形生成
			
			//明度不变，编码值计算一遍即可
			Ls = 655.35 * L0;
			
			for(i=0;i<WaveImgV;i++)
			{
				for(j=0;j<WaveImgH;j++)
				{
					//波形方向角等于0度
					if(StiTestParam.StiAngle == 0)
					{
						//沿横向计算
						x = (double)j/(double)(WaveImgH - 1);				
					}
					else	//波形方向角不等于0度
					{
						v = (double)i/(double)StiTestParam.TestZonePixelV;
						u = (double) j/(double)StiTestParam.TestZonePixelH;						
						
						//转角计算
						x = u * cos(StiAngle_R) + v * sin(StiAngle_R);						
					}	
					
					//正弦波的函数值
					//基础函数为 y = sin(2*(pi)*f*x +(3/2)pi) + 1
					y = 0.5 * (H1 - H0) 
						* (sin(6.2831852 * ImgFreq * x + 4.71238898) + 1.0) 
						+ H0;

					Lch2Lab(&C0, &y, &As, &Bs);
				
					//计算A,B的编码值，仍为浮点数
					As = 256.0 * As;
					Bs = 256.0 * Bs;
					
					//波形区域内的偏移量
					WaveZoneOffs =  3 * (i * WaveImgH + j);
					
					//Lab编码值取整，放入测试图像区域
					*(WaveZoneImg + WaveZoneOffs) = (WORD)Ls;
					*(WaveZoneImg + WaveZoneOffs + 1) = (WORD) As;
					*(WaveZoneImg + WaveZoneOffs + 2) = (WORD) Bs;				
				}
			}
			
		}
		
		//波形区嵌入Tiff图像
		for(i=0;i<WaveImgV;i++)
		{
			//测试区在全图中的偏移量
			WaveZoneOffs = 3 * ((LeftTopVer + i) * StiImgH + LeftTopHor);
			
			//原来的实例：WaveZoneOffs = 3*((384 + i)*1280 + 512);
			
			for(j=0;j<WaveImgH;j++)
			{
				*( StiImg + WaveZoneOffs + 3*j     )= *(WaveZoneImg + 3* (i * WaveImgH + j)    );
				*((StiImg + WaveZoneOffs + 3*j + 1))= *(WaveZoneImg + 3* (i * WaveImgH + j) + 1);
				*((StiImg + WaveZoneOffs + 3*j + 2))= *(WaveZoneImg + 3* (i * WaveImgH + j) + 2);				
			}
		}
		
		//打开：创建TIFF文件
		fp = fopen(TiffFileName,"ab");
		
		//出错返回-3
		if(fp==NULL) return -3;
		
		
		fseek(fp,TiffHdTable->Intent_StripOffs,SEEK_SET);
		
		fwrite(StiImg,sizeof(WORD), 3*StiImgH * StiImgV,fp);
		
		fclose(fp);
		
		
		//	free(TiffHdTable);
		free(StiImg);
		free(WaveZoneImg);
		//	free(TiffFileName);

	}


	return 0;
}

void ErrWarning_TiffImgFileGen(HWND hdwnd,int Err_num)
{
	switch(Err_num)
	{
	case -1:
		MessageBox(hdwnd,"为Tiff图像分配内存出错","分配内存错误",MB_ICONWARNING|MB_OK);
		break;
	case -2:
		MessageBox(hdwnd,"为测试区图像分配内存出错","分配内存错误",MB_ICONWARNING|MB_OK);
		break;
	case -3:
		MessageBox(hdwnd,"写Tiff文件出错","文件错误",MB_ICONWARNING|MB_OK);
		break;

	default:
		break;
	}

}

void Lch2Lab(double *Lch_C, double *Lch_H, double *Lab_A, double *Lab_B)
{

	double	pi = 3.1415926;
	double	H_Rad;
	
	if((*Lch_H!=90)&&(*Lch_H!=270))
	{
		//色调角：度-->弧度
		H_Rad = *Lch_H * pi / 180.0;
		
		*Lab_A = (double)*Lch_C /(double)(sqrt(1 + tan(H_Rad )*tan(H_Rad)));
		*Lab_B = *Lab_A * tan(H_Rad);
		
		*Lab_A = fabs(*Lab_A);
		*Lab_B = fabs(*Lab_B);
	}
	else
	{
		//90度和270度的处理
		if(*Lch_H==90)
		{
			*Lab_A = 0.0;
			*Lab_B = *Lch_C;
		}
		if(*Lch_H==270) 
		{
			*Lab_A = 0.0;
			*Lab_B = (-1.0)*(*Lch_C);
		}		
	}
	
	//象限处理
	if((*Lch_H>90)&&(*Lch_H<=180))
	{
		*Lab_A *= (-1.0);
	}
	
	if((*Lch_H>180)&&(*Lch_H<270))
	{
		*Lab_A *= (-1.0);
		*Lab_B *= (-1.0);
	}
	if((*Lch_H>270)&&(*Lch_H<360))
	{
		*Lab_B *= (-1.0);
	}
	
}

void GenTimeString(unsigned char *TimeString)
{
	unsigned char tmp_str[256];

	//清空字符串变量
	strcpy(tmp_str,"");
	strcpy(TimeString,"");

	//获取计算机当地时间-->时间结构变量
	GetLocalTime(&StiGenTime);

	//将结构元素的年月日时分秒整数分别转换成字符串
	//字符串连结成时间字符串
	itoa(StiGenTime.wYear,tmp_str,10);
	strcat(TimeString, tmp_str);
	strcat(TimeString, "_");

	itoa(StiGenTime.wMonth,tmp_str,10);
	strcat(TimeString, tmp_str);
	strcat(TimeString, "_");

	itoa(StiGenTime.wDay,tmp_str,10);
	strcat(TimeString, tmp_str);
	strcat(TimeString, "_");

	itoa(StiGenTime.wHour,tmp_str,10);
	strcat(TimeString, tmp_str);
	strcat(TimeString, "_");

	itoa(StiGenTime.wMinute,tmp_str,10);
	strcat(TimeString, tmp_str);
	strcat(TimeString, "_");

	itoa(StiGenTime.wSecond,tmp_str,10);
	strcat(TimeString, tmp_str);

}
