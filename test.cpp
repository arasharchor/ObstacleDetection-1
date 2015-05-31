/*
 * ������ɫ��Ϣ���ϰ����⡣
 */
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "Queue.h"
using namespace cv;
using namespace std;


static int NeighborDirection[8][2] = {{0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1}};

//��������ǵ�ǰ���ص�8����img_binaryԭͼ��img_label�������ͼ
//Reference http://blog.stevenwang.name/connected-component-labeling-rg-545001.html
void SearchNeighbor8(Mat img_binary, Mat img_label,
		int label_index, int pixel_index, Queue* queue)
{
	img_label.data[pixel_index] = label_index;
	for(int i = 0; i < 8; i++)
	{
		int search_index = pixel_index + NeighborDirection[i][0] * img_binary.cols + NeighborDirection[i][1];
		if(search_index > 0 && search_index < img_binary.cols * img_binary.rows
						&& img_binary.data[search_index] == 0 && img_label.data[search_index] == 0)
		{
			img_label.data[search_index] = label_index;
			queue->EnQueue(search_index);
		}
	}
}

//����label_index������img_label��ʼȫΪ0����Ϊ1,2,...,label_index������
int ConnectLabelingComponent(Mat img_binary, Mat img_label)
{
	int label_index = 0;
	for(int i = 0; i < img_binary.rows; i++)
		for(int j = 0; j < img_binary.cols; j++)
		{
			Queue* queue = new Queue();

			int pixel_index = i * img_binary.cols + j;
			if(img_binary.data[pixel_index] == 0 && img_label.data[pixel_index] == 0)
			{
				label_index++;
				printf("%d\n", label_index);
				SearchNeighbor8(img_binary, img_label, label_index, pixel_index, queue);
				int pop_index = queue->DeQueue();
				//printf("%d\n", pop_index);
				while(pop_index > -1)
				{
					SearchNeighbor8(img_binary, img_label, label_index, pop_index, queue);
					pop_index = queue->DeQueue();
				}
			}
			delete(queue);
		}
	return label_index;
}

//���ݱ��ͼ��ԭͼ��Ŀ��������Ϊ��ɫ
void Labeling(Mat img_label, Mat img_rgb)
{
	for(int i = 0; i < img_label.rows; i++)
		for(int j = 0; j < img_label.cols; j++)
		{
			int pixel_index = i * img_label.cols + j;
			if(!(img_label.data[pixel_index] == 0))
			{
				img_rgb.at<cv::Vec3b>(i,j)[0] = 255;//b
				img_rgb.at<cv::Vec3b>(i,j)[1] = 0;//g
				img_rgb.at<cv::Vec3b>(i,j)[2] = 0;//r
			}
		}
}

//���ݱ��ͼ���������ԭͼ��Ŀ�����򻭺�ɫ��
void FindRange(Mat img_label, int num_label, Mat img_rgb)
{
	//int num_label = 4;
	int array_range[num_label+1][4]; //�����������ı߽�ֵ,�ֱ�Ϊ��������
	for(int i = 1; i <= num_label; i++)//��ʼ��
	{
		array_range[i][0] = img_label.cols;
		array_range[i][1] = 0;
		array_range[i][2] = img_label.rows;
		array_range[i][3] = 0;
		//printf("Initialation: %d,%d,%d,%d\n", array_range[i][0],array_range[i][1],array_range[i][2],array_range[i][3]);
	}
	//printf("Initialize done!\n");
	//Ѱ����������߽�
	for(int i = 0; i < img_label.rows; i++)
		for(int j = 0; j < img_label.cols; j++)
		{
			int pixel_index = i * img_label.cols + j;
			int label = img_label.data[pixel_index];
			if(j < array_range[label][0])
				array_range[label][0] = j;
			if(j > array_range[label][1])
				array_range[label][1] = j;
			if(i < array_range[label][2])
				array_range[label][2] = i;
			if(i > array_range[label][3])
				array_range[label][3] = i;
		}

	//���ݱ߽续��
	int mask = 4;//��������Χ
	for(int j = 1; j <= num_label; j++)
	{
		// ��΢��������
		if(array_range[j][0] >= mask)
			array_range[j][0] -= mask;
		if(array_range[j][1] < img_label.cols - mask)
			array_range[j][1] += mask;
		if(array_range[j][2] >= mask)
			array_range[j][2] -= mask;
		if(array_range[j][3] < img_label.rows - mask)
			array_range[j][3] += mask;

		for(int i = array_range[j][0]; i <= array_range[j][1]; i++)
		{
			//up
			//img_rgb.data[img_rgb.channels()*(img_rgb.cols * i + j) + 0]
			img_rgb.at<cv::Vec3b>(array_range[j][2], i)[0] = 0;//B
			img_rgb.at<cv::Vec3b>(array_range[j][2], i)[1] = 255;//g
			img_rgb.at<cv::Vec3b>(array_range[j][2], i)[2] = 0;//r
			//down
			img_rgb.at<cv::Vec3b>(array_range[j][3], i)[0] = 0;//B
			img_rgb.at<cv::Vec3b>(array_range[j][3], i)[1] = 255;//g
			img_rgb.at<cv::Vec3b>(array_range[j][3], i)[2] = 0;//r
		}

		for(int i = array_range[j][2]; i <= array_range[j][3]; i++)//up
		{
			//left
			img_rgb.at<cv::Vec3b>(i, array_range[j][0])[0] = 0;//B
			img_rgb.at<cv::Vec3b>(i, array_range[j][0])[1] = 255;//g
			img_rgb.at<cv::Vec3b>(i, array_range[j][0])[2] = 0;//r
			//right
			img_rgb.at<cv::Vec3b>(i, array_range[j][1])[0] = 0;//B
			img_rgb.at<cv::Vec3b>(i, array_range[j][1])[1] = 255;//g
			img_rgb.at<cv::Vec3b>(i, array_range[j][1])[2] = 0;//r
		}
	}
}

