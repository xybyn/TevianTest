#include "Viewport.h"

#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <algorithm>
Viewport::Viewport(QWidget* parent)
	: QWidget{ parent }
{

}
Viewport::~Viewport()
{
	qDebug() << "~Viewport";
}
namespace
{
	float quint(float x)
	{
		return x * x * x * x * x;
	}
}
void Viewport::wheelEvent(QWheelEvent* event)
{
	float diff = event->angleDelta().y() / 128.0f / 32.0f;
	zoom_factor += diff;
	zoom_factor = std::min(1.0f, std::max(0.2f, zoom_factor));
	repaint();
}

void Viewport::paintEvent(QPaintEvent* e)
{
	Q_UNUSED(e);
	QPainter painter(this);
	if (image != nullptr)
	{
		QPoint center_of_viewport = QPoint(this->width() / 2, this->height() / 2) * pan_transform.inverted();
		// for a smooth zooming
		float quintk = quint(zoom_factor);
		scale = (1.0 - quintk) * min_scale + quintk * max_scale;
		QTransform scaling_transform;
		scaling_transform.translate(center_of_viewport.x(), center_of_viewport.y());
		scaling_transform.scale(scale, scale);
		scaling_transform.translate(-center_of_viewport.x(), -center_of_viewport.y());

		painter.setTransform(scaling_transform * pan_transform);
		QPixmap pixmap = QPixmap::fromImage(*image);
		painter.drawPixmap(QPoint(0, 0), pixmap);

		QPen current_pen = painter.pen();
		current_pen.setWidth(current_pen.width() * scale);

		current_pen.setColor(Qt::blue);
		painter.setPen(current_pen);
		painter.drawRects(bounds);

		current_pen.setColor(Qt::red);
		painter.setPen(current_pen);
		painter.drawRects(landmarks);

		painter.setPen(Qt::green);
		QFont font = painter.font();
		constexpr int initial_font_size = 12;
		int new_font_size = initial_font_size / scale;
		font.setPixelSize(new_font_size);
		painter.setFont(font);

		for (int i = 0; i < masks.size(); ++i)
		{
			painter.drawText(masks[i].position + QPoint(0, new_font_size * 1), QString("full_face_mask: ") + QString::number(masks[i].full_face_mask));
			painter.drawText(masks[i].position + QPoint(0, new_font_size * 2), QString("lower_face_mask: ") + QString::number(masks[i].lower_face_mask));
			painter.drawText(masks[i].position + QPoint(0, new_font_size * 3), QString("no_mask: ") + QString::number(masks[i].no_mask));
			painter.drawText(masks[i].position + QPoint(0, new_font_size * 4), QString("other_mask: ") + QString::number(masks[i].other_mask));
		}
	}
}

void Viewport::mousePressEvent(QMouseEvent* event)
{
	prev_mouse_x = event->x();
	prev_mouse_y = event->y();
}

void Viewport::mouseReleaseEvent(QMouseEvent* event)
{
	pan(0, 0);
}

void Viewport::mouseMoveEvent(QMouseEvent* event)
{
	int current_x = event->x();
	int current_y = event->y();

	int diff_x = current_x - prev_mouse_x;
	int diff_y = current_y - prev_mouse_y;

	pan(diff_x, diff_y);

	prev_mouse_x = current_x;
	prev_mouse_y = current_y;

	repaint();
}

void Viewport::onProcessFinished(const QVector<QRect>& bounds, const QVector<QRect>& landmarks, const QVector<TevianApiWrapper::MaskData>& masks)
{
	this->bounds = bounds;
	this->landmarks = landmarks;
	this->masks = masks;

	repaint();
}

void Viewport::pan(int dx, int dy)
{
	pan_transform.translate(dx / scale, dy / scale);
}

void Viewport::clear()
{
	bounds.clear();
	landmarks.clear();
}
