#ifndef AUTHWINDOW_H
#define AUTHWINDOW_H

#include "ViewportWindow.h"

#include <QWidget>
#include <QNetworkAccessManager>
#include "TevianApiWrapper.h"
namespace Ui {
	class AuthWindow;
}

class AuthWindow : public QWidget
{
	Q_OBJECT

public:
	AuthWindow(QWidget* parent = nullptr);
	~AuthWindow();

private slots:
	void onTokenGenerated(const QString& token);
	void onUserRegistered();
	void onProcessError(const QString& message);
	// ui slots
	void on_sign_in_button_clicked();
	void on_generate_token_button_clicked();
	void on_get_token_clicked();
private:
	Ui::AuthWindow* ui = nullptr;
	ViewportWindow* viewport_window = nullptr;
	// user credentials path
	QString auth_path = "auth.ini";
	QNetworkAccessManager* network_manager = nullptr;
	TevianApiWrapper* tevian_api_wrapper = nullptr;
};

#endif // AUTHWINDOW_H
