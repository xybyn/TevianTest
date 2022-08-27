#ifndef VIEWPORTWINDOW_H
#define VIEWPORTWINDOW_H

#include "TevianApiWrapper.h"

#include <QMainWindow>
#include <QImage>
#include <QNetworkAccessManager>
#include "ProgressBarWorker.h"
namespace Ui {
	class ViewportWindow;
}

class ViewportWindow : public QMainWindow
{
	Q_OBJECT

public:
	ViewportWindow(const QString& token, TevianApiWrapper* manager, QWidget* parent = nullptr);
	~ViewportWindow();

private slots:
	void onProcessError(const QString& error_message);
	void onProcessFinished(const QVector<QRect>& bounds, const QVector<QRect>& landmarks, const QVector<TevianApiWrapper::MaskData>& masks);

    // ui slots
    void on_process_button_clicked();
    void on_open_image_button_clicked();
private:
	void on_progress(int value);

	// loads image and set to viewport
	void load_image(const QString& path);
	void start_progress_bar_worker();

	QString image_path;
	QString token;

	Ui::ViewportWindow* ui;
	QImage* image = nullptr;
	QNetworkAccessManager* network_manager = nullptr;
	TevianApiWrapper* tevian_api_wrapper = nullptr;
	ProgressBarWorker* worker = nullptr;
	QThread* thread = nullptr;
};

#endif // VIEWPORTWINDOW_H
