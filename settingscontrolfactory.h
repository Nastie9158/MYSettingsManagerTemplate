#ifndef SETTINGSCONTROLFACTORY_H
#define SETTINGSCONTROLFACTORY_H

#include <QWidget>

class SettingsControlFactory {
public:
    virtual ~SettingsControlFactory() = default;
    virtual QWidget* create() const = 0;
};

#endif
