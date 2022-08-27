#ifndef TEVIANAPIWRAPPER_H
#define TEVIANAPIWRAPPER_H

#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPoint>
class TevianApiWrapper : public QObject
{
	Q_OBJECT
private:
	const QString API_URL_PATH = QString("https://backend.facecloud.tevian.ru/api/v1");

public:
	struct MaskData
	{
		QPoint position;
		double full_face_mask;
		double lower_face_mask;
		double no_mask;
		double other_mask;
	};
	TevianApiWrapper(QNetworkAccessManager* manager)
		: manager(manager) {}
	~TevianApiWrapper();
	void registerNewUser(const QString& email, const QString& password);
	void login(const QString& email, const QString& password);
	void processImage(const QByteArray& image_bytes, const QString& token);
	QString getDetectUrlPath() const { return QString("%1/detect?demographics=true&landmarks=true&masks=true").arg(API_URL_PATH); }
	QString getRegisterUrlPath() const { return QString("%1/users").arg(API_URL_PATH); }
	QString getLoginUrlPath() const { return QString("%1/login").arg(API_URL_PATH); }
public slots:
	void onProcessFinished(QNetworkReply* rep);
signals:
	void on_detected(const QVector<QRect>& bounds, const QVector<QRect>& landmarks, const QVector<TevianApiWrapper::MaskData>& masks);
	void on_token_generated(const QString& token);
	void on_registered();
	void on_process_error(const QString& error_message);
private:
	enum class REQUEST_TYPE 
	{
		DETECT,
		REGISTER,
		LOGIN,
		NONE
	} type = REQUEST_TYPE::NONE;
	QNetworkAccessManager* manager = nullptr;
};

#endif // TEVIANAPIWRAPPER_H
