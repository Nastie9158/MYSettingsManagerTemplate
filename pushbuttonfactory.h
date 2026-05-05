#ifndef PUSHBUTTONFACTORY_H
#define PUSHBUTTONFACTORY_H

#include <QWidget>
#include "settingscontrolfactory.h"

class PushButtonFactory : public SettingsControlFactory
{
public:
    PushButtonFactory(const QString& buttonText);
    QWidget* create() const override;
    QString* buttonText;
private:
    QString buttonText_;
};

#endif
