#ifndef CHECKBOXFACTORY_H
#define CHECKBOXFACTORY_H

#include "settingscontrolfactory.h"

class CheckBoxFactory : public SettingsControlFactory {
public:
    CheckBoxFactory(const bool defaultValue);
    QWidget* create() const override;
private:
    bool defaultValue_;

};

#endif
