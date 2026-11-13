#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main() {
    
    VideoCapture cap(0);

    
    if (!cap.isOpened()) {
        cout << "Error: No se pudo conectar a la cámara." << endl;
        return -1;
    }

    Mat frame;
    cout << "Camara iniciada correctamente. Presiona 'q' en la ventana de video para salir..." << endl;

  
    while (true) {
        
        cap.read(frame);

        if (frame.empty()) {
            cout << "Fotograma vacío recibido. Saliendo..." << endl;
            break;
        }

        imshow("Proyecto 1 - Camara OpenCV", frame);

        if (waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();

    return 0;
}