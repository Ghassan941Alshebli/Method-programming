#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QStackedWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include "tcpclientsingleton.h"

/**
 * @file mainwindow.h
 * @brief Объявление главного окна графического интерфейса TCP-клиента.
 */

 /**
  * @brief Главное окно приложения с графическим интерфейсом.
  *
  * Реализует две страницы: аутентификацию (вход/регистрация)
  * и основную панель для отправки запросов к алгоритмам сервера.
  */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор главного окна.
     * @param parent Родительский виджет.
     */
    MainWindow(QWidget* parent = nullptr);

    /** @brief Деструктор. */
    ~MainWindow();

private slots:
    /** @brief Обрабатывает нажатие кнопки регистрации. */
    void onRegisterClicked();

    /** @brief Обрабатывает нажатие кнопки входа. */
    void onLoginClicked();

    /** @brief Обрабатывает нажатие кнопки отправки запроса. */
    void onSendClicked();

    /** @brief Обновляет поля ввода при смене типа запроса. */
    void onRequestTypeChanged(int index);

    /** @brief Обрабатывает входящие данные от сервера. */
    void onReadyRead();

    /** @brief Обрабатывает отключение от сервера. */
    void onDisconnected();

private:
    /** @brief Создаёт страницу аутентификации. */
    void setupLoginPage();

    /** @brief Создаёт основную страницу с запросами. */
    void setupMainPage();

    /** @brief Показывает страницу аутентификации. */
    void showLoginPage();

    /** @brief Показывает основную страницу. */
    void showMainPage();

    /**
     * @brief Отправляет сообщение на сервер.
     * @param message Строка сообщения.
     */
    void sendToServer(const QString& message);

    TcpClientSingleton* client;          ///< TCP-сокет для связи с сервером.
    QStackedWidget* stackedWidget; ///< Контейнер для переключения страниц.

    QLineEdit* usernameEdit;     ///< Поле ввода имени пользователя.
    QLineEdit* emailEdit;        ///< Поле ввода email.
    QLineEdit* passwordEdit;     ///< Поле ввода пароля.
    QLabel* authStatusLabel;     ///< Метка статуса аутентификации.

    QComboBox* requestTypeCombo; ///< Выпадающий список типов запросов.
    QLineEdit* input1Edit;       ///< Первое поле ввода данных запроса.
    QLineEdit* input2Edit;       ///< Второе поле ввода данных запроса.
    QLabel* input1Label;         ///< Метка первого поля ввода.
    QLabel* input2Label;         ///< Метка второго поля ввода.
    QTextEdit* responseEdit;     ///< Область вывода ответов сервера.
    QLabel* welcomeLabel;        ///< Метка приветствия пользователя.
};

#endif