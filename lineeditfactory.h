#ifndef LINEEDITFACTORY_H
#define LINEEDITFACTORY_H

#include <QWidget>
#include "settingscontrolfactory.h"

class LineEditFactory : public SettingsControlFactory
{
public:
    LineEditFactory(const QString& defaultText);
    QWidget* create() const override;
private:
    QString defaultText_;
};

#endif
