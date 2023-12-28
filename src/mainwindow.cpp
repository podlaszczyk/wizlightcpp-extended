// src/mainwindow.cpp
#include <QHBoxLayout>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    label = new QLabel("Click the button", this);
    label->setAlignment(Qt::AlignCenter);
    setCentralWidget(label);

    button = new QPushButton("Click me", this);
    connect(button, &QPushButton::clicked, this, &MainWindow::changeLabelText);

    setFixedSize(300, 150);
    setWindowTitle("Qt Widget Example");

    // Add the button to the layout
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(button);
    layout->addWidget(label);

    QWidget *centralWidget = new QWidget;
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {
    // Destructor implementation
    delete button;
}

void MainWindow::changeLabelText() {
    static int clickCount = 0;
    label->setText(QString("Button clicked %1 times").arg(++clickCount));
}
