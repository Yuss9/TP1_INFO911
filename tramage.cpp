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


int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <nom-fichier-image>" << endl;
        return 1;
    }

    Mat f = imread(argv[1], IMREAD_GRAYSCALE);

    if (f.empty()) {
        cout << "Impossible de charger l'image " << argv[1] << endl;
        return 1;
    }

    namedWindow("Image initial");
    imshow("Image initial", f);


    Mat final_image = tramage_floyd_steinberg(f);
    namedWindow("Image finale");
    imshow("Image finale", final_image);


    waitKey(0);

    return 0;

}