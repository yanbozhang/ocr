
// POP_testDlg.cpp : implementation file
//

#include "stdafx.h"
#include "POP_test.h"
#include "POP_testDlg.h"
#include "afxdialogex.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define PORT 55555
typedef struct Messagerec
{
	char head;
	char label;
	char endmark;
}mes_rec;
typedef struct Mesagesend
{
	char head;
	char label;
	char mes_amount;
	char all_page;
	char current_page;
	int tim;
	char imgname[16];
	char zifubuf[1024];
	char endmark;
}mes_send;

typedef struct Mesagesend_pic
{
	char head;
	char label;
	short result1;
	short result2;
	short result3;
	char imgname[16];
	char endmark;
}mes_send_pic;

typedef struct Mesagesend_pic_jin
{
	char head;
	char label;
	short result;
	char imgname[16];
	char endmark;
}mes_send_pic_jin;
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPOP_testDlg dialog




CPOP_testDlg::CPOP_testDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPOP_testDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPOP_testDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPOP_testDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CPOP_testDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_start, &CPOP_testDlg::OnBnClickedstart)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_openCam, &CPOP_testDlg::OnBnClickedopencam)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPOP_testDlg message handlers
string Utf82Ansi(const char* srcCode)
{	
	int srcCodeLen=0;
	//计算接收到待转换字符串的缓冲区所需宽字符数
	srcCodeLen=MultiByteToWideChar(CP_UTF8,NULL,srcCode,strlen(srcCode),NULL,0);
	wchar_t* result_t=new wchar_t[srcCodeLen+1];
	//向result_t缓冲区写入宽字符
	MultiByteToWideChar(CP_UTF8,NULL,srcCode,strlen(srcCode),result_t,srcCodeLen);
	result_t[srcCodeLen]='\0';
	//计算接收到待转换字符串的缓冲区所需字节数
	srcCodeLen=WideCharToMultiByte(CP_ACP,NULL,result_t,wcslen(result_t),NULL,0,NULL,NULL);
	char* result=new char[srcCodeLen+1];
	//向result缓冲区写入字符
	WideCharToMultiByte(CP_ACP,NULL,result_t,wcslen(result_t),result,srcCodeLen,NULL,NULL);
	result[srcCodeLen]='\0';
	string srcAnsiCode="";
	srcAnsiCode=(string)result;
	delete result_t;
	delete result;
	return srcAnsiCode;
}

//去掉字符串中的空格
void trim(string &s)
 {
     int index = 0;
     if( !s.empty())
    {
        while( (index = s.find(' ',index)) != string::npos)
         {
             s.erase(index,1);
         }
    }
 }
