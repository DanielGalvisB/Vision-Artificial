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

    Mat frame, gray, boxFiltered, medianFiltered;
    Mat gradX, gradY, absGradX, absGradY, sobelOutput;

    cout << "Iniciando Proyecto 3: Filtros Espaciales..." << endl;
    cout << "Presiona 'q' en cualquier ventana de video para salir." << endl;

    while (true) {
        cap.read(frame);
        if (frame.empty()) break;

  
        boxFilter(frame, boxFiltered, -1, Size(9, 9));

   
        medianBlur(frame, medianFiltered, 9);

        cvtColor(frame, gray, COLOR_BGR2GRAY);

        
        Sobel(gray, gradX, CV_16S, 1, 0, 3); 
        Sobel(gray, gradY, CV_16S, 0, 1, 3); 

      
        convertScaleAbs(gradX, absGradX);
        convertScaleAbs(gradY, absGradY);

  
        addWeighted(absGradX, 0.5, absGradY, 0.5, 0, sobelOutput);

        
        imshow("1. Original", frame);
        imshow("2. Filtro de Caja (Desenfocado)", boxFiltered);
        imshow("3. Filtro de Mediana", medianFiltered);
        imshow("4. Filtro Sobel (Bordes)", sobelOutput);

       
        if (waitKey(1) == 'q') {
            break;
        }
    }

    cap.release();
    destroyAllWindows();
    return 0;
}