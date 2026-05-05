#include "mainwindow.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QIntValidator>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    // Set window title and size
    setWindowTitle("Форма");
    resize(400, 500);

    // Main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Title bar (simulated with a colored label)
    QLabel *titleBar = new QLabel("Заголовок");
    titleBar->setStyleSheet("background-color: #DDA0DD; padding: 5px; font-weight: bold;");
    titleBar->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleBar);

    // Close button (simulated, not functional in this example)
    QPushButton *closeButton = new QPushButton("X");
    closeButton->setStyleSheet("background-color: transparent; border: none; font-weight: bold; color: black;");
    closeButton->setFixedSize(20, 20);
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->addWidget(titleBar);
    titleLayout->addWidget(closeButton);
    titleLayout->setStretch(0, 1); // Stretch the title bar
    mainLayout->addLayout(titleLayout);

    // Language (Язык) - ComboBox
    QHBoxLayout *languageLayout = new QHBoxLayout;
    QLabel *languageLabel = new QLabel("Язык");
    QComboBox *languageCombo = new QComboBox;
    languageCombo->addItems({"Русский", "Английский", "Испанский"});
    languageLayout->addWidget(languageLabel);
    languageLayout->addWidget(languageCombo);
    mainLayout->addLayout(languageLayout);

    // User Language Proficiency (Успешность языка пользователя) - LineEdit
    QHBoxLayout *proficiencyLayout = new QHBoxLayout;
    QLabel *proficiencyLabel = new QLabel("Успешность языка пользователя");
    QLineEdit *proficiencyEdit = new QLineEdit;
    proficiencyEdit->setPlaceholderText("Введите текст");
    proficiencyLayout->addWidget(proficiencyLabel);
    proficiencyLayout->addWidget(proficiencyEdit);
    mainLayout->addLayout(proficiencyLayout);

    // Auto-upload (Автозагрузка) - CheckBox
    QCheckBox *autoUploadCheck = new QCheckBox("Автозагрузка");
    autoUploadCheck->setChecked(false);
    mainLayout->addWidget(autoUploadCheck);

    // Upload on System Start (Загрузка при старте системы) - CheckBox
    QCheckBox *startUploadCheck = new QCheckBox("Загрузка при старте системы");
    startUploadCheck->setChecked(false);
    mainLayout->addWidget(startUploadCheck);

    // Timing (Тайминг) - Request Wait Time with numeric LineEdit
    QHBoxLayout *timingLayout = new QHBoxLayout;
    QLabel *timingLabel = new QLabel("Время ожидания запроса");
    QLineEdit *timingEdit = new QLineEdit;
    timingEdit->setText("300");
    timingEdit->setValidator(new QIntValidator(0, 9999, this)); // Only numbers
    timingEdit->setMaximumWidth(100);
    timingLayout->addWidget(timingLabel);
    timingLayout->addWidget(timingEdit);
    QLabel *msLabel = new QLabel("мс ▼");
    timingLayout->addWidget(msLabel);
    mainLayout->addLayout(timingLayout);

    // Oil (Маслон) - Oil Search File with LineEdit for file extension
    QHBoxLayout *oilLayout = new QHBoxLayout;
    QLabel *oilLabel = new QLabel("Маслон поиска файла");
    QLineEdit *oilEdit = new QLineEdit;
    oilEdit->setText("*.png");
    oilEdit->setMaximumWidth(100);
    oilLayout->addWidget(oilLabel);
    oilLayout->addWidget(oilEdit);
    mainLayout->addLayout(oilLayout);

    // Stored (Хранимые) - Path to Storage Folder with LineEdit and Browse button
    QHBoxLayout *storageLayout = new QHBoxLayout;
    QLabel *storageLabel = new QLabel("Путь к папке хранения");
    QLineEdit *storageEdit = new QLineEdit;
    storageEdit->setText("D:/storage");
    QPushButton *browseButton = new QPushButton("Обзор...");
    storageLayout->addWidget(storageLabel);
    storageLayout->addWidget(storageEdit);
    storageLayout->addWidget(browseButton);
    mainLayout->addLayout(storageLayout);

    // Connect Browse button to open file dialog
    connect(browseButton, &QPushButton::clicked, this, [=]() {
        QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку", storageEdit->text());
        if (!dir.isEmpty()) {
            storageEdit->setText(dir);
        }
    });

    // Set spacing and margins
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);

    // Apply basic styling
    setStyleSheet("QLineEdit, QComboBox, QCheckBox, QPushButton { font-size: 12px; padding: 5px; }"
                  "QLabel { font-size: 12px; }");
}