BOOL CPOP_testDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	GetClientRect(&m_rect);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	cfont.CreatePointFont(200,_T("宋体"),NULL);
	GetDlgItem(IDC_STATIC0)->SetFont(&cfont); 
	GetDlgItem(IDC_STATIC4)->SetFont(&cfont); 
	GetDlgItem(IDC_STATIC1)->SetFont(&cfont); 
	GetDlgItem(IDC_STATIC2)->SetFont(&cfont); 
	GetDlgItem(IDC_STATIC3)->SetFont(&cfont); 
	//GetDlgItem(IDC_STATIC5)->SetFont(&cfont); 
	GetDlgItem(IDC_STATIC6)->SetFont(&cfont); 
	GetDlgItem(IDC_STATIC7)->SetFont(&cfont); 
	GetDlgItem(IDC_STATIC8)->SetFont(&cfont); 
	//GetDlgItem(IDC_STATIC9)->SetFont(&cfont); 
	GetDlgItem(IDC_STATIC10)->SetFont(&cfont); 
	GetDlgItem(IDC_STATIC11)->SetFont(&cfont); 
	//GetDlgItem(IDC_STATIC12)->SetFont(&cfont); 
	CamCount = CCameraDS::CameraCount();
	char camera_name[1024];
	char istr[25];
	for (int i = 0; i < CamCount; i++)
	{
		int retval = CCameraDS::CameraName(i, camera_name, sizeof(camera_name));
		sprintf_s(istr, " # %d", i);
		strcat_s(camera_name, istr);
		CString camstr(camera_name);
		if (retval <0||retval ==0)
			AfxMessageBox(_T("不能获取摄像头的名称"));
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPOP_testDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPOP_testDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
		showImage(img_input,IDC_camera);
		showImage(img_screen,IDC_screen);
		showImage(img_cas,IDC_cas);
		showImage(img_gear,IDC_gear);
		showImage(img_cas,IDC_casrow);
		showImage(img_gear1,IDC_gear1);
		showImage(img_gear2,IDC_gear2);
		showImage(img_gear3,IDC_gear3);
		showImage(img_gear11,IDC_gear11);
		showImage(img_gear21,IDC_gear21);
		showImage(img_gear31,IDC_gear31);
		showImage(img_slat,IDC_SLAT);
		showImage(img_trim,IDC_TRIM);
		showImage(img_jinfeng,IDC_SLAT);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
Mat CPOP_testDlg::getsimilarity(Mat img,int &gear_ID)
{
	char file[100];
	char out[100];
	int count_best=0;
	temBest=0;
	for(int i=1;i<11;i++)
	{
		sprintf(file,"muban%u.jpg",i);
		Mat orgImg = imread(file);
		Mat tmpImg;
		resize(img, tmpImg, Size(orgImg.cols, orgImg.rows));
		vector<unsigned int> count1(64), count2(64);
		int nRows = orgImg.rows,nCols = orgImg.cols * orgImg.channels();
		const uchar* p1;
		const uchar* p2;
		for (int i = 0; i < nRows; ++i)
		{
			p1 = tmpImg.ptr<uchar>(i);
			p2 = orgImg.ptr<uchar>(i);
			for (int j = 0; j < nCols; j += 3)
			{
				int r1 = int(p1[j])/64;    
				int g1 = int(p1[j + 1])/64;
				int b1 = int(p1[j + 2])/64;
				count1[r1 * 16 + g1 * 4 + b1]++;
				int r2 = int(p2[j])/64;    
				int g2 = int(p2[j + 1])/64;
				int b2 = int(p2[j + 2])/64;
				count2[r2 * 16 + g2 * 4 + b2]++;
			}
		}
		long long res1 = 0;
		for (int i = 0; i < count1.size(); i++)
			res1 += count1[i] * count1[i];
		double len1 = sqrtf(res1);
		long long res2 = 0;
		for (int i = 0; i < count2.size(); i++)
			res2 += count2[i] * count2[i];
		double len2 = sqrtf(res2);
		assert(len1 != 0 && len2 != 0);
		long long sum = 0;
		for (int i = 0; i < count1.size(); i++)
			sum += count1[i] * count2[i];
		similarityValue = (double)sum / len1 / len2 >0 ? (double)sum / len1 / len2:0;
		if(similarityValue>temBest)
		{
			temBest=similarityValue;
			count_best=i;
		}
	}	
	sprintf(out,"muban%u.jpg",count_best);
	gear_ID = count_best;
	Mat out_img=imread(out);
	temBest=0;	
	return out_img;
}

Mat CPOP_testDlg::getjinfeng(Mat img,int &jinfeng_ID)
{
	char file[100];
	char out[100];
	int count_best=0;
	temBest=0;
	for(int i=31;i<43;i++)
	{
		sprintf(file,"jinfeng%u.jpg",i);
		Mat orgImg = imread(file);
		Mat tmpImg;
		resize(orgImg,orgImg,Size(orgImg.cols/2, orgImg.rows/2));
		resize(img, tmpImg, Size(orgImg.cols, orgImg.rows));
		vector<unsigned int> count1(64), count2(64);
		int nRows = orgImg.rows,nCols = orgImg.cols * orgImg.channels();
		const uchar* p1;
		const uchar* p2;
		for (int i = 0; i < nRows; ++i)
		{
			p1 = tmpImg.ptr<uchar>(i);
			p2 = orgImg.ptr<uchar>(i);
			for (int j = 0; j < nCols; j += 3)
			{
				int r1 = int(p1[j])/64;    
				int g1 = int(p1[j + 1])/64;
				int b1 = int(p1[j + 2])/64;
				count1[r1 * 16 + g1 * 4 + b1]++;
				int r2 = int(p2[j])/64;    
				int g2 = int(p2[j + 1])/64;
				int b2 = int(p2[j + 2])/64;
				count2[r2 * 16 + g2 * 4 + b2]++;
			}
		}
		long long res1 = 0;
		for (int i = 0; i < count1.size(); i++)
			res1 += count1[i] * count1[i];
		double len1 = sqrtf(res1);
		long long res2 = 0;
		for (int i = 0; i < count2.size(); i++)
			res2 += count2[i] * count2[i];
		double len2 = sqrtf(res2);
		assert(len1 != 0 && len2 != 0);
		long long sum = 0;
		for (int i = 0; i < count1.size(); i++)
			sum += count1[i] * count2[i];
		similarityValue = (double)sum / len1 / len2 >0 ? (double)sum / len1 / len2:0;
		if(similarityValue>temBest)
		{
			temBest=similarityValue;
			count_best=i;
		}
	}	
	sprintf(out,"jinfeng%u.jpg",count_best);
	jinfeng_ID = count_best;
	Mat out_img=imread(out);
	temBest=0;	
	return out_img;
}

Mat CPOP_testDlg::compareSameByHist(Mat img)
{
	char file[100];
	char out[100];
	int count_best=0;
	temBest=0;
	for(int i=1;i<11;i++)
	{
		sprintf(file,"muban%u.jpg",i);
		Mat orgImg = imread(file);
		Mat tmpImg;
		resize(img, tmpImg, Size(orgImg.cols, orgImg.rows));
		//HSV颜色特征模型(色调H,饱和度S，亮度V)
		cvtColor(tmpImg, tmpImg, COLOR_BGR2HSV);
		cvtColor(orgImg, orgImg, COLOR_BGR2HSV);
		//直方图尺寸设置
		//一个灰度值可以设定一个bins，256个灰度值就可以设定256个bins
		//对应HSV格式,构建二维直方图
		//每个维度的直方图灰度值划分为256块进行统计，也可以使用其他值
		int hBins = 256, sBins = 256;
		int histSize[] = { hBins,sBins };
		//H:0~180, S:0~255,V:0~255
		//H色调取值范围
		float hRanges[] = { 0,180 };
		//S饱和度取值范围
		float sRanges[] = { 0,255 };
		const float* ranges[] = { hRanges,sRanges };
		int channels[] = { 0,1 };//二维直方图
		MatND hist1, hist2;
		calcHist(&tmpImg, 1, channels, Mat(), hist1,2,histSize, ranges, true, false);
		normalize(hist1, hist1, 0, 1, NORM_MINMAX, -1, Mat());
		calcHist(&orgImg, 1, channels, Mat(), hist2, 2, histSize, ranges, true, false);
		normalize(hist2, hist2, 0, 1, NORM_MINMAX, -1, Mat());
		similarityValue = compareHist(hist1, hist2, CV_COMP_CORREL);//相关性CV_COMP_CORREL 卡方比较CV_COMP_CHISQR 十字交叉性CV_COMP_INTERSECT 巴氏距离CV_COMP_BHATTACHARYYA
		if(similarityValue>temBest)
		{
			temBest=similarityValue;
			count_best=i;
		}
	}	
	sprintf(out,"muban%u.jpg",count_best);
	Mat out_img=imread(out);
	temBest=0;	
	return out_img;
}

Mat gammaTransform(Mat &srcImage, float kFactor)
{
	unsigned char LUT[256];
	for (int i = 0; i < 256; i++)
	{
		float f = (i + 0.5f) / 255;
		f = (float)(pow(f, kFactor));
		LUT[i] = saturate_cast<uchar>(f*255.0f - 0.5f);
	}
	Mat resultImage = srcImage.clone();

	if (srcImage.channels() == 1)
	{

		MatIterator_<uchar> iterator = resultImage.begin<uchar>();
		MatIterator_<uchar> iteratorEnd = resultImage.end<uchar>();
		for (; iterator != iteratorEnd; iterator++)
		{
			*iterator = LUT[(*iterator)];
		}
	}
	else
	{


		MatIterator_<Vec3b> iterator = resultImage.begin<Vec3b>();
		MatIterator_<Vec3b> iteratorEnd = resultImage.end<Vec3b>();
		for (; iterator != iteratorEnd; iterator++)
		{
			(*iterator)[0] = LUT[((*iterator)[0])];//b
			(*iterator)[1] = LUT[((*iterator)[1])];//g
			(*iterator)[2] = LUT[((*iterator)[2])];//r
		}
	}
	return resultImage;
}

Rect detecMark(Mat &srcImage)
{
	vector<vector<Point> > contours;
	findContours(srcImage, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
	int mark_count=0;
	Point2f mark_last;
	Point2f mark[4];
	Point2f top_left;
	Point2f lower_right;
	for(size_t i = 0; i < contours.size(); i++)
	{
		size_t count = contours[i].size();
		if( count < 6 )
			continue;
		Mat pointsf;
		Mat(contours[i]).convertTo(pointsf, CV_32F);	
		RotatedRect box = fitEllipse(pointsf);
		if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*1.5 )
			continue;
		if(box.size.width < 30 || box.size.height < 30)
			continue;
		if(box.size.width > 50 || box.size.height > 50)
			continue;
		//if(box.center.x<50||box.center.y<20||box.center.x>srcImage.cols-80||box.center.y>srcImage.rows-40)
			//continue;
		if(box.center.x>srcImage.cols/4&&box.center.x<srcImage.cols*3/4)
			continue;
	   if(box.center.y>srcImage.rows/4&&box.center.y<srcImage.rows*3/4)
			continue;
		if(mark_count>3)
		{
			break;
		}
		if(mark_count==0)
		{
			mark_last=box.center;
			mark[mark_count]=box.center;
			mark_count++;
		}
		if(mark_count!=0 && (abs((box.center.x-mark_last.x)*(box.center.y-mark_last.y))>100))
		{
			mark[mark_count]=box.center;
			mark_count++;
		}
		mark_last=box.center;
	}
	int average_x;
	int average_y;
	average_x = (mark[0].x+mark[1].x+mark[2].x+mark[3].x)/4;
	average_y = (mark[0].y+mark[1].y+mark[2].y+mark[3].y)/4;
	int j1=0;
	int j2=0;
	int j3=0;
	int j4=0;
	int da_x[2]={0};
	int da_y[2]={0};
	int xiao_x[2]={0};
	int xiao_y[2]={0};
	for(int i=0;i<4;i++)
	{
		if(mark[i].x<average_x)
		{
			xiao_x[j1]=mark[i].x;
			j1++;
		}
		else 
		{
			da_x[j2]=mark[i].x;
			j2++;
		}			
		if(mark[i].y<average_y)
		{
			xiao_y[j3]=mark[i].y;
			j3++;
		}
		else 
		{
			da_y[j4]=mark[i].y;
			j4++;
		}
		if(j1==3||j2==3||j3==3||j4==3)
			break;
	}
	if(da_x[1]*da_x[0]*da_y[1]*da_y[0]*xiao_x[1]*xiao_x[0]*xiao_y[1]*xiao_y[0]==0)
	{
		Rect tmp(0,0,0,0);
		return tmp;
	}
	else if(j1*j2*j3*j4!=0)
	{
		/*top_left.x = (xiao_x[1]+xiao_x[0])*2;
		top_left.y = (xiao_y[1]+xiao_y[0])*2;
		lower_right.x = (da_x[1]+da_x[0])*2;
		lower_right.y = (da_y[1]+da_y[0])*2;*/
		top_left.x = (xiao_x[1]+xiao_x[0]);
		top_left.y = (xiao_y[1]+xiao_y[0]);
		lower_right.x = (da_x[1]+da_x[0]);
		lower_right.y = (da_y[1]+da_y[0]);
		Rect rect(top_left.x,top_left.y, lower_right.x-top_left.x, lower_right.y-top_left.y);
		return rect;
	}
	
}

int bSums(Mat src)
{

	int counter = 0;
	//迭代器访问像素点
	Mat_<uchar>::iterator it = src.begin<uchar>();
	Mat_<uchar>::iterator itend = src.end<uchar>();  
	for (; it!=itend; ++it)
	{
		if((*it)>0) counter+=1;//二值化后，像素点是0或者255
	}			
	return counter;
}
void CPOP_testDlg::showImage_Ipl(IplImage* image,int UID)
{
	CWnd *pWnd = GetDlgItem(UID);
	CDC*pDC=pWnd->GetDC();
	HDC hdc=pDC->GetSafeHdc();
	CRect rect;
	pWnd->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(image,image->nChannels);
	cimg.DrawToHDC(hdc,&rect);
}

void CPOP_testDlg::showImage(Mat& src, UINT ID)
{
	if (src.empty())
		return;
	CRect rect;
	Mat dst = src.clone();
	GetDlgItem(ID)->GetClientRect(&rect);	// 获取控件尺寸位置
	if (dst.channels() == 1)
		cvtColor(dst, dst, CV_GRAY2BGR);
	CDC* pDC = GetDlgItem(ID)->GetDC();
	HDC hDC = pDC->GetSafeHdc();				// 获取 HDC(设备句柄) 来进行绘图操作
	CvvImage cimg;
	IplImage cpy = dst;
	cimg.CopyOf(&cpy);						// 复制图片
	cimg.DrawToHDC(hDC, &rect);				// 将图片绘制到显示控件的指定区域内
	ReleaseDC(pDC);
}

void CPOP_testDlg::clearEdit()
{
	static int Edit_id[]={IDC_EDIT1,IDC_EDIT2,IDC_EDIT3,IDC_EDIT4,IDC_EDIT5,IDC_EDIT6,IDC_EDIT7,IDC_EDIT8,
		IDC_EDIT9,IDC_EDIT10,IDC_EDIT11,IDC_EDIT12,IDC_EDIT13,IDC_EDIT14,IDC_EDIT15,IDC_EDIT16,IDC_EDIT17,
		IDC_EDIT18,IDC_EDIT19,IDC_EDIT20};
	for(int i=1;i<21;i++)
	{
		GetDlgItem(Edit_id[i])->SetWindowText(_T(""));
	}
}

HCURSOR CPOP_testDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CPOP_testDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	exit(0);
}

