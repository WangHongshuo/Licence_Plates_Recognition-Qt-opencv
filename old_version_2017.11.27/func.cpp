#include <QString>
#include <QImage>
#include <QDir>
#include <QDebug>
#include <QCoreApplication>
#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "func.h"

using namespace cv;
using namespace std;


void size300(Mat &src)
{
    int w;
    int h;
    double k = double(src.rows) / double(src.cols);
    w = 300;
    h = w * k;
    resize(src,src, Size(w, h), 3);
}

Mat reverse_pic(Mat src)
{
    Mat dst = src<100;
    return dst;
}

void binary(Mat &src)
{
    //自适应阈值
//    adaptiveThreshold(src,src,255,ADAPTIVE_THRESH_MEAN_C,THRESH_BINARY,31,ADAPTIVE_THRESH_MEAN_C);
    //最值
    double min, max;
    double* minp = &min;
    double* maxp = &max;
    Mat temp = src(Rect(round(src.cols*0.2),round(src.rows*0.2),round(src.cols*0.6),round(src.rows*0.6)));
    minMaxIdx(temp, minp, maxp);
    //cout << "min = " << min << " max = " <<max << endl;
    int T = round((max - (max - min) / 2)* 0.92);
    //cout << T;
    threshold(src, src, T, 255, THRESH_BINARY);
    //平均值
//    Mat temp = src(Rect(round(src.cols*0.1),round(src.rows*0.1),round(src.cols*0.8),round(src.rows*0.8)));
//    Scalar avg = mean(temp);
//    threshold(src,src,round(avg.val[0]),255,THRESH_BINARY);
}

Mat add_frame(Mat src)
{
    //画黑边框消除寻找边界造成的覆盖区域不全
    Point P1, P2;
    P1.x = 0; P1.y = 0;
    P2.x = src.cols - 1; P2.y = src.rows - 1;
    rectangle(src, P1, P2, 0, 1);
    return src;
}

void bwareaopen(Mat &data, int n)
{
    Mat labels,stats,centroids;
    connectedComponentsWithStats(data,labels,stats,centroids,8,CV_16U);
    int regions_count = stats.rows - 1;
    int regions_size, regions_x1, regions_y1, regions_x2, regions_y2;

    for(int i=1;i<=regions_count;i++)
    {
        regions_size = stats.ptr<int>(i)[4];
        if(regions_size < n)
        {
            regions_x1 = stats.ptr<int>(i)[0];
            regions_y1 = stats.ptr<int>(i)[1];
            regions_x2 = regions_x1 + stats.ptr<int>(i)[2];
            regions_y2 = regions_y1 + stats.ptr<int>(i)[3];

            for(int j=regions_y1;j<regions_y2;j++)
            {
                for(int k=regions_x1;k<regions_x2;k++)
                {
                    if(labels.ptr<ushort>(j)[k] == i)
                        data.ptr<uchar>(j)[k] = 0;
                }
            }
        }
    }
}

Mat V_Shadwo(Mat &src,int a,string str)
{
    int *colheight = new int[src.cols];//列
    memset(colheight, 0, src.cols * 4);//每个占4字节
    int value;
    for (int x = 0; x < src.cols; x++)
    {
        for (int y = 0; y < src.rows; y++)
        {
            value = src.at<uchar>(y, x);
            if (value == 255)
            {
                colheight[x]++;
            }
        }
    }
    //格式为Mat name(行，列，数据类型)
    Mat histogramImage(src.rows, src.cols, CV_8UC1);
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
         //图像全设置为黑色。
            histogramImage.at<uchar>(y, x) = 0;
        }
    }
    for (int x = 0;x < src.cols; x++)
    {
        for (int y = 0; y < colheight[x]; y++)
        {
            //设置为白色
            histogramImage.at<uchar>(y, x) = 255;
        }
    }
    flip(histogramImage, histogramImage, 0);
    if (a != 0)
    {
        namedWindow(str, WINDOW_NORMAL);
        imshow(str, histogramImage);
    }
    //返回col高度数组Mat
    Mat h(1, src.cols, CV_16UC1);
    for (int i = 0; i < src.cols; i++)
        h.at<ushort>(0, i) = colheight[i];
    delete[] colheight;
    return h;
}

