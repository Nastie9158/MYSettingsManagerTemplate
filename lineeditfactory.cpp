#include "lineeditfactory.h"
#include <QLineEdit>

LineEditFactory::LineEditFactory(const QString& defaultText) : defaultText_(defaultText){}

QWidget* LineEditFactory::create() const {
    QLineEdit* lineEdit = new QLineEdit();
    lineEdit->setPlaceholderText(defaultText_);
    return lineEdit;
}