//ͳ��h,s,v������Ϣ��ֵ|����
void HSVStatic(Mat img_hsv, int* table_h, int* table_s, int* table_v)
{
	int h,s,v;
	for(int i = 0; i < img_hsv.rows; i++)
		for(int j = 0; j < img_hsv.cols; j++)
		{
			h = int(img_hsv.at<Vec3b>(i,j)[0]);
			s = int(img_hsv.at<Vec3b>(i,j)[1]);
			v = int(img_hsv.at<Vec3b>(i,j)[2]);
			table_h[h]++;
			table_s[s]++;
			table_v[v]++;
		}
}

//����H,S,V���б������Ƿ����ڣ����棩�������ɶ�ֵͼ
void Detection(Mat img_hsv, Mat img_binary, int* table_h, int* table_s, int* table_v)
{
	int h,s,v;
	int threshold_h = 50;
	int threshold_v = 100;
	for(int i = 0; i < img_hsv.rows; i++)
		for(int j = 0; j < img_hsv.cols; j++)
		{
			int pixel_index = i * img_binary.cols + j;
			h = int(img_hsv.at<Vec3b>(i,j)[0]);
			s = int(img_hsv.at<Vec3b>(i,j)[1]);
			v = int(img_hsv.at<Vec3b>(i,j)[2]);
			//if((table_h[h] != 0) && (table_v[v] != 0)!(table_h[h] > threshold_h) && (table_v[v] > threshold_v))
			if((table_h[h] > threshold_h) || (table_v[v] > threshold_v))
			{
				//cout << pixel_index << " " << table_h[h] << " " << table_v[v] << endl;
				img_binary.data[pixel_index] = 255;
			}
		}
}

int main()
{
	//ͳ�Ƶ���
	int table_h[180] = {0};
	int table_s[255] = {0};
	int table_v[255] = {0};

	// ������ȡ����ͼƬ
	Mat img_rgb, img_hsv;
	char filename[100];
	for(int i = 1; i <= 5; i++)
	{
		sprintf(filename, "D:\\AppData\\MyProject\\Eclipse\\MyObstacleDetection\\images\\floor\\%d.jpg", i);
		img_rgb = imread(filename);
		printf("Read %d'th\n", i);
		cvtColor(img_rgb,img_hsv,CV_RGB2HSV);//h:0-180 s:0-255 v:0-255
		HSVStatic(img_hsv, table_h, table_s, table_v);
	}

	//ͳ�����ϰ����ͼ��
	int table_h1[180] = {0};
	int table_s1[255] = {0};
	int table_v1[255] = {0};
	//�������ϰ����ͼ��
	Mat img_in, img_input, img_hsv_input, img_binary;

	img_in = imread("../images/obstacle/2.jpg");
	//imshow("origin", img_binary);

	GaussianBlur(img_in, img_input, Size(5,5), 0.1, 0.1);
	cvtColor(img_input,img_hsv_input,CV_RGB2HSV);

	HSVStatic(img_hsv_input, table_h1, table_s1, table_v1);
	for(int i=0; i<180; i++)
	{
		if(table_h[i] != 0 || table_h1[i] != 0)
			cout << i << " " << table_h[i] << " "<< table_h1[i]<< endl;
	}
	for(int i=0; i<255; i++)
	{
		if(table_s[i] != 0 || table_s1[i] != 0)
			cout << i << " " << table_s[i] << " "<< table_s1[i]<< endl;
	}

	img_binary = Mat(img_input.size(),CV_8UC1, 1);//������ֵͼ��ȫ��
	Detection(img_hsv_input, img_binary, table_h, table_s, table_v);//����ͳ�����ݣ������ϰ����ֵͼ
	//imshow("binary1", img_binary);

	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(11, 11));
	dilate(img_binary, img_binary, kernel_dilate);//����
	imshow("binary2", img_binary);
	erode(img_binary, img_binary, kernel_erode);//��ʴ
	imshow("binary3", img_binary);

	img_binary = img_binary > 128;
	//imshow("binary4", img_binary);

	Mat img_label = Mat::zeros(img_binary.size(),CV_8UC1);//�������ͼ
	int num_label = ConnectLabelingComponent(img_binary, img_label);// Ѱ����ͨ����
	cout << "There are " << num_label << "obstacles." << endl;//��ӡ��ͨ��������

	FindRange(img_label, num_label, img_in);//��ɫ���ס��ͨ����


	imshow("Result", img_in);
	waitKey(0);
	return 0;
}