void CPOP_testDlg::cas_recognition(Mat img,int &info_count,int &all_page,int &current_page,string &img_name,string &ocr_out)
{
	clearEdit();
	STRING text_out,cas_color_ID; 
	string ocr_out_tmp;
	info_count = 0;
	img_screen = img(rect).clone();
	showImage(img_screen,IDC_screen);
	img_cas = img_screen(rect_cas).clone();
	showImage(img_cas,IDC_cas);
	time_t nowtime = time(NULL);
	struct tm *time_current;
	time_current = gmtime(&nowtime);
	char tmp_name[16];
	sprintf(tmp_name, "%04d%02d%02d%02d%02d%02d;", 1900 + time_current->tm_year, 1 + time_current->tm_mon, time_current->tm_mday, time_current->tm_hour, time_current->tm_min, time_current->tm_sec);
	Sleep(100);//为了保存图片
	img_name = tmp_name;
	cvtColor(img_cas, img_cas_hsv, CV_BGR2HSV);
	split(img_cas_hsv, img_cas_hsv_vec);
	img_cas_v=img_cas_hsv_vec[2];
	img_cas_v_enhance=gammaTransform(img_cas_v,2);
	imwrite("tmp1.jpg",img_cas_v_enhance);
	Mat element_pz(1,50, CV_8U, Scalar(1));
	morphologyEx(img_cas_v_enhance, img_cas_dilate, MORPH_DILATE, element_pz);
	//imwrite("tmp3.jpg",img_cas_dilate);
	threshold(img_cas_dilate, img_cas_dilate_binary, 110, 255, CV_THRESH_BINARY);
	imwrite("tmp2.jpg",img_cas_dilate_binary);
	findContours(img_cas_dilate_binary, contours_cas, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);  
	vector<Rect> boundRect(contours_cas.size());
	api.Init(NULL, "eng", tesseract::OEM_DEFAULT);  
	api.SetVariable( "tessedit_char_whitelist", "123+ABCDEFGHIJKLMNOPQRSTUVWXYZ-" );  //0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz-_
	cas_line =1;
	static int ocr_id[]={IDC_EDIT1,IDC_EDIT2,IDC_EDIT3,IDC_EDIT4,IDC_EDIT5,IDC_EDIT6,IDC_EDIT7,IDC_EDIT8,IDC_EDIT9,IDC_EDIT10};
	static int color_id[]={IDC_EDIT11,IDC_EDIT12,IDC_EDIT13,IDC_EDIT14,IDC_EDIT15,IDC_EDIT16,IDC_EDIT17,IDC_EDIT18,IDC_EDIT19,IDC_EDIT20};
	for (int i = contours_cas.size()-1; i >=0 ; i--)//contours.size()
	{
		boundRect[i] = boundingRect(Mat(contours_cas[i])); 
			int width = boundRect[i].width;
			int height = boundRect[i].height;
			if(height<width/3&&height>8&&width>20)
			{
				printf("开始识别字符，第%d行\n",i);
				info_count++;
 				int x = boundRect[i].x;
				int y = boundRect[i].y;
				Mat temp,temphsv,tempouty,tempoutg,tempoutr,tempoutw,tmp_color;
				Rect roi(x, y, width, height);
				//tmp_color = gammaTransform(img_cas,2);
				temp=img_cas(roi);
				//imwrite("tmp5.jpg",temp);
				cvtColor(temp,temphsv,COLOR_BGR2HSV);
				printf("开始识别颜色，第%d行\n",i);
				inRange(temphsv,Scalar(26,43,46),Scalar(34,255,255),tempouty); //黄色
				inRange(temphsv,Scalar(100,43,46),Scalar(124,255,255),tempoutg); //蓝色
				inRange(temphsv,Scalar(0,43,46),Scalar(10,255,255),tempoutr); //红色
				inRange(temphsv,Scalar(156,43,46),Scalar(180,255,255),tempoutr);//红色
				inRange(temphsv,Scalar(0,0,221),Scalar(180,30,255),tempoutw); //白色
				int ycount=bSums(tempouty);
				int gcount=bSums(tempoutg);
				int rcount=bSums(tempoutr);
				int wcount=bSums(tempoutw);
				int maxcount=0;
				maxcount=__max(ycount,__max(gcount,__max(rcount,wcount)));
				if(wcount>50)
				{
					maxcount = wcount;
				}
				if(maxcount==ycount)
				{
					imwrite("cas.jpg",gammaTransform(img_cas_v(roi),3) );
					cas_image="cas.jpg";
					if (!api.ProcessPages(cas_image, NULL, 0, &text_out))  
					{  
						printf("字符识别错误\n");
						return ;  
					}
					if(cas_line>10)
					{
						printf("字符行分割错误\n");
						return;
					}
					CEdit* pEdit_ocr = (CEdit*)GetDlgItem(ocr_id[cas_line-1]);
					int nLength_ocr = pEdit_ocr->GetWindowTextLength();
					cas_ocr = text_out.string();
					pEdit_ocr->SetSel(nLength_ocr, nLength_ocr);
					pEdit_ocr->ReplaceSel(cas_ocr); 
					CEdit* pEdit_color = (CEdit*)GetDlgItem(color_id[cas_line-1]);
					int nLength_color = pEdit_color->GetWindowTextLength();
					cas_color = "黄色";
					cas_color_ID = "2";
					pEdit_color->SetSel(nLength_color, nLength_color);
					pEdit_color->ReplaceSel(cas_color); 
					printf("识别的颜色为黄色\n");
				}
				if(maxcount==gcount)
				{
					imwrite("cas.jpg",gammaTransform(img_cas_v(roi),3));
					cas_image="cas.jpg";
					if (!api.ProcessPages(cas_image, NULL, 0, &text_out))  
					{  
						printf("字符识别错误\n");
						return ;  
					}
					if(cas_line>10)
					{
						printf("字符行分割错误\n");
						return;
					}

					CEdit* pEdit_ocr = (CEdit*)GetDlgItem(ocr_id[cas_line-1]);
					int nLength_ocr = pEdit_ocr->GetWindowTextLength();
					cas_ocr = text_out.string();
					pEdit_ocr->SetSel(nLength_ocr, nLength_ocr);
					pEdit_ocr->ReplaceSel(cas_ocr); 
					CEdit* pEdit_color = (CEdit*)GetDlgItem(color_id[cas_line-1]);
					int nLength_color = pEdit_color->GetWindowTextLength();
					cas_color = "蓝色";
					cas_color_ID = "3";
					printf("识别的颜色为蓝色\n");
					/*if(ycount>0)
					{
						cas_color = "黄色";
						cas_color_ID = "2";
					}*/
					pEdit_color->SetSel(nLength_color, nLength_color);
					pEdit_color->ReplaceSel(cas_color); 
				}
				if(maxcount==rcount)
				{
					imwrite("cas.jpg",gammaTransform(img_cas_v(roi),0.8));
					cas_image="cas.jpg";
					if (!api.ProcessPages(cas_image, NULL, 0, &text_out))  
					{  
						printf("字符识别错误\n");
						return ;  
					}
					if(cas_line>10)
					{
						printf("字符行分割错误\n");
						return;
					}
					CEdit* pEdit_ocr = (CEdit*)GetDlgItem(ocr_id[cas_line-1]);
					int nLength_ocr = pEdit_ocr->GetWindowTextLength();
					cas_ocr = text_out.string();
					pEdit_ocr->SetSel(nLength_ocr, nLength_ocr);
					pEdit_ocr->ReplaceSel(cas_ocr); 
					CEdit* pEdit_color = (CEdit*)GetDlgItem(color_id[cas_line-1]);
					int nLength_color = pEdit_color->GetWindowTextLength();
					cas_color = "红色";
					printf("识别的颜色为红色\n");
					cas_color_ID = "1";
					pEdit_color->SetSel(nLength_color, nLength_color);
					pEdit_color->ReplaceSel(cas_color); 
				}
				if(maxcount==wcount)
				{
					Rect roi_page(roi.x+width/2,roi.y+5,width/2-35,height-10);
					/*Mat tmpgray,tmpbinary;
					cvtColor(temp,tmpgray,CV_BGR2GRAY);
					threshold(tmpgray, tmpbinary, 150, 255, CV_THRESH_BINARY);*/
					temp= gammaTransform(img_cas_v(roi_page),3) ;
					imwrite("cas.jpg",temp);
					cas_image="cas.jpg";
					api.SetVariable( "tessedit_char_whitelist", "0123456789/" );
					if (!api.ProcessPages(cas_image, NULL, 0, &text_out))  
					{  
						printf("字符识别错误\n");
						return ;  
					}
					if(cas_line>10)
					{
						printf("字符行分割错误\n");
						return;
					}
					string tmp_page= Utf82Ansi(text_out.string());
					trim(tmp_page);
					int len = tmp_page.length();
					if (len>1)
					{
						//strcpy(current_page, tmp_page.substr(0,1).c_str());
						//strcpy(all_page, tmp_page.substr(2,3).c_str());
						current_page = atoi(tmp_page.substr(0,1).c_str());
						all_page = atoi(tmp_page.substr(2,3).c_str());
					}
					else
					{
						current_page = 0;
						all_page = 0;
						printf("页码异常，页码页字符个数: %d\n",len);
					}
					CEdit* pEdit_ocr = (CEdit*)GetDlgItem(ocr_id[cas_line-1]);
					int nLength_ocr = pEdit_ocr->GetWindowTextLength();
					cas_ocr = text_out.string();
					pEdit_ocr->SetSel(nLength_ocr, nLength_ocr);
					CString Page("PAGE");
					pEdit_ocr->ReplaceSel(Page+cas_ocr); 
					CEdit* pEdit_color = (CEdit*)GetDlgItem(color_id[cas_line-1]);
					int nLength_color = pEdit_color->GetWindowTextLength();
					cas_color = "白色";
					printf("识别的颜色为白色\n");
					cas_color_ID = "4";
					pEdit_color->SetSel(nLength_color, nLength_color);
					pEdit_color->ReplaceSel(cas_color); 
					rectangle(img_cas, roi_page, Scalar(0,100,255), 4, 8);
				}
				if(maxcount!=wcount)
				{
					rectangle(img_cas, Rect(x, y, width, height), Scalar(0,0,255), 4, 8);
					ocr_out_tmp = ocr_out_tmp + text_out.string()+ "," +cas_color_ID.string()+";";
				}
				cas_line++;				
	     }
  }
  ocr_out = ocr_out_tmp;
  showImage(img_cas,IDC_casrow);
}

