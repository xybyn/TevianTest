#include "ProgressBarWorker.h"
#include <QThread>
void ProgressBarWorker::stop()
{
	stopped = true;
}
void ProgressBarWorker::complete()
{
    completed = true;
}
void ProgressBarWorker::onProcess()
{
	float delay = mseconds / 100.0f;
	for (int i = 0; i <= 100; i++)
	{
		if (stopped)
		{
            emit on_progress(0);
			emit on_finished();
			return;
        }
        else if(completed)
        {
            emit on_progress(100);
            emit on_finished();
            return;
        }
		emit on_progress(i);
		QThread::msleep(delay);
	}
	emit on_finished();
}
