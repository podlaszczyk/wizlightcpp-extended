#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include "wizcontrol.h"
#include "LightIntesityWidgetWithSlider.h"
#include <QSlider>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void turnOffAllBulbs();
    void turnOnAllBulbs();
    void findAllBulbs();

private:
    QLabel *label;
    QPushButton *buttonAllOn;
    QPushButton *buttonAllOff;
    QPushButton *buttonfindAllBulbs;
    LightIntensityWidgetWithSlider *intensityWidget;
    LightIntensityWidgetWithSlider *intensityWidget2;
    LightIntensityWidgetWithSlider *intensityWidget3;

    WizControl wizControl;

};

#endif // MAINWINDOW_H
