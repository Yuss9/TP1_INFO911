#include <iostream>
#include <opencv2/highgui.hpp>
using namespace cv;
using namespace std;

int main(int argc, char* argv[])
{
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <nom-fichier-image>" << endl;
    return 1;
  }

  int old_value = 0;
  int value = 128;
  namedWindow("TP1");                // crée une fenêtre
  createTrackbar("track", "TP1", &value, 255, NULL); // un slider
  Mat f = imread(argv[1]);           // lit l'image spécifiée en paramètre
  if (f.empty()) {
    cout << "Impossible de charger l'image " << argv[1] << endl;
    return 1;
  }
  imshow("TP1", f);                  // l'affiche dans la fenêtre

  while (waitKey(50) < 0) {
    if (value != old_value) {
      old_value = value;
      cout << "value=" << value << endl;
    }
  }

  return 0;
}
