#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    obrazek_sceny=NULL;
    wybrany_plik.clear();

}

MainWindow::~MainWindow()
{
    delete ui;
    //ui->setupUi(this);
}

void MainWindow::on_actionwyjdz_triggered()
{
    QMessageBox okienko_wyjscia(QMessageBox::Warning,"Koniec programu",
                                "Czy chcesz wyjsc z programu?",QMessageBox::Yes|QMessageBox::No);
    okienko_wyjscia.setButtonText(QMessageBox::Yes, "Tak");
    okienko_wyjscia.setButtonText(QMessageBox::No, "Nie");
    if(okienko_wyjscia.exec()==QMessageBox::Yes){
        QApplication::quit();

    }
}

void MainWindow::on_actionzapisz_triggered()
{
    if(obrazek_sceny!=NULL)
    {
        QString sciezka_pliku = QFileDialog::getSaveFileName(this,tr("Wybierz plik do zapisu"),
                                                             "",tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
        if(!sciezka_pliku.isEmpty())
        {
            QImage obrazek_do_zapisu = obrazek_sceny->pixmap().toImage();
            if (obrazek_do_zapisu.size().isValid()==true) obrazek_do_zapisu.save(sciezka_pliku);
        }

    }
    else
    {
        QMessageBox(QMessageBox::Information,"Blad",
                    "Nie ma zaladowanego pliku do zapisania",
                    QMessageBox::Ok).exec();
    }

}

void MainWindow::on_actionotworz_triggered()
{
    QString sciezka_pliku = QFileDialog::getOpenFileName(this, tr("Wybierz plik do otwarcia"),
                                                         "", tr("JPEG (*.jpg *.jpeg);;PNG (*.png)"));
    if (!sciezka_pliku.isEmpty())
    {
        wybrany_plik = sciezka_pliku;
        image = new QImage();
        image ->load(sciezka_pliku);

        obrazek = QPixmap::fromImage(*image);
        scena = new QGraphicsScene(this);
        obrazek_sceny = scena->addPixmap(obrazek);
        scena->setSceneRect(obrazek.rect());
        ui->graphicsView->setScene(scena);
        delete image;

    }
}

void MainWindow::on_actionpokaz_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::Mat obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str());
        cv::imshow("Obrazek bazowy", obrazek_bazowy);
    }

}


void MainWindow::on_actionPodzia_RGB_2_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::Mat obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str());
        cv::Mat pokaz_red = cv::imread(wybrany_plik.toStdString().c_str());
        cv::Mat pokaz_green = cv::imread(wybrany_plik.toStdString().c_str());
        cv::Mat pokaz_blue = cv::imread(wybrany_plik.toStdString().c_str());

        cv::Mat skladowa_red[3];
        cv::Mat skladowa_green[3];
        cv::Mat skladowa_blue[3];

        cv::split(obrazek_bazowy, skladowa_red);
        cv::split(obrazek_bazowy, skladowa_green);
        cv::split(obrazek_bazowy, skladowa_blue);

        skladowa_red[0] = cv::Mat::zeros(skladowa_red[1].size(), CV_8UC1);
        skladowa_red[1] = cv::Mat::zeros(skladowa_red[3].size(), CV_8UC1);
        skladowa_green[0] = cv::Mat::zeros(skladowa_green[2].size(), CV_8UC1);
        skladowa_green[2] = cv::Mat::zeros(skladowa_green[3].size(), CV_8UC1);
        skladowa_blue[1] = cv::Mat::zeros(skladowa_blue[1].size(), CV_8UC1);
        skladowa_blue[2] = cv::Mat::zeros(skladowa_blue[2].size(), CV_8UC1);

        cv::merge(skladowa_red, 3, pokaz_red);
        cv::merge(skladowa_green, 3, pokaz_green);
        cv::merge(skladowa_blue, 3, pokaz_blue);

        cv::imshow("Skladowa R", pokaz_red);
        cv::imshow("Skladowa G", pokaz_green);
        cv::imshow("Skladowa B", pokaz_blue);

    }
}



void MainWindow::on_actionPodzia_yCrCb_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::Mat obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str());
        cv::Mat konwersja_YCrCb;
        cv::Mat skladowe_YCrCb[3];

        cv::cvtColor(obrazek_bazowy, konwersja_YCrCb, cv::COLOR_BGR2YCrCb);
        cv::split(konwersja_YCrCb, skladowe_YCrCb);

        cv::imshow("Składowa Y", skladowe_YCrCb[0]);
        cv::imshow("Składowa Cr", skladowe_YCrCb[1]);
        cv::imshow("Składowa Cb", skladowe_YCrCb[2]);

    }

}


void MainWindow::on_actionPodzia_HSV_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::Mat obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str());
        cv::Mat konwersja_HSV;
        cv::Mat skladowe_HSV[3];

        cv::cvtColor(obrazek_bazowy, konwersja_HSV, cv::COLOR_BGR2HSV);
        cv::split(konwersja_HSV, skladowe_HSV);

        cv::imshow("Składowa H", skladowe_HSV[0]);
        cv::imshow("Składowa S", skladowe_HSV[1]);
        cv::imshow("Składowa V", skladowe_HSV[2]);

    }
}


void MainWindow::on_actionJasno_Kontrast_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::Mat obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str());
        cv::Mat obrazek_bazowy_new = cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());

        double beta = ui->brightnessSlider->value();
        double alfa = ui->contrastSlider->value();
        alfa/=100;

        a_Red = a_Green = a_Blue = alfa;
        b_Red = b_Green = b_Blue = beta;

        for(int x=0; x < obrazek_bazowy.cols; x++)
        {
            for(int y=0; y < obrazek_bazowy.rows; y++)
            {
                obrazek_bazowy_new.at<cv::Vec3b>(x, y)[0] = cv::saturate_cast<uchar>(a_Blue*(obrazek_bazowy.at<cv::Vec3b>(x, y)[0])+b_Blue);
                obrazek_bazowy_new.at<cv::Vec3b>(x, y)[1] = cv::saturate_cast<uchar>(a_Green*(obrazek_bazowy.at<cv::Vec3b>(x, y)[1])+b_Green);
                obrazek_bazowy_new.at<cv::Vec3b>(x, y)[2] = cv::saturate_cast<uchar>(a_Red*(obrazek_bazowy.at<cv::Vec3b>(x, y)[2])+b_Red);
            }
        }
        obrazek_bazowy.convertTo(obrazek_bazowy_new, -1, alfa, beta);
        cv::imshow("Jasność i kontrast RGB", obrazek_bazowy_new);
    }
}


void MainWindow::on_brightnessSlider_valueChanged(int value)
{
    alfa = value;
    on_actionJasno_Kontrast_triggered();
}


void MainWindow::on_contrastSlider_valueChanged(int value)
{
    beta = value;
    on_actionJasno_Kontrast_triggered();
}


