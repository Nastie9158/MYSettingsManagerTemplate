#include "pushbuttonfactory.h"
#include <QPushButton>

PushButtonFactory::PushButtonFactory(const QString& buttonText) : buttonText_(buttonText){}

QWidget* PushButtonFactory::create() const {
    QPushButton* pushButton = new QPushButton();
    pushButton->setText(buttonText_);
    return pushButton;
}
