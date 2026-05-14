#ifndef TCPCLIENTSINGLETON_H
#define TCPCLIENTSINGLETON_H

/**
 * @file tcpclientsingleton.h
 * @brief Объявление класса TCP-клиента, реализованного как Singleton.
 */

#include <QTcpSocket>
#include <QString>

 /**
  * @brief Singleton-класс для управления TCP-подключением к серверу.
  *
  * Гарантирует единственное подключение к серверу на протяжении
  * всего времени работы приложения.
  */
class TcpClientSingleton : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Возвращает единственный экземпляр TcpClientSingleton.
     * @return Указатель на глобальный экземпляр.
     */
    static TcpClientSingleton* getInstance();

    /**
     * @brief Подключается к серверу.
     * @param host Адрес сервера.
     * @param port Порт сервера.
     * @return true — если подключение успешно.
     */
    bool connectToServer(const QString& host, int port);

    /**
     * @brief Отправляет сообщение на сервер.
     * @param message Строка сообщения.
     */
    void sendMessage(const QString& message);

    /**
     * @brief Возвращает указатель на сокет.
     * @return Указатель на QTcpSocket.
     */
    QTcpSocket* getSocket();

    /**
     * @brief Проверяет активность подключения.
     * @return true — если подключён к серверу.
     */
    bool isConnected() const;

private:
    /// Приватный конструктор — запрещает создание экземпляров извне.
    explicit TcpClientSingleton(QObject* parent = nullptr);

    /// Единственный экземпляр класса.
    static TcpClientSingleton* instance;

    /// TCP-сокет для связи с сервером.
    QTcpSocket* socket;
};

#endif