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


// lab 5

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


// lab 6

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

// lab 7

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

        for(int y=0; y < obrazek_bazowy.cols; y++)
        {
            for(int x=0; x < obrazek_bazowy.rows; x++)
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


// lab 8

static void Prog_callback(int v, void* p)
{
    MainWindow *img = (MainWindow*) p;
    img->Progowanie();
}

void MainWindow::Progowanie()
{
    cv::threshold(obrazek_bazowy, wynik, prog, 255,typ);
    cv::imshow("Progowanie", wynik );
}

void MainWindow::on_actionProgowanie_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str());
        cv::cvtColor(obrazek_bazowy, src_gray, cv::COLOR_BGR2GRAY);
        cv::namedWindow("Progowanie", cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Rodzaj progowania: \n 0: Binary \n 1: Binary Inverted \n 2: Truncate"
                           " \n 3: To Zero \n 4: To Zero Inverted",
                           "Progowanie", &typ,
                           max_typ, Prog_callback, this); // Create a Trackbar to choose type of Threshold
        cv::createTrackbar("Wartość progu", "Progowanie", &prog, 255, Prog_callback, this);
        Progowanie();
    }
}

// lab 9

static void Rotacja_callback(int v, void* p)
{
    MainWindow *img = (MainWindow*) p;
    img->Rotacja();
}

static void Skaluj_callback(int v, void* p)
{
    MainWindow *img = (MainWindow*) p;
    img->Skaluj();
}

void MainWindow::Rotacja()
{
    wynik = cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());
    punkt = cv::Point(obrazek_bazowy.rows/2., obrazek_bazowy.cols/2.);
    cv::Mat r = cv::getRotationMatrix2D(punkt, kat_obrotu, 1.);
    cv::warpAffine(obrazek_bazowy, wynik, r, obrazek_bazowy.size());
    cv::imshow("Rotacja", wynik);
}

void MainWindow::Skaluj()
{
    Skala_x = (wsp_skali/100);
    Skala_y = (wsp_skali/100);
    if(wsp_skali <= 50) wsp_skali = 50;
    wynik = cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());
    cv::resize(obrazek_bazowy, wynik, (obrazek_bazowy.size()*wsp_skali)/100, Skala_x, Skala_y, cv::INTER_LINEAR);
    cv::imshow("Skalowanie", wynik);

}

void MainWindow::on_actionRotacja_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str(),cv::IMREAD_COLOR);
        cv::namedWindow("Rotacja", cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Kat obrotu", "Rotacja",
                           &kat_obrotu, 360, Rotacja_callback, this);
        Rotacja();
    }
}

void MainWindow::on_actionSkalowanie_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str(),cv::IMREAD_COLOR);
        cv::namedWindow("Skalowanie", cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Wspol. Skali", "Skalowanie",
                           &wsp_skali, 200, Skaluj_callback, this);

        Skaluj();
    }
}


// lab 10


static void Usrednianie_callback(int v, void* p)
{
    MainWindow *img = (MainWindow*) p;
    img->Usrednianie();
}

static void Gauss_callback(int v, void* p)
{
    MainWindow *img = (MainWindow*) p;
    img->Gauss();
}


static void Mediana_callback(int v, void* p)
{
    MainWindow *img = (MainWindow*) p;
    img->Mediana();
}

void MainWindow::Usrednianie()
{
    obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str(),cv::IMREAD_COLOR);
    wynik = cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());
    wartosc = cv::getTrackbarPos("Wielkość okna", "Filtr usredniajacy");
    if (wartosc == 0) wartosc = 1;
    cv::blur(obrazek_bazowy, wynik, cv::Size(wartosc, wartosc));
    cv::imshow("Filtr usredniajacy", wynik );
}

void MainWindow::Gauss()
{
    obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str(),cv::IMREAD_COLOR);
    wynik = cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());
    wartosc = cv::getTrackbarPos("Wielkość okna", "Filtr Gaussa");
    if (wartosc == 0) wartosc = 1;
    if (wartosc % 2 == 0) wartosc++;
    cv::GaussianBlur(obrazek_bazowy, wynik, cv::Size(wartosc, wartosc), 0, 0);
    cv::imshow("Filtr Gaussa", wynik);
}


