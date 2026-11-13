#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cout << "Error: No se pudo conectar a la cámara." << endl;
        return -1;
    }

    
    Size patternSize(7, 5);
    Mat frame, gray;
    vector<Point2f> corners;

    cout << "Buscando tablero de ajedrez... Presiona 'q' para salir." << endl;

    while (true) {
        cap.read(frame);
        if (frame.empty()) break;

        
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        bool found = findChessboardCorners(gray, patternSize, corners,
            CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);

        
        if (found) {
            cout << "¡Tablero encontrado!" << endl;

        
            cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
                TermCriteria(TermCriteria::EPS + TermCriteria::MAX_ITER, 30, 0.1));

        
            drawChessboardCorners(frame, patternSize, corners, found);
        }

     
        imshow("Proyecto 2 - Deteccion de Tablero", frame);

        if (waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}