void CPOP_testDlg::gear_recognition(Mat img,int &gear1,int &gear2,int &gear3,string &img_name)
{
	img_screen = img(rect).clone();
	img_gear = img_screen(rect_gear).clone();
	showImage(img_gear,IDC_gear);
	img_gear_enhance=gammaTransform(img_gear,3);
	cvtColor(img_gear_enhance,img_gear_gray,CV_BGR2GRAY);
	imwrite("qi_gray.jpg",img_gear_gray);
	threshold(img_gear_gray, img_gear_binary, 60, 255, CV_THRESH_BINARY);
	imwrite("qi_binary.jpg",img_gear_binary);
	Mat element1(3,3, CV_8U, Scalar(1));
	morphologyEx(img_gear_binary, img_gear_dilate, MORPH_DILATE, element1);
	imwrite("qi_dilate.jpg",img_gear_dilate);
	findContours(img_gear_dilate, contours_gear, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);	
	vector<Rect> boundRect1(contours_gear.size());
	int count_qi=0;
	vector<Rect> boundqi(10);
	for(size_t i = 0; i < contours_gear.size(); i++)
	{
		boundRect1[i] = boundingRect(Mat(contours_gear[i])); 
		int width = boundRect1[i].width;
		int height = boundRect1[i].height;
		if(height>(img_gear_binary.rows/1.5)&&width>(img_gear_binary.cols/10)&&width<(img_gear_binary.cols/5))
		{
			boundqi[count_qi]=boundRect1[i];
			count_qi++;
		}
		if(count_qi>3)
		{
			printf("起落架识别数量出错\n");
			break;
		}
			
	}
	printf("起落架识别数量：%d\n",count_qi);
	if(count_qi==3)
	{
		if (boundqi[0].y==min(boundqi[0].y,min(boundqi[1].y,boundqi[2].y)))
		{
			img_gear2 = img_gear(boundqi[0]);
			img_gear20 = img_gear_enhance(boundqi[0]);
			showImage(img_gear2,IDC_gear2);
			if(boundqi[1].x<boundqi[2].x)
			{
				img_gear1 = img_gear(boundqi[1]);
				img_gear3 = img_gear(boundqi[2]);
				img_gear10 = img_gear_enhance(boundqi[1]);
				img_gear30 = img_gear_enhance(boundqi[2]);
				showImage(img_gear1,IDC_gear1);
				showImage(img_gear3,IDC_gear3);
			}
			else
			{
				img_gear1 = img_gear(boundqi[2]);
				img_gear3 = img_gear(boundqi[1]);
				img_gear10 = img_gear_enhance(boundqi[2]);
				img_gear30 = img_gear_enhance(boundqi[1]);
				showImage(img_gear1,IDC_gear1);
				showImage(img_gear3,IDC_gear3);
			}
		}
		else if (boundqi[1].y==min(boundqi[0].y,min(boundqi[1].y,boundqi[2].y)))
		{
			img_gear2 = img_gear(boundqi[1]);
			img_gear20 = img_gear_enhance(boundqi[1]);
			showImage(img_gear2,IDC_gear2);
			if(boundqi[0].x<boundqi[2].x)
			{
				img_gear1 = img_gear(boundqi[0]);
				img_gear3 = img_gear(boundqi[2]);
				img_gear10 = img_gear_enhance(boundqi[0]);
				img_gear30 = img_gear_enhance(boundqi[2]);
				showImage(img_gear1,IDC_gear1);
				showImage(img_gear3,IDC_gear3);
			}
			else
			{
				img_gear1 = img_gear(boundqi[2]);
				img_gear3 = img_gear(boundqi[0]);
				img_gear10 = img_gear_enhance(boundqi[2]);
				img_gear30 = img_gear_enhance(boundqi[0]);
				showImage(img_gear1,IDC_gear1);
				showImage(img_gear3,IDC_gear3);
			}
		}
		else
		{
			img_gear2 = img_gear(boundqi[2]);
			img_gear20 = img_gear_enhance(boundqi[2]);
			showImage(img_gear2,IDC_gear2);
			if(boundqi[0].x<boundqi[1].x)
			{
				img_gear1 = img_gear(boundqi[0]);
				img_gear3 = img_gear(boundqi[1]);
				img_gear10 = img_gear_enhance(boundqi[0]);
				img_gear30 = img_gear_enhance(boundqi[1]);
				showImage(img_gear1,IDC_gear1);
				showImage(img_gear3,IDC_gear3);
			}
			else
			{
				img_gear1 = img_gear(boundqi[1]);
				img_gear3 = img_gear(boundqi[0]);
				img_gear10 = img_gear_enhance(boundqi[1]);
				img_gear30 = img_gear_enhance(boundqi[0]);
				showImage(img_gear1,IDC_gear1);
				showImage(img_gear3,IDC_gear3);
			}
		}
		/*img_gear11 = compareSameByHist(img_gear10);
		showImage(img_gear11, IDC_gear11);
		img_gear21 = compareSameByHist(img_gear20);
		showImage(img_gear21, IDC_gear21);
		img_gear31 = compareSameByHist(img_gear30);
		showImage(img_gear31, IDC_gear31);*/
		img_gear11 = getsimilarity(img_gear1,gear1);
		showImage(img_gear11, IDC_gear11);
		img_gear21 = getsimilarity(img_gear2,gear2);
		showImage(img_gear21, IDC_gear21);
		img_gear31 = getsimilarity(img_gear3,gear3);
		showImage(img_gear31, IDC_gear31);
		char tmp_name[16];
		sprintf(tmp_name, "abcd");
		Sleep(100);//为了保存图片
		img_name = tmp_name;
		//Rect rect_slat_out(rect.width*(screen_width-slat_width)/screen_width,rect.height*(cas_height+gear_height)/screen_height,rect.width*slat_width/screen_width,rect.height*slat_height/screen_height);
		}	
}

