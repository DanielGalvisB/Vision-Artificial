#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

vector<vector<int>> misColores = {
    { 85, 100, 100, 135, 255, 255 },
    { 35, 100, 100, 85,  255, 255 }
};

vector<Scalar> valoresColores = {
    Scalar(255, 0, 0),
    Scalar(0, 255, 0)
};

vector<vector<int>> puntosDibujados;

Point obtenerPuntaContorno(Mat imagen) {
    vector<vector<Point>> contornos;
    vector<Vec4i> jerarquia;

    findContours(imagen, contornos, jerarquia, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Point miPunto(0, 0);

    for (size_t i = 0; i < contornos.size(); i++) {
        if (contornos[i].empty()) continue;

        int minX = imagen.cols, minY = imagen.rows, maxX = 0, maxY = 0;

        for (size_t j = 0; j < contornos[i].size(); j++) {
            Point pt = contornos[i][j];
            if (pt.x < minX) minX = pt.x;
            if (pt.y < minY) minY = pt.y;
            if (pt.x > maxX) maxX = pt.x;
            if (pt.y > maxY) maxY = pt.y;
        }

        int width = maxX - minX;
        int height = maxY - minY;
        int areaAprox = width * height;

        if (areaAprox > 1500) {
            miPunto.x = minX + (width / 2);
            miPunto.y = minY;
        }
    }
    return miPunto;
}

void detectarColor(Mat img) {
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);

    for (size_t i = 0; i < misColores.size(); i++) {
        Scalar limiteInferior(misColores[i][0], misColores[i][1], misColores[i][2]);
        Scalar limiteSuperior(misColores[i][3], misColores[i][4], misColores[i][5]);
        Mat mascara;

        inRange(imgHSV, limiteInferior, limiteSuperior, mascara);

        Point puntoActual = obtenerPuntaContorno(mascara);
        if (puntoActual.x != 0 && puntoActual.y != 0) {
            puntosDibujados.push_back({ puntoActual.x, puntoActual.y, (int)i });
        }
    }
}

void dibujarEnLienzo(Mat img) {
    for (size_t i = 0; i < puntosDibujados.size(); i++) {
        circle(img, Point(puntosDibujados[i][0], puntosDibujados[i][1]), 10, valoresColores[puntosDibujados[i][2]], FILLED);
    }
}

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error al abrir la camara." << endl;
        return -1;
    }

    Mat frame;

    while (true) {
        cap.read(frame);
        if (frame.empty()) break;

        flip(frame, frame, 1);

        detectarColor(frame);
        dibujarEnLienzo(frame);

        imshow("Virtual Painter", frame);

        char key = (char)waitKey(1);
        if (key == 'q') {
            break;
        }
        else if (key == 'c') {
            puntosDibujados.clear();
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}