#ifndef LPRECOGNIZER_H
#define LPRECOGNIZER_H
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <QString>

using namespace cv;


class LPRecognizer
{
public:
    LPRecognizer();
    LPRecognizer(Mat &input);
    void set_img(Mat &input);
    Mat pre_processed_img;
    Mat fixed_img;


private:
    void start_process();
    void pre_process(const Mat &input, Mat &output);
    void optimize_binary_image(const Mat &input, Mat &output);
    void fix_frame(Mat &input);
    void segment(const Mat &input, Mat *output_array);
    void recognize(const Mat *input_array,QString &ans,double *cov_ans);

    void fix_size(Mat &input);
    void binary(Mat &input);

    Mat input_img;


};

#endif // LPRECOGNIZER_H
