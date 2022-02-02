#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QImage>
#include <QLabel>
#include <QTime>
#include <QMessageBox>
#include <QString>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QProgressBar>
#include <QPixmap>
#include <QDebug>
#include <QTimer>
#include <opencv2/opencv.hpp>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    double alfa, beta;
    double a_Red, a_Green, a_Blue;
    double b_Red, b_Green, b_Blue;

    void Progowanie();
    int typ = 3;
    int prog = 0;
    int const max_typ = 4;
    int const max_value = 255;
    int const max_binary_value = 255;
    cv::Mat obrazek_bazowy, src_gray, wynik;

    void Skaluj();
    void Rotacja();
    double kat, Skala_x, Skala_y;
    int wsp_skali = 100;
    int kat_obrotu = 0;
    cv::Point punkt;
    cv::Size rozmiar;


private slots:
    void on_actionwyjdz_triggered();

    void on_actionzapisz_triggered();

    void on_actionotworz_triggered();

    void on_actionpokaz_triggered();

    void on_actionPodzia_RGB_2_triggered();

    void on_actionPodzia_HSV_triggered();

    void on_actionPodzia_yCrCb_triggered();

    void on_brightnessSlider_valueChanged(int value);

    void on_contrastSlider_valueChanged(int value);

    void on_actionJasno_Kontrast_triggered();

    void on_actionProgowanie_triggered();

    void on_actionRotacja_triggered();

    void on_actionSkalowanie_triggered();


private:
    Ui::MainWindow *ui;
    QImage *image;
    QPixmap obrazek;
    QGraphicsScene *scena;
    QGraphicsPixmapItem *obrazek_sceny;
    QString wybrany_plik;
};

#endif // MAINWINDOW_H
