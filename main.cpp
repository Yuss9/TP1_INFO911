#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
using namespace std;

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

// Fonction pour afficher les histogrammes
cv::Mat afficheHistogrammes(const std::vector<double>& h_I, const std::vector<double>& H_I) {
    cv::Mat image(256, 512, CV_8UC1, Scalar(255));

    double max_value = *std::max_element(h_I.begin(), h_I.end());
    double scale_factor = 256.0 / max_value;

    for (int v = 0; v < 256; v++) {
        int h_value = static_cast<int>(h_I[v] * scale_factor);
        int H_value = static_cast<int>(H_I[v] * scale_factor);

        if(255 - h_value > 255){
            h_value = 255;
        }
        if(255 - H_value > 255){
            H_value = 255;
        }

        for (int i = h_value; i < 256; i++) {
            image.at<uchar>(i, v * 2) = 0;
        }

        for (int i = H_value; i < 256; i++) {
            image.at<uchar>(i, v * 2 + 1) = 0;
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




int main(int argc, char* argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <nom-fichier-image>" << endl;
        return 1;
    }

    Mat f = imread(argv[1], IMREAD_GRAYSCALE); // Chargez l'image en niveaux de gris

    if (f.empty()) {
        cout << "Impossible de charger l'image " << argv[1] << endl;
        return 1;
    }

    std::vector<double> h_I = histogramme(f); // Calcul de l'histogramme
    std::vector<double> H_I = histogramme_cumule(h_I); // Calcul de l'histogramme cumulé
    
    Mat hist_image = afficheHistogrammes(h_I, H_I); 
    // Mat hist_image = afficheHistogramme(H_I); ou avec aussi h_I

    namedWindow("Histogrammes");
    imshow("Histogrammes", hist_image);
 
    waitKey(0);

    return 0;
}
