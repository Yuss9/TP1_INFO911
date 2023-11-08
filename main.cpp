#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp> // Ajoutez cette ligne pour utiliser cvtColor
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

  // Vérifiez si l'image est en niveaux de gris, sinon convertissez-la
  if (f.channels() > 1) {
    cvtColor(f, f, COLOR_BGR2GRAY);
  }

  imshow("TP1", f);                  // affiche l'image dans la fenêtre

  while (waitKey(50) < 0) {
    if (value != old_value) {
      old_value = value;
      cout << "value=" << value << endl;
    }
  }

  return 0;
}
