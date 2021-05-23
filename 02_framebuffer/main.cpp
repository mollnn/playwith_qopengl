#include "glwidget.h"
#include <QApplication>
#include <QTextEdit>
#include <QGridLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget *pWindow = new QWidget;

    GlWidget *pGlWidget = new GlWidget;

    QGridLayout *pGridLayout = new QGridLayout;

    pGridLayout->addWidget(pGlWidget, 0, 0, 2, 2);

    pWindow->setLayout(pGridLayout);

    pWindow->setSizeIncrement(100,100);

    pWindow->show();

    return a.exec();
}
