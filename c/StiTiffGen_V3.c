 // Windows ����

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "resource.h"
#include "StiTiffGen_V3.h"

/*********************************************************************************************/
//��������

//Windows���ڴ�����
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
//�Ի��������̼�ͼ�����ü�����	
BOOL CALLBACK DlgProc1(HWND,UINT,WPARAM,LPARAM);
//�Ի���������ʾ���������Ž�����
BOOL CALLBACK DlgProc2(HWND,UINT,WPARAM,LPARAM);
//�Ի�����3���Ӿ��۲�������
BOOL CALLBACK DlgProc3(HWND,UINT,WPARAM,LPARAM);
//�Ի�����4���洢������
BOOL CALLBACK DlgProc4(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam);

//�̼�ͼ��������ú���
void StiImgParamSetting(HWND hdwnd,struct StimulusParams *StiTestParam);
//�̼�ͼ��������÷�������
int	StiImgParamAnalyse(struct StimulusParams *StiTestParam);
//��ʾ���۲�����������������
int	DispViewParamAnalyse(struct DisplayParams DispParam,struct ViewCond ViewParam);
//�̼�ͼ��������󾯸���ʾ
void ErrWarning_StiImg(HWND hdwnd,int Err_num);
//ɫ��LAB->LABת��
void StiParamLchToLab(struct StimulusParams *StiTestParam);
void Lch2Lab(double *Lch_C, double *Lch_H, double *Lab_A, double *Lab_B);
//���㣺������ֵ���������������ֵ����Сֵ���̼�ͼ�����
void StiImgCount(struct StimulusParams *StiTestParam);
//TIFF�ļ�ͷ��ͼ���ļ�Ŀ¼��������
void TiffHeaderTableSet(struct TiffHeaderTable *TiffHdTable,struct StimulusParams *StiTestParam);
//TIFF�ļ�ͷ��ͼ���ļ�Ŀ¼����д���ļ��ĺ���
int TiffHeaderTableWrite(unsigned char *TiffFileName,struct TiffHeaderTable *TiffHdTable);
//TIFFͼ���ļ��������ɺ���
int TiffWaveImgFileGen(struct TiffHeaderTable *TiffHdTable);
//Tiffͼ���ļ����ɴ��󾯸���ʾ
void ErrWarning_TiffImgFileGen(HWND hdwnd,int Err_num);
//����ʱ���ַ���
void GenTimeString(unsigned char *TimeString);

/*********************************************************************************************/
//ȫ�ֱ�������

//������ر���
char					*szWinName="WinMain";	//��������
HINSTANCE				hInst;					//ʵ�����

//���Բ����ṹ����
struct	StimulusParams	StiTestParam;			//���Դ̼�ͼ������Ľṹ����

//��ʾ�������ṹ����
struct	DisplayParams	DispParam;

//�Ӿ��۲�����ṹ����
struct	ViewCond		ViewParam;

//TIFF�ļ�ͷ�ͱ�Ǳ�ṹ��ָ�����
struct	TiffHeaderTable	TiffHdTable;

//ʱ��ṹ
SYSTEMTIME	StiGenTime;

//��������
unsigned char			tempstr[256];			//����ַ�������ʱ����
int						Err_num;				//��Ŵ������


/*****************************************************************************************/
//Windows������

int WINAPI WinMain(HINSTANCE hThisInst,HINSTANCE hPrevInst,PSTR pszCmdline,int nCmdShow)

{
   HWND     hwnd;  // ���ھ����
   MSG      uMsg;  // ��Ϣ�ṹ��
   WNDCLASS wndclass; //���崰���ࣻ 

   //���ô�����ĸ�������
   wndclass.style=0;  //ʹ��ȱʡ���ͣ�
   wndclass.lpfnWndProc=WndProc;  //���ں�������
   wndclass.cbClsExtra=0;  //������չ��
   wndclass.cbWndExtra=0;  //�޴�����չ��
   wndclass.hInstance=hThisInst;  //������ʵ�������ʹ��ǰ��������ģ���
   wndclass.hIcon=LoadIcon(NULL,IDI_APPLICATION);  //��С��ʱ��ͼ�ꣻ
   wndclass.hCursor=LoadCursor(NULL,IDC_ARROW);  //������Ͷ��壻
   wndclass.hbrBackground=GetStockObject(WHITE_BRUSH);  //���ڱ���ɫΪ����ɫ
   wndclass.lpszMenuName=MAKEINTRESOURCE(IDR_MENU1); //�˵���
   wndclass.lpszClassName=szWinName;  // ����������ƣ�ʹ��ǰ�涨��ģ�

   //ע�ᴰ����
   RegisterClass(&wndclass);

   //��������
   hwnd=CreateWindow(szWinName,"�Ӿ��̼�����ͼ��������",
	                 WS_OVERLAPPEDWINDOW|WS_HSCROLL|WS_VSCROLL,
					 CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
					 CW_USEDEFAULT,NULL,NULL,hThisInst,NULL);

   //��ʾ����

   ShowWindow(hwnd,nCmdShow);
   UpdateWindow(hwnd);

   //��Ϣѭ������

   while(GetMessage(&uMsg,NULL,0,0))
   {
	   TranslateMessage(&uMsg);
	   DispatchMessage(&uMsg);
     
   }

   return uMsg.wParam;

}

/*******************************************************************************************/
//���������ڵĴ����������������Ϣ����Windows����

LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)

{

      switch(message)
	  {
	    case WM_COMMAND:
			switch(LOWORD(wParam))
			{

				case IDM_PROJ_NEW:
					MessageBox(hwnd,"�½���Ŀ","�½�",MB_OK);
					break;

				case IDM_PROJ_OPEN:
					MessageBox(hwnd,"����Ŀ","����",MB_OK);
					break;

				case IDM_PROJ_SAVE:
					MessageBox(hwnd,"������Ŀ","����",MB_OK);
					break;

				case IDM_PROJ_INFO:
					MessageBox(hwnd,"��ʾ��Ŀ��Ϣ","����",MB_OK);
					break;

				case IDM_EXIT:
				//	MessageBox(hwnd,"�˳�����","�˳�",MB_OK);
					PostQuitMessage(0);
					break;

				case IDM_SET_TIFFGEN:
									
					//���öԻ��������̼�ͼ�����ü�����
					DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG1),hwnd,DlgProc1);
					break;

				case IDM_SET_DISPVIEW:

					//���öԻ�������������ʾ��������ÿ�����ӽ�
					DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG2),hwnd,DlgProc2);

					break;

				case IDM_FILEPATH:

					//���öԻ������������ļ��洢��
					DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG4),hwnd,DlgProc4);

					break;
				case IDM_ABOUT:
					MessageBox(hwnd,"�Ӿ��̼�ͼ��������V3.01\n\n 2009.11.08\n\n 2014.04.26 ����\n\nJinYang","����",MB_OK);
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
//�Ի�����1���̼�ͼ���������

