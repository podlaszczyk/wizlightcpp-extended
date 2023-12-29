// LightIntensityWidget.h
#ifndef LIGHTINTENSITYWIDGET_H
#define LIGHTINTENSITYWIDGET_H

#include <QWidget>
#include <QSlider>

class LightIntensityWidget : public QWidget {
Q_OBJECT

public:
    explicit LightIntensityWidget(QWidget *parent = nullptr);

    void setIntensity(int intensity);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int intensity;

};

#endif // LIGHTINTENSITYWIDGET_H
