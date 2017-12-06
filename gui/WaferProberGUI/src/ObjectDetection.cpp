#include "ObjectDetection.h"
#include "OpenCVHelper.h"

#include <vector>

#include <QDebug>

Q_DECLARE_METATYPE(cv::Mat)
ObjectDetection::ObjectDetection(QObject *parent) : QObject(parent)
{
    // const char* input_template = "/Users/xju/Documents/2017/RD53/code/labRemote/image_process/jupyter/selfied_template.png";
    const char* input_template = "/home/pixel/Documents/probing_station/code/labRemote/gui/WaferProberGUI/images/RD53A_template_less.png";
    // const char* input_template = "/home/pixel/Documents/probing_station/code/labRemote/gui/WaferProberGUI/images/RD53A_template_new.png";
    m_object = cv::imread( input_template,
                           cv::IMREAD_GRAYSCALE);
    m_sourceLocation.reserve(4);
    qInfo()<<"object is loaded";
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

double ObjectDetection::getMean(QVariant image)
{
    double res = -1;
    if(image.canConvert<cv::Mat>()){
        cv::Scalar mean_value = cv::mean(image.value<cv::Mat>());
        res = mean_value[0];
    } else {
        qInfo()<< "input is not a Mat";
    }
    return res;
}
