#include "lprecognizer.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QString>
#include <QDebug>
#include <QTime>
#include <iostream>

// UFT-8 without BOM

LPRecognizer::LPRecognizer()
{
    initialization_data();
}

LPRecognizer::~LPRecognizer()
{
    delete [] NumberLetter;
    delete [] NL_character;
    delete [] ChineseCharacter;
    delete [] CC_character;
}

void LPRecognizer::set_img(Mat &input)
{
    input_img = input.clone();
    start_process();
}

void LPRecognizer::initialization_data()
{
//    NumberLetter = new Mat[36];
//    ChineseCharacter = new Mat[39];

    int i = 0;
    QString path = QCoreApplication::applicationDirPath();
    QString Q_ImgName;
    std::string ImgName;
    while (i < 36)
    {
        Q_ImgName = path + "/NumberLetter/" + QString::number(i) + ".bmp";
        ImgName = Q_ImgName.toStdString();
        NumberLetter[i] = imread(ImgName,0);
        if(!NumberLetter[i].data)
        {
            QMessageBox msgBox(QMessageBox::Critical,
                               ("LPRecognizer initialization failed!"),
                               ("Can't load (.exe path)/NumberLetter/"+QString::number(i)+".bmp"));
            msgBox.exec();
            is_initialized = false;
            break;
        }
        i++;
    }
    //    imshow("1",NumberLetter[1]);
    i = 1;
    while (i < 39)
    {

        Q_ImgName = path + "/ChineseCharacter/" + QString::number(i) + ".bmp";
        ImgName = Q_ImgName.toStdString();
        ChineseCharacter[i] = imread(ImgName,0);
        if(!ChineseCharacter[i].data)
        {
            QMessageBox msgBox(QMessageBox::Critical,
                               ("LPRecognizer initialization failed!"),
                               ("Can't load (.exe path)/ChineseCharacter/"+QString::number(i)+".bmp"));
            msgBox.exec();
            is_initialized = false;
            break;
        }
        i++;
    }
    //    imshow("1",ChineseCharacter[1]);
    // UTF-8 无BOM
    NL_character = new QString[37]{"0","1","2","3","4",
            "5","6","7","8","9",
            "A","B","C","D","E",
            "F","G","H","J","K",
            "L","M","N","O","P",
            "Q","R","S","T","U",
            "V","W","X","Y","Z",
            "7","O"};
    CC_character = new QString[40] {" ","京","津","沪","渝",
            "冀","豫","云","辽","黑",
            "湘","皖","鲁","苏","赣",
            "浙","粤","鄂","桂","甘",
            "晋","蒙","陕","吉","闽",
            "贵","青","藏","川","宁",
            "新","琼","云","渝","沪",
            "冀","京","云","藏","*" };
    //    qDebug() << NL_character[2] << CC_character[5];
}

