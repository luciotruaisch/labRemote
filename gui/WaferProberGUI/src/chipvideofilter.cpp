#include "chipvideofilter.h"

ChipVideoFilter::ChipVideoFilter(QAbstractVideoFilter *parent) :
    QAbstractVideoFilter(parent)
{

}

ChipVideoFilterRunnable::ChipVideoFilterRunnable(ChipVideoFilter *parent) :
m_parent(parent)
{

}

QVideoFrame ChipVideoFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags)
{
    Q_UNUSED(surfaceFormat)
    Q_UNUSED(flags)
    return *input;
}
