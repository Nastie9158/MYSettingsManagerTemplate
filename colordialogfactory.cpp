#include "colordialogfactory.h"
#include <QHBoxLayout>
#include <QColorDialog>
#include <QColor>
#include <QLabel>

ColorDialogFactory::ColorDialogFactory(LineEditFactory* lineEdit, PushButtonFactory* pushButton)
    : lineEdit_(lineEdit), pushButton_(pushButton), lineEditWidget_(nullptr), pushButtonWidget_(nullptr) {}

ColorDialogFactory::~ColorDialogFactory() {
    delete lineEditWidget_;
    delete pushButtonWidget_;
}

QWidget* ColorDialogFactory::create() const {
    QWidget* container = new QWidget();
    QHBoxLayout* layout = new QHBoxLayout(container);


    lineEditWidget_ = qobject_cast<QLineEdit*>(lineEdit_->create());
    pushButtonWidget_ = qobject_cast<QPushButton*>(pushButton_->create());

    layout->addWidget(lineEditWidget_);
    layout->addWidget(pushButtonWidget_);


    QAbstractButton::connect(pushButtonWidget_, &QPushButton::clicked, [this]() {
        QColorDialog dialog;
        dialog.setOptions(QColorDialog::ShowAlphaChannel); // Показываем альфа-канал
        dialog.setCurrentColor(QColor(lineEditWidget_->text())); // Устанавливаем текущий цвет из QLineEdit

        if (dialog.exec() == QColorDialog::Accepted) {
            QColor selectedColor = dialog.currentColor();
            QString colorPattern = selectedColor.name(QColor::HexArgb).toUpper(); // Получаем HEX с альфа-каналом
            lineEditWidget_->setText(colorPattern);
        }
    });

    container->setLayout(layout);
    return container;
}

QLineEdit* ColorDialogFactory::getLineEdit() const {
    return lineEditWidget_;
}

QPushButton* ColorDialogFactory::getPushButton() const {
    return pushButtonWidget_;
}
