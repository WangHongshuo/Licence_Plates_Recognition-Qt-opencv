#ifndef LPRECOGNIZER_H
#define LPRECOGNIZER_H
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <QString>

using namespace cv;


class LPRecognizer
{
public:
    LPRecognizer();
    LPRecognizer(Mat &input);


private:
    void start_process();
    void pre_process(const Mat &input, Mat &output);
    void fix_frame(Mat &input);
    void segment(const Mat &input, Mat *output_array);
    void recognize(const Mat *input_array,QString &ans,double *cov_ans);
    Mat input_img;
    Mat pre_processed_img;


};

#endif // LPRECOGNIZER_H
