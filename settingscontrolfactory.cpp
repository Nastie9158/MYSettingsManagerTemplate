#include <QWidget>
#include "settingscontrolfactory.h"

class SettingsControlFactory {
public:
    virtual ~SettingsControlFactory() {}
    virtual QWidget* create() const = 0;
};
