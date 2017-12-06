#include "lprecognizer.h"
#include "QMessageBox"

LPRecognizer::LPRecognizer()
{

}

LPRecognizer::LPRecognizer(Mat &input)
{
    input_img = input.clone();
}

void LPRecognizer::set_img(Mat &input)
{
    input_img = input.clone();
}

void LPRecognizer::start_process()
{
    if(!input_img.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("LPRecognizer::Image data is null!"));
        msgBox.exec();
    }
    else
    {
        pre_process(input_img,pre_processed_img);
        optimize_binary_image(pre_processed_img,fixed_img);

    }

}

void LPRecognizer::pre_process(const Mat &input, Mat &output)
{
    if(input.channels() == 3)
        cvtColor(input,output,CV_BGR2GRAY);
    else if(input.channels() == 4)
        cvtColor(input,output,CV_BGRA2GRAY);
    fix_size(output);
    medianBlur(output,output,3);
    binary(output);
}

void LPRecognizer::optimize_binary_image(const Mat &input, Mat &output)
{


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
    double k = double(src.rows) / double(src.cols);
    w = 300;
    h = w * k;
    resize(input,input, Size(w, h),0,0,INTER_CUBIC);
}

void LPRecognizer::binary(Mat &input)
{
    double min, max;
    double* minp = &min;
    double* maxp = &max;
    Mat temp = src(Rect(round(src.cols*0.2),round(src.rows*0.2),round(src.cols*0.6),round(src.rows*0.6)));
    minMaxIdx(temp, minp, maxp);
    int T = round((max - (max - min) / 2)* 0.92);
    threshold(src, src, T, 255, THRESH_BINARY);
}

