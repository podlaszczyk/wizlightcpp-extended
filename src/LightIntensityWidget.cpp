// LightIntensityWidget.cpp
#include "LightIntensityWidget.h"
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QVBoxLayout>

LightIntensityWidget::LightIntensityWidget(QWidget *parent)
        : QWidget(parent), intensity(50) {

//    intensitySlider = new QSlider(Qt::Horizontal);
//    QObject::connect(intensitySlider, &QSlider::valueChanged, this, &LightIntensityWidget::setIntensity);
//
//    QVBoxLayout *layout = new QVBoxLayout;
//
//    layout->addWidget(intensitySlider);
//
//    setLayout(layout);
}

void LightIntensityWidget::setIntensity(int intensity) {
    // Ensure the intensity is within valid range (0 to 100)
    this->intensity = qBound(0, intensity, 100);

    // Trigger a repaint to update the widget
    update();
}

void LightIntensityWidget::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);

    QPainter painter(this);

    // Draw background
//    painter.fillRect(rect(), Qt::white);

    // Draw bar representing light intensity
    qreal barWidth = (qreal)width() * intensity / 100.0;
    QRectF barRect(0, 0, barWidth, height());

    // Calculate color based on intensity
    QColor color = QColor(255, 255, 0);  // Yellow color
    color = color.lighter( intensity ); // Adjust brightness based on intensity

    // Draw circle representing light intensity
    int circleDiameter = qMin(width(), height());
    QRectF circleRect((width() - circleDiameter) / 2.0, (height() - circleDiameter) / 2.0, circleDiameter, circleDiameter);

    QBrush circleBrush(color);
    painter.setBrush(circleBrush);
    painter.drawEllipse(circleRect);



}
