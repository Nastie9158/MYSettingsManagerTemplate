#include "checkboxfactory.h"
#include <QCheckBox>

CheckBoxFactory::CheckBoxFactory(const bool defaultValue) : defaultValue_(defaultValue) {}

QWidget* CheckBoxFactory::create() const {
    QCheckBox* checkBox = new QCheckBox();
    checkBox->setChecked(defaultValue_);
    return checkBox;
}
