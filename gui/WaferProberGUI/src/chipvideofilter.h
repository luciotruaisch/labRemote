#ifndef CHIPFILTER_H
#define CHIPFILTER_H

#include <QVideoFilterRunnable>
#include <QAbstractVideoFilter>

class ChipVideoFilter;

class ChipVideoFilterRunnable : public QVideoFilterRunnable {
public:
    explicit ChipVideoFilterRunnable(ChipVideoFilter* parent);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);

private:
    // needed for actual implemntation.
    ChipVideoFilter* m_parent;
};

class ChipVideoFilter : public QAbstractVideoFilter
{
public:
    explicit ChipVideoFilter(QAbstractVideoFilter* parent = 0);
    QVideoFilterRunnable* createFilterRunnable();

signals:
    void finished(QObject* result);
};

#endif // CHIPFILTER_H
