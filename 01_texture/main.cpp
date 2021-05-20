#include "glwidget.h"
#include <QApplication>
#include <QTextEdit>
#include <QGridLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QWidget *pWindow = new QWidget;

    QTextEdit *pTextEdit = new QTextEdit;

    GlWidget *pGlWidget = new GlWidget;

    pGlWidget->setFixedHeight(720);

    pWindow->setFixedSize(1366, 768);

    QGridLayout *pGridLayout = new QGridLayout;

    pGridLayout->addWidget(pGlWidget, 0, 0, 2, 2);
    pGridLayout->addWidget(pTextEdit, 3, 0, 1, 2);

    pWindow->setLayout(pGridLayout);

    pWindow->show();

    return a.exec();
}
