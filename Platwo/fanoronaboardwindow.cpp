#include "fanaronaboardwindow.h"
#include "ui_fanaronaboardwindow.h"

FanaronaBoardWindow::FanaronaBoardWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FanaronaBoardWindow)
{
    ui->setupUi(this);
}

FanaronaBoardWindow::~FanaronaBoardWindow()
{
    delete ui;
}
