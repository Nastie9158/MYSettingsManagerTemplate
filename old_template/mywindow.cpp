#include "mywindow.h"
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>

MyWindow::MyWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Задание для семинара");

    QScrollArea *scrollArea = new QScrollArea(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Язык + Выпадающий список
    QHBoxLayout *languageLayout = new QHBoxLayout();
    QVBoxLayout *languageTextLayout = new QVBoxLayout();
    QLabel *languageLabel = new QLabel("Язык", this);
    QLabel *languageDescription = new QLabel("Установка языка интерфейса", this);
    languageLayout->addWidget(languageLabel);
    languageTextLayout->addWidget(languageDescription);
    comboBox = new QComboBox(this);
    comboBox->addItem("Русский");
    comboBox->addItem("Английский");
    languageLayout->addWidget(comboBox);
    mainLayout->addLayout(languageLayout);
    mainLayout->addLayout(languageTextLayout);

    // Автозагрузка + Чекбокс
    QHBoxLayout *autoLayout = new QHBoxLayout();
    QVBoxLayout *autoTextLayout = new QVBoxLayout();
    QLabel *autoDescrLabel = new QLabel("Запуск при старте системы");
    QLabel *autoLabel = new QLabel("Автозагрузка", this);
    checkBox = new QCheckBox("", this);
    autoLayout->addWidget(autoLabel);
    autoTextLayout->addWidget(autoDescrLabel);
    autoLayout->addWidget(checkBox);
    mainLayout->addLayout(autoLayout);
    mainLayout->addLayout(autoTextLayout);

    // Таймаут ответа запроса + SpinBox
    QHBoxLayout *timeoutLayout = new QHBoxLayout();
    QVBoxLayout *timeoutTextLayout = new QVBoxLayout();
    QLabel *timeoutLabel = new QLabel("Таймаут", this);
    QLabel *timeoutDescrLabel = new QLabel("Время ответа запроса (мс)", this);
    spinBox = new QSpinBox(this);
    spinBox->setRange(0, 1000); // Диапазон от 0 до 1000 мс
    spinBox->setValue(300);      // Значение по умолчанию 300 мс
    timeoutTextLayout->addWidget(timeoutDescrLabel);
    timeoutLayout->addWidget(timeoutLabel);
    timeoutLayout->addWidget(spinBox);
    mainLayout->addLayout(timeoutLayout);
    mainLayout->addLayout(timeoutTextLayout);

    // Магнит дисков файл + Кнопка выбора файла
    QHBoxLayout *fileLayout = new QHBoxLayout();
    QVBoxLayout *fileTextLayout = new QVBoxLayout();
    QLabel *fileLabel = new QLabel("Шаблон");
    QLabel *fileTextLabel = new QLabel("Шаблон поиска файлов", this);
    lineEditFile = new QLineEdit("*.png",this);
    fileLayout->addWidget(fileLabel);
    fileTextLayout->addWidget(fileTextLabel);
    fileLayout->addWidget(lineEditFile);
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(fileTextLayout);

    // Путь и папка хранения + Кнопка выбора папки
    QHBoxLayout *folderLayout = new QHBoxLayout();
    QVBoxLayout *folderTextLayout = new QVBoxLayout();
    QLabel *folderLabel = new QLabel("Хранилище",this);
    QLabel *folderDescrLabel = new QLabel("Путь и папка хранения", this);
    lineEditFolder = new QLineEdit("D:/storage", this);
    btnFolder = new QPushButton("Обзор...", this);
    folderLayout->addWidget(folderLabel);
    folderTextLayout->addWidget(folderDescrLabel);
    folderLayout->addWidget(lineEditFolder);
    folderLayout->addWidget(btnFolder);
    mainLayout->addLayout(folderLayout);
    mainLayout->addLayout(folderTextLayout);


    // сигналы и слоты
    connect(btnFolder, &QPushButton::clicked, this, &MyWindow::openFolderDialog);

    scrollArea->setLayout(mainLayout);

    setLayout(mainLayout);
}

MyWindow::~MyWindow()
{
}

void MyWindow::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "", "PNG Files (*.png)");
    if (!fileName.isEmpty()) {
        lineEditFile->setText(fileName);
    }
}

void MyWindow::openFolderDialog()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку", "", QFileDialog::ShowDirsOnly);
    if (!dir.isEmpty()) {
        lineEditFolder->setText(dir);
    }
}
