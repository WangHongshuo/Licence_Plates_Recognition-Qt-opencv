#include "lprecognizer.h"

LPRecognizer::LPRecognizer()
{

}

LPRecognizer::LPRecognizer(Mat &input)
{
    input_img = input.clone();
}

void LPRecognizer::start_process()
{

}

void LPRecognizer::pre_process(const Mat &input, Mat &output)
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

