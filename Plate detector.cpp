#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) return -1;

    Mat frame, gray, blurImg, thresh, placaRecortada;

    cout << "=== LICENSE PLATE DETECTION ===" << endl;
    cout << "Apunta la camara hacia una placa o texto rectangular." << endl;
    cout << "Presiona 'q' para salir." << endl;

    while (true) {
        cap.read(frame);
        if (frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, blurImg, Size(5, 5), 0);
        Canny(blurImg, thresh, 100, 200);

        vector<vector<Point>> contornos;
        findContours(thresh, contornos, RETR_TREE, CHAIN_APPROX_SIMPLE);

        int mejorMinX = 0, mejorMinY = 0, mejorMaxX = 0, mejorMaxY = 0;
        int maxScore = 0;

        for (size_t i = 0; i < contornos.size(); i++) {
            if (contornos[i].empty()) continue;

            int minX = frame.cols, minY = frame.rows, maxX = 0, maxY = 0;
            for (size_t j = 0; j < contornos[i].size(); j++) {
                Point pt = contornos[i][j];
                if (pt.x < minX) minX = pt.x;
                if (pt.y < minY) minY = pt.y;
                if (pt.x > maxX) maxX = pt.x;
                if (pt.y > maxY) maxY = pt.y;
            }

            int w = maxX - minX;
            int h = maxY - minY;

            if (h > 0) {
                float proporcion = (float)w / (float)h;
                int area = w * h;

               
                if (proporcion > 2.0 && proporcion < 6.0 && area > 1000 && area < 50000) {
                    if (area > maxScore) {
                        maxScore = area;
                        mejorMinX = minX;
                        mejorMinY = minY;
                        mejorMaxX = maxX;
                        mejorMaxY = maxY;
                    }
                }
            }
        }

        if (maxScore > 1000) {
            Rect cajaPlaca(mejorMinX, mejorMinY, mejorMaxX - mejorMinX, mejorMaxY - mejorMinY);
            rectangle(frame, cajaPlaca, Scalar(0, 255, 0), 2);

            if (cajaPlaca.x >= 0 && cajaPlaca.y >= 0 && (cajaPlaca.x + cajaPlaca.width) <= frame.cols && (cajaPlaca.y + cajaPlaca.height) <= frame.rows) {
                placaRecortada = frame(cajaPlaca);
                imshow("Placa Detectada (Zoom)", placaRecortada);
            }
        }

        imshow("Deteccion de Placas en Vivo", frame);

        if (waitKey(30) == 'q') break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}