/**
 * @file tcpclientsingleton.cpp
 * @brief Реализация TCP-клиента по паттерну Singleton.
 */

#include "tcpclientsingleton.h"

 /// Инициализация статического экземпляра.
TcpClientSingleton* TcpClientSingleton::instance = nullptr;

/**
 * @brief Приватный конструктор — инициализирует сокет.
 * @param parent Родительский объект Qt.
 */
TcpClientSingleton::TcpClientSingleton(QObject* parent)
    : QObject(parent)
{
    socket = new QTcpSocket(this);
}

/**
 * @brief Возвращает единственный экземпляр TcpClientSingleton.
 *
 * Если экземпляр ещё не создан — создаёт его.
 *
 * @return Указатель на глобальный экземпляр.
 */
TcpClientSingleton* TcpClientSingleton::getInstance()
{
    if (instance == nullptr)
        instance = new TcpClientSingleton();
    return instance;
}

/**
 * @brief Подключается к серверу по указанному адресу и порту.
 *
 * Если уже подключён — повторное подключение не выполняется.
 *
 * @param host Адрес сервера.
 * @param port Порт сервера.
 * @return true — если подключение успешно, false — в случае ошибки.
 */
bool TcpClientSingleton::connectToServer(const QString& host, int port)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
        return true;

    socket->connectToHost(host, port);
    return socket->waitForConnected(3000);
}

/**
 * @brief Отправляет сообщение на сервер.
 *
 * Сообщение отправляется только если есть активное подключение.
 *
 * @param message Строка сообщения для отправки.
 */
void TcpClientSingleton::sendMessage(const QString& message)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
        socket->flush();
    }
}

/**
 * @brief Возвращает указатель на внутренний сокет.
 * @return Указатель на QTcpSocket.
 */
QTcpSocket* TcpClientSingleton::getSocket()
{
    return socket;
}

/**
 * @brief Проверяет, активно ли подключение к серверу.
 * @return true — если сокет в состоянии ConnectedState.
 */
bool TcpClientSingleton::isConnected() const
{
    return socket->state() == QAbstractSocket::ConnectedState;
}