Mat H_Shadwo(Mat &src,int a,string str)
{
    int *rowheight = new int[src.rows];//列
    memset(rowheight, 0, src.rows * 4);//每个占4字节
    int value;
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            value = src.at<uchar>(y, x);
            if (value == 255)
            {
                rowheight[y]++;
            }
        }
        //cout << colheight[y] << " ";
    }
    //格式为Mat name(行，列，数据类型)
    Mat histogramImage(src.rows, src.cols, CV_8UC1);
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < src.cols; x++)
        {
            //图像全设置为黑色。
            histogramImage.at<uchar>(y, x) = 0;
        }
    }
    for (int y = 0; y < src.rows; y++)
    {
        for (int x = 0; x < rowheight[y]; x++)
        {
            //设置为白色
            histogramImage.at<uchar>(y, x) = 255;
        }
    }
    flip(histogramImage, histogramImage, 1);
    if(a != 0)
    {

        namedWindow(str, WINDOW_NORMAL);
        imshow(str, histogramImage);
    }
    //返回row宽度Mat
    Mat r(1, src.rows, CV_16UC1);
    for (int i = 0; i < src.rows; i++)
        r.at<ushort>(0, i) = rowheight[i];
    delete[] rowheight;
    return r;
}

int H1_cut_value(Mat &H,Mat &input)
{
    int H1;
    int n = H.cols;
    for (int i = round(n*0.25); i > 0; i--)
    {
        int t1 = H.at<ushort>(0, i);
        int t2 = H.at<ushort>(0, i - 1);
        if ((t1 > round(input.cols*0.14) && t2 <= round(input.cols*0.14)) || t1 > round(input.cols*0.75))
        {
            H1 = i;
            return H1;
        }
        else if (i == 2)
        {
            H1 = 1;
            return H1;
        }
    }
    return 0;
}

int H2_cut_value(Mat &H, Mat &input)
{
    int H2;
    int n = H.cols;
    for (int i = round(n*0.7); i <= n; i++)
    {
        int t1 = H.at<ushort>(0, i);
        int t2 = H.at<ushort>(0, i - 1);
        if ((t1 <= round(input.cols*0.16) && t2 > round(input.cols*0.16)) || t1 > round(input.cols*0.75))
        {
            H2 = i - 1;
            return H2;
        }
        else if (i ==n)
        {
            H2 = n;
            return H2;
        }
    }
    return n;
}

int W1_cut_value(Mat &V)
{
    volatile int W[2] = { 0 };
    volatile int edge_count = 2;
    volatile int n = V.cols;
    for (int i = round(n*0.2); i > 0; i--)
    {
        int t1 = V.at<ushort>(0, i);
        int t2 = V.at<ushort>(0, i + 1);
        if ((edge_count == 2) && (t1 > 2 && t2 <= 2))
        {
            W[edge_count - 1] = i + 1;
            edge_count -= 1;
        }
        else if (edge_count == 1 && t1 <= 2 && t2 > 2)
        {
            W[edge_count - 1] = i + 1;
            edge_count -= 1;
        }
        else if (edge_count == 0 && W[1] - W[0] >= n*0.1)
            break;
        else if (edge_count == 0 && W[1] - W[0] < n*0.1)
            edge_count += 1;
        else if (i == 1 && edge_count == 1)
            W[0] = 0;
    }
    return W[0];
}

int W2_cut_value(Mat &V)
{
    volatile int n = V.cols;
    for (int i = round(n*0.9); i < n - 1; i++)
    {
        int t1 = V.at<ushort>(0, i);
        int t2 = V.at<ushort>(0, i + 1);
        if (t1 > 4 && t2 <= 4)
            return i;
    }
    return V.cols;
}

vector<int> seg_character(Mat &V,Mat &input)
{
    //W[14]作为分割是否出错判定位，0为没错
    vector<int> W(15,0);
    W[0] = 0;
    W[13] = V.cols;
    volatile int g = 13;
    volatile int n = V.cols;
    for (int i = V.cols - 2; i > -1; i--)
    {
        int t1 = V.at<ushort>(0, i);
        int t2 = V.at<ushort>(0, i + 1);
        if ((t1 <= 3 && t2 > 3) || (t1 > 3 && t2 <= 3))
        {
            W[g - 1] = i;
            if (g % 2 == 0)
            {
                W[g - 1] += 1;
            }
            g -= 1;
            if (g == 1 && W[3] - W[2] < round(n*0.09))
            {
                g += 2;
            }
            if (g < 13 && g > 2 && g % 2 == 1 && W[g + 2] - W[g + 1] < round(n*0.03))
            {
                //Mat temp = input(Rect(W[i * 2], 0, W[i * 2 + 1] - W[i * 2], input.rows));
                int sum_point = countNonZero(input(Rect(W[g + 1], 0, W[g + 2] - W[g + 1], input.rows)));
                double k = double(sum_point) / double(input.rows) / double(W[g + 2] - W[g + 1]);
                if (k < 0.75)
                {
                    W[g + 1] = 0;
                    W[g] = 0;
                    g += 2;
                }
            }
            //判定汉字宽度
            if(g == 0 && W[1] - W[0] < round(n*0.09))
            {
                g += 1;
            }
            if (g > 1 && i == 0)
            {
                W[14] = 1;
                break;
            }
            if (g == 0)
                break;
        }
        else if(i == 0 && g == 1)
        {
            W[0] = 0;
        }
    }
    return W;
}

