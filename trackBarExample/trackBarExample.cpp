#include <cv.h>
#include <highgui.h>
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  

using namespace std;  
using namespace cv;  


/// ȫ�ֱ������������ʼ��
const int alpha_slider_max = 100;
int alpha_slider;
double alpha;
double beta;

/// �����洢ͼ��ı���
Mat src1;
Mat src2;
Mat dst;

/**
* @function on_trackbar
* @������Ӧ�������Ļص�����
*/
void on_trackbar( int, void* )
{
	alpha = (double) alpha_slider/alpha_slider_max ;
	beta = ( 1.0 - alpha );

	addWeighted( src1, alpha, src2, beta, 0.0, dst);

	imshow( "Linear Blend", dst );

	return;
}

int main( int argc, char** argv )
{
	/// ����ͼ�� (��ͼ��Ĵ�С������Ҫ��ͬ)
	src1 = imread("./project_src_image/LinuxLogo.jpg");
	src2 = imread("./project_src_image/WindowsLogo.jpg"); 

	if( !src1.data ) 
	{
		printf("Error loading src \n"); 
		return -1;
	}

	if( !src2.data )
	{
		printf("Error loading src2 \n");
		return -1;
	}

	/// ��ʼ��Ϊ��
	alpha_slider = 0;

	/// ��������
	namedWindow("Linear Blend", 1);

	/// �ڴ����Ĵ����д���һ���������ؼ�
	char TrackbarName[50];
	sprintf( TrackbarName, "Alpha x %d", alpha_slider_max );

	createTrackbar( TrackbarName, "Linear Blend", &alpha_slider, alpha_slider_max, on_trackbar );

	/// ����ڻص���������ʾ
	on_trackbar( alpha_slider, 0 );

	/// ��������˳�
	waitKey(0);
	return 0;
}