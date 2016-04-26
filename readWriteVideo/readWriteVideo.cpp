
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  

using namespace std;  
using namespace cv;  


int main()  
{  
	//����Ƶ�ļ�����ʵ���ǽ���һ��VideoCapture�ṹ  
	std::string strSrcVideoPatch = "../SourceVideoFolder/zhubo.mp4";
	VideoCapture capture( strSrcVideoPatch.c_str() ); 

	//����Ƿ�������:�ɹ���ʱ��isOpened����ture  
	if(!capture.isOpened())
	{
		cout<<"fail to open video "<<strSrcVideoPatch<<" !"<<endl;
		return 1;
	}

	//��ȡ����֡��  
	long totalFrameNumber = capture.get(CV_CAP_PROP_FRAME_COUNT);  
	cout<<"������Ƶ��"<<totalFrameNumber<<"֡"<<endl;  

	//���ÿ�ʼ֡()  
	long frameToStart = 0;  
	capture.set( CV_CAP_PROP_POS_FRAMES,frameToStart);  
	cout<<"�ӵ�"<<frameToStart<<"֡��ʼ��"<<endl;  

	//���ý���֡  
	int frameToStop = 20;  

	if(frameToStop < frameToStart)  
	{  
		cout<<"����֡С�ڿ�ʼ֡��������󣬼����˳���"<<endl;  
		return -1;  
	}  
	else  
	{  
		cout<<"����֡Ϊ����"<<frameToStop<<"֡"<<endl;  
	}  

	//��ȡ֡��  
	double rate = capture.get(CV_CAP_PROP_FPS);  
	cout<<"֡��Ϊ:"<<rate<<endl;  

	//����һ���������ƶ�ȡ��Ƶѭ�������ı���  
	bool stop = false;  
	//����ÿһ֡��ͼ��  
	Mat frame;  
	//��ʾÿһ֡�Ĵ���  
	namedWindow("frame");  
	//��֡��ļ��ʱ��:  
	int delay = 1000/rate;  

	//����whileѭ����ȡ֡  
	//currentFrame����ѭ�����п��ƶ�ȡ��ָ����֡��ѭ�������ı���  
	long currentFrame = frameToStart;  


	/** �������Ƶ�ļ� */
	cv::VideoWriter writer;
	int fcc = CV_FOURCC('D','I','V','3'); 
	writer.open( "output.avi", // �����Ƶ�ļ���
		fcc,/*(int)capture.get( CV_CAP_PROP_FOURCC ), */// Ҳ����ΪCV_FOURCC_PROMPT��������ʱѡȡ
		(double)capture.get( CV_CAP_PROP_FPS ), // ��Ƶ֡��
		cv::Size( (int)capture.get( CV_CAP_PROP_FRAME_WIDTH ), (int)capture.get( CV_CAP_PROP_FRAME_HEIGHT ) ), // ��Ƶ��С
		true ); // �Ƿ������ɫ��Ƶ

	if ( !writer.isOpened() )
	{
		std::cout<<"writer is not open!!!"<<std::endl;
	}

	while(!stop)  
	{  
		//��ȡ��һ֡  
		if(!capture.read(frame))  
		{  
			cout<<"��ȡ��Ƶʧ��"<<endl;  
			return -1;    
		}
		//capture >> frame;


		//������˲�����  
		imshow("frame",frame);  

		cout<<"���ڶ�ȡ��"<<currentFrame<<"֡"<<endl;  
		//waitKey(int delay=0)��delay �� 0ʱ����Զ�ȴ�����delay>0ʱ��ȴ�delay����  
		//��ʱ�����ǰû�а�������ʱ������ֵΪ-1�����򷵻ذ���  


		int c = waitKey(delay);  
		//����ESC���ߵ���ָ���Ľ���֡���˳���ȡ��Ƶ  
		if((char) c == 27 || currentFrame > frameToStop)
		{  
			stop = true;  
		}  
		//���°������ͣ���ڵ�ǰ֡���ȴ���һ�ΰ���  
		if( c >= 0)  
		{  
			waitKey(0);  
		}  
		currentFrame++;  

		/** ����Ƶд���ļ� */
		//vw << frame;
		writer.write(frame);

	}  
	//�ر���Ƶ�ļ�  
	capture.release();  
	waitKey(0);  
	return 0;  
}  
