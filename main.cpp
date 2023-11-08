#include <iostream>
#include <opencv2/highgui.hpp>
using namespace cv;
int main()
{
  int old_value = 0;
  int value = 128;
  namedWindow( "TP1");               // crée une fenêtre
  createTrackbar( "track", "TP1", &value, 255, NULL); // un slider
  Mat f = imread("lena.png");        // lit l'image "lena.png"
  imshow( "TP1", f );                // l'affiche dans la fenêtre
  while ( waitKey(50) < 0 )          // attend une touche
  { // Affiche la valeur du slider
    if ( value != old_value )
    {
      old_value = value;
      std::cout << "value=" << value << std::endl;
    }
  }
} 