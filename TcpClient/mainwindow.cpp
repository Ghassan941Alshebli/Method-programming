/**
 * @file mainwindow.cpp
 * @brief Реализация главного окна графического интерфейса TCP-клиента.
 */

#include "mainwindow.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>

 /**
  * @brief Конструктор — инициализирует окно, сокет и страницы интерфейса.
  * @param parent Родительский виджет.
  */
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Method Programming Client");
    setMinimumSize(500, 400);

    client = TcpClientSingleton::getInstance();

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    setupLoginPage();
    setupMainPage();
    showLoginPage();

    client->connectToServer("127.0.0.1", 12345);
    connect(client->getSocket(), &QTcpSocket::readyRead, this, &MainWindow::onReadyRead);
    connect(client->getSocket(), &QTcpSocket::disconnected, this, &MainWindow::onDisconnected);
}

/** @brief Деструктор. */
MainWindow::~MainWindow() {}

/**
 * @brief Создаёт страницу аутентификации с полями входа и регистрации.
 */
void MainWindow::setupLoginPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setSpacing(12);
    layout->setContentsMargins(40, 40, 40, 40);

    QLabel* title = new QLabel("Method Programming");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 20px; font-weight: bold; margin-bottom: 10px;");

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Username");

    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Email");

    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);

    authStatusLabel = new QLabel("");
    authStatusLabel->setAlignment(Qt::AlignCenter);
    authStatusLabel->setStyleSheet("color: red;");

    QPushButton* registerBtn = new QPushButton("Register");
    QPushButton* loginBtn = new QPushButton("Login");

    connect(registerBtn, &QPushButton::clicked, this, &MainWindow::onRegisterClicked);
    connect(loginBtn, &QPushButton::clicked, this, &MainWindow::onLoginClicked);

    layout->addWidget(title);
    layout->addWidget(usernameEdit);
    layout->addWidget(emailEdit);
    layout->addWidget(passwordEdit);
    layout->addWidget(registerBtn);
    layout->addWidget(loginBtn);
    layout->addWidget(authStatusLabel);
    layout->addStretch();

    stackedWidget->addWidget(page);
}

/**
 * @brief Создаёт основную страницу с выбором алгоритма и полями запроса.
 */
void MainWindow::setupMainPage()
{
    QWidget* page = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(page);
    layout->setSpacing(12);
    layout->setContentsMargins(20, 20, 20, 20);

    welcomeLabel = new QLabel("Welcome!");
    welcomeLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    QGroupBox* requestGroup = new QGroupBox("Request");
    QGridLayout* grid = new QGridLayout(requestGroup);

    requestTypeCombo = new QComboBox();
    requestTypeCombo->addItems({ "MD5", "VIGENERE", "SECANT", "GRAPH" });
    connect(requestTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::onRequestTypeChanged);

    input1Label = new QLabel("Text:");
    input1Edit = new QLineEdit();
    input1Edit->setPlaceholderText("Enter text...");

    input2Label = new QLabel("Key:");
    input2Edit = new QLineEdit();
    input2Edit->setPlaceholderText("Enter key...");
    input2Label->hide();
    input2Edit->hide();

    QPushButton* sendBtn = new QPushButton("Send");
    connect(sendBtn, &QPushButton::clicked, this, &MainWindow::onSendClicked);

    grid->addWidget(new QLabel("Algorithm:"), 0, 0);
    grid->addWidget(requestTypeCombo, 0, 1);
    grid->addWidget(input1Label, 1, 0);
    grid->addWidget(input1Edit, 1, 1);
    grid->addWidget(input2Label, 2, 0);
    grid->addWidget(input2Edit, 2, 1);
    grid->addWidget(sendBtn, 3, 1);

    responseEdit = new QTextEdit();
    responseEdit->setReadOnly(true);
    responseEdit->setPlaceholderText("Server responses will appear here...");

    layout->addWidget(welcomeLabel);
    layout->addWidget(requestGroup);
    layout->addWidget(new QLabel("Server responses:"));
    layout->addWidget(responseEdit);

    stackedWidget->addWidget(page);
}

