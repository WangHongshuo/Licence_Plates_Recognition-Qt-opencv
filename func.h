#ifndef FUNC_H
#define FUNC_H

#include <QString>
#include <QImage>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
//调整图像大小
void size300(Mat &src);
//取反
Mat reverse_pic(Mat src);
//二值化
void binary(Mat &src);
//消除边框影响
Mat add_frame(Mat src);
//去除小面积（输入：原图，出去小面积参数）
vector<vector<Point> > bwareaopen(Mat &src, int n);
//求垂直投影
Mat V_Shadwo(Mat &src,int a = 0,string str = "垂直投影");
//求水平投影
Mat H_Shadwo(Mat &src,int a = 0,string str = "水平投影");
//水平裁剪H1(输入：垂直投影Mat，原图像；输出水平裁剪第一个y坐标)
int H1_cut_value(Mat &H,Mat &input);
//水平裁剪H2(输入：垂直投影Mat，原图像；输出水平裁剪第二个y坐标)
int H2_cut_value(Mat &H,Mat &input);
//垂直裁剪W1
int W1_cut_value(Mat &V);
//垂直裁剪W2
int W2_cut_value(Mat &V);
//字符分割
vector<int> seg_character(Mat &V,Mat &input);
//强制分割
vector<int> force_seg(Mat &V,vector<int> W);
//字符识别
vector<QString> recognition(vector<Mat> &character);
//字符去黑边(垂直裁剪)
void character_cut_H(Mat &src,int k);
//字符去黑边(水平裁剪)
void character_cut_V(Mat &src,int k);
//QImage与Mat转换
QImage cvMat2QImage(const cv::Mat& mat);

#endif // FUNC_H