void CPOP_testDlg::slat_recognition(Mat img,int &jinfeng,string &img_name)
{
	img_screen = img(rect).clone();
	img_slat = img_screen(rect_slat).clone();
	showImage(img_slat,IDC_SLAT);
	img_trim = img_screen(rect_trim).clone();
	showImage(img_trim,IDC_TRIM);
	char tmp_name[16];
	sprintf(tmp_name, "efg");
	Sleep(100);//为了保存图片
	img_name = tmp_name;
	//Rect rect_slat_out(rect.width*(screen_width-slat_width)/screen_width,rect.height*(cas_height+gear_height)/screen_height,rect.width*slat_width/screen_width,rect.height*slat_height/screen_height);
	img_jinfeng = getjinfeng(img_slat,jinfeng);
	showImage(img_jinfeng, IDC_SLATOUT);
}
void CPOP_testDlg::ocrshibie()
{
	AllocConsole();
	freopen("CON", "r", stdin );
	freopen("CON", "w", stdout);
	freopen("CON", "w", stderr);
	//ShowWindow(FALSE);
	//ShowWindow(TRUE);
	WSADATA wsaData;//存储被WSAStartup函数调用后返回的Windows Sockets数据
	WORD sockVersion = MAKEWORD(2,2);//调用不同的Winsock版本，2.2
	if(WSAStartup(sockVersion, &wsaData) != 0)
	{
		return ;
	}
	SOCKET serSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
	if(serSocket == INVALID_SOCKET)
	{
		printf("socket error !");
		return ;
	}
	sockaddr_in serAddr;
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(PORT);
	serAddr.sin_addr.S_un.S_addr = INADDR_ANY;
	if(bind(serSocket, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
	{
		printf("bind error !");
		closesocket(serSocket);
		return ;
	}
	sockaddr_in remoteAddr;
	int nAddrLen = sizeof(remoteAddr); 
	mes_rec info_rec;
	mes_send info_send;
	mes_send_pic info_send_pic;
	mes_send_pic_jin info_send_pic_jin;
	//img_input = imread("test12.bmp");
	//img_input = cvarrToMat(queryframe).clone();
	/*b_takePic = true;
	Sleep(100);
	while(b_takePic == true)
	{              
		Sleep(100);
	}*/
	img_input_tmp = imread("test13.bmp");
	resize(img_input_tmp,img_resize,Size(img_input_tmp.cols/2,img_input_tmp.rows/2));
	//showImage(img_input,IDC_camera);
	//img_enhance = gammaTransform(img_resize, 2);
	cvtColor(img_resize,img_gray,CV_BGR2GRAY);
	threshold(img_gray, img_binary, 100, 255, CV_THRESH_TOZERO_INV);
	Mat element(15,15, CV_8U, Scalar(1));
	morphologyEx(img_binary, img_erode, MORPH_ERODE, element);
	GaussianBlur(img_erode, img_fliter, Size(5, 5),0, 0, BORDER_DEFAULT);
	Canny(img_fliter,img_canny,30,70);
	rect = detecMark(img_canny);
	if(rect.width==0) 
	{
		printf("屏幕检测失败\n");
		return;
	}
	img_screen = img_input_tmp(rect).clone();
	showImage(img_screen,IDC_screen);
	rect_cas = Rect(rect.width*(screen_width-cas_width)/screen_width-5,0,rect.width*cas_width/screen_width-30,rect.height*cas_height/screen_height-5);
	rect_gear = Rect(rect.width*(screen_width-gear_width)/screen_width,rect.height*cas_height/screen_height+3,rect.width*gear_width/screen_width,rect.height*gear_height/screen_height-3);
	rect_slat = Rect(rect.width*(screen_width-slat_width)/screen_width,rect.height*(cas_height+gear_height)/screen_height,rect.width*slat_width/screen_width,rect.height*slat_height/screen_height);
	rect_trim = Rect(rect.width*(screen_width-trim_width)/screen_width,rect.height*(cas_height+gear_height+slat_height)/screen_height,rect.width*trim_width/screen_width,rect.height*trim_height/screen_height);
	/*img_cas = img_screen(rect_cas).clone();
	imwrite("cas.jpg",img_cas);
	showImage(img_cas,IDC_cas);
	img_gear = img_screen(rect_gear).clone();
	showImage(img_gear,IDC_gear);*/
	while(true)
	{
        //int ret = recvfrom(serSocket, (char*)&info_rec, sizeof(info_rec), 0, (sockaddr *)&remoteAddr, &nAddrLen);
		if (1)//(ret > 0)
		{
			//printf("接收到一个连接，对方IP：%s \r\n", inet_ntoa(remoteAddr.sin_addr));
			if (1)//(info_rec.label == 1)
			{
				time_t start_t = clock();
				//img_input = imread("test12.bmp");
				/*b_takePic = true;
				Sleep(100);
				while(b_takePic == true)
				{              
					Sleep(100);
				}*/
				printf("开始识别字符\n");
				/*b_takePic = true;
				Sleep(100);
				while(b_takePic == true)
				{              
					Sleep(100);
				}*/
				img_input_tmp = imread("test13.bmp");
				cas_recognition(img_input_tmp,info_count,all_page,current_page,img_name,ocr_out);
				//imwrite("tmp1.jpg",img_input);
				info_send.head = 170;
				info_send.label = 2;
				info_send.mes_amount = info_count-1;
				info_send.all_page = all_page;
				info_send.current_page = current_page;
				info_send.tim = clock()-start_t;
				strncpy_s(info_send.imgname, img_name.c_str(),16);
				strncpy_s(info_send.zifubuf, ocr_out.c_str(),1024);
				info_send.endmark = 255;
				int ret_cas = sendto(serSocket, (char*)&info_send, sizeof(info_send), 0, (sockaddr *)&remoteAddr, nAddrLen); 
				if(ret_cas!=SOCKET_ERROR)
			    {
					printf("字符识别结果发送成功\n");
				}
				//Sleep(2000);
			}
			if(1)// (info_rec.label == 3)
			{
				//img_input = imread("test12.bmp");
				/*b_takePic = true;
				Sleep(100);
				while(b_takePic == true)
				{              
					Sleep(100);
				}*/
				printf("开始识别起落架图符\n");
				/*b_takePic = true;
				Sleep(100);
				while(b_takePic == true)
				{              
					Sleep(100);
				}*/
				img_input_tmp = imread("test13.bmp");
				gear_recognition(img_input_tmp,gear1,gear2,gear3,img_name);
				info_send_pic.head = 170;
				info_send_pic.label = 4;
				info_send_pic.result1 = gear1;
				info_send_pic.result2 = gear2;
				info_send_pic.result3 = gear3;
				strncpy_s(info_send_pic.imgname, img_name.c_str(),16);
				info_send_pic.endmark = 255;
				int ret_pic = sendto(serSocket, (char*)&info_send_pic, sizeof(info_send_pic), 0, (sockaddr *)&remoteAddr, nAddrLen); 
				if(ret_pic!=SOCKET_ERROR)
				{
					printf("起落架图符识别结果发送成功\n");
				}
			}
			if(1)//(info_rec.label == 5)
			{
				//img_input = imread("test12.bmp");
				/*b_takePic = true;
				Sleep(100);
				while(b_takePic == true)
				{              
					Sleep(100);
				}*/
				printf("开始识别襟缝翼图符\n");
				/*b_takePic = true;
				Sleep(100);
				while(b_takePic == true)
				{              
					Sleep(100);
				}*/
				img_input_tmp = imread("test13.bmp");
				slat_recognition(img_input_tmp,jinfeng,img_name);
				info_send_pic_jin.head = 170;
				info_send_pic_jin.label = 6;
				info_send_pic_jin.result = jinfeng;
				strncpy_s(info_send_pic_jin.imgname, img_name.c_str(),16);
				info_send_pic_jin.endmark = 255;
				int ret_pic_jin = sendto(serSocket, (char*)&info_send_pic_jin, sizeof(info_send_pic_jin), 0, (sockaddr *)&remoteAddr, nAddrLen); 
				if(ret_pic_jin!=SOCKET_ERROR)
				{
					printf("襟缝翼图符识别结果发送成功\n");
				}
			}
		}
		
		//img_input = camframe.clone();
		//Mat tmp = cvarrToMat(queryframe);
		//img_input = tmp.clone();
				
		
		//img_cas_v_enhance=gammaTransform(img_cas,3.3);
				
		}
		
		closesocket(serSocket); 
		WSACleanup();
	}
	


UINT MyThreadFunction(LPVOID lp)
{
	CPOP_testDlg* pDlg = (CPOP_testDlg*) lp;
	pDlg->ocrshibie();
	return 0;
}

void CPOP_testDlg::OnBnClickedstart()
{
    screen_width = 335;
	screen_height = 210;
	cas_width = 85;
	cas_height = 100;
	gear_width = 85;
	gear_height = 12.5;
	slat_width = 85;
	slat_height = 20;
	trim_width = 85;
	trim_height = 20;
	AfxBeginThread(MyThreadFunction, this);
}


HBRUSH CPOP_testDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	if( IDC_STATIC1 ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	{
		pDC->SetTextColor(RGB(0,0,0));//设置文本颜色
		pDC->SetBkMode(OPAQUE);//设置文本背景模式
		pDC->SetBkColor(RGB(155,155,155));//设置文本背景
		hbr=CreateSolidBrush(RGB(155,155,155));//控件的背景色
	}
	if( IDC_STATIC2 ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	{
		pDC->SetTextColor(RGB(0,0,0));//设置文本颜色
		pDC->SetBkMode(OPAQUE);//设置文本背景模式
		pDC->SetBkColor(RGB(155,155,155));//设置文本背景
		hbr=CreateSolidBrush(RGB(155,155,155));//控件的背景色
	}
	if( IDC_STATIC3 ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	{
		pDC->SetTextColor(RGB(0,0,0));//设置文本颜色
		pDC->SetBkMode(OPAQUE);//设置文本背景模式
		pDC->SetBkColor(RGB(155,155,155));//设置文本背景
		hbr=CreateSolidBrush(RGB(155,155,155));//控件的背景色
	}
	if( IDC_STATIC6 ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	{
		pDC->SetTextColor(RGB(0,0,0));//设置文本颜色
		pDC->SetBkMode(OPAQUE);//设置文本背景模式
		pDC->SetBkColor(RGB(155,155,155));//设置文本背景
		hbr=CreateSolidBrush(RGB(155,155,155));//控件的背景色
	}
	if( IDC_STATIC7 ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	{
		pDC->SetTextColor(RGB(0,0,0));//设置文本颜色
		pDC->SetBkMode(OPAQUE);//设置文本背景模式
		pDC->SetBkColor(RGB(155,155,155));//设置文本背景
		hbr=CreateSolidBrush(RGB(155,155,155));//控件的背景色
	}
	if( IDC_STATIC8 ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	{
		pDC->SetTextColor(RGB(0,0,0));//设置文本颜色
		pDC->SetBkMode(OPAQUE);//设置文本背景模式
		pDC->SetBkColor(RGB(155,155,155));//设置文本背景
		hbr=CreateSolidBrush(RGB(155,155,155));//控件的背景色
	}
	if( IDC_STATIC10 ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	{
		pDC->SetTextColor(RGB(0,0,0));//设置文本颜色
		pDC->SetBkMode(OPAQUE);//设置文本背景模式
		pDC->SetBkColor(RGB(155,155,155));//设置文本背景
		hbr=CreateSolidBrush(RGB(155,155,155));//控件的背景色
	}
	if( IDC_STATIC11 ==  pWnd->GetDlgCtrlID())//判断发出消息的空间是否是该静态文本框
	{
		pDC->SetTextColor(RGB(0,0,0));//设置文本颜色
		pDC->SetBkMode(OPAQUE);//设置文本背景模式
		pDC->SetBkColor(RGB(155,155,155));//设置文本背景
		hbr=CreateSolidBrush(RGB(155,155,155));//控件的背景色
	}
	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}









void CPOP_testDlg::closeCam()
{
	//尝试关闭摄像头
	closeCam_Flag = true;
	Sleep(100);
	cameraDS.CloseCamera();
}

DWORD WINAPI CaptureThread(LPVOID lpParameter)
{
	CPOP_testDlg* pDlg = (CPOP_testDlg*)lpParameter;
	while (true)
	{
		if (pDlg->closeCam_Flag)//退出循环
			break;
		//从directX中获得当前图像并显示出来
		pDlg->queryframe = pDlg->cameraDS.QueryFrame();
		if (pDlg->b_takePic)
		{
			pDlg->img_input = cvarrToMat(pDlg->queryframe);
			//pDlg->img_input = imread("test12.bmp");
			imwrite("test13.bmp",pDlg->img_input);
			pDlg->b_takePic = false;
		}
		//pDlg->showImage(pDlg->camframe, IDC_camera);
		pDlg->showImage_Ipl(pDlg->queryframe,IDC_camera);
	}
	return 0;
}

void CPOP_testDlg::OnBnClickedopencam()
{
	if (CamCount >= 1)//开视频捕获线程
	{
		HANDLE hThread = NULL;
		DWORD dwThreadID = 0;
		closeCam();//首先关闭现有摄像头
		bool bret = cameraDS.OpenCamera(CamCount, false, 1920, 1080); //尝试打开摄像头
		if (bret)
		{
			closeCam_Flag = false;
			hThread = CreateThread(NULL, 0, CaptureThread, this, 0, &dwThreadID);
		}
	}
	else
	{
		AfxMessageBox(_T("当前没有连接摄像头"));
	}
}




void CPOP_testDlg::ChangeSize(CWnd *pWnd, int cx, int cy)
{
	if(pWnd)  //判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建	
	{
		CRect rect;   //获取控件变化前的大小  
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标

		//    cx/m_rect.Width()为对话框在横向的变化比例
		rect.left=rect.left*cx/m_rect.Width();//调整控件大小
		rect.right=rect.right*cx/m_rect.Width();
		rect.top=rect.top*cy/m_rect.Height();
		rect.bottom=rect.bottom*cy/m_rect.Height();
		pWnd->MoveWindow(rect);//设置控件大小
	}
}


void CPOP_testDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(nType==1) return;//最小化则什么都不做
	// TODO: Add your message handler code here
	CWnd *pWnd; 
	pWnd = GetDlgItem(IDC_STATIC12);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC11);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC10);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC9);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC8);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC7);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC6);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC5);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC4);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC3);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC2);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC1);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC0);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_camera);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_screen);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_cas);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_gear);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_SLAT);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_TRIM);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_casrow);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_gear1);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_gear2);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_gear3);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_gear11);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_gear21);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_gear31);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_SLATOUT);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT1);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT2);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT3);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT4);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT5);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT6);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT7);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT8);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT9);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT10);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT11);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT12);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT13);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT14);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT15);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT16);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT17);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT18);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT19);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_EDIT20);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC20);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC21);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC22);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC23);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC24);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC25);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC26);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC27);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC28);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC29);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC30);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC31);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC33);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC34);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC35);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC36);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC37);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC38);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC39);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_STATIC40);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_openCam);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDC_start);
	ChangeSize(pWnd, cx, cy);
	pWnd = GetDlgItem(IDOK);
	ChangeSize(pWnd, cx, cy);
	GetClientRect(&m_rect);// 将变化后的对话框大小设为旧大小
	Invalidate();
	UpdateWindow();
	// TODO: Add your message handler code here
}
