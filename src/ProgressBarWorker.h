#ifndef PROGRESSBARTHREAD_H
#define PROGRESSBARTHREAD_H
#include <QObject>
class ProgressBarWorker : public QObject
{
	Q_OBJECT
public slots:
	void onProcess();
    void complete();
    void stop();
signals:
	void on_finished();
	void on_progress(int i);

private:
	bool stopped = false;
    bool completed = false;
	const float mseconds = 5000.0f; // the time this worker will run
};

#endif // PROGRESSBARTHREAD_H
