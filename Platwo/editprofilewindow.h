#ifndef EDITPROFILEWINDOW_H
#define EDITPROFILEWINDOW_H
#include <QWidget>
#include "videobackgroundwidget.h"
namespace Ui {
class EditProfileWindow;
}

class EditProfileWindow : public QWidget {
    Q_OBJECT

public:
    explicit EditProfileWindow(QWidget *parent = nullptr);
    ~EditProfileWindow();

private slots:
    void on_saveButton_clicked();
    void on_backButton_clicked();

private:
   Ui::EditProfileWindow *ui;
   void loadUserData();
   bool validateInput();

   VideoBackgroundWidget *videoBackground;
protected:
   void resizeEvent(QResizeEvent *event) override;

};

#endif // EDITPROFILEWINDOW_H