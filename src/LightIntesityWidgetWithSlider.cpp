#include "LightIntesityWidgetWithSlider.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QVBoxLayout>


LightIntensityWidgetWithSlider::LightIntensityWidgetWithSlider(QWidget *parent)
        : QWidget(parent) {

    intensitySlider = new QSlider(Qt::Horizontal);
    lightIntensityWidget = new LightIntensityWidget();
    QObject::connect(intensitySlider, &QSlider::valueChanged, lightIntensityWidget, &LightIntensityWidget::setIntensity);

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(lightIntensityWidget);
    layout->addWidget(intensitySlider);

    setLayout(layout);
}

