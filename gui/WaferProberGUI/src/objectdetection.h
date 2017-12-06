#ifndef OBJECTDETECTION_H
#define OBJECTDETECTION_H

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

#include <QObject>
#include <QVariant>

Q_DECLARE_METATYPE(cv::Mat)
class ObjectDetection : public QObject
{
    Q_OBJECT

public:
    explicit ObjectDetection(QObject *parent = nullptr);

    // source image. all "destination" image will be matched to it.
    Q_INVOKABLE void setSourceImage(QVariant input);

    // provide a new destination image; it will emit a "correctionGenerated" signal
    Q_INVOKABLE void dstImage(QVariant dstImage);

    // provide an image for focus studies
    Q_INVOKABLE void focusImage(QVariant image);

signals:
    void correctionGenerated(float dx, float dy);

public slots:

private:
    cv::Mat m_object;
    std::vector<cv::Point2f> m_sourceLocation;
};

#endif // OBJECTDETECTION_H
