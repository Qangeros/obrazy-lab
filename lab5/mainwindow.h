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

private slots:
    void on_actionwyjdz_triggered();

    void on_actionzapisz_triggered();

    void on_actionotworz_triggered();

    void on_actionpokaz_triggered();


    void on_actionPodzia_RGB_2_triggered();

    void on_actionPodzia_HSV_triggered();

    void on_actionPodzia_yCrCb_triggered();

private:
    Ui::MainWindow *ui;
    QImage *image;
    QPixmap obrazek;
    QGraphicsScene *scena;
    QGraphicsPixmapItem *obrazek_sceny;
    QString wybrany_plik;
};

#endif // MAINWINDOW_H
