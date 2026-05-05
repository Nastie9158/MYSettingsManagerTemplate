#ifndef COMBOBOXFACTORY_H
#define COMBOBOXFACTORY_H

#include "settingscontrolfactory.h"
#include <QStringList>

class ComboBoxFactory : public SettingsControlFactory {
public:
    ComboBoxFactory(const QString& defaultValue, const QStringList& options);
    QWidget* create() const override;
private:
    QString defaultValue_;
    QStringList options_;
};

#endif
