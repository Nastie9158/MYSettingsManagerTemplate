#ifndef FILEBROWSEFACTORY_H
#define FILEBROWSEFACTORY_H

#include <QWidget>
#include "settingscontrolfactory.h"
#include "pushbuttonfactory.h"
#include "lineeditfactory.h"
#include <QLineEdit>
#include <QPushButton>

class FileBrowseFactory : public SettingsControlFactory {
public:
    FileBrowseFactory(LineEditFactory* lineEdit, PushButtonFactory* pushButton);
    QWidget* create() const override;
    QLineEdit* getLineEdit() const;
    QPushButton* getPushButton() const;

private:
    LineEditFactory* lineEdit_;
    PushButtonFactory* pushButton_;
    mutable QLineEdit* lineEditWidget_;
    mutable QPushButton* pushButtonWidget_;
};

#endif
