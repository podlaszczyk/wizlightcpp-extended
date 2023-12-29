
#ifndef WIZLIGHTCPP_LIGHTINTESITYWIDGETWITHSLIDER_H
#define WIZLIGHTCPP_LIGHTINTESITYWIDGETWITHSLIDER_H


#include <QWidget>
#include <QSlider>
#include "LightIntensityWidget.h"

class LightIntensityWidgetWithSlider : public QWidget {
Q_OBJECT

public:
    explicit LightIntensityWidgetWithSlider(QWidget *parent = nullptr);


private:
    LightIntensityWidget *lightIntensityWidget;
    QSlider *intensitySlider;

};


#endif //WIZLIGHTCPP_LIGHTINTESITYWIDGETWITHSLIDER_H
