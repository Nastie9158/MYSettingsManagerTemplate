#include "comboboxfactory.h"
#include <QComboBox>

ComboBoxFactory::ComboBoxFactory(const QString& defaultValue, const QStringList& options) : defaultValue_(defaultValue), options_(options) {}

QWidget* ComboBoxFactory::create() const {
    QComboBox* comboBox = new QComboBox();
    comboBox->addItems(options_);
    comboBox->setCurrentText(defaultValue_);
    return comboBox;
}
