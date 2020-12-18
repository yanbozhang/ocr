
// POP_testDlg.h : header file
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include "CvvImage.h"
#include "CameraDS.h"
#include "afxcmn.h"
#include "afxwin.h"
#include <tchar.h>
#include <fstream> 
#include <string>
#include <math.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib") 
#pragma comment(lib, "libtesseract302.lib")
#pragma comment(lib, "liblept168.lib")
#include "tesseract\baseapi.h"  
#include "tesseract\strngs.h" 
using namespace cv;
#pragma once


Mat gammaTransform(Mat &srcImage, float kFactor);//伽马变换图像增强
Rect detecMark(Mat &srcImage);//检测标志点，返回屏幕位置
int bSums(Mat src);//颜色信息统计
// CPOP_testDlg dialog
class CPOP_testDlg : public CDialogEx
{
// Construction
public:
	CPOP_testDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_POP_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void showImage(Mat& src, UINT ID);
	void showImage_Ipl(IplImage* image,int UID);
	void clearEdit();
	afx_msg void OnBnClickedOk();
	Mat img_input,img_resize,img_enhance,img_gray,img_binary,img_erode,img_fliter,img_canny;
	Mat img_screen,img_cas,img_gear,img_slat,img_trim;
	int screen_width,screen_height,cas_width,cas_height,gear_width,gear_height,slat_width,slat_height,trim_width,trim_height;
	Mat img_cas_hsv,img_cas_v,img_cas_v_enhance,img_cas_dilate,img_cas_dilate_binary;
	Mat img_gear_enhance,img_gear_gray,img_gear_binary,img_gear_dilate;
	Mat img_gear1,img_gear2,img_gear3,img_gear10,img_gear20,img_gear30,img_gear11,img_gear21,img_gear31,img_jinfeng0,img_jinfeng;
	vector<cv::Mat> img_cas_hsv_vec;
	vector<vector<Point> > contours_cas;
	vector<vector<Point> > contours_gear;
	int cas_line;
	const char * cas_image;
	tesseract::TessBaseAPI  api; 
	//STRING text_out,cas_color_ID; 
	CString cas_ocr,cas_color; 
	CFont cfont; 
	double temBest;
	double similarityValue;
	Mat compareSameByHist(Mat img);
	afx_msg void OnBnClickedstart();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void ocrshibie();
	Mat getsimilarity(Mat img,int&);
	Mat getjinfeng(Mat img,int&);
	afx_msg void OnBnClickedopencam();
	int CamCount;//摄像头数量
	void closeCam();//关闭摄像头函数
	bool closeCam_Flag;//关闭摄像头标志位
	CCameraDS cameraDS;//摄像头操作类
	IplImage* queryframe;//当前帧
	void cas_recognition(Mat img,int &,int &,int &,string &,string &);
	void gear_recognition(Mat img,int &,int &,int &,string &);
	void slat_recognition(Mat img,int &,string &);
	int info_count,all_page,current_page,gear1,gear2,gear3,jinfeng;
	string img_name,ocr_out;
	Rect rect,rect_cas,rect_gear,rect_slat,rect_trim;
	CRect m_rect;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void CPOP_testDlg::ChangeSize(CWnd *pWnd, int cx, int cy);
	bool b_takePic;
	Mat img_input_tmp;
};