vector<int> force_seg(Mat &V,vector<int> W)
{
    volatile int n = V.cols;
    W[0] = W[14] = 0;
    W[1] = W[0] + round(n*0.11);
    W[2] = W[1] + round(n*0.02);
    W[3] = W[2] + round(n*0.11);
    W[4] = W[3] + round(n*0.08);
    W[13] = n;
    for (int i = 2; i < 6; i++)
    {
        W[i * 2 + 1] = W[i * 2] + round(n*0.135);
        W[i * 2 + 2] = W[i * 2 + 1];
    }
    return W;
}

vector<QString> recognition(vector<Mat> &character)
{
//    qDebug()<<"current applicationDirPath: "<< QCoreApplication::applicationDirPath();
//    qDebug()<<"current currentPath: "<<QDir::currentPath();
    //读入模版图片
    //数字和字母
    vector<Mat> NumberLetter(36);
    int i = 0;
    while (i < 36)
    {
        QString path;
        path = QCoreApplication::applicationDirPath();
        QString Q_ImgName;
        string ImgName;
        Q_ImgName = path + "/NumberLetter/" + QString::number(i) + ".bmp";
        ImgName = Q_ImgName.toStdString();
        NumberLetter[i] = imread(ImgName,0);
        i++;
    }
    //姹夊瓧
    vector<Mat> ChineseCharacter(39);
    i = 1;
    while (i < 39)
    {
        QString path;
        path = QCoreApplication::applicationDirPath();
        QString Q_ImgName;
        string ImgName;
        Q_ImgName = path + "/ChineseCharacter/" + QString::number(i) + ".bmp";
        ImgName = Q_ImgName.toStdString();
        ChineseCharacter[i] = imread(ImgName,0);
        i++;
    }
    //汉字
    vector<QString> str(37);
    str = { "0","1","2","3","4","5","6","7","8","9","A","B","C","D","E","F","G","H","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","7","O" };
    vector<QString> cn(40);
    cn = {" ","京","津","沪","渝","冀","豫","云","辽","黑","湘","皖","鲁","苏","赣","浙","粤","鄂","桂","甘","晋","蒙","陕","吉","闽","贵","青","藏","川","宁","新","琼","云","渝","沪","冀","京","云","藏","*" };
    //字符库
    vector<QString> ans(7);
    //识别字符
    for (i = 0; i < 7; i++)
    {
        if (i == 0)
        {
            //cn[0]和ChineseCharacter[0]为空，脚标+1
            character_cut_V(character[i],i);
            character_cut_H(character[i],i);
            resize(character[i], character[i], Size(48, 84), 3);
            //threshold(character[i], character[i], 90, 255, THRESH_BINARY);
            binary(character[i]);
            Mat result = Mat(1, 1, CV_32FC1);
            vector<float> result_value(38);
            for (volatile int a = 0; a < 38; a++)
            {
                matchTemplate(character[i], ChineseCharacter[a+1], result, TM_CCOEFF_NORMED);
                result_value[a] = result.at<float>(0, 0);
            }
            vector<float>::iterator biggest = max_element(begin(result_value), end(result_value));
            int index = distance(begin(result_value), biggest);
            ans[i] = cn[index + 1];
            continue;
        }
        else if (double(character[i].cols) / double(character[i].rows) < 0.3)
        {
            ans[i] = "1";
            continue;
        }
        else if(i == 1)
        {
            character_cut_V(character[i],i);
            character_cut_H(character[i],i);
            resize(character[i], character[i], Size(24, 42), 3);
            //threshold(character[i], character[i], 90, 255, THRESH_BINARY);
            binary(character[i]);
            Mat result = Mat(1, 1, CV_32FC1);
            vector<float> result_value(36,0.0);
            for (volatile int a = 10; a < 36; a++)
            {
                matchTemplate(character[i], NumberLetter[a], result, TM_CCORR_NORMED);
                result_value[a] = result.at<float>(0, 0);
            }
            vector<float>::iterator biggest = max_element(begin(result_value), end(result_value));
            int index = distance(begin(result_value), biggest);
            if(index == 13 || index == 23)
            {
                index = featureMatch(character[i],13);
                if(index == 0)
                    index = 23;
            }
            ans[i] = str[index];
            continue;
        }
        else
        {
            character_cut_V(character[i],i);
            character_cut_H(character[i],i);
            resize(character[i], character[i], Size(24, 42), 3);
            //threshold(character[i], character[i], 90, 255, THRESH_BINARY);
            binary(character[i]);
            Mat result = Mat(1, 1, CV_32FC1);
            vector<float> result_value(36);
            for (volatile int a = 0; a < 36; a++)
            {
                matchTemplate(character[i], NumberLetter[a], result, TM_CCORR_NORMED);
                result_value[a] = result.at<float>(0, 0);
            }
            vector<float>::iterator biggest = max_element(begin(result_value), end(result_value));
            int index = distance(begin(result_value), biggest);
            if(index == 0 || index == 13)
            {
                index = featureMatch(character[i],index);
            }
            ans[i] = str[index];
        }

    }
    return ans;
}