BOOL CALLBACK DlgProc1(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	int				i;
	unsigned char	tmp_str[100];
	double			val1;

	switch(message)
	{
	case WM_INITDIALOG:
		
		//�Ի����ʼ��

		//���ô̼����������ַ���Ϊ�գ���ʾ��δ����
		strcpy(StiTestParam.TestType,"");

		//���ô̼�ͼ��LCH���ݱ༭��Ϊ��ֻ����״̬
		for(i=0;i<12;i++)
		{
			SendDlgItemMessage(hdwnd,IDC_EDIT5+i,EM_SETREADONLY,1,0);
		}

		//���ñ���ɫ���ݱ༭��Ϊ��ֻ����״̬
		SendDlgItemMessage(hdwnd,IDC_EDIT21,EM_SETREADONLY,1,0);
		SendDlgItemMessage(hdwnd,IDC_EDIT22,EM_SETREADONLY,1,0);
		SendDlgItemMessage(hdwnd,IDC_EDIT23,EM_SETREADONLY,1,0);
		
		//����ͼ��������ȱʡֵ
		strcpy(tmp_str,"1280");
		SetDlgItemText(hdwnd,IDC_EDIT1,tmp_str);
		strcpy(tmp_str,"1024");
		SetDlgItemText(hdwnd,IDC_EDIT2,tmp_str);
		strcpy(tmp_str,"256");
		SetDlgItemText(hdwnd,IDC_EDIT3,tmp_str);
		strcpy(tmp_str,"256");
		SetDlgItemText(hdwnd,IDC_EDIT4,tmp_str);

		//�������Ҳ������Ͳ��η����ȱʡֵ
		strcpy(tmp_str,"5");
		SetDlgItemText(hdwnd,IDC_EDIT17,tmp_str);
		strcpy(tmp_str,"0");
		SetDlgItemText(hdwnd,IDC_EDIT20,tmp_str);

		//����ͼ�����ȱʡֵ("5"����������ֵ�𣬰��������Ӻͼ��ٸ�5��ͼ�񣬹�10��)
		strcpy(tmp_str,"5");
		SetDlgItemText(hdwnd,IDC_EDIT18,tmp_str);

		
		return FALSE;
			
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{

		case IDC_RADIO1_TEST_L:	//�����L�仯��C��H�̶�����ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO1_TEST_L,BM_GETCHECK,0,0))
			{
				//����L�仯��C��H�̶�����ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO1_TEST_L,BM_GETCHECK,1,0);
				//��ѡ����������ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO2_TEST_C,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO3_TEST_H,BM_GETCHECK,0,0);
			}

			for(i=0;i<4;i++)
			{
				//���L���ݱ༭��ġ�ֻ����״̬
				SendDlgItemMessage(hdwnd,IDC_EDIT5+i,EM_SETREADONLY,0,0);			
			}

			for(i=0;i<8;i++)
			{
				//����C��H���ݱ༭��Ϊ��ֻ����״̬����������༭����ַ�
				SendDlgItemMessage(hdwnd,IDC_EDIT9+i,EM_SETREADONLY,1,0);
				SetDlgItemText(hdwnd,IDC_EDIT9+i,"");
			}

			//���ò�������Ϊ�����Ȳ���
			strcpy(StiTestParam.TestType,"LL");

			return 1;

		case IDC_RADIO2_TEST_C:	//�����C�仯��L��H�̶�����ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO2_TEST_C,BM_GETCHECK,0,0))
			{
				//����C�仯��L��H�̶�����ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO2_TEST_C,BM_GETCHECK,1,0);
				//��ѡ����������ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO1_TEST_L,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO3_TEST_H,BM_GETCHECK,0,0);
			}

			for(i=0;i<4;i++)
			{
				//���C���ݱ༭��ġ�ֻ����״̬
				SendDlgItemMessage(hdwnd,IDC_EDIT9+i,EM_SETREADONLY,0,0);			
			}

			for(i=0;i<4;i++)
			{
				//����L��H���ݱ༭��Ϊ��ֻ����״̬����������༭����ַ�
				SendDlgItemMessage(hdwnd,IDC_EDIT5+i,EM_SETREADONLY,1,0);
				SendDlgItemMessage(hdwnd,IDC_EDIT13+i,EM_SETREADONLY,1,0);
				SetDlgItemText(hdwnd,IDC_EDIT5+i,"");
				SetDlgItemText(hdwnd,IDC_EDIT13+i,"");
			}

			//���ò�������Ϊ���ʶȲ���
			strcpy(StiTestParam.TestType,"CC");

			return 1;

		case IDC_RADIO3_TEST_H:	//�����H�仯��L��C�̶�����ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO3_TEST_H,BM_GETCHECK,0,0))
			{
				//����H�仯��L��C�̶�����ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO3_TEST_H,BM_GETCHECK,1,0);
				//��ѡ����������ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO1_TEST_L,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO2_TEST_C,BM_GETCHECK,0,0);
			}
	
			for(i=0;i<4;i++)
			{
				//���H���ݱ༭��ġ�ֻ����״̬
				SendDlgItemMessage(hdwnd,IDC_EDIT13+i,EM_SETREADONLY,0,0);			
			}

			for(i=0;i<8;i++)
			{
				//����L��C���ݱ༭��Ϊ��ֻ����״̬����������༭����ַ�
				SendDlgItemMessage(hdwnd,IDC_EDIT5+i,EM_SETREADONLY,1,0);
				SetDlgItemText(hdwnd,IDC_EDIT5+i,"");
			}

			//���ò�������Ϊ��ɫ���ǲ���
			strcpy(StiTestParam.TestType,"HH");

			return 1;

		case IDC_RADIO4_AVR_L:	//����ɫ"ȡL0"��ѡť
			
			//�����δ���ò������ͣ��򾯸�
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"Ӧ��ѡ��������͡�","˳�����",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO4_AVR_L,BM_GETCHECK,0,0);
				
				return 0;
			}
			//����������Ͳ���"LL"���򾯸�
			if(strcmp(StiTestParam.TestType,"LL")!=0)
			{
				MessageBox(hdwnd,"�������ͣ��ʶȺ�ɫ���ǣ����ܰ��˷�ʽѡ�񱳾�ɫ��",
							"��ʽ����",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO4_AVR_L,BM_GETCHECK,0,0);
				
				return 0;
			}

			SendDlgItemMessage(hdwnd,IDC_RADIO4_AVR_L,BM_GETCHECK,1,0);

			//��������ֵ����Ϊ����ɫL
			GetDlgItemText(hdwnd,IDC_EDIT5,tmp_str,20);//��ȡ�ؼ����ı���������Ϊ20���ַ���tmp_str 
			val1 = atof(tmp_str);//��tmp_str�ַ���ת��Ϊ������ 
			sprintf(tmp_str,"%5.2f",val1);//��κ�������val1 ��������tmp_str 
			SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);

			//���ñ���ɫ��������ΪLCH
			strcpy(StiTestParam.BKG_Type,"LCH");
			
			//����ɫ����ֵ������ɫ�༭��
			GetDlgItemText(hdwnd,IDC_EDIT7,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"��δ����̼����ε�Hֵ","ȱ��ɫ����ֵ",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT23,tmp_str);

			//���ݲʶ�ֵ������ɫ�༭��
			GetDlgItemText(hdwnd,IDC_EDIT8,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"��δ����̼����ε�Cֵ","ȱ�ٲʶ�ֵ",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT22,tmp_str);

			return 1;

		case IDC_RADIO5_AVR_C:	//����ɫ"ȡC0"��ѡť
			
			//�����δ���ò������ͣ��򾯸�
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"Ӧ��ѡ��������͡�","˳�����",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO5_AVR_C,BM_GETCHECK,0,0);
				
				return 0;
			}
			
			//����������Ͳ���"CC"���򾯸�
			if(strcmp(StiTestParam.TestType,"CC")!=0)
			{
				MessageBox(hdwnd,"���Ⱥ�ɫ���ǲ������Ͳ��ܰ��˷�ʽѡ�񱳾�ɫ��",
							"��ʽ����",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO5_AVR_C,BM_GETCHECK,0,0);
				
				return 0;
			}

			SendDlgItemMessage(hdwnd,IDC_RADIO5_AVR_C,BM_GETCHECK,1,0);

			//����ʶ�C0ֵ����Ϊ����ɫ
			GetDlgItemText(hdwnd,IDC_EDIT9,tmp_str,20);
			val1 = atof(tmp_str);
			sprintf(tmp_str,"%5.2f",val1);
			SetDlgItemText(hdwnd,IDC_EDIT22,tmp_str);
			
			//����ɫ����ֵ������ɫ�༭��
			GetDlgItemText(hdwnd,IDC_EDIT11,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"��δ����̼����ε�Hֵ","ȱ��ɫ����ֵ",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT23,tmp_str);

			//��������ֵ������ɫ�༭��
			GetDlgItemText(hdwnd,IDC_EDIT12,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"��δ����̼����ε�Lֵ","ȱ������ֵ",MB_OK|MB_ICONWARNING);
			}

			SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);

			//���ñ���ɫ��������ΪLCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			return 1;

		case IDC_RADIO6_AVR_H:	//����ɫ"ȡH0"��ѡť
			
			//�����δ���ò������ͣ��򾯸�
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"Ӧ��ѡ��������͡�","˳�����",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO6_AVR_H,BM_GETCHECK,0,0);
				
				return 0;
			}
			//����������Ͳ���"HH"���򾯸�
			if(strcmp(StiTestParam.TestType,"HH")!=0)
			{
				MessageBox(hdwnd,"���ȺͲʶȲ������Ͳ��ܰ��˷�ʽѡ�񱳾�ɫ��",
							"��ʽ����",MB_OK|MB_ICONWARNING);
				
				SendDlgItemMessage(hdwnd,IDC_RADIO6_AVR_H,BM_GETCHECK,0,0);
				
				return 0;
			}

			SendDlgItemMessage(hdwnd,IDC_RADIO6_AVR_H,BM_GETCHECK,1,0);

			//����ɫ����ƽ��ֵ����Ϊ����ɫ
			GetDlgItemText(hdwnd,IDC_EDIT13,tmp_str,20);
			val1 = atof(tmp_str);
			sprintf(tmp_str,"%5.2f",val1);
			SetDlgItemText(hdwnd,IDC_EDIT23,tmp_str);
			
			//��������ֵ������ɫ�༭��
			GetDlgItemText(hdwnd,IDC_EDIT15,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"��δ����̼����ε�Lֵ","ȱ������ֵ",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);

			//���ݲʶ�ֵ������ɫ�༭��
			GetDlgItemText(hdwnd,IDC_EDIT16,tmp_str,20);
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"��δ����̼����ε�Cֵ","ȱ�ٲʶ�ֵ",MB_OK|MB_ICONWARNING);
			}
			SetDlgItemText(hdwnd,IDC_EDIT22,tmp_str);

			//���ñ���ɫ��������ΪLCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			return 1;

		case IDC_RADIO7_NEUTRAL:	//����ɫ"���Ի�(ȡL0)"��ѡť	

			//�����δ���ò������ͣ��򾯸�
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"Ӧ��ѡ��������͡�","˳�����",MB_OK|MB_ICONWARNING);

				//ȡ��ѡ��˰�ť
				SendDlgItemMessage(hdwnd,IDC_RADIO7_NEUTRAL,BM_GETCHECK,0,0);

				return 0;
			}

			//ȡ����ֵL0�������Իұ���ɫ
			if(strcmp(StiTestParam.TestType,"LL")==0)
			{
				//ȡ����ֵL0����Ϊ����ɫ
				GetDlgItemText(hdwnd,IDC_EDIT5,tmp_str,20);
				val1 = atof(tmp_str);
				sprintf(tmp_str,"%5.2f",val1);
				SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);
				//����ɫ�Ĳ�ɫ������Ϊ"0.0"
				SetDlgItemText(hdwnd,IDC_EDIT22,"0.0");
				SetDlgItemText(hdwnd,IDC_EDIT23,"0.0");
			}

			//CCģʽ����������ֵ������ɫ����ֵ�༭��
			if(strcmp(StiTestParam.TestType,"CC")==0)
			{
				//��ȡ����ֵ����Ϊ����ɫ
				GetDlgItemText(hdwnd,IDC_EDIT12,tmp_str,20);
				//�����ַ���������ɫ����ֵ�༭��
				SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);
				//����ɫ�Ĳ�ɫ������Ϊ"0.0"
				SetDlgItemText(hdwnd,IDC_EDIT22,"0.0");
				SetDlgItemText(hdwnd,IDC_EDIT23,"0.0");
			}

			//HHģʽ����������ֵ������ɫ����ֵ�༭��
			if(strcmp(StiTestParam.TestType,"HH")==0)
			{
				//��ȡ����ƽ��ֵ����Ϊ����ɫ
				GetDlgItemText(hdwnd,IDC_EDIT15,tmp_str,20);
				//�����ַ���������ɫ����ֵ�༭��
				SetDlgItemText(hdwnd,IDC_EDIT21,tmp_str);
				//����ɫ�Ĳ�ɫ������Ϊ"0.0"
				SetDlgItemText(hdwnd,IDC_EDIT22,"0.0");
				SetDlgItemText(hdwnd,IDC_EDIT23,"0.0");
			}

			//���ñ���ɫ��������ΪLCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			//ѡ��˰�ť
			SendDlgItemMessage(hdwnd,IDC_RADIO7_NEUTRAL,BM_GETCHECK,1,0);

			return 1;

		case IDC_RADIO8_NEUTRAL2:	//����ɫ"���Ի�(�Զ�L)"��ѡť

			//��ʾ���뱳��ɫ����ֵ
			MessageBox(hdwnd,"�����뱳��ɫ����ֵ","Ҫ����������",MB_OK);
				
			//�������ɫL�༭��ġ�ֻ����״̬
			SendDlgItemMessage(hdwnd,IDC_EDIT21,EM_SETREADONLY,0,0);			
			//����ɫCH�༭��ֻ����
			SendDlgItemMessage(hdwnd,IDC_EDIT22,EM_SETREADONLY,1,0);			
			SendDlgItemMessage(hdwnd,IDC_EDIT23,EM_SETREADONLY,1,0);			
			//����ɫCH�༭��Ϊ0
			SetDlgItemText(hdwnd,IDC_EDIT22,"0.0");
			SetDlgItemText(hdwnd,IDC_EDIT23,"0.0");

			//���ñ���ɫ��������ΪLCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			//ѡ��˰�ť
			SendDlgItemMessage(hdwnd,IDC_RADIO8_NEUTRAL2,BM_GETCHECK,1,0);

			return 1;

		case IDC_RADIO9_LCH_SELF:	//����ɫ"�Զ�LCH"��ѡť

			//ѡ��˰�ť
			SendDlgItemMessage(hdwnd,IDC_RADIO9_LCH_SELF,BM_GETCHECK,1,0);

			//���ñ���ɫ���ݱ༭��Ϊ������״̬
			SendDlgItemMessage(hdwnd,IDC_EDIT21,EM_SETREADONLY,0,0);
			SendDlgItemMessage(hdwnd,IDC_EDIT22,EM_SETREADONLY,0,0);
			SendDlgItemMessage(hdwnd,IDC_EDIT23,EM_SETREADONLY,0,0);

			//��ʾ���뱳��ɫ����ֵ
			MessageBox(hdwnd,"�����뱳��ɫ����L���ʶ�C��ɫ����Hֵ","Ҫ�����뱳��ɫ����",MB_OK);

			//���ñ���ɫ��������ΪLCH
			strcpy(StiTestParam.BKG_Type,"LCH");

			return 1;

		case IDC_RADIO10_LAB_SELF:		//����ɫ"�Զ�LAB"��ѡť

			//ѡ��˰�ť
			SendDlgItemMessage(hdwnd,IDC_RADIO10_LAB_SELF,BM_GETCHECK,1,0);

			//���ñ���ɫ���ݱ༭��Ϊ������״̬
			SendDlgItemMessage(hdwnd,IDC_EDIT21,EM_SETREADONLY,0,0);
			SendDlgItemMessage(hdwnd,IDC_EDIT22,EM_SETREADONLY,0,0);
			SendDlgItemMessage(hdwnd,IDC_EDIT23,EM_SETREADONLY,0,0);			
			
			//��ʾ���뱳��ɫ����ֵ
			MessageBox(hdwnd,"�����뱳��ɫLABֵ","Ҫ�����뱳��ɫ����",MB_OK);

			//���ñ���ɫ��������ΪLAB
			strcpy(StiTestParam.BKG_Type,"LAB");

			return 1;

		case IDC_DISPLAY_SET:

			//���öԻ�������������ʾ��������ÿ�����ӽ�
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG2),hdwnd,DlgProc2);

			return 1;

		case IDC_VIEWDISTANCE:

			//���öԻ������������Ӿ�
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG3),hdwnd,DlgProc3);

			return 1;

		case IDC_TIFFSAVEPATH:

			//���öԻ������������ļ��洢��
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG4),hdwnd,DlgProc4);

			return 1;

		case IDC_TIFFGEN:

			//�����δ���ò������ͣ��򾯸�
			if(strcmp(StiTestParam.TestType,"")==0)
			{
				MessageBox(hdwnd,"Ӧ��ѡ��������͡�","��������δ����",MB_OK|MB_ICONWARNING);

				return 0;
			}

			//�����δ���ñ���ɫ���ͣ��򾯸�
			if(strcmp(StiTestParam.BKG_Type,"")==0)
			{
				MessageBox(hdwnd,"����ɫ����δѡ��","����ɫ���ó���",MB_OK|MB_ICONWARNING);

				return 0;
			}

			//���ô̼�ͼ�������ʹ�����ṹ����
			StiImgParamSetting(hdwnd,&StiTestParam);
			
			//��������Ĳ������ó��������
			Err_num = StiImgParamAnalyse(&StiTestParam);
			//���պ��������
			ErrWarning_StiImg(hdwnd,Err_num);

			//�޴������д̼�ͼ��������ҵ
			if(Err_num==0)
			{
				//���㣺LCH-->LAB
				StiParamLchToLab(&StiTestParam);
				//����ͼ�����������С/���ֵ�Ͳ���
				StiImgCount(&StiTestParam);
/*
				//���labֵ
				sprintf(tempstr,"L= %f, a=%f, b=%f",
					StiTestParam.End_L,StiTestParam.End_A,StiTestParam.End_B);

				MessageBox(hdwnd,tempstr,"Labֵ",MB_OK|MB_ICONWARNING);
*/
				//TIFF�̼�ͼ��������ҵ
				Err_num = TiffWaveImgFileGen(&TiffHdTable);	
				//������
				ErrWarning_TiffImgFileGen(hdwnd,Err_num);

				//������ϣ��˳��Ի���
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
//�Ի�����2����ʾ���������۲���������

BOOL CALLBACK DlgProc2(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	unsigned char	tmp_str[100];

	switch(message)
	{
	case WM_INITDIALOG:
		
		//�Ի����ʼ��
		
		//����ͼ��������ȱʡֵ
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

		case IDC_RADIO1_ANGLE05:	//�����ÿ�����ӽ�0.5�֡���ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,0,0))
			{
				//����ÿ�����ӽ�0.5�֡���ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,1,0);
				//��ѡ��ÿ�����ӽ�1.0�֡���ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,0,0);
			}

			//���ù۲���������������Ϊ����ֵ�������ж��û��Ƿ����ù�
			ViewParam.AnglePerPixel = 0.5;

			return 1;

		case IDC_RADIO2_ANGLE10:	//�����ÿ�����ӽ�1.0�֡���ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,0,0))
			{
				//����ÿ�����ӽ�1.0�֡���ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,1,0);
				//��ѡ��ÿ�����ӽ�0.5�֡���ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,0,0);
			}

			//���ù۲�����������ÿ�����ӽ�1.0��
			ViewParam.AnglePerPixel =1.0;

			return 1;

		case IDOK:

			//�ӱ༭���ȡ��ʾ������������ʾ���ߴ�,������
			GetDlgItemText(hdwnd,IDC_EDIT21,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"δ������ʾ������������","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispParam.DispPixelH  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT22,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0)
			{
				MessageBox(hdwnd,"δ������ʾ������������","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispParam.DispPixelV  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT23,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"δ������ʾ������ߴ�","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispParam.DispSizeH  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT24,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"δ������ʾ������ߴ�","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispParam.DispSizeV  = abs(atoi(tmp_str));}

			if(ViewParam.AnglePerPixel == -10000) 
			{
				MessageBox(hdwnd,"δѡ��ÿ�����ӽ�","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else 
			{
				//�����Ի���
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
//�Ի�����3���Ӿ��۲�������

BOOL CALLBACK DlgProc3(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	unsigned char	tmp_str[100];

	static	DWORD	DispPixH,DispPixV;
	static	double	DispH,DispV, PixelSize, AnglePerPixel,AnglePerPixelRad, ViewDistance;

	switch(message)
	{
	case WM_INITDIALOG:
		
		//�Ի����ʼ��
		
		//�ɴ̼�ͼ���������ͼ��������ȱʡֵ
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

		case IDC_RADIO1_ANGLE05:	//�����ÿ�����ӽ�0.5�֡���ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,0,0))
			{
				//����ÿ�����ӽ�0.5�֡���ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,1,0);
				//��ѡ��ÿ�����ӽ�1.0�֡���ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,0,0);
			}

			//���ù۲�����������ÿ�����ӽ�0.5��
			AnglePerPixel = 0.5;

			return 1;

		case IDC_RADIO2_ANGLE10:	//�����ÿ�����ӽ�1.0�֡���ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,0,0))
			{
				//����ÿ�����ӽ�1.0�֡���ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO2_ANGLE10,BM_GETCHECK,1,0);
				//��ѡ��ÿ�����ӽ�0.5�֡���ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO1_ANGLE05,BM_GETCHECK,0,0);
			}

			//���ù۲�����������ÿ�����ӽ�1.0��
			AnglePerPixel = 1.0;

			return 1;

		case IDOK:

			//�ӱ༭���ȡ��ʾ������������ʾ���ߴ�,������
			GetDlgItemText(hdwnd,IDC_EDIT21,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"δ������ʾ������������","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispPixH  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT22,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"δ������ʾ������������","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispPixV  = abs(atoi(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT23,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"δ������ʾ������ߴ�","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispH  = fabs(atof(tmp_str));}

			GetDlgItemText(hdwnd,IDC_EDIT24,tmp_str,20);
			
			if(strcmp(tmp_str,"")==0) 
			{
				MessageBox(hdwnd,"δ������ʾ������ߴ�","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else {DispV  = fabs(atof(tmp_str));}

			if(AnglePerPixel == -10000) 
			{
				MessageBox(hdwnd,"δѡ��ÿ�����ӽ�","����ȱʧ",
							MB_OK|MB_ICONWARNING);
				break;
			}	
			else 
			{
				//�����Ӿ�
				PixelSize = DispH / (double)DispPixH;//ÿ�����سߴ� =��ʾ������ߴ� / �������ظ��� 
				AnglePerPixelRad = 3.1415926 * (AnglePerPixel/ (2.0 * 60.0)) / 180.0;//ÿ�����ӽ�תΪ���� ��1��=60�� 

				ViewDistance = PixelSize / (2 * tan(AnglePerPixelRad));

				//��ʾ�Ӿൽ�༭��
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
//�Ի�����4���洢������

BOOL CALLBACK DlgProc4(HWND hdwnd,UINT message,WPARAM wParam,LPARAM lParam)
{

	switch(message)
	{
			
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{

		case IDC_RADIO_DISK1:	//�����C:\����ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0))
			{
				//����C:\����ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,1,0);
				//��ѡ������ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//����ΪC��
			strcpy(StiTestParam.StiFilePath,"c:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);

			return 1;

		case IDC_RADIO_DISK2:	//�����D:\����ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0))
			{
				//����D:\����ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,1,0);
				//��ѡ������ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//����ΪD��
			strcpy(StiTestParam.StiFilePath,"d:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);
			return 1;

		case IDC_RADIO_DISK3:	//�����E:\����ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0))
			{
				//����E:\����ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,1,0);
				//��ѡ������ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//����ΪE��
			strcpy(StiTestParam.StiFilePath,"e:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);

			return 1;

		case IDC_RADIO_DISK4:	//�����F:\����ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0))
			{
				//����F:\����ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,1,0);
				//��ѡ������ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//����ΪF��
			strcpy(StiTestParam.StiFilePath,"f:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);
			
			return 1;

		case IDC_RADIO_DISK5:	//�����G:\����ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0))
			{
				//����G:\����ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,1,0);
				//��ѡ������ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0);

			}

			//����ΪG��
			strcpy(StiTestParam.StiFilePath,"g:\\StiTest\\");
			CreateDirectory(StiTestParam.StiFilePath,NULL);

			return 1;

		case IDC_RADIO_DISK6:	//�����H:\����ѡ��ť
			if(!SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,0,0))
			{
				//����H:\����ѡ��ťѡ��
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK6,BM_GETCHECK,1,0);
				//��ѡ������ѡ��ť
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK1,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK2,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK3,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK4,BM_GETCHECK,0,0);
				SendDlgItemMessage(hdwnd,IDC_RADIO_DISK5,BM_GETCHECK,0,0);

			}

			//����ΪH��
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
//�̼�ͼ��������ú���

void StiImgParamSetting(HWND hdwnd,struct StimulusParams *StiTestParam)
{
	unsigned char tempstr[256];

	//��ȡ�����仯����
	GetDlgItemText(hdwnd,IDC_EDIT18,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->PM_Steps = -80000;}
	else {StiTestParam->PM_Steps = abs(atol(tempstr));}

	//�����������Ϊ0����Ϊ1
	if(StiTestParam->PM_Steps == 0) {StiTestParam->PM_Steps =1;}
	
	if(strcmp(StiTestParam->TestType,"LL")==0)	//��������Ƶ��
	{	
		//��ȡ���༭���е��ִ���ת����С��
		//δ���룬��������Ϊ-10000;

		//L0
		GetDlgItemText(hdwnd,IDC_EDIT5,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_L = -10000;}
		else {StiTestParam->Central_L  = fabs(atof(tempstr));}
		//dL
		GetDlgItemText(hdwnd,IDC_EDIT6,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Delta_L = -10000;}
		else {StiTestParam->Delta_L = fabs(atof(tempstr));}
		//H��
		GetDlgItemText(hdwnd,IDC_EDIT7,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_H = -10000;}
		else 
		{
			StiTestParam->Central_H = fabs(atof(tempstr));
		}
		//C��
		GetDlgItemText(hdwnd,IDC_EDIT8,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_C = -10000;}
		else 
		{
			StiTestParam->Central_C = fabs(atof(tempstr));	
		}
		
	}
	
	if(strcmp(StiTestParam->TestType,"CC")==0)	//���Բʶ�Ƶ��
	{	
		//��ȡ���༭���е��ִ���ת����С��
		//δ���룬��������Ϊ-20000;

		//C0
		GetDlgItemText(hdwnd,IDC_EDIT9,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_C = -20000;}
		else {StiTestParam->Central_C = fabs(atof(tempstr));}
		//dC
		GetDlgItemText(hdwnd,IDC_EDIT10,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Delta_C = -20000;}
		else {StiTestParam->Delta_C = fabs(atof(tempstr));}
		//H��
		GetDlgItemText(hdwnd,IDC_EDIT11,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_H  = -20000;}
		else 
		{
			StiTestParam->Central_H  = fabs(atof(tempstr));
		}
		//L��
		GetDlgItemText(hdwnd,IDC_EDIT12,tempstr,20);
		if(strcmp(tempstr,"")==0) {StiTestParam->Central_L  = -20000;}
		else
		{
			StiTestParam->Central_L = fabs(atof(tempstr));
		}

	}
	
	if(strcmp(StiTestParam->TestType,"HH")==0)	//����ɫ����Ƶ��
	{	
		//��ȡ���༭���е��ִ���ת����С��
		//δ���룬��������Ϊ-30000;

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
	
	//����ɫ����
	//����ΪLCH����
	if(strcmp(StiTestParam->BKG_Type,"LCH")==0)
	{
		//δ��������(�ַ���Ϊ��)����������Ϊ-40000
		//����LCH����
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
	
	//����ΪLAB����
	if(strcmp(StiTestParam->BKG_Type,"LAB")==0)
	{
		//δ��������(�ַ���Ϊ��)����������Ϊ-40000
		//����Lab���ݣ�a,b��ȡ����ֵ
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
		
	//ͼ���в���
	//��ȡ���༭���е��ִ���ת����С��
	//δ���룬��������Ϊ-50000;

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

	//��ȡ���༭���е��ִ���ת����С��
	//δ���룬��������Ϊ-60000;

	//F(�㶨Ƶ�ʣ����/��Сֵ���)
	GetDlgItemText(hdwnd,IDC_EDIT17,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->StiFreqMin = -60000;}
	else 
	{
		StiTestParam->StiFreqMin = fabs(atof(tempstr));//���Ҳ�����=���������������ֵ���������ͣ������� 
		StiTestParam->StiFreqMax  = StiTestParam->StiFreqMin;
	}

	//Angle
	GetDlgItemText(hdwnd,IDC_EDIT20,tempstr,20);
	if(strcmp(tempstr,"")==0) {StiTestParam->StiAngle = -60000;}
	else {StiTestParam->StiAngle = fabs(atof(tempstr));}

	//ͼ��洢·��
	//strcpy(StiTestParam->StiFilePath,"e:\\StiTest\\");
	//CreateDirectory(StiTestParam->StiFilePath,NULL);

	//���ɴ�ʱ������ļ����ַ���
	GenTimeString(tempstr);
	strcat(StiTestParam->StiFilePath,tempstr);
	strcat(StiTestParam->StiFilePath,"\\");

}

/*******************************************************************************************/
//�̼�ͼ�������������

int	StiImgParamAnalyse(struct StimulusParams *StiTestParam)
{
	unsigned long m,n;

	//�����仯����δ���룬����-80
	if(StiTestParam->PM_Steps ==-80000) return -80;

	//��������Ƶ��
	if(strcmp(StiTestParam->TestType,"LL")==0)
	{
		m = (unsigned long)(StiTestParam->Central_L / (StiTestParam->Delta_L/2.0));
		n = (unsigned long)((100.0 - StiTestParam->Central_L)/(StiTestParam->Delta_L/2.0));


		//������δ���룬����-10
		if((StiTestParam->Central_L ==-10000)||(StiTestParam->Delta_L==-10000)||
			(StiTestParam->Central_H==-10000)||(StiTestParam->Central_C==-10000)) 
			return -10;

		//��������ֵС��0�����100������-11
		if((StiTestParam->Central_L >100)||(StiTestParam->Central_L <0)) return -11;
		//L����=0������-12
		if((StiTestParam->Delta_L) == 0) return -12;
		
		if(m <StiTestParam->PM_Steps) return -13;
		if(n <StiTestParam->PM_Steps) return -14;		
		
		//ɫ���Ǵ���360�Ȼ�С��0�ȣ�����-15
		if((StiTestParam->Central_H >360)||(StiTestParam->Central_H <0)) return -15;
		//�ʶ�ֵС��0������-16
		if(StiTestParam->Central_C <0) return -16;
		//��������+�౶�������100������-17
		if((StiTestParam->Central_L - (StiTestParam->PM_Steps * StiTestParam->Delta_L/2.0)) < 0) 
			return -17;
		//��������+�౶�������100������-18
		if((StiTestParam->Central_L + (StiTestParam->PM_Steps * StiTestParam->Delta_L/2.0))>100.0)
		return -18;
		
		
	//	if((StiTestParam->Central_L + (StiTestParam->PM_Steps * StiTestParam->Delta_L/2.0)) >100 0) 
	//		return -18;
		
	}
	
	//���Բʶ�Ƶ��
	if(strcmp(StiTestParam->TestType,"CC")==0)
	{
		m = (unsigned long)(StiTestParam->Central_C / (StiTestParam->Delta_C/2.0));
		n = (unsigned long)((180.3136 - StiTestParam->Central_C)/(StiTestParam->Delta_C/2.0));

		//������δ���룬����-20
		if((StiTestParam->Central_C==-20000)||(StiTestParam->Delta_C==-20000)||
			(StiTestParam->Central_L==-20000)||(StiTestParam->Central_H==-20000)) 
			return -20;

		//���Ĳʶ�ֵС��0�����180.3136������-21
		if((StiTestParam->Central_C >180.3136)||(StiTestParam->Central_C <0)) return -21;
		//C����=0������-22
		if((StiTestParam->Delta_C) == 0) return -22;

		if(m <StiTestParam->PM_Steps) return -23;
		if(n <StiTestParam->PM_Steps) return -24;		
		
		//��������ֵС��0�����100������-11
		if((StiTestParam->Central_L >100)||(StiTestParam->Central_L <0)) return -25;
		//ɫ���Ǵ���360�Ȼ�С��0�ȣ�����-26
		if((StiTestParam->Central_H >360)||(StiTestParam->Central_H <0)) return -26;
		//���Ĳʶ�-�౶����С��0������-27
		if((StiTestParam->Central_C - (StiTestParam->PM_Steps * StiTestParam->Delta_C/2.0)) < 0) return -27;
		//���Ĳʶ�+�౶�������100������-28
		if((StiTestParam->Central_C + (StiTestParam->PM_Steps * StiTestParam->Delta_C/2.0)) >180.3136) return -28;

	}

	//����ɫ����Ƶ��
	if(strcmp(StiTestParam->TestType,"HH")==0)
	{
		m = (unsigned long)(StiTestParam->Central_H / (StiTestParam->Delta_H/2.0));
		n = (unsigned long)((360 - StiTestParam->Central_H)/(StiTestParam->Delta_H/2.0));

		//������δ���룬����-30
		if((StiTestParam->Central_H==-30000)||(StiTestParam->Delta_H==-30000)||
			(StiTestParam->Central_L==-30000)||(StiTestParam->Central_C==-30000)) 
			return -30;

		//����ɫ����С��0�����360������-31
		if((StiTestParam->Central_H >360)||(StiTestParam->Central_H <0)) return -31;
		//H����=0������-32
		if((StiTestParam->Delta_H) == 0) return -32;

		if(m <StiTestParam->PM_Steps) return -23;
		if(n <StiTestParam->PM_Steps) return -24;		
		
		//��������ֵС��0�����100������-35
		if((StiTestParam->Central_L >100)||(StiTestParam->Central_L <0)) return -35;
		//���Ĳʶȴ���180.3136��С��0������-36
		if((StiTestParam->Central_C >180.3136)||(StiTestParam->Central_C <0)) return -36;
		//����ɫ����-�౶����С��0������-37
		if((StiTestParam->Central_H - (StiTestParam->PM_Steps * StiTestParam->Delta_H/2.0)) < 0) return -37;
		//����ɫ����+�౶�������100������-38
		if((StiTestParam->Central_H + (StiTestParam->PM_Steps * StiTestParam->Delta_H/2.0)) >360) return -38;

	}

	//����ɫ
	//�б���ɫ����δ���룬����-41
	if((StiTestParam->BKG_L==-40000)||(StiTestParam->BKG_C==-40000)||
	   (StiTestParam->BKG_H==-40000)||(StiTestParam->BKG_A==-40000)||
	   (StiTestParam->BKG_B==-40000)) return -41;

	//���ȳ���Χ������-42
	if((StiTestParam->BKG_L>100)||(StiTestParam->BKG_L<0)) return -42;
	//�ʶȳ���Χ������-43
	if((StiTestParam->BKG_C>180.3136156)||(StiTestParam->BKG_C<0)) return -43;
	//ɫ���ǳ���Χ������-44
	if((StiTestParam->BKG_H>360)||(StiTestParam->BKG_H<0)) return -44;
	//a*����Χ������-45
	if((StiTestParam->BKG_A>127)||(StiTestParam->BKG_A<-128)) return -45;
	//b*����Χ������-46
	if((StiTestParam->BKG_B>127)||(StiTestParam->BKG_B<-128)) return -46;

	//��ͼ�����ز���δ����
	if((StiTestParam->TestZonePixelH ==-50000)||(StiTestParam->TestZonePixelV==-50000)||
	   (StiTestParam->ImgPixelH==-50000)||(StiTestParam->ImgPixelV==-50000)) 
	   return -50;

	//ͼ�����Ĳ���������������>ͼ�����������������-51
	if(StiTestParam->TestZonePixelH > StiTestParam->ImgPixelH) return -51;
	//ͼ�����Ĳ���������������>ͼ������������������-52
	if(StiTestParam->TestZonePixelV > StiTestParam->ImgPixelV) return -52;

	//Ƶ������������δ���룬����-60
	if((StiTestParam->StiFreqMin==-60000)||(StiTestParam->StiAngle==-60000)) 
		return -60;

	//����ͼ���ļ��У�������-71
	if(CreateDirectory(StiTestParam->StiFilePath,NULL)==0)	return -71;
	
	//ȫ����ȷ����0
	return 0;
}

/*******************************************************************************************/
//��ʾ���۲�����������������

int	DispViewParamAnalyse(struct DisplayParams DispParam,struct ViewCond ViewParam)
{

	return 0;
}

//���󾯸���ʾ����
void ErrWarning_StiImg(HWND hdwnd,int Err_num)
{
	switch(Err_num)
	{
	case -10:
		MessageBox(hdwnd,"�������Ҳ����ԣ��в���δ����","��������",MB_ICONWARNING|MB_OK);
		break;
	case -11:
		MessageBox(hdwnd,"��������ֵ����[0~100]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;
	case -12:
		MessageBox(hdwnd,"���Ȳ���Ϊ0","��������",MB_ICONWARNING|MB_OK);
		break;
	case -13:
		MessageBox(hdwnd,"��������ֵ�ݼ��౶�����<0����С�����򲽾�","��������",MB_ICONWARNING|MB_OK);
		break;
	case -14:
		MessageBox(hdwnd,"��������ֵ�����౶�����>100����С�����򲽾�","��������",MB_ICONWARNING|MB_OK);
		break;
	case -15:
		MessageBox(hdwnd,"����ɫ���ǳ���[0~360]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;
	case -16:
		MessageBox(hdwnd,"���βʶȳ���Χ(С��0��)","��������",MB_ICONWARNING|MB_OK);
		break;

	case -20:
		MessageBox(hdwnd,"�ʶ����Ҳ����ԣ��в���δ����","��������",MB_ICONWARNING|MB_OK);
	case -21:
		MessageBox(hdwnd,"���Ĳʶ�ֵС��0�����180.31","��������",MB_ICONWARNING|MB_OK);
		break;
	case -22:
		MessageBox(hdwnd,"�ʶ�ֵ����Ϊ0","��������",MB_ICONWARNING|MB_OK);
		break;
	case -23:
		MessageBox(hdwnd,"���Ĳʶ�ֵ�ݼ��౶�����<0����С�����򲽾�","��������",MB_ICONWARNING|MB_OK);
		break;
	case -24:
		MessageBox(hdwnd,"���Ĳʶ�ֵ�ݼ��౶�����>180����С�����򲽾�","��������",MB_ICONWARNING|MB_OK);
		break;
	case -25:
		MessageBox(hdwnd,"��������ֵ����[0~100]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;
	case -26:
		MessageBox(hdwnd,"����ɫ���ǳ���[0~360]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;

	case -30:
		MessageBox(hdwnd,"ɫ�������Ҳ����ԣ��в���δ����","��������",MB_ICONWARNING|MB_OK);
	case -31:
		MessageBox(hdwnd,"����ɫ���ǳ���[0~360]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;
	case -32:
		MessageBox(hdwnd,"ɫ���ǲ���Ϊ0","��������",MB_ICONWARNING|MB_OK);
		break;
	case -33:
		MessageBox(hdwnd,"����ɫ���ǵݼ��౶�����<0����С�����򲽾�","��������",MB_ICONWARNING|MB_OK);
		break;
	case -34:
		MessageBox(hdwnd,"���Ĳʶ�ֵ�����౶�����>360����С�����򲽾�","��������",MB_ICONWARNING|MB_OK);
		break;
	case -35:
		MessageBox(hdwnd,"��������ֵ����[0~100]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;
	case -36:
		MessageBox(hdwnd,"���βʶ�ֵ����Χ(С��0��)","��������",MB_ICONWARNING|MB_OK);
		break;

	case -41:
		MessageBox(hdwnd,"�б���ɫ����δ����","��������",MB_ICONWARNING|MB_OK);
		break;
	case -42:
		MessageBox(hdwnd,"����ɫ����ֵ����[0~100]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;
	case -43:
		MessageBox(hdwnd,"����ɫ�ʶ�ֵ��[0~180.3136516]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;
	case -44:
		MessageBox(hdwnd,"����ɫɫ���ǳ���[0~360]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;
	case -45:
		MessageBox(hdwnd,"����ɫa*����[-128~+127]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;
	case -46:
		MessageBox(hdwnd,"����ɫb*����[-128~+127]��Χ","��������",MB_ICONWARNING|MB_OK);
		break;

	case -50:
		MessageBox(hdwnd,"ͼ��������������������δ����","��������",MB_ICONWARNING|MB_OK);
		break;
	case -51:
		MessageBox(hdwnd,"��������������������ͼ�����������","��������",MB_ICONWARNING|MB_OK);
		break;

	case -60:
		MessageBox(hdwnd,"���Ҳ�Ƶ�ʡ�����ǣ�������δ���롣","��������",MB_ICONWARNING|MB_OK);
		break;

	case -71:
		MessageBox(hdwnd,"��Ҫ���ͼ���ļ��в�������","��������",MB_ICONWARNING|MB_OK);
		break;

	case -80:
		MessageBox(hdwnd,"δ���������仯�Ĵ���","��������",MB_ICONWARNING|MB_OK);
		break;


	default:
		break;
	}

}

/*******************************************************************************************/
//ɫ��LCH->LABת������
void StiParamLchToLab(struct StimulusParams *StiTestParam)
{

	//����С���LCHת����Lab
	Lch2Lab(&StiTestParam->End_C, &StiTestParam->End_H, 
		&StiTestParam->End_A, &StiTestParam->End_B);
	
	Lch2Lab(&StiTestParam->Start_C, &StiTestParam->Start_H, 
		&StiTestParam->Start_A, &StiTestParam->Start_B);


	//����ɫת��LCHת����Lab
	Lch2Lab(&StiTestParam->BKG_C, &StiTestParam->BKG_H, 
		&StiTestParam->BKG_A, &StiTestParam->BKG_B);

}

/*******************************************************************************************/
//���㣺������ֵ���������������ֵ����Сֵ���̼�ͼ�����
void StiImgCount(struct StimulusParams *StiTestParam)
{
	//���������Сֵ������ͬ���Է�ʽ
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

	//ͼ�����= ��������
	StiTestParam->StiImgCount = StiTestParam->PM_Steps ;

}

/*******************************************************************************************/

//TIFF�ļ�ͷ��ͼ���ļ�Ŀ¼�������ú���

void TiffHeaderTableSet(struct TiffHeaderTable *TiffHdTable,struct StimulusParams *StiTestParam)
{
	
	//Tiff�ļ�ͷ�ṹ��ֵ
	TiffHdTable->i1 = 'I';
	TiffHdTable->i2 = 'I';
	TiffHdTable->vers = 42;
	TiffHdTable->ifd_offs = 8;
	TiffHdTable->tag_count = 13;
	
	//Tiff�ļ�Ŀ¼��Ϣ�ṹ��ֵ
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
//TIFF�ļ�ͷ��ͼ���ļ�Ŀ¼����д���ļ��ĺ���
int TiffHeaderTableWrite(unsigned char *TiffFileName,struct TiffHeaderTable *TiffHdTable)
{
	
	//�����ļ�ָ��
	FILE *fp;

	//���ļ����ڡ�������д��
	fp = fopen(TiffFileName,"wb");

	//������-1
	if(fp==NULL) {return -1;}
	
	//���ļ�ͷ�ͱ�Ǳ�д��
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
	
	//�ر��ļ�
	fclose(fp);

	return 0;

}
/*******************************************************************************************/
//TIFFͼ���ļ��������ɺ���
int TiffWaveImgFileGen(struct TiffHeaderTable *TiffHdTable)
{
	
	//ѭ������
	DWORD			i,j,k;

	//�ļ�ָ��
	FILE			*fp;
	
	//Tiff�����ļ�����Tiff�����ļ����ַ�������
	unsigned char	TiffBaseName[200],TiffFileName[300];
	
	//��ʱ�ַ�������
	unsigned char	tmpstr[256];
	
	//TIFFͼ��ռ�Ƶ��
	double	ImgFreq;

	//��ǰTIFFͼ��LCH��A��B�ĳ�ֵ����ֵֹ
	double L0, A0, B0, C0, H0;
	double L1, A1, B1, C1, H1;


	//�������
	double	x,u,v;

	//����ֵ����
	double	y;

	//ɫ��ֵ�м����
	double	Ls,As,Bs;

	//����ǻ���ֵ�м����
	double	StiAngle_R;

	//Բ����ֵ
	double	pi=3.1415926;

	//ͼ���ڴ�ָ�����
	WORD	*StiImg,*WaveZoneImg;	//StiImg���������̼�ͼ��
									//WaveZoneImg����������ͼ��
									//16bitͼ����˫�ֽڵ�WORD����(�޷�������)
									
	//ȫͼ�񼰲���������ͼ��ĺ���������
	DWORD	StiImgH,StiImgV,WaveImgH,WaveImgV;

	//����������ͼ�����Ͻ��������
	DWORD	LeftTopHor,LeftTopVer;

	//���������ڴ��е�ƫ����
	DWORD	WaveZoneOffs;

	//*************** Tiffͼ������ ********************


	//ѭ�����ɶ��TIFFͼ�񣺿ռ�Ƶ�ʲ�ͬ
	for(k=1;k<=StiTestParam.PM_Steps;k++)
	{
		//���㵱ǰTIFF�ļ���LCH��AB��ʼֵ/��ֵֹ
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

		//Tiff�ļ������
		strcpy(TiffFileName,"");

		//�����ļ������
		strcpy(TiffBaseName,"");

		//���ղ������ͣ��ļ���������"CSF_����˫��ĸLL_FƵ��ֵ_L��С_���_Cֵ_Hֵ.tif"
		//							"CSF_����˫��ĸCC_FƵ��ֵ_Lֵ_C��С_���_Hֵ.tif"
		//							"CSF_����˫��ĸHH_FƵ��ֵ_Lֵ_Cֵ_H��С_���.tif"

		//�ļ�����������������
		if((strcmp(StiTestParam.TestType,"LL"))==0) {strcpy(TiffBaseName,"CSF_LL_F");}
		if((strcmp(StiTestParam.TestType,"CC"))==0) {strcpy(TiffBaseName,"CSF_CC_F");}
		if((strcmp(StiTestParam.TestType,"HH"))==0) {strcpy(TiffBaseName,"CSF_HH_F");}

		//ͼ��ռ�Ƶ��ֵ(�̶�����)
		ImgFreq = StiTestParam.StiFreqMin;

		//����Ƶ���ַ���
		sprintf(tmpstr,"%5.2f",ImgFreq);

		//Ƶ���ַ������������ļ���
		strcat(TiffBaseName,tmpstr);
		strcat(TiffBaseName,"_L");

		//LCHֵ-->�ļ�������
		if((strcmp(StiTestParam.TestType,"LL"))==0) 
		{
			//��С�����ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",L0);
			strcat(TiffBaseName,tmpstr);
			//�����
			strcat(TiffBaseName,"_");
			//��������ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",L1);
			strcat(TiffBaseName,tmpstr);
			//�����+�ʶ���ʾ��C
			strcat(TiffBaseName,"_C");
			//�ʶ��ַ���-->�������ļ��ַ���
			sprintf(tmpstr,"%5.2f",C0);
			strcat(TiffBaseName,tmpstr);
			//�����+ɫ������ʾ��H
			strcat(TiffBaseName,"_H");
			//ɫ�����ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",H0);
			strcat(TiffBaseName,tmpstr);
			//���뷽���
			strcat(TiffBaseName,"_ANG");
			//�����-->�����ļ����ַ���
			sprintf(tmpstr,"%4.1f",StiTestParam.StiAngle);
			strcat(TiffBaseName,tmpstr);
			//����".tif"
			strcat(TiffBaseName,".tif");
		}

		if((strcmp(StiTestParam.TestType,"CC"))==0) 
		{
			//�����ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",L0);
			strcat(TiffBaseName,tmpstr);
			//�����+�ʶ���ʾ��C
			strcat(TiffBaseName,"_C");
			//��С�ʶ��ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",C0);
			strcat(TiffBaseName,tmpstr);
			//�����
			strcat(TiffBaseName,"_");
			//���ʶ��ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",C1);
			strcat(TiffBaseName,tmpstr);
			//�����+ɫ������ʾ��H
			strcat(TiffBaseName,"_H");
			//ɫ�����ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",H0);
			strcat(TiffBaseName,tmpstr);
			//����".tif"
			strcat(TiffBaseName,".tif");
		}
		
		if((strcmp(StiTestParam.TestType,"HH"))==0) 
		{
			//�����ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",L0);
			strcat(TiffBaseName,tmpstr);
			//�����+�ʶ���ʾ��C
			strcat(TiffBaseName,"_C");
			//�ʶ��ַ���-->�������ļ��ַ���
			sprintf(tmpstr,"%5.2f",C0);
			strcat(TiffBaseName,tmpstr);
			//�����+ɫ������ʾ��H
			strcat(TiffBaseName,"_H");
			//��Сɫ�����ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",H0);
			strcat(TiffBaseName,tmpstr);
			//�����
			strcat(TiffBaseName,"_");
			//���ɫ�����ַ���-->�����ļ����ַ���
			sprintf(tmpstr,"%5.2f",H1);
			strcat(TiffBaseName,tmpstr);
			//����".tif"
			strcat(TiffBaseName,".tif");
		}

		//����Tiff�ļ�����Tiff·�� + Tiff������
		strcat(TiffFileName,StiTestParam.StiFilePath);
		strcat(TiffFileName,TiffBaseName);
		
		//ΪTIFF�ļ�ͷ�ͱ�Ǳ�������ݣ�����TIFF�ļ�ͷ�ͱ�Ǳ���ͬ
		TiffHeaderTableSet(TiffHdTable,	&StiTestParam);
		
		//Tiff�ļ�ͷ�ͱ�Ǳ�д���ļ�
		TiffHeaderTableWrite(TiffFileName,TiffHdTable);

		//ͼ������������
		StiImgH = StiTestParam.ImgPixelH;
		StiImgV = StiTestParam.ImgPixelV;
		WaveImgH = StiTestParam.TestZonePixelH;
		WaveImgV = StiTestParam.TestZonePixelV;

		//���η���ǻ���ֵ���㣺��ʱ��ת�ǣ��Ƕ�Ϊ��
		StiAngle_R =  (-1.0) * StiTestParam.StiAngle * pi /180.0;		
		
		//ΪLAB����ͼ������ڴ棬������-1
		StiImg = (WORD *)calloc(3 * StiImgH * StiImgV,sizeof(WORD));
		if(StiImg == NULL) return -1;
		
		//���㣺����ͼ�����Ͻ��������
		LeftTopHor = (DWORD)((double)(StiImgH - WaveImgH) / 2.0 +.5);
		LeftTopVer = (DWORD)((double)(StiImgV - WaveImgV) / 2.0 +.5);
		
		//ΪLAB���Ҳ�ͼ������ڴ�
		WaveZoneImg = (WORD *)calloc(3 * WaveImgH * WaveImgV,sizeof(WORD));
		if(WaveZoneImg == NULL) return -2;
		
		//ȫͼ�񱳾�ɫ����
		for(i=0;i<StiImgV;i++)
		{
			for(j=0;j<StiImgH;j++)
			{
				*(StiImg + 3*(i * StiImgH + j))     = (WORD) (655.35 * (WORD)StiTestParam.BKG_L);
				*(StiImg + 3*(i * StiImgH + j) + 1) = (WORD) (256.0  * (WORD)StiTestParam.BKG_A);
				*(StiImg + 3*(i * StiImgH + j) + 2) = (WORD) (256.0  * (WORD)StiTestParam.BKG_B);	
			}
		}
		
		//���Ȳ�������
		if((strcmp(StiTestParam.TestType,"LL"))==0)
		{
			
			//�����ȱ仯��a*b*���䣬����һ�鼴��
			As = 256.0 * A0;
			Bs = 256.0 * B0;
			
			//�������Ĳ�������
			for(i=0;i<WaveImgV;i++)
			{
				for(j=0;j<WaveImgH;j++)
				{
					//���η���ǵ���0��
					if(StiTestParam.StiAngle == 0)
					{
						//�غ������
						x = (double)j/(double)(WaveImgH - 1);				
					}
					else	//���η���ǲ�����0��
					{
						v = (double)i/(double)StiTestParam.TestZonePixelV;
						u = (double) j/(double)StiTestParam.TestZonePixelH;						
						
						//ת�Ǽ���
						x = u * cos(StiAngle_R) + v * sin(StiAngle_R);							
					}
					
					//���Ҳ��ĺ���ֵ
					y = 0.5 * (L1 - L0) 
						* (sin(6.2831852 * ImgFreq * x) + 1.0) 
						+ L0;
					
					//���������ڵ�ƫ����
					WaveZoneOffs =  3 * (i * WaveImgH + j);
					
					//����ֵL�ı���
					Ls = 655.35 * y;
					
					//Lab����ֵȡ�����������ͼ������
					*(WaveZoneImg + WaveZoneOffs) = (WORD)Ls;
					*(WaveZoneImg + WaveZoneOffs + 1) = (WORD) As;
					*(WaveZoneImg + WaveZoneOffs + 2) = (WORD) Bs;
					
				}
			}			
		}	
		
		//�ʶȲ�������
		if((strcmp(StiTestParam.TestType,"CC"))==0)
		{
			
			//�������Ĳ�������
			
			//���Ȳ��䣬����ֵ����һ�鼴��
			Ls = 655.35 * L0;
			
			for(i=0;i<WaveImgV;i++)
			{
				for(j=0;j<WaveImgH;j++)
				{
					//���η���ǵ���0��
					if(StiTestParam.StiAngle == 0)
					{
						//�غ������
						x = (double)j/(double)(WaveImgH - 1);				
					}
					else	//���η���ǲ�����0��
					{
						v = (double)i/(double)StiTestParam.TestZonePixelV;
						u = (double) j/(double)StiTestParam.TestZonePixelH;						
						
						//ת�Ǽ���
						x = u * cos(StiAngle_R) + v * sin(StiAngle_R);							
					}
					
					//���Ҳ��ĺ���ֵ
					y = 0.5 * (C1 - C0) 
						* (sin(6.2831852 * ImgFreq * x) + 1.0) 
						+ C0;

					Lch2Lab(&y, &H0, &As, &Bs);

					//����A,B�ı���ֵ����Ϊ������
					As = 256.0 * As;
					Bs = 256.0 * Bs;
					
					//���������ڵ�ƫ����
					WaveZoneOffs =  3 * (i * WaveImgH + j);
					
					//Lab����ֵȡ�����������ͼ������
					*(WaveZoneImg + WaveZoneOffs) = (WORD)Ls;
					*(WaveZoneImg + WaveZoneOffs + 1) = (WORD) As;
					*(WaveZoneImg + WaveZoneOffs + 2) = (WORD) Bs;				
				}
			}
		}
		
		//ɫ���ǲ�������
		if((strcmp(StiTestParam.TestType,"HH"))==0)
		{
			
			//�������Ĳ�������
			
			//���Ȳ��䣬����ֵ����һ�鼴��
			Ls = 655.35 * L0;
			
			for(i=0;i<WaveImgV;i++)
			{
				for(j=0;j<WaveImgH;j++)
				{
					//���η���ǵ���0��
					if(StiTestParam.StiAngle == 0)
					{
						//�غ������
						x = (double)j/(double)(WaveImgH - 1);				
					}
					else	//���η���ǲ�����0��
					{
						v = (double)i/(double)StiTestParam.TestZonePixelV;
						u = (double) j/(double)StiTestParam.TestZonePixelH;						
						
						//ת�Ǽ���
						x = u * cos(StiAngle_R) + v * sin(StiAngle_R);						
					}	
					
					//���Ҳ��ĺ���ֵ
					//��������Ϊ y = sin(2*(pi)*f*x +(3/2)pi) + 1
					y = 0.5 * (H1 - H0) 
						* (sin(6.2831852 * ImgFreq * x + 4.71238898) + 1.0) 
						+ H0;

					Lch2Lab(&C0, &y, &As, &Bs);
				
					//����A,B�ı���ֵ����Ϊ������
					As = 256.0 * As;
					Bs = 256.0 * Bs;
					
					//���������ڵ�ƫ����
					WaveZoneOffs =  3 * (i * WaveImgH + j);
					
					//Lab����ֵȡ�����������ͼ������
					*(WaveZoneImg + WaveZoneOffs) = (WORD)Ls;
					*(WaveZoneImg + WaveZoneOffs + 1) = (WORD) As;
					*(WaveZoneImg + WaveZoneOffs + 2) = (WORD) Bs;				
				}
			}
			
		}
		
		//������Ƕ��Tiffͼ��
		for(i=0;i<WaveImgV;i++)
		{
			//��������ȫͼ�е�ƫ����
			WaveZoneOffs = 3 * ((LeftTopVer + i) * StiImgH + LeftTopHor);
			
			//ԭ����ʵ����WaveZoneOffs = 3*((384 + i)*1280 + 512);
			
			for(j=0;j<WaveImgH;j++)
			{
				*( StiImg + WaveZoneOffs + 3*j     )= *(WaveZoneImg + 3* (i * WaveImgH + j)    );
				*((StiImg + WaveZoneOffs + 3*j + 1))= *(WaveZoneImg + 3* (i * WaveImgH + j) + 1);
				*((StiImg + WaveZoneOffs + 3*j + 2))= *(WaveZoneImg + 3* (i * WaveImgH + j) + 2);				
			}
		}
		
		//�򿪣�����TIFF�ļ�
		fp = fopen(TiffFileName,"ab");
		
		//������-3
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
		MessageBox(hdwnd,"ΪTiffͼ������ڴ����","�����ڴ����",MB_ICONWARNING|MB_OK);
		break;
	case -2:
		MessageBox(hdwnd,"Ϊ������ͼ������ڴ����","�����ڴ����",MB_ICONWARNING|MB_OK);
		break;
	case -3:
		MessageBox(hdwnd,"дTiff�ļ�����","�ļ�����",MB_ICONWARNING|MB_OK);
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
		//ɫ���ǣ���-->����
		H_Rad = *Lch_H * pi / 180.0;
		
		*Lab_A = (double)*Lch_C /(double)(sqrt(1 + tan(H_Rad )*tan(H_Rad)));
		*Lab_B = *Lab_A * tan(H_Rad);
		
		*Lab_A = fabs(*Lab_A);
		*Lab_B = fabs(*Lab_B);
	}
	else
	{
		//90�Ⱥ�270�ȵĴ���
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
	
	//���޴���
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

	//����ַ�������
	strcpy(tmp_str,"");
	strcpy(TimeString,"");

	//��ȡ���������ʱ��-->ʱ��ṹ����
	GetLocalTime(&StiGenTime);

	//���ṹԪ�ص�������ʱ���������ֱ�ת�����ַ���
	//�ַ��������ʱ���ַ���
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
