#ifndef SPINBOXFACTORY_H
#define SPINBOXFACTORY_H

#include "settingscontrolfactory.h"

class SpinBoxFactory : public SettingsControlFactory {
public:
    SpinBoxFactory(const int defaultValue, const int numFrom, const int numTo);
    QWidget* create() const override;

private:
    int defaultValue_;
    int numFrom_;
    int numTo_;
};

#endif