void MainWindow::Mediana()
{
    obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str(),cv::IMREAD_COLOR);
    wynik = cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());
    wartosc = cv::getTrackbarPos("Wielkość okna", "Filtr medianowy");
    if (wartosc == 0) wartosc = 1;
    if (wartosc % 2 == 0) wartosc++;
    cv::medianBlur(obrazek_bazowy, wynik, wartosc);
    cv::imshow("Filtr medianowy", wynik );
}


void MainWindow::on_actionFiltr_u_redniaj_cy_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::namedWindow("Filtr usredniajacy", cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Wielkość okna", "Filtr usredniajacy", &wielkosc_okna, 11, Usrednianie_callback, this);
        Usrednianie();
    }
}

void MainWindow::on_actionFiltr_Gaussa_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::namedWindow("Filtr Gaussa", cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Wielkość okna", "Filtr Gaussa", &wielkosc_okna, 11, Gauss_callback, this);
        Gauss();
    }
}

void MainWindow::on_actionFiltr_Medianowy_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::namedWindow("Filtr medianowy", cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Wielkość okna", "Filtr medianowy", &wielkosc_okna, 11, Mediana_callback, this);
        Mediana();
    }
}

// lab 11

static void Sobel_callback(int v, void* p)
{
    MainWindow *img = (MainWindow*) p;
    img->Sobel();
}

static void Prewitt_callback(int v, void* p)
{
    MainWindow *img = (MainWindow*) p;
    img->Prewitt();
}


static void Canny_callback(int v, void* p)
{
    MainWindow *img = (MainWindow*) p;
    img->Canny();
}


void MainWindow::Sobel()
{
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;


    obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str(),cv::IMREAD_COLOR);
    wynik = cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());

    cv::Sobel(obrazek_bazowy, grad_y, CV_16S, 1, 0, 3, 1, 0, CV_HAL_BORDER_REFLECT_101);
    cv::Sobel(obrazek_bazowy, grad_x, CV_16S, 0, 1, 3, 1, 0, CV_HAL_BORDER_REFLECT_101);

    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, wynik);
    cv::imshow("Filtr Sobela", wynik);
}



void MainWindow::Prewitt()
{
    cv::Mat grad_x, grad_y;
    cv::Mat abs_grad_x, abs_grad_y;
    obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str(),cv::IMREAD_COLOR);
    wynik = cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());
    cv::filter2D(obrazek_bazowy, grad_y, CV_16S, 0, cv::Point(-1, -1), 0, CV_HAL_BORDER_REFLECT_101);
    cv::filter2D(obrazek_bazowy, grad_x, CV_16S, 1, cv::Point(-1, -1), 0, CV_HAL_BORDER_REFLECT_101);

    cv::convertScaleAbs(grad_y, abs_grad_y);
    cv::convertScaleAbs(grad_x, abs_grad_x);
    cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, wynik);
    cv::imshow("Prewitta", wynik);

}

void MainWindow::Canny()
{
    obrazek_bazowy = cv::imread(wybrany_plik.toStdString().c_str(),cv::IMREAD_COLOR);
    wynik = cv::Mat::zeros(obrazek_bazowy.size(), obrazek_bazowy.type());
    cv::Canny(obrazek_bazowy, wynik, T_min, T_max, 3);
    cv::imshow("Filtr Canny'ego", wynik);

}

void MainWindow::on_actionFiltr_Sobela_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::namedWindow("Filtr Sobela", cv::WINDOW_AUTOSIZE);
        Sobel();
    }
}

void MainWindow::on_actionFiltr_Prewitta_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::namedWindow("Filtr Prewitta", cv::WINDOW_AUTOSIZE);
        Prewitt();
    }
}

void MainWindow::on_actionFiltr_Canny_ego_triggered()
{
    if(!wybrany_plik.isEmpty())
    {
        cv::namedWindow("Filtr Canny'ego", cv::WINDOW_AUTOSIZE);
        cv::createTrackbar("Próg T_min", "Filtr Canny'ego", &T_min, 100, Canny_callback, this);
        Canny();
    }
}