int featureMatch(Mat &src,int index)
{
    int n_index = 0;
    if(index == 0 || index == 13)
    {
        Mat temp = src(Rect(1,0,4,42));
        Mat V = V_Shadwo(temp);
        double min,max;
        double* minp = &min;
        double* maxp = &max;
        minMaxIdx(V,minp,maxp);
        if(max <= 40.0)
            n_index = 0;
        else
            n_index =  13;
    }
    return n_index;
}

void character_cut_V(Mat &src,int k)
{
    volatile double left, right;
    if (k == 0)
    {
        left = 0.15;
        right = 0.85;
    }
    else
    {
        left = 0.4;
        right = 0.6;
    }
    Mat H = H_Shadwo(src);
    volatile int H1, H2;
    for (volatile int i = round(src.rows*left); i != -1; i--)
    {
        volatile int t1 = H.at<ushort>(0, i);
        volatile int t2 = H.at<ushort>(0, i + 1);
        if (t1 <= 3 && t2 > 3)
        {
            H1 = i + 1;
            break;
        }
        else if (i == 0)
        {
            H1 = 0;
        }
    }
    for (volatile int i = round(src.rows*right); i < src.rows - 1; i++)
    {
        volatile int t1 = H.at<ushort>(0, i);
        volatile int t2 = H.at<ushort>(0, i + 1);
        if (t1 >= 3 && t2 < 3)
        {
            H2 = i;
            break;
        }
        else if (i == src.rows - 2)
        {
            H2 = src.rows;
        }
    }
    src = src(Rect(0, H1,src.cols , H2 - H1));
}

void character_cut_H(Mat &src,int k)
{
    volatile double left, right;
    if (k == 0)
    {
        left = 0.2;
        right = 0.9;
    }
    else
    {
        left = 0.4;
        right = 0.6;
    }
    Mat V = V_Shadwo(src);
    volatile int W1, W2;
    for (volatile int i = round(src.cols*left); i != -1; i--)
    {
        int t1 = V.at<ushort>(0, i);
        int t2 = V.at<ushort>(0, i + 1);
        if (t1 <= 3 && t2 > 3)
        {
            W1 = i + 1;
            break;
        }
        else if (i == 0)
        {
            W1 = 0;
        }
    }
    for (volatile int i = round(src.cols*right); i < src.cols - 1; i++)
    {
        int t1 = V.at<ushort>(0, i);
        int t2 = V.at<ushort>(0, i + 1);
        if (t1 >= 3 && t2 < 3)
        {
            W2 = i;
            break;
        }
        else if (i == src.cols - 2)
        {
            W2 = src.cols;
        }
    }
    src = src(Rect(W1, 0, W2 - W1, src.rows));
}

QImage cvMat2QImage(const cv::Mat& mat)
{
    //1
    if(mat.type() == CV_8UC1)
    {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        // Set the color table (used to translate colour indexes to qRgb values)
        image.setColorCount(256);
        for(int i = 0; i < 256; i++)
        {
            image.setColor(i, qRgb(i, i, i));
        }
        // Copy input Mat
        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row ++)
        {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    //3
    else if(mat.type() == CV_8UC3)
    {
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    else if(mat.type() == CV_8UC4)
    {
        qDebug() << "CV_8UC4";
        // Copy input Mat
        const uchar *pSrc = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}