/** @brief Показывает страницу аутентификации. */
void MainWindow::showLoginPage()
{
    stackedWidget->setCurrentIndex(0);
}

/** @brief Показывает основную страницу. */
void MainWindow::showMainPage()
{
    stackedWidget->setCurrentIndex(1);
}

/**
 * @brief Отправляет сообщение на сервер через Singleton.
 * @param message Строка сообщения.
 */
void MainWindow::sendToServer(const QString& message)
{
    if (client->isConnected()) {
        client->sendMessage(message);
    }
    else {
        authStatusLabel->setText("Not connected to server.");
    }
}

/** @brief Обрабатывает нажатие кнопки регистрации. */
void MainWindow::onRegisterClicked()
{
    QString username = usernameEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();

    if (username.isEmpty() || email.isEmpty() || password.isEmpty()) {
        authStatusLabel->setText("Please fill in all fields.");
        return;
    }

    sendToServer("REGISTER_USER|" + username + "|" + email + "|" + password);
}

/** @brief Обрабатывает нажатие кнопки входа. */
void MainWindow::onLoginClicked()
{
    QString email = emailEdit->text().trimmed();
    QString password = passwordEdit->text().trimmed();

    if (email.isEmpty() || password.isEmpty()) {
        authStatusLabel->setText("Please enter email and password.");
        return;
    }

    sendToServer("LOGIN|" + email + "|" + password);
}

/** @brief Обрабатывает нажатие кнопки отправки запроса. */
void MainWindow::onSendClicked()
{
    QString type = requestTypeCombo->currentText();
    QString input1 = input1Edit->text().trimmed();
    QString input2 = input2Edit->text().trimmed();
    QString request;

    if (type == "MD5")
        request = "MD5|" + input1;
    else if (type == "VIGENERE")
        request = "VIGENERE|" + input1 + "|" + input2;
    else if (type == "SECANT")
        request = "SECANT|" + input1 + "|" + input2;
    else if (type == "GRAPH")
        request = "GRAPH|" + input1;

    sendToServer(request);
}

/**
 * @brief Обновляет видимость полей ввода при смене типа запроса.
 * @param index Индекс выбранного элемента в списке.
 */
void MainWindow::onRequestTypeChanged(int index)
{
    if (index == 0) {
        input1Label->setText("Text:");
        input1Edit->setPlaceholderText("Enter text...");
        input2Label->hide();
        input2Edit->hide();
    }
    else if (index == 1) {
        input1Label->setText("Text:");
        input1Edit->setPlaceholderText("Enter text...");
        input2Label->setText("Key:");
        input2Edit->setPlaceholderText("Enter key...");
        input2Label->show();
        input2Edit->show();
    }
    else if (index == 2) {
        input1Label->setText("x0:");
        input1Edit->setPlaceholderText("Enter x0...");
        input2Label->setText("x1:");
        input2Edit->setPlaceholderText("Enter x1...");
        input2Label->show();
        input2Edit->show();
    }
    else if (index == 3) {
        input1Label->setText("Edges:");
        input1Edit->setPlaceholderText("e.g. A-B,B-C,C-A");
        input2Label->hide();
        input2Edit->hide();
    }
}

/** @brief Обрабатывает входящие данные от сервера. */
void MainWindow::onReadyRead()
{
    QString response = QString::fromUtf8(client->getSocket()->readAll()).trimmed();

    if (stackedWidget->currentIndex() == 0) {
        if (response == "Login successful") {
            QString username = usernameEdit->text().trimmed();
            welcomeLabel->setText("Welcome, " + username + "!");
            showMainPage();
        }
        else {
            authStatusLabel->setStyleSheet("color: red;");
            authStatusLabel->setText(response);
        }
    }
    else {
        responseEdit->append("> " + response);
    }
}

/** @brief Обрабатывает отключение от сервера. */
void MainWindow::onDisconnected()
{
    QMessageBox::warning(this, "Disconnected", "Connection to server lost.");
    showLoginPage();
}