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
#include <QTableWidget>
#include <QHeaderView>
#include <QStringList>

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

    // --- 1. ВЕРХНЯЯ ЧАСТЬ (Приветствие и роль пользователя) ---
    QHBoxLayout* headerLayout = new QHBoxLayout();
    
    welcomeLabel = new QLabel("Добро пожаловать!");
    welcomeLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    
    roleLabel = new QLabel("Роль: Ожидание..."); // Отображение роли (находится в mainwindow.h)
    roleLabel->setStyleSheet("font-size: 14px; color: #d35400; font-weight: bold;");
    roleLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    
    headerLayout->addWidget(welcomeLabel);
    headerLayout->addWidget(roleLabel);

    // --- 2. СРЕДНЯЯ ЧАСТЬ (Основная секция запроса) ---
    QGroupBox* requestGroup = new QGroupBox("Запрос");
    QGridLayout* grid = new QGridLayout(requestGroup);

    requestTypeCombo = new QComboBox();
    requestTypeCombo->addItems({ "MD5", "VIGENERE", "SECANT", "GRAPH" });
    connect(requestTypeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
        this, &MainWindow::onRequestTypeChanged);

    input1Label = new QLabel("Текст:");
    input1Edit = new QLineEdit();
    input1Edit->setPlaceholderText("Введите текст...");

    input2Label = new QLabel("Ключ:");
    input2Edit = new QLineEdit();
    input2Edit->setPlaceholderText("Введите ключ...");
    input2Label->hide();
    input2Edit->hide();

    QPushButton* sendBtn = new QPushButton("Отправить");
    connect(sendBtn, &QPushButton::clicked, this, &MainWindow::onSendClicked);

    grid->addWidget(new QLabel("Алгоритм:"), 0, 0);
    grid->addWidget(requestTypeCombo, 0, 1);
    grid->addWidget(input1Label, 1, 0);
    grid->addWidget(input1Edit, 1, 1);
    grid->addWidget(input2Label, 2, 0);
    grid->addWidget(input2Edit, 2, 1);
    grid->addWidget(sendBtn, 3, 1);

    // --- 3. НОВАЯ СЕКЦИЯ (Кнопки администратора и таблица) ---
    QPushButton* btnGetUsers = new QPushButton("Показать всех пользователей (Админ)");
    QPushButton* btnKickUser = new QPushButton("Удалить пользователя (Админ)");
    
    // Подключение действий кнопок к ранее созданным функциям
    connect(btnGetUsers, &QPushButton::clicked, this, &MainWindow::onGetUsersClicked);
    connect(btnKickUser, &QPushButton::clicked, this, &MainWindow::onKickUserClicked);

    QHBoxLayout* adminBtnLayout = new QHBoxLayout();
    adminBtnLayout->addWidget(btnGetUsers);
    adminBtnLayout->addWidget(btnKickUser);

    usersTable = new QTableWidget(); // Создание таблицы
    usersTable->hide(); // При входе таблица изначально скрыта

    // --- 4. НИЖНЯЯ ЧАСТЬ (Окно вывода ответов сервера) ---
    responseEdit = new QTextEdit();
    responseEdit->setReadOnly(true);
    responseEdit->setPlaceholderText("Здесь будут отображаться ответы сервера...");

    // --- 5. СБОРКА ВСЕХ КОМПОНЕНТОВ В ГЛАВНЫЙ МАКЕТ (LAYOUT) ---
    layout->addLayout(headerLayout);       // Верхнее приветствие
    layout->addWidget(requestGroup);       // Секция отправки алгоритма
    
    layout->addLayout(adminBtnLayout);     // Две кнопки администратора
    layout->addWidget(usersTable);         // Таблица с данными пользователей
    
    layout->addWidget(new QLabel("Ответы сервера:"));
    layout->addWidget(responseEdit);       // Текстовое поле для ответов

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
/** @brief Обрабатывает входящие данные от сервера. */
/** @brief Обрабатывает входящие данные от сервера. */
void MainWindow::onReadyRead()
{
    QString response = QString::fromUtf8(client->getSocket()->readAll()).trimmed();

    // 1. Если находимся на странице авторизации (Login) (currentIndex == 0)
    if (stackedWidget->currentIndex() == 0) {
        
        // От сервера приходит ответ вида "Login successful|ADMIN" или "Login successful|USER"
        if (response.startsWith("Login successful")) {
            
            // Разделение прав доступа (ролей)
            QStringList parts = response.split("|");
            if (parts.size() >= 2) {
                currentRole = parts[1]; // Сохраняем "ADMIN" или "USER"
                
                // Выводим роль в roleLabel (используя функцию getRoleLabel)
                if(roleLabel) {
                    roleLabel->setText("Роль: " + getRoleLabel(currentRole));
                }
            } else {
                // Если сервер по старинке прислал только "Login successful",
                // то по умолчанию считаем пользователя обычным юзером
                currentRole = "USER";
                if(roleLabel) {
                    roleLabel->setText("Роль: " + getRoleLabel(currentRole));
                }
            }

            QString username = usernameEdit->text().trimmed();
            welcomeLabel->setText("Добро пожаловать, " + username + "!");
            showMainPage();
        } 
        else {
            authStatusLabel->setStyleSheet("color: red;");
            authStatusLabel->setText(response);
        }
    } 
    // 2. Если находимся на главной странице (после успешного входа)
    else {
        // Когда от сервера приходит список пользователей
        if (response.startsWith("USERS_DATA|")) {
            QString tableData = response.mid(11); // Отрезаем префикс "USERS_DATA|"
            displayUserTable(tableData);
        }
        // Когда приходят ответы от обычных алгоритмов
        else {
            responseEdit->append("> " + response);
        }
    }
}

