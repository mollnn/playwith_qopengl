#include "glwidget.h"
#include <QApplication>
#include <QTextEdit>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>

void SetVSync(int param)
{
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setSwapInterval(param);
    QSurfaceFormat::setDefaultFormat(surfaceFormat);
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    SetVSync(1); // vsync demo: modify the param here (0-no sync, k-refresh per k vsignal)

    QWidget *pWindow = new QWidget;

    QSlider * pctlDeltaWidth= new QSlider(Qt::Horizontal);
    pctlDeltaWidth->setMinimum(0);
    pctlDeltaWidth->setMaximum(20);
    pctlDeltaWidth->setValue(1);

    QLabel *pctlLabelDeltaWidth = new QLabel(QString("Delta Width:"));

    GlWidget *pGlWidget = new GlWidget;
    QGridLayout *pGridLayout = new QGridLayout;

    pGridLayout->addWidget(pGlWidget, 0, 0, 2,2);
    pGridLayout->addWidget(pctlLabelDeltaWidth,2,0,1,1);
    pGridLayout->addWidget(pctlDeltaWidth,2,1,1,1);

    pWindow->setLayout(pGridLayout);

    pGlWidget->setFixedSize(1920,1200);

    QObject::connect(pctlDeltaWidth, &QSlider::valueChanged, [&](){
        pGlWidget->setDeltaWidth(pctlDeltaWidth->value());
    });

    pWindow->show();

    return a.exec();
}
