#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "TevianApiWrapper.h"

#include <QTransform>
#include <QWidget>

class Viewport : public QWidget
{
	Q_OBJECT
public:
	Viewport(QWidget* parent = nullptr);
	~Viewport();
	void wheelEvent(QWheelEvent* event) override;
	void paintEvent(QPaintEvent*) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void setBackgroundImage(const QImage* image) { this->image = image; }
	// panoraming
	void pan(int dx, int dy);
	void clear();
public slots:
	void onProcessFinished(const QVector<QRect>& bounds, const QVector<QRect>& landmarks, const QVector<TevianApiWrapper::MaskData>& masks);
private:
	const QImage* image = nullptr;
	QTransform pan_transform;
	QVector<QRect> bounds, landmarks;
	QVector<TevianApiWrapper::MaskData> masks;

	int prev_mouse_x = 0, prev_mouse_y = 0;

	float scale = 1.0f;
	float min_scale = 0.125f;
	float max_scale = 4.0f;
	float zoom_factor = 0.5f;
};

#endif // VIEWPORT_H