/** @brief Обрабатывает отключение от сервера. */
void MainWindow::onDisconnected()
{
    QMessageBox::warning(this, "Disconnected", "Connection to server lost.");
    showLoginPage();
}
// --- ФУНКЦИИ ДЛЯ РОЛЕЙ АДМИНИСТРАТОРА И ПОЛЬЗОВАТЕЛЯ ---

QString MainWindow::getRoleLabel(const QString& role) {
    // Возвращает текстовое описание роли пользователя
    if (role == "ADMIN") 
        return "Администратор (Полный доступ)";
    
    return "Обычный пользователь";
}

void MainWindow::sendAdminCommand(const QString& cmd) {
    // Проверяет, является ли текущий пользователь администратором
    if (currentRole != "ADMIN") {
        if(responseEdit) {
            responseEdit->append("Ошибка: У вас нет прав администратора!");
        }
        return;
    }

    // Отправляет команду серверу
    sendToServer(cmd);
}

void MainWindow::onGetUsersClicked() {
    // Запрашивает у сервера список всех пользователей
    sendAdminCommand("GET_USERS_LIST");
}

void MainWindow::onKickUserClicked() {
    // Проверяет существование таблицы пользователей
    if (!usersTable) return;
    
    // Получает индекс выбранной строки
    int currentRow = usersTable->currentRow();

    if (currentRow >= 0) {
        // Получает ID выбранного пользователя из первой колонки таблицы
        QString userId = usersTable->item(currentRow, 0)->text();

        // Отправляет серверу команду на удаление пользователя
        sendAdminCommand("KICK_USER|" + userId);
    } else {
        if(responseEdit) {
            responseEdit->append("Выберите пользователя из таблицы для удаления.");
        }
    }
}

void MainWindow::displayUserTable(const QString& data) {
    // Проверяет существование таблицы
    if (!usersTable) return;
    
    // Очищает предыдущее содержимое таблицы
    usersTable->clearContents();
    
    // Ожидаемый формат данных от сервера:
    // "ID,Name,Role;ID,Name,Role;"
    QStringList rows = data.split(";", Qt::SkipEmptyParts);

    // Устанавливает количество строк и колонок
    usersTable->setRowCount(rows.size());
    usersTable->setColumnCount(3);
    
    // Устанавливает заголовки колонок
    usersTable->setHorizontalHeaderLabels({
        "ID",
        "Имя пользователя",
        "Роль"
    });

    // Заполняет таблицу данными
    for (int i = 0; i < rows.size(); ++i) {
        QStringList columns = rows[i].split(",");

        if (columns.size() >= 3) {
            usersTable->setItem(i, 0, new QTableWidgetItem(columns[0]));
            usersTable->setItem(i, 1, new QTableWidgetItem(columns[1]));
            usersTable->setItem(i, 2, new QTableWidgetItem(columns[2]));
        }
    }
    
    // Растягивает колонки по ширине окна
    usersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Показывает таблицу
    usersTable->show();
}