void LPRecognizer::start_process()
{
    if(!input_img.data)
    {
        QMessageBox msgBox;
        msgBox.setText("LPRecognizer::Image data is null!");
        msgBox.exec();
    }
    else if(!is_initialized)
    {
        QMessageBox msgBox(QMessageBox::Critical,
                           ("LPRecognizer initialization failed!"),
                           ("Templates load error!"));
        msgBox.exec();
    }
    else
    {
		// 清理
        recognized_Licence_Plate.clear();
        std::fill(&correlation_value[0],&correlation_value[0]+7,0.0);
		// 开始处理
        pre_process(input_img,pre_processed_img);
//        imshow("pre",pre_processed_img);
        optimize_binary_image(pre_processed_img,fixed_img);
//        imshow("fixed",fixed_img);
        character_segmentation(fixed_img,character);
//        for(int i=0;i<7;i++)
//            imshow((QString::number(i)).toStdString(),character[i]);
        recognize_characters(character,recognized_character,correlation_value);
        for(int i=0;i<7;i++)
            recognized_Licence_Plate += recognized_character[i];
//        qDebug() << recognized_Licence_Plate;

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

    // 优化字体粗细
    fix_font_weight(output);

    // 去除小部分面积
    Mat tmpImage = output(cv::Rect(0, 0, round(input.cols*0.3), input.rows));
    bwareaopen(tmpImage, 30);
    tmpImage = output(cv::Rect(round(input.cols*0.3), 0, input.cols - round(input.cols*0.3), input.rows));
    bwareaopen(tmpImage, 150);

    //修正边框
    fix_frame(output);
}

void LPRecognizer::fix_frame(Mat &input)
{
    int x1, x2, y1, y2;
    projection(input,H_projection,LPR_HORIZONTAL);
    //    std::cout << H_projection;
    get_horizontal_segmentation_value(H_projection,input.cols,y1,y2);
    //    qDebug() << y1 << y2;
    input = input(Rect(0,y1,input.cols,y2-y1));
    projection(input,V_projection,LPR_VERTICAL);
    //    qDebug() << H_projection.ptr<int>(0)[40] << V_projection.ptr<int>(0)[40];
    get_vertical_segmentation_value(V_projection,x1,x2);
    //    qDebug() << x1 << x2;
    input = input(Rect(x1,0,x2-x1,input.rows));
}

void LPRecognizer::character_segmentation(const Mat &input, Mat (&character)[7])
{
    projection(input,V_projection,LPR_VERTICAL);
    int W[15] = {0};
    W[13] = V_projection.cols;
    int g = 13;
    int n = V_projection.cols;
    int t1, t2;
    for (int i = V_projection.cols - 2; i > -1; i--)
    {
        t1 = V_projection.ptr<int>(0)[i];
        t2 = V_projection.ptr<int>(0)[i + 1];
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
    // 校验分割是否有错误 有则强制分割
    if (W[14] == 0)
    {
        for (int i = 0; i < 7; i++)
        {
            if (W[i * 2 + 1] - W[i * 2] > round(input.cols*0.135))
            {
                W[14] += 1;
            }
        }
    }
    //    qDebug() << W[3];
    if(W[14] != 0)
        force_character_segmentation(V_projection.cols,W);
    //    qDebug() << W[3];
    // 分割
    for (int i = 0; i < 7; i++)
    {
        character[i] = input(Rect(W[i * 2], 0, W[i * 2 + 1] - W[i * 2], input.rows));
    }
}

void LPRecognizer::force_character_segmentation(int image_width, int (&W)[15])
{
    int n = image_width;
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
}

void LPRecognizer::recognize_characters(Mat (&character)[7], QString (&ans)[7], float (&correlation_value)[7])
{
    Mat result = Mat(1, 1, CV_32FC1);
    int index = 0;
    for (int i = 0; i < 7; i++)
    {
        // 直接判定特殊情况下的数字1
        if (double(character[i].cols) / double(character[i].rows) < 0.3 && i > 0)
        {
            ans[i] = "1";
            correlation_value[i] = -1.0;
            continue;
        }
        character_optimization(character[i],i);
        if (i == 0)
        {
            //cn[0]和ChineseCharacter[0]为空，脚标+1
            resize(character[i], character[i], Size(48, 84), 3);
            binary(character[i]);
            for (int j = 0; j < 38; j++)
            {
                matchTemplate(character[i], ChineseCharacter[j+1], result, TM_CCOEFF_NORMED);
                if(correlation_value[i] < result.ptr<float>(0)[0])
                {
                    correlation_value[i] = result.ptr<float>(0)[0];
                    index = j;
                }
            }
            ans[i] = CC_character[index + 1];
            //            qDebug() << ans[i] << correlation_value[i];
            continue;
        }
        else
        {
            resize(character[i], character[i], Size(24, 42), 3);
            binary(character[i]);
            // 车牌第二位不为数字
            if(i == 1)
            {
                for (int j = 10; j < 36; j++)
                {
                    matchTemplate(character[i], NumberLetter[j], result, TM_CCORR_NORMED);
                    if(correlation_value[i] < result.ptr<float>(0)[0])
                    {
                        correlation_value[i] = result.ptr<float>(0)[0];
                        index = j;
                    }
                }
                if(index == 13 || index == 23)
                {
                    if(i == 1)
                    {
                        feature_match(character[i],13,index);
                        if(index == 0 && i == 1)
                            index = 23;
                    }
                }
                ans[i] = NL_character[index];
//                qDebug() << ans[i] << correlation_value[i];
                continue;
            }
            else
            {
                for (int j = 0; j < 36; j++)
                {
                    matchTemplate(character[i], NumberLetter[j], result, TM_CCORR_NORMED);
                    if(correlation_value[i] < result.ptr<float>(0)[0])
                    {
                        correlation_value[i] = result.ptr<float>(0)[0];
                        index = j;
                    }
                }
                if(index == 0 || index == 13)
                {
                    feature_match(character[i],index,index);
                }
                ans[i] = NL_character[index];
//                qDebug() << ans[i] << correlation_value[i];
            }
        }
    }

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
    if(direction == LPR_HORIZONTAL)
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

void LPRecognizer::get_horizontal_segmentation_value(const Mat &H_projection, const int &image_width, int &y1, int &y2)
{
    // 减少循环中if判别次数
    bool is_value_got = false;
    int n = H_projection.cols;
    int t1, t2;
    for (int i = round(n*0.25); i > 0; i--)
    {
        t1 = H_projection.ptr<int>(0)[i];
        t2 = H_projection.ptr<int>(0)[i-1];
        if ((t1 > round(image_width*0.2) && t2 <= round(image_width*0.2)) || t1 > round(image_width*0.7))
        {
            y1 = i;
            is_value_got = true;
            break;
        }
    }
    if(!is_value_got)
    {
        if (H_projection.ptr<int>(0)[0] > H_projection.ptr<int>(0)[1])
            y1 = 1;
        else
            y1 = 0;
    }

    is_value_got = false;
    for (int i = round(n*0.7); i <= n; i++)
    {
        t1 = H_projection.ptr<int>(0)[i];
        t2 = H_projection.ptr<int>(0)[i-1];
        if ((t1 <= round(image_width*0.2) && t2 > round(image_width*0.2)) || t1 > round(image_width*0.7))
        {
            y2 = i - 1;
            is_value_got = true;
            break;
        }
    }
    if(!is_value_got)
    {
        if (H_projection.ptr<int>(0)[image_width] > H_projection.ptr<int>(0)[image_width-1])
            y2 = n - 2;
        else
            y2 = n - 1;
    }
}

void LPRecognizer::get_vertical_segmentation_value(const Mat &V_projiction, int &x1, int &x2)
{
    // 未检查 未优化
    bool is_value_got = false;
    int W[2] ={0,0};
    int edge_count = 2;
    int n = V_projiction.cols;
    int t1, t2;
    for (int i = round(n*0.2); i > 0; i--)
    {
        t1 = V_projiction.ptr<int>(0)[i];
        t2 = V_projiction.ptr<int>(0)[i+1];
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
    x1 = W[0];

    for (int i = round(n*0.9); i < n - 1; i++)
    {
        t1 = V_projiction.ptr<int>(0)[i];
        t2 = V_projiction.ptr<int>(0)[i+1];
        if (t1 > 4 && t2 <= 4)
        {
            x2 = i;
            is_value_got = true;
            break;
        }
    }
    if(!is_value_got)
        x2 = n - 1;
}

void LPRecognizer::character_optimization(Mat &input, const int &character_index)
{
    double k1, k2;
    int t1, t2, p1, p2;
    Mat temp_projection;
    // 水平
    if(character_index == 0)
    {
        k1 = 0.15;
        k2 = 0.85;
    }
    else
    {
        k1 = 0.4;
        k2 = 0.6;
    }

    projection(input,temp_projection,LPR_HORIZONTAL);
    for(int i = round(input.rows*k1); i != -1; i--)
    {
        t1 = temp_projection.ptr<int>(0)[i];
        t2 = temp_projection.ptr<int>(0)[i+1];
        if (t1 <= 3 && t2 > 3)
        {
            p1 = i + 1;
            break;
        }
        else if (i == 0)
        {
            p1 = 0;
        }
    }
    for(int i = round(input.rows*k2); i < input.rows-1; i++)
    {
        t1 = temp_projection.ptr<int>(0)[i];
        t2 = temp_projection.ptr<int>(0)[i+1];
        if (t1 >= 3 && t2 < 3)
        {
            p2 = i;
            break;
        }
        else if (i == input.rows - 2)
        {
            p2 = input.rows - 1;
        }
    }

    // 找到裁剪点则进行裁剪
    if(!(p1 == 0 && p2 == input.rows - 1))
        input = input(Rect(0, p1,input.cols , p2 - p1));
    // 垂直
    if (character_index == 0)
    {
        k1 = 0.2;
        k2 = 0.9;
    }
    else
    {
        k1 = 0.4;
        k2 = 0.6;
    }
    projection(input,temp_projection,LPR_VERTICAL);
    for (int i= round(input.cols*k1); i != -1; i--)
    {
        t1 = temp_projection.ptr<int>(0)[i];
        t2 = temp_projection.ptr<int>(0)[i+1];
        if (t1 <= 3 && t2 > 3)
        {
            p1 = i + 1;
            break;
        }
        else if (i == 0)
        {
            p1 = 0;
        }
    }
    for (int i = round(input.cols*k2); i < input.cols - 1; i++)
    {
        t1 = temp_projection.ptr<int>(0)[i];
        t2 = temp_projection.ptr<int>(0)[i+1];
        if (t1 >= 3 && t2 < 3)
        {
            p2 = i;
            break;
        }
        else if (i == input.cols - 2)
        {
            p2 = input.cols - 1;
        }
    }

    if(!(p1 == 0 && p2 == input.cols - 1))
        input = input(Rect(p1, 0, p2 - p1, input.rows));
}

void LPRecognizer::feature_match(const Mat &input, const int input_index, int &output_index)
{
    // 特征匹配 有时间加入更多特征
    if(input_index == 0 || input_index == 13)
    {
        Mat temp = input(Rect(1,0,4,42));
        Mat temp_projection;
        projection(temp,temp_projection,LPR_VERTICAL);
        double min,max;
        double* minp = &min;
        double* maxp = &max;
        minMaxIdx(temp_projection,minp,maxp);
        if(max <= 40.0)
            output_index = 0;
        else
            output_index =  13;
    }
}

