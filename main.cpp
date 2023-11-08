#include <iostream>
#include <vector>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;


/* 
##############################################################################
*/


// Fonction pour calculer l'histogramme
std::vector<double> histogramme(Mat image) {
    std::vector<double> hist(256, 0);
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int pixel_value = static_cast<int>(image.at<uchar>(i, j));
            hist[pixel_value]++;
        }
    }
    return hist;
}

// Fonction pour calculer l'histogramme cumulé
std::vector<double> histogramme_cumule(const std::vector<double>& h_I) {
    std::vector<double> H_I(256, 0);
    H_I[0] = h_I[0];
    for (int i = 1; i < 256; i++) {
        H_I[i] = H_I[i - 1] + h_I[i];
    }
    return H_I;
}

/* 
##############################################################################
*/

cv::Mat afficheHistogrammes(const std::vector<double>& h_I, const std::vector<double>& H_I) {
    cv::Mat image(256, 512, CV_8UC1, Scalar(255));

    double max_h = *std::max_element(h_I.begin(), h_I.end());
    double max_H = *std::max_element(H_I.begin(), H_I.end());

    for (int v = 0; v < 256; v++) {
        int h_value = static_cast<int>(h_I[v] * 255.0 / max_h);
        int H_value = static_cast<int>(H_I[v] * 255.0 / max_H);

        for (int i = 255; i >= 255 - h_value; i--) {
            image.at<uchar>(i, v) = 0;
        }

        for (int i = 255; i >= 255 - H_value; i--) {
            image.at<uchar>(i, v + 256) = 0; // Affichage à droite
        }
    }

    return image;
}


/* 
Affiche un seule histogramme
*/
cv::Mat afficheHistogramme(const std::vector<double>& h_I) {
    cv::Mat image(256, 256, CV_8UC1, Scalar(255));

    double max_value = *std::max_element(h_I.begin(), h_I.end());
    double scale_factor = 256.0 / max_value;

    for (int v = 0; v < 256; v++) {
        int h_value = static_cast<int>(h_I[v] * scale_factor);

        // Garantir que l'indice reste dans les limites
        h_value = std::min(255, std::max(0, 255 - h_value));

        for (int i = h_value; i < 256; i++) {
            image.at<uchar>(i, v) = 0;
        }
    }

    return image;
}

/* 
##############################################################################
*/

// Fonction pour égaliser l'histogramme
Mat egaliserHistogramme(const Mat& image, const std::vector<double>& H_I) {
    Mat resultat = image.clone();
    int total_pixels = image.rows * image.cols;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            int pixel_value = static_cast<int>(image.at<uchar>(i, j));
            int nouvelle_valeur = static_cast<int>(255.0 * H_I[pixel_value] / total_pixels);
            resultat.at<uchar>(i, j) = nouvelle_valeur;
        }
    }
    return resultat;
}

/* 
##############################################################################
*/

void gestionColor(String filename){
    Mat f = imread(filename);

    Mat e;
    cv::cvtColor(f,e, COLOR_BGR2HSV);

    std::vector<Mat> HSV;
    cv::split(e,HSV);

    std::vector<double> h_I = histogramme(HSV[2]); 
    std::vector<double> H_I = histogramme_cumule(h_I); 
    
    Mat egalisee = egaliserHistogramme(HSV[2], H_I); // egalise
    
    std::vector<Mat> final_channels = {HSV[0],HSV[1], egalisee};
    Mat merged;
    cv::merge(final_channels,merged);
    cv::cvtColor(merged,merged, COLOR_HSV2BGR );

    namedWindow("Image original");
    imshow("Image original", f);

    Mat hist_image = afficheHistogrammes(h_I, H_I);
    namedWindow("Histogrammes");
    imshow("Histogrammes", hist_image);

    namedWindow("Image final");
    imshow("Image final", merged);

    std::vector<double> h_I_merged = histogramme(merged); 
    std::vector<double> H_I_merged = histogramme_cumule(h_I_merged); 

    Mat hist_image_merged = afficheHistogrammes(h_I_merged, H_I_merged);
    imshow("Histogrammes merged ", hist_image_merged);

    waitKey(0);
}

/* 
##############################################################################
*/

void egaliseImageGrey(String filename) {
    Mat f = imread(filename, IMREAD_GRAYSCALE); // Chargez l'image en niveaux de gris

    std::vector<double> h_I = histogramme(f); // Calcul de l'histogramme
    std::vector<double> H_I = histogramme_cumule(h_I); // Calcul de l'histogramme cumulé

    Mat hist_image = afficheHistogrammes(h_I, H_I);
    namedWindow("Histogrammes");
    imshow("Histogrammes", hist_image);

    Mat egalisee = egaliserHistogramme(f, H_I);
    namedWindow("Image egalisee");
    imshow("Image egalisee", egalisee);

    std::vector<double> h_egalisee = histogramme(egalisee);

    waitKey(0);
}

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

    gestionColor(argv[1]);

    return 0;

}