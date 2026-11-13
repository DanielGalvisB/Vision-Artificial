#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) return -1;

    Mat frame, gray, blurImg, thresh, cropped;

    cout << "=== DOCUMENT SCANNER (Modo Robusto) ===" << endl;
    cout << "Presiona 'q' para salir." << endl;

    while (true) {
        cap.read(frame);
        if (frame.empty()) break;

        cvtColor(frame, gray, COLOR_BGR2GRAY);
        GaussianBlur(gray, blurImg, Size(5, 5), 0);
        Canny(blurImg, thresh, 75, 200);

        vector<vector<Point>> contornos;
        findContours(thresh, contornos, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

        int mejorMinX = 0, mejorMinY = 0, mejorMaxX = 0, mejorMaxY = 0;
        int maxArea = 0;

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
            int area = w * h;

            if (area > 5000 && area > maxArea) {
                maxArea = area;
                mejorMinX = minX;
                mejorMinY = minY;
                mejorMaxX = maxX;
                mejorMaxY = maxY;
            }
        }

        if (maxArea > 5000) {
            Rect rectDoc(mejorMinX, mejorMinY, mejorMaxX - mejorMinX, mejorMaxY - mejorMinY);
            rectangle(frame, rectDoc, Scalar(0, 255, 0), 2);

            if (rectDoc.x >= 0 && rectDoc.y >= 0 && (rectDoc.x + rectDoc.width) <= frame.cols && (rectDoc.y + rectDoc.height) <= frame.rows) {
                cropped = frame(rectDoc);
                imshow("Documento Escaneado", cropped);
            }
        }

        imshow("Camara Original", frame);

        if (waitKey(30) == 'q') break;
    }

    cap.release();
    destroyAllWindows();
    return 0;
}