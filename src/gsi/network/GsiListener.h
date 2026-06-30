#ifndef GSILISTENER_H
#define GSILISTENER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonObject>
#include <QStringList>

namespace GSI {

/**
 * @brief HTTP GSI 监听器，接收 CS2 游戏客户端推送的 JSON 数据
 * @details 基于 QTcpServer 实现简单的 HTTP 服务器，仅处理 POST 请求
 */
class GsiListener : public QObject {
    Q_OBJECT

public:
    /**
     * @brief 构造 GSI 监听器
     * @param parent 父对象
     */
    explicit GsiListener(QObject *parent = nullptr);
    ~GsiListener() override;

    /**
     * @brief 启动监听
     * @param port 监听端口，默认 3000
     * @return 是否成功启动
     */
    bool start(quint16 port = 3000);

    /**
     * @brief 停止监听
     */
    void stop();

    /**
     * @brief 是否正在监听
     */
    bool isListening() const;

signals:
    /**
     * @brief 收到 GSI 更新数据
     * @param json 解析后的 JSON 对象
     */
    void gsiUpdateReceived(const QJsonObject &json);

    /**
     * @brief 监听器就绪
     * @param port 监听端口
     */
    void ready(quint16 port);

    /**
     * @brief 发生错误
     * @param error 错误描述
     */
    void errorOccurred(const QString &error);

private slots:
    /** @brief 处理新的 TCP 连接 */
    void onNewConnection();

private:
    /**
     * @brief 处理 HTTP 请求
     * @param socket TCP 套接字
     */
    void handleRequest(QTcpSocket *socket);

    /**
     * @brief 发送 HTTP 响应
     * @param socket TCP 套接字
     * @param statusCode HTTP 状态码
     * @param body 响应体 JSON 字符串
     */
    void sendResponse(QTcpSocket *socket, int statusCode, const QString &body);

    QTcpServer *m_server = nullptr;
};

} // namespace GSI

#endif // GSILISTENER_H
