#ifndef OBJECTDETECTION_H
#define OBJECTDETECTION_H

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

// This class finds the object provided by the objectName in a "reference" image (or "source image")
// and save the coordinates of the matched object in the reference image.
// This class then finds the object in a "destination" image (or a new image).
// and emit the difference of the coordinates of the matched object between reference image and destination image.
// This difference is then used to correct the destination image.

#include <QObject>
#include <QVariant>

class ObjectDetection : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString objectName READ objectName WRITE setObjectName NOTIFY objectNameChanged)
public:
    explicit ObjectDetection(QObject *parent = nullptr);

    // source image. all "destination" image will be matched to it.
    Q_INVOKABLE void setSourceImage(QVariant input);

    // provide a new destination image; it will emit a "correctionGenerated" signal
    Q_INVOKABLE void dstImage(QVariant dstImage);

public:
    QString objectName() {return m_objectName;}
    void setObjectName(QString objectName);

signals:
    void correctionGenerated(float dx, float dy);
    void objectNameChanged();

public slots:

public:
    static void SIFT_obj_identify(const cv::Mat& img1, const cv::Mat& img2, std::vector<cv::Point2f>& matchedCorners);
private:
    QString m_objectName;
    cv::Mat m_object;
    std::vector<cv::Point2f> m_sourceLocation;
};

#endif // OBJECTDETECTION_H
