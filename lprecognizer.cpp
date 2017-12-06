#include "lprecognizer.h"
#include <QMessageBox>
#include <QString>
#include <QDebug>


LPRecognizer::LPRecognizer()
{

}

LPRecognizer::LPRecognizer(Mat &input)
{
    input_img = input.clone();
    start_process();
}

void LPRecognizer::set_img(Mat &input)
{
    input_img = input.clone();
    start_process();
}

void LPRecognizer::start_process()
{
    if(!input_img.data)
    {
        QMessageBox msgBox;
        msgBox.setText("LPRecognizer::Image data is null!");
        msgBox.exec();
    }
    else
    {
        pre_process(input_img,pre_processed_img);
        imshow("pre",pre_processed_img);
        optimize_binary_image(pre_processed_img,fixed_img);
        imshow("fixed",fixed_img);

    }

}

void LPRecognizer::pre_process(const Mat &input, Mat &output)
{
    // 灰度化、归一化大小、中值滤波、二值化
    if(input.channels() == 3)
        cvtColor(input,output,CV_BGR2GRAY);
    else if(input.channels() == 4)
        cvtColor(input,output,CV_BGRA2GRAY);
    fix_size(output);
    medianBlur(output,output,3);
    binary(output);
}

void LPRecognizer::optimize_binary_image(Mat &input, Mat &output)
{
    // 判断车牌颜色
    if (double(countNonZero(input)) / double(input.rows) / double(input.cols) > 0.5)
        reverse_binary_img(input,output);
    else
        output = input.clone();

    fix_font_weight(output);

    // 去除小部分面积
    Mat tmpImage = output(cv::Rect(0, 0, round(input.cols*0.3), input.rows));
    bwareaopen(tmpImage, 30);
    tmpImage = output(cv::Rect(round(input.cols*0.3), 0, input.cols - round(input.cols*0.3), input.rows));
    bwareaopen(tmpImage, 150);
    Mat H_projection, V_projection;
    projection(output,H_projection,LPR_Horizontal);
    projection(output,V_projection,LPR_Vertical);
    qDebug() << H_projection.ptr<int>(0)[40] << V_projection.ptr<int>(0)[40];

}

void LPRecognizer::fix_frame(Mat &input)
{

}

void LPRecognizer::segment(const Mat &input, Mat *output_array)
{

}

void LPRecognizer::recognize(const Mat *input_array, QString &ans, double *cov_ans)
{

}

void LPRecognizer::fix_size(Mat &input)
{
    int w;
    int h;
    double k = double(input.rows) / double(input.cols);
    w = 300;
    h = w * k;
    resize(input,input, Size(w, h),0,0,INTER_CUBIC);
}

void LPRecognizer::binary(Mat &input)
{
    double min, max;
    double* minp = &min;
    double* maxp = &max;
    Mat temp = input(Rect(round(input.cols*0.2),round(input.rows*0.2),round(input.cols*0.6),round(input.rows*0.6)));
    minMaxIdx(temp, minp, maxp);
    int T = round((max - (max - min) / 2)* 0.92);
    threshold(input, input, T, 255, THRESH_BINARY);
}

void LPRecognizer::reverse_binary_img(Mat &input, Mat &output)
{
    output = input < 127;
}

void LPRecognizer::fix_font_weight(Mat &input)
{
    Mat temp = input(Rect(round(input.cols*0.1),round(input.rows*0.1),round(input.cols*0.8),round(input.rows*0.8)));
    int sum_point = countNonZero(temp);
    if(double(sum_point) / double(temp.rows) / double(temp.cols) < 0.28 )
    {
        Mat element = getStructuringElement(MORPH_RECT,Size(3,3));
        dilate(input,input,element);
    }
}

void LPRecognizer::bwareaopen(Mat &input, int n)
{
    Mat labels,stats,centroids;
    connectedComponentsWithStats(input,labels,stats,centroids,8,CV_16U);
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
                        input.ptr<uchar>(j)[k] = 0;
                }
            }
        }
    }
}

void LPRecognizer::projection(const Mat &input, Mat &data, int direction)
{
    Mat temp = input / 255;
    // 水平
    if(direction == LPR_Horizontal)
    {
        data = Mat::zeros(1,temp.rows,CV_32SC1);
        for(int i=0;i<temp.rows;i++)
            for(int j=0;j<temp.cols;j++)
                data.ptr<int>(0)[i] += (int)temp.ptr<uchar>(i)[j];
//        qDebug() << data.ptr<int>(0)[40];

    }
    // 垂直
    else
    {
        data = Mat::zeros(1,temp.cols,CV_32SC1);
        for(int i=0;i<temp.cols;i++)
            for(int j=0;j<temp.rows;j++)
                data.ptr<int>(0)[i] += (int)temp.ptr<uchar>(j)[i];
//        qDebug() << data.ptr<int>(0)[40];
    }
}

