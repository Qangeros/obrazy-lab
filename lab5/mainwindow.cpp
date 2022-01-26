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

