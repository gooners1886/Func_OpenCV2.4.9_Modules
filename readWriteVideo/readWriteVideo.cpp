
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  

using namespace std;  
using namespace cv;  



// 总结：
/*
视频写入失败的原因：
1. frame和写入视频size大小不匹配
2. 写入视频对应的fcc设置错误
*/


int main()  
{  
	//打开视频文件：其实就是建立一个VideoCapture结构  
	std::string strSrcVideoPath = "../SourceVideoFolder/《傲慢与偏见》 东植热舞天台浪漫亲吻[高清] 00_01_34-00_01_59.mp4";
	VideoCapture capture( strSrcVideoPath.c_str() ); 

	//检测是否正常打开:成功打开时，isOpened返回ture  
	if(!capture.isOpened())
	{
		cout<<"fail to open video "<<strSrcVideoPath<<" !"<<endl;
		return 1;
	}

	//获取整个帧数  
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);  
	cout<<"整个视频共"<<totalFrameNumber<<"帧"<<endl;  

	//设置开始帧()  
	long frameToStart = 620;  
	capture.set( CV_CAP_PROP_POS_FRAMES,frameToStart);  
	cout<<"从第"<<frameToStart<<"帧开始读"<<endl;  

	//设置结束帧  
	int frameToStop = totalFrameNumber-1;  

	if(frameToStop < frameToStart)  
	{  
		cout<<"结束帧小于开始帧，程序错误，即将退出！"<<endl;  
		return -1;  
	}  
	else  
	{  
		cout<<"结束帧为：第"<<frameToStop<<"帧"<<endl;  
	}  

	//获取帧率  
	double rate = capture.get(CV_CAP_PROP_FPS);  
	cout<<"帧率为:"<<rate<<endl;  

	//定义一个用来控制读取视频循环结束的变量  
	bool stop = false;  
	//承载每一帧的图像  
	Mat frame;  
	//显示每一帧的窗口  
	namedWindow("frame");  
	//两帧间的间隔时间:  
	int delay = 1000/rate;  

	//利用while循环读取帧  
	//currentFrame是在循环体中控制读取到指定的帧后循环结束的变量  
	long currentFrame = frameToStart;  


	/** 打开输出视频文件 */
	cv::VideoWriter writer;
	int fcc = CV_FOURCC('D','I','V','3'); 
	writer.open( "output.avi", // 输出视频文件名
		fcc,/*(int)capture.get( CV_CAP_PROP_FOURCC ), */// 也可设为CV_FOURCC_PROMPT，在运行时选取
		(double)capture.get( CV_CAP_PROP_FPS ), // 视频帧率
		cv::Size( (int)capture.get( CV_CAP_PROP_FRAME_WIDTH ), (int)capture.get( CV_CAP_PROP_FRAME_HEIGHT ) ), // 视频大小
		true ); // 是否输出彩色视频

	if ( !writer.isOpened() )
	{
		std::cout<<"writer is not open!!!"<<std::endl;
	}

	while(!stop)  
	{
		//waitKey(int delay=0)当delay ≤ 0时会永远等待；当delay>0时会等待delay毫秒  
		//当时间结束前没有按键按下时，返回值为-1；否则返回按键  
		int c = waitKey(delay);  
		
		//按下ESC或者到达指定的结束帧后退出读取视频  
		if((char) c == 27 || currentFrame >= frameToStop)
		{  
			stop = true;  
		}  
		//按下按键后会停留在当前帧，等待下一次按键  
		if( c >= 0)  
		{  
			waitKey(0);  
		}

		//读取下一帧  
		if(!capture.read(frame))  
		{  
			cout<<"读取视频失败"<<endl;  
			return -1;    
		}
		//capture >> frame;


		//这里加滤波程序  
		imshow("frame",frame);  

		cout<<"正在读取第"<<currentFrame<<"帧"<<endl;  
		
		currentFrame++;  

		/** 将视频写入文件 */
		//vw << frame;
		writer.write(frame);

	} 
	cout<<"视频结束！"<<endl;  
	//关闭视频文件  
	capture.release();  
	waitKey(0);  
	return 0;  
}  
