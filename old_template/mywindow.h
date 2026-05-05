#ifndef MYWINDOW_H
#define MYWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

class MyWindow : public QWidget
{
    Q_OBJECT

public:
    MyWindow(QWidget *parent = nullptr);
    ~MyWindow();

private:
    QLineEdit *lineEdit1;    // Успешная жизнь интересна
    QLineEdit *lineEdit2;    // Автозагрузка
    QCheckBox *checkBox;     // Загрузка при старте системы
    QComboBox *comboBox;     // Язык
    QSpinBox *spinBox;       // Таймаут ответа запроса
    QLineEdit *lineEditFile; // Магнит дисков файл
    QPushButton *btnFile;    // Кнопка для выбора файла
    QLineEdit *lineEditFolder; // Путь и папка хранения
    QPushButton *btnFolder;  // Кнопка для выбора папки

private slots:
    void openFileDialog();
    void openFolderDialog();
};

#endif // MYWINDOW_H
