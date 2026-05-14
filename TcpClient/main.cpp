/**
 * @file main.cpp
 * @brief Точка входа графического TCP-клиента.
 *
 * Инициализирует Qt-приложение с графическим интерфейсом
 * и запускает главное окно.
 */

#include <QApplication>
#include "mainwindow.h"

 /**
  * @brief Точка входа приложения.
  * @param argc Количество аргументов командной строки.
  * @param argv Массив аргументов командной строки.
  * @return Код завершения приложения.
  */
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}