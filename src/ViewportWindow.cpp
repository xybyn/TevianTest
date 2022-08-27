#include <QBuffer>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>
#include "ViewportWindow.h"
#include "ui_ViewportWindow.h"
#include "ProgressBarWorker.h"
ViewportWindow::ViewportWindow(const QString& token, TevianApiWrapper* tevian_api, QWidget* parent) :
	QMainWindow(parent),
	ui(new Ui::ViewportWindow), token(token),
	tevian_api_wrapper(tevian_api)
{
	ui->setupUi(this);
	connect(tevian_api_wrapper, &TevianApiWrapper::on_detected, ui->viewport, &Viewport::onProcessFinished);
	connect(tevian_api_wrapper, &TevianApiWrapper::on_detected, this, &ViewportWindow::onProcessFinished);
	connect(tevian_api_wrapper, &TevianApiWrapper::on_process_error, this, &ViewportWindow::onProcessError);
}

ViewportWindow::~ViewportWindow()
{
	qDebug() << "~ViewportWindow";
	delete image;
	delete ui;
}



void ViewportWindow::on_progress(int value)
{
	ui->progress_bar->setValue(value);
}

void ViewportWindow::load_image(const QString& file_name)
{
	ui->image_path->setText(file_name);
	delete image;
	image = new QImage(file_name);
	ui->viewport->setBackgroundImage(image);
	ui->viewport->repaint();
	ui->viewport->clear();
	image_path = file_name;
}

void ViewportWindow::start_progress_bar_worker()
{
	worker = new ProgressBarWorker();
	thread = new QThread();
	worker->moveToThread(thread);
	connect(thread, &QThread::started, worker, &ProgressBarWorker::onProcess);
	connect(worker, &ProgressBarWorker::on_finished, thread, &QThread::quit);
	connect(worker, &ProgressBarWorker::on_finished, worker, &ProgressBarWorker::deleteLater);
	connect(worker, &ProgressBarWorker::on_progress, this, &ViewportWindow::on_progress);
	thread->start();
}



void ViewportWindow::onProcessError(const QString& error_message)
{
	worker->stop();
	ui->progress_bar->setValue(0);
	QMessageBox messageBox;
	messageBox.critical(0, "Error", error_message);
}

void ViewportWindow::onProcessFinished(const QVector<QRect>& bounds, const QVector<QRect>& landmarks, const QVector<TevianApiWrapper::MaskData>& masks)
{
	Q_UNUSED(bounds);
	Q_UNUSED(landmarks);
	Q_UNUSED(masks);
    worker->complete();
	ui->progress_bar->setValue(100);
	worker = nullptr; // already destroyed
	thread = nullptr;
}



void ViewportWindow::on_process_button_clicked()
{
    QFile* file = new QFile(image_path);
    if (!file->open(QIODevice::ReadOnly))
    {
        onProcessError("File is not open!");
        return;
    }
    start_progress_bar_worker();
    tevian_api_wrapper->processImage(file->readAll(), token);
    file->close();
    delete file;
}


void ViewportWindow::on_open_image_button_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"),
        "C:\\",
        tr("Images (*.jpg)"));
    load_image(file_name);
}

