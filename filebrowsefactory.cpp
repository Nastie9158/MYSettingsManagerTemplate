#include "filebrowsefactory.h"
#include <QHBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QDir>

FileBrowseFactory::FileBrowseFactory(LineEditFactory* lineEdit, PushButtonFactory* pushButton)
    : lineEdit_(lineEdit), pushButton_(pushButton), lineEditWidget_(nullptr), pushButtonWidget_(nullptr) {}

QWidget* FileBrowseFactory::create() const {
    QWidget* container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(container);

    lineEditWidget_ = qobject_cast<QLineEdit*>(lineEdit_->create());
    pushButtonWidget_ = qobject_cast<QPushButton*>(pushButton_->create());

    layout->addWidget(lineEditWidget_);
    layout->addWidget(pushButtonWidget_);

    container->setLayout(layout);

    QAbstractButton::connect(pushButtonWidget_, &QPushButton::clicked, [this](){
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Выберите файл", QDir::homePath());
        if (!filePath.isEmpty()) {
            lineEditWidget_->setText(filePath);
        }
    });

    return container;
}
