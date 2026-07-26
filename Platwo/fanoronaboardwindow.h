#ifndef FANARONABOARDWINDOW_H
#define FANARONABOARDWINDOW_H

#include <QWidget>

namespace Ui {
class FanaronaBoardWindow;
}

class FanaronaBoardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FanaronaBoardWindow(QWidget *parent = nullptr);
    ~FanaronaBoardWindow();

private:
    Ui::FanaronaBoardWindow *ui;
};

#endif // FANARONABOARDWINDOW_H
