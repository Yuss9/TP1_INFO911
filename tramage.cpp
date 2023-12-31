#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;


/* 
##############################################################################
*/

cv::Mat tramage_floyd_steinberg( cv::Mat input ){
  int rowSize = input.rows;
  int columnSize = input.cols;

  cv::Mat output( rowSize, columnSize, CV_8UC1 );
  output.setTo(255);
  cv::Mat input2;
  input.convertTo(input2, CV_32FC1);

  for(int row = 0; row < rowSize; row++) {
      for(int col = 0; col < columnSize; col++) {
        float pixel = input2.at<float>(row, col);
        
        //calcul de la couleur la plus proche
        float new_pixel = pixel > 127.0 ? 255.0 : 0;
        
        output.at<uchar>(row, col) = new_pixel;
        float error = pixel - new_pixel;
        if(col + 1 < columnSize) input2.at<float>(row, col + 1) += error * 7.0 / 16.0;
        if(row + 1 < rowSize) {
          if(col - 1 >= 0) input2.at<float>(row + 1, col - 1) += error * 3.0 / 16.0;
          input2.at<float>(row + 1, col) += error * 5.0 / 16.0;
          if(col + 1 < columnSize) input2.at<float>(row + 1, col + 1) += error * 1.0 / 16.0;
        }
      }
  }
  return output;
}

void test_tramage_floyd_steinberg(String filename){
    Mat f = imread(filename, IMREAD_GRAYSCALE);

    if (f.empty()) {
        cout << "Impossible de charger l'image " << filename << endl;
        return;
    }


    namedWindow("Image initial");
    imshow("Image initial", f);


    Mat final_image = tramage_floyd_steinberg(f);
    namedWindow("Image finale");
    imshow("Image finale", final_image);


    waitKey(0);

}

/*
#################################################################### 
*/


cv::Mat tramage_floyd_steinberg_color(cv::Mat input) {
  int rowSize = input.rows;
  int columnSize = input.cols;

    cv::Mat output(rowSize, columnSize, input.type());
    output.setTo(255);

    for (int row = 0; row < rowSize; row++) {
        for (int col = 0; col < columnSize; col++) {
            for (int channel = 0; channel < input.channels(); channel++) {
                uchar pixel = input.at<Vec3b>(row, col)[channel];

                // Calcul de la couleur la plus proche
                uchar new_pixel = pixel > 127 ? 255 : 0;
                output.at<Vec3b>(row, col)[channel] = new_pixel;

                int error = pixel - new_pixel;

                if (col + 1 < columnSize) input.at<Vec3b>(row, col + 1)[channel] += error * 5 / 16;
                if (row + 1 < rowSize) {
                    if (col - 1 >= 0) input.at<Vec3b>(row + 1, col - 1)[channel] += error * 3 / 16;
                    input.at<Vec3b>(row + 1, col)[channel] += error * 3 / 16; // Ajustement ici
                    if (col + 1 < columnSize) input.at<Vec3b>(row + 1, col + 1)[channel] += error * 1 / 16;
                }
            }
        }
    }
    return output;
}

void test_tramage_floyd_steinberg_color(String filename){
    Mat f = imread(filename);

    if (f.empty()) {
        cout << "Impossible de charger l'image " << filename << endl;
        return;
    }
    namedWindow("Image initial");
    imshow("Image initial", f);


    Mat final_image = tramage_floyd_steinberg_color(f);
    namedWindow("Image finale");
    imshow("Image finale", final_image);


    waitKey(0);

}


/*
#################################################################### 
*/


float distance_color_l2(Vec3f bgr1, Vec3f bgr2) {
    return sqrt(pow(bgr1[0] - bgr2[0], 2) + pow(bgr1[1] - bgr2[1], 2) + pow(bgr1[2] - bgr2[2], 2));
}

int best_color(Vec3f bgr, vector<Vec3f> colors) {
    int index = 0;
    float minDist = distance_color_l2(bgr, colors[0]);

    for (int i = 1; i < colors.size(); i++) {
        float dist = distance_color_l2(bgr, colors[i]);
        if (dist < minDist) {
            minDist = dist;
            index = i;
        }
    }

    return index;
}

Vec3f error_color(Vec3f bgr1, Vec3f bgr2) {
    return Vec3f(bgr1[0] - bgr2[0], bgr1[1] - bgr2[1], bgr1[2] - bgr2[2]);
}

cv::Mat tramage_floyd_steinberg_final(cv::Mat input, std::vector<cv::Vec3f> colors) {
    cv::Mat fs;
    input.convertTo(fs, CV_32FC3, 1 / 255.0);

    int rowSize = fs.rows;
    int columnSize = fs.cols;

    for (int row = 0; row < rowSize; row++) {
        for (int col = 0; col < columnSize; col++) {
            Vec3f c = fs.at<Vec3f>(row, col);
            int i = best_color(c, colors);
            Vec3f e = error_color(c, colors[i]);

            fs.at<Vec3f>(row, col) = colors[i];

            if (col + 1 < columnSize) fs.at<Vec3f>(row, col + 1) += e * (7.0 / 16.0);
            if (row + 1 < rowSize) {
                if (col - 1 >= 0) fs.at<Vec3f>(row + 1, col - 1) += e * (1.0 / 16.0);
                fs.at<Vec3f>(row + 1, col) += e * (5.0 / 16.0);
                if (col + 1 < columnSize) fs.at<Vec3f>(row + 1, col + 1) += e * (3.0 / 16.0);
            }
        }
    }

    cv::Mat output;
    fs.convertTo(output, CV_8UC3, 255.0);

    return output;
}


void test_tramage_floyd_steinberg_final(String filename){
    Mat f = imread(filename);


    vector<cv::Vec3f> COLORS;
    Vec3f a(0.0f, 0.0f, 0.0f);
    Vec3f b(1.0f, 0.0f, 1.0f);
    Vec3f c(0.0f, 1.0f, 1.0f);
    Vec3f d(1.0f, 1.0f, 0.0f);
    Vec3f e(1.0f, 1.0f, 1.0f);

    Vec3f r(0.0f, 0.0f, 1.0f);
    Vec3f g(0.0f, 1.0f, 0.0f);
    Vec3f bl(1.0f, 0.0f, 0.0f);
    
    std::vector<Vec3f> vect;
    COLORS.push_back(a);
    COLORS.push_back(b);
    COLORS.push_back(c);
    COLORS.push_back(d);
    COLORS.push_back(e);

    namedWindow("Image initial");
    imshow("Image initial", f);

    Mat final_image = tramage_floyd_steinberg_final(f, COLORS);
    namedWindow("Image tramee CMYK");
    imshow("Image tramee CMYK", final_image);

    waitKey(0);
}

/*
#################################################################### 
*/


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <nom-fichier-image>" << endl;
        return 1;
    }

    Mat f = imread(argv[1]);

    if (f.empty()) {
        cout << "Impossible de charger l'image " << argv[1] << endl;
        return 1;
    }

    test_tramage_floyd_steinberg_final(argv[1]);

    return 0;
}