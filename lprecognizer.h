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
    enum{LPR_HORIZONTAL, LPR_VERTICAL};

    LPRecognizer();
    ~LPRecognizer();
    void set_img(Mat &input);
    Mat pre_processed_img;
    Mat fixed_img;
    Mat character[7];
    QString recognized_character[7];
    QString recognized_Licence_Plate;
    float corr2_value[7] = {0.0};


private:
    void initialization_data();
    void start_process();
    void pre_process(const Mat &input, Mat &output);
    void optimize_binary_image(Mat &input, Mat &output);
    void fix_frame(Mat &input);
    void character_segmentation(const Mat &input, Mat (&character)[7]);
    void force_character_segmentation(const int image_width, int (&W)[15]);
    void recognize_characters(Mat (&character)[7], QString (&ans)[7], float (&corr2_value)[7]);

    void fix_size(Mat &input);
    void binary(Mat &input);
    void reverse_binary_img(Mat &input, Mat &output);
    void fix_font_weight(Mat &input);
    void bwareaopen(Mat &input, int n);
    void projection(const Mat &input, Mat &data, int direction);
    void get_horizontal_segmentation_value(const Mat &H_projection, const int &image_width, int &y1, int &y2);
    void get_vertical_segmentation_value(const Mat &V_projiction, int &x1, int &x2);
    void character_optimization(Mat &input, const int &character_index);
    void feature_match(const Mat &input, const int input_index, int &output_index);

    Mat input_img;
    Mat H_projection, V_projection;
    Mat* NumberLetter = new Mat[36];
    QString* NL_character;
    Mat* ChineseCharacter = new Mat[39];
    QString* CC_character;


};

#endif // LPRECOGNIZER_H
