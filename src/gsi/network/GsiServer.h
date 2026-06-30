#ifndef GSISERVER_H
#define GSISERVER_H

#include <QObject>
#include <QWebSocketServer>
#include <QWebSocket>
#include <QSet>
#include <QJsonObject>

namespace GSI {

/**
 * @brief WebSocket 广播服务器
 * @details 将 GSI 事件以 JSON 格式广播给所有连接的 WebSocket 客户端
 */
class GsiServer : public QObject {
    Q_OBJECT

public:
    explicit GsiServer(QObject *parent = nullptr);
    ~GsiServer() override;

    /** @brief 启动 WebSocket 服务器 */
    bool start(quint16 port = 4000);

    /** @brief 停止服务器 */
    void stop();

    /** @brief 广播消息给所有连接的客户端 */
    void broadcast(const QString &type, const QJsonObject &payload);

    /** @brief 获取连接客户端数量 */
    int getClientCount() const;

    /** @brief 是否正在运行 */
    bool isRunning() const;

signals:
    void clientConnected();
    void clientDisconnected();
    void started(quint16 port);

private slots:
    void onNewConnection();
    void onClientDisconnected();

private:
    QWebSocketServer *m_wsServer = nullptr;
    QSet<QWebSocket*> m_clients;
};

} // namespace GSI

#endif // GSISERVER_H
