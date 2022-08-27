#include "TevianApiWrapper.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonArray>
#include <QRect>


TevianApiWrapper::~TevianApiWrapper()
{
	qDebug() << "~TevianApiWrapper";
}

void TevianApiWrapper::registerNewUser(const QString& email, const QString& password)
{
	type = REQUEST_TYPE::REGISTER;
	QUrl url(getRegisterUrlPath());
	QNetworkRequest request(url);
	request.setRawHeader("Content-Type", "application/json");
	QString data = QString("{\"billing_type\":\"demo\","
		"\"data\": { \"some_data\" : [\"\"]},"
		"\"email\":\"%1\",\"password\":\"%2\"}").arg(email, password);
	QByteArray bytes = data.toUtf8();
	request.setRawHeader("Content-Length", QString::number(bytes.size()).toUtf8());
	manager->post(request, bytes);

}

void TevianApiWrapper::login(const QString& email, const QString& password)
{
	type = REQUEST_TYPE::LOGIN;
	QUrl url(getLoginUrlPath());
	QNetworkRequest request(url);
	request.setRawHeader("Content-Type", "application/json");
	QString data = QString("{\"email\":\"%1\",\"password\":\"%2\"}").arg(email, password);
	QByteArray bytes = data.toUtf8();
	request.setRawHeader("Content-Length", QString::number(bytes.size()).toUtf8());
	manager->post(request, bytes);
}

void TevianApiWrapper::processImage(const QByteArray& image_bytes, const QString& token)
{
	type = REQUEST_TYPE::DETECT;
	QUrl url(getDetectUrlPath());
	QNetworkRequest request(url);
	request.setRawHeader("Authorization", ("Bearer " + token).toUtf8());
	request.setRawHeader("Content-Type", "image/jpeg");
	int size = image_bytes.size();
	request.setRawHeader("Content-Length", QString::number(size).toUtf8());
	manager->post(request, image_bytes);
}

void TevianApiWrapper::onProcessFinished(QNetworkReply* rep)
{
	QByteArray received_bytes = rep->readAll();
	QJsonDocument doc = QJsonDocument::fromJson(received_bytes);
	int status = doc["status_code"].toInt();
	if (status == 400 || status == 401)
	{
		emit on_process_error(doc["message"].toString());
		return;
	}
	if (type == REQUEST_TYPE::DETECT)
	{
		QVector<QRect> bounds;
		QVector<QRect> landmarks;
		QVector<MaskData> mask_datas;
		QJsonArray data = doc["data"].toArray();
		for (int i = 0; i < data.count(); i++)
		{
			auto bbox = doc["data"][i]["bbox"];
			int height = bbox["height"].toInt();
			int width = bbox["width"].toInt();
			int x = bbox["x"].toInt();
			int y = bbox["y"].toInt();
			bounds.append(QRect{ x, y, width, height });
			QJsonArray landmarks_data = doc["data"][i]["landmarks"].toArray();
			for (int j = 0; j < landmarks_data.count(); j++)
			{
				int x = doc["data"][i]["landmarks"][j]["x"].toInt();
				int y = doc["data"][i]["landmarks"][j]["y"].toInt();
				int w = 5;
				landmarks.append(QRect{ x - w / 2, y - w / 2, w, w });
			}

			double full_face_mask = doc["data"][i]["masks"]["full_face_mask"].toDouble();
			double lower_face_mask = doc["data"][i]["masks"]["lower_face_mask"].toDouble();
			double no_mask = doc["data"][i]["masks"]["no_mask"].toDouble();
			double other_mask = doc["data"][i]["masks"]["other_mask"].toDouble();
			mask_datas.append(MaskData{
								  QPoint(x, y),
								  full_face_mask,
								  lower_face_mask,
								  no_mask,
								  other_mask
				});
		}
		emit on_detected(bounds, landmarks, mask_datas);
	}
	else if (type == REQUEST_TYPE::REGISTER)
	{
		emit on_registered();
	}
	else if (type == REQUEST_TYPE::LOGIN)
	{
		emit on_token_generated(doc["data"]["access_token"].toString());
	}
}
