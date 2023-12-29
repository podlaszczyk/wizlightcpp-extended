// src/mainwindow.cpp
#include <QHBoxLayout>
#include <QVBoxLayout>
#include "mainwindow.h"
#include "LightIntesityWidgetWithSlider.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent, Qt::Window | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint) {
    label = new QLabel("Click the button", this);
    label->setAlignment(Qt::AlignCenter);
    setCentralWidget(label);

    buttonAllOn = new QPushButton("Turn of All Bulbs", this);
    connect(buttonAllOn, &QPushButton::clicked, this, &MainWindow::turnOnAllBulbs);

    buttonAllOff = new QPushButton("Turn off All Bulbs", this);
    connect(buttonAllOff, &QPushButton::clicked, this, &MainWindow::turnOffAllBulbs);

    buttonfindAllBulbs = new QPushButton("Find All Bulbs", this);
    connect(buttonfindAllBulbs, &QPushButton::clicked, this, &MainWindow::findAllBulbs);

    setFixedSize(300, 300);
    setMinimumSize(200, 100);
    setMaximumSize(800, 400);
    setWindowTitle("Wiz Bulbs");

    intensityWidget = new LightIntensityWidgetWithSlider();
    intensityWidget2 = new LightIntensityWidgetWithSlider();
    intensityWidget3 = new LightIntensityWidgetWithSlider();

    // Add the button to the layout
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(buttonAllOn);
    layout->addWidget(buttonAllOff);
    layout->addWidget(buttonfindAllBulbs);
    layout->addWidget(label);
    layout->addWidget(intensityWidget);
    layout->addWidget(intensityWidget2);
    layout->addWidget(intensityWidget3);


    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);


}

MainWindow::~MainWindow() {
    // Destructor implementation
//    delete button;
}

void MainWindow::turnOffAllBulbs() {
    label->setText(QString("Turned Off"));
    wizControl.turnOffAll();
}

void MainWindow::turnOnAllBulbs() {
    label->setText(QString("Turned On"));
    wizControl.turnOnAll();
}

void MainWindow::findAllBulbs() {
    auto ips = wizControl.findAllBulbs();
    std::string ipsString;
    for (const auto& ip : ips)
    {
        ipsString.append(ip);
        ipsString.append("\n");
    }
    label->setText(QString(ipsString.c_str()));
}
