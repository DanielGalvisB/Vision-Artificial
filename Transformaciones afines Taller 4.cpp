#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error: No se pudo conectar a la cámara." << endl;
        return -1;
    }

    Mat frame, reflected, rotated, sheared;
    double angle = 0.0;

    cout << "Iniciando Proyecto 4: Transformaciones Afines (Matriz Manual)..." << endl;
    cout << "Presiona 'q' para salir." << endl;

    while (true) {
        cap.read(frame);
        if (frame.empty()) break;

        Mat Mx = Mat::zeros(2, 3, CV_64F);
        Mx.at<double>(0, 0) = -1.0;
        Mx.at<double>(0, 1) = 0.0;
        Mx.at<double>(0, 2) = frame.cols;
        Mx.at<double>(1, 0) = 0.0;
        Mx.at<double>(1, 1) = 1.0;
        Mx.at<double>(1, 2) = 0.0;
        warpAffine(frame, reflected, Mx, frame.size());

        double scale = 0.8;
        double angleRad = angle * CV_PI / 180.0;
        double alpha = scale * cos(angleRad);
        double beta = scale * sin(angleRad);
        double cx = frame.cols / 2.0;
        double cy = frame.rows / 2.0;

        Mat rotMat = Mat::zeros(2, 3, CV_64F);
        rotMat.at<double>(0, 0) = alpha;
        rotMat.at<double>(0, 1) = beta;
        rotMat.at<double>(0, 2) = (1.0 - alpha) * cx - beta * cy;
        rotMat.at<double>(1, 0) = -beta;
        rotMat.at<double>(1, 1) = alpha;
        rotMat.at<double>(1, 2) = beta * cx + (1.0 - alpha) * cy;

        warpAffine(frame, rotated, rotMat, frame.size());
        angle += 3.0;

        Mat shearMat = Mat::zeros(2, 3, CV_64F);
        shearMat.at<double>(0, 0) = 0.8;
        shearMat.at<double>(0, 1) = 0.3;
        shearMat.at<double>(0, 2) = 0.0;
        shearMat.at<double>(1, 0) = 0.0;
        shearMat.at<double>(1, 1) = 0.8;
        shearMat.at<double>(1, 2) = 0.0;
        warpAffine(frame, sheared, shearMat, frame.size());

        imshow("1. Original", frame);
        imshow("2. Reflexion", reflected);
        imshow("3. Rotacion en Vivo", rotated);
        imshow("4. Cizallamiento", sheared);

        if (waitKey(30) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}