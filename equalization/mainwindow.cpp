#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <math.h>
#include <iostream>
#include <QFileDialog>
#include <QMessageBox>
#include <string>

#define L 256

using namespace cv;
using namespace std;

string mode = "webcam";
Mat chosenimg;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->display1->setScaledContents(true);
    ui->display2->setScaledContents(true);
    ui->display3->setScaledContents(true);
    ui->display4->setScaledContents(true);

    ui->display1->setFixedSize( QLWidth, QLHeight );
    ui->display2->setFixedSize( QLWidth, QLHeight );
    ui->display3->setFixedSize( QLWidth, QLHeight );
    ui->display4->setFixedSize( QLWidth, QLHeight );

    cap.open(0);

    // Call update every 100ms
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(333);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::update()
{
    Mat capture;
    if(mode == "webcam"){
        cap >> capture;
    }
    else{
        capture = chosenimg;
    }


    cvtColor(capture, capture, COLOR_BGR2GRAY);

    auto equalized = Histogram::equalizeHistogram(capture);

    double* inputHistogram = Histogram::calculateHistogram(capture);
    double* inputPdf = Histogram::calculatePdf(inputHistogram, capture.rows * capture.cols);

    double* outputHistogram = Histogram::calculateHistogram(equalized);
    double* outputPdf = Histogram::calculatePdf(outputHistogram, equalized.rows * equalized.cols);

    QImage img1 = MatToQImage(capture);
    QImage img2 = MatToQImage(equalized);
    QImage img3 = MatToQImage(dip::drawHistogram(inputPdf, L));
    QImage img4 = MatToQImage(dip::drawHistogram(outputPdf, L));


    ui->display1->setPixmap(QPixmap::fromImage(img1));
    ui->display2->setPixmap(QPixmap::fromImage(img2));
    ui->display3->setPixmap(QPixmap::fromImage(img3));
    ui->display4->setPixmap(QPixmap::fromImage(img4));
}

void webcam(){

}

void MainWindow::on_imgpcbtn_clicked()
{


    QString fileName;

    fileName = QFileDialog::getOpenFileName(this, "Open Image", ".", "Image Files(*.png *.jpg *.jpeg)");

    if (!fileName.isEmpty()){
        chosenimg = cv::imread(fileName.toLatin1().data());

        mode = "img";
        cap.open(1);

    }

}


void MainWindow::on_webcambtn_clicked()
{
    mode = "webcam";
    cap.open(0);

}


void MainWindow::on_infobtn_clicked()
{
    QMessageBox::information(this, "Step by step of equalizeHistogram function found in histogram.cpp",
                             "1. Calculate histogram \n"
                             "2. Extract probability density function(PDF) from the histogram"
                             "3. Cumulate probability function(CDF) from PDF"
                             "4. Map old image intenstity values to the new image");
}
