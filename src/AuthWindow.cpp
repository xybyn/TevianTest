#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include "AuthWindow.h"
#include "ui_AuthWindow.h"
AuthWindow::AuthWindow(QWidget* parent) :
	QWidget(parent),
	ui(new Ui::AuthWindow)
{
	ui->setupUi(this);

	QSettings auth_values(auth_path, QSettings::IniFormat);
	ui->email_line_edit->setText(auth_values.value("email").toString());
	ui->password_line_edit->setText(auth_values.value("password").toString());
	ui->token_text_edit->setText(auth_values.value("token").toString());

	network_manager = new QNetworkAccessManager(this);
	tevian_api_wrapper = new TevianApiWrapper(network_manager);

	connect(network_manager, &QNetworkAccessManager::finished, tevian_api_wrapper, &TevianApiWrapper::onProcessFinished);
	connect(tevian_api_wrapper, &TevianApiWrapper::on_registered, this, &AuthWindow::onUserRegistered);
	connect(tevian_api_wrapper, &TevianApiWrapper::on_token_generated, this, &AuthWindow::onTokenGenerated);
	connect(tevian_api_wrapper, &TevianApiWrapper::on_process_error, this, &AuthWindow::onProcessError);
}

AuthWindow::~AuthWindow()
{
	QSettings auth_values(auth_path, QSettings::IniFormat);
	auth_values.setValue("email", ui->email_line_edit->text());
	auth_values.setValue("password", ui->password_line_edit->text());
	auth_values.setValue("token", ui->token_text_edit->toPlainText());

	delete ui;
	delete viewport_window;
	delete tevian_api_wrapper;
	delete network_manager;
	qDebug() << "~AuthWindow";
}

void AuthWindow::onUserRegistered()
{
	tevian_api_wrapper->login(ui->email_line_edit->text(), ui->password_line_edit->text());
}

void AuthWindow::on_sign_in_button_clicked()
{
	viewport_window = new ViewportWindow(ui->token_text_edit->toPlainText(), tevian_api_wrapper);
	viewport_window->show();
	this->close();
	disconnect(tevian_api_wrapper, &TevianApiWrapper::on_process_error, this, &AuthWindow::onProcessError);
}


void AuthWindow::on_generate_token_button_clicked()
{
	tevian_api_wrapper->registerNewUser(ui->email_line_edit->text(), ui->password_line_edit->text());
}
void AuthWindow::onProcessError(const QString& error_message)
{
	QMessageBox messageBox;
	messageBox.critical(0, "Error", error_message);
}
void AuthWindow::onTokenGenerated(const QString& token)
{
	ui->token_text_edit->setText(token);
}


void AuthWindow::on_get_token_clicked()
{
	tevian_api_wrapper->login(ui->email_line_edit->text(), ui->password_line_edit->text());
}

