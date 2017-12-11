#include "ObjectDetection.h"
#include "OpenCVHelper.h"

#include <vector>

#include <QDebug>

Q_DECLARE_METATYPE(cv::Mat)
ObjectDetection::ObjectDetection(QObject *parent) : QObject(parent)
{
    m_sourceLocation.reserve(4);
}

void ObjectDetection::setSourceImage(QVariant input) {
    if(input.canConvert<cv::Mat>()){
        OpenCVHelper::SIFT_obj_identify(m_object, input.value<cv::Mat>(), m_sourceLocation);
    } else {
        qInfo()<< "input is not a Mat";
    }
}

void ObjectDetection::dstImage(QVariant dstImage) {
    if(dstImage.canConvert<cv::Mat>()){
        std::vector<cv::Point2f> m_dstLocation(4);
        OpenCVHelper::SIFT_obj_identify(m_object, dstImage.value<cv::Mat>(), m_dstLocation);
        //only return the differences in left-top corner
        float dx = m_dstLocation[0].x - m_sourceLocation[0].x;
        float dy = m_dstLocation[0].y - m_sourceLocation[0].y;
        emit correctionGenerated(dx, dy);
    } else {
        qInfo()<< "input is not a Mat";
    }
}

void ObjectDetection::setObjectName(QString objectName)
{
        m_objectName = objectName;
        m_object = cv::imread( m_objectName.toLatin1().data(),
                               cv::IMREAD_GRAYSCALE);
        qInfo()<< objectName << " is loaded";
        emit objectNameChanged();
}
