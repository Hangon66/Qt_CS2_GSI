#include "GsiListener.h"
#include <QJsonDocument>
#include <QDebug>

namespace GSI {

GsiListener::GsiListener(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, &GsiListener::onNewConnection);
}

GsiListener::~GsiListener() {
    stop();
}

bool GsiListener::start(quint16 port) {
    if (m_server->isListening()) {
        qWarning() << "GsiListener: Already listening.";
        return true;
    }

    if (!m_server->listen(QHostAddress::Any, port)) {
        const QString err = m_server->errorString();
        qWarning() << "GsiListener: Failed to start:" << err;
        emit errorOccurred(err);
        return false;
    }

    qDebug() << "GsiListener: Listening on port" << port;
    emit ready(port);
    return true;
}

void GsiListener::stop() {
    if (m_server && m_server->isListening()) {
        m_server->close();
        qDebug() << "GsiListener: Stopped.";
    }
}

bool GsiListener::isListening() const {
    return m_server && m_server->isListening();
}

void GsiListener::onNewConnection() {
    while (m_server->hasPendingConnections()) {
        QTcpSocket *socket = m_server->nextPendingConnection();
        if (!socket) continue;

        // 连接 readyRead 信号来处理请求数据
        connect(socket, &QTcpSocket::readyRead, this, [this, socket]() {
            handleRequest(socket);
        });

        // 连接断开信号自动清理
        connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
    }
}

void GsiListener::handleRequest(QTcpSocket *socket) {
    if (!socket || !socket->isOpen()) return;

    const QByteArray data = socket->readAll();
    if (data.isEmpty()) return;

    // 解析 HTTP 请求
    const QString request = QString::fromUtf8(data);
    const int headerEnd = request.indexOf("\r\n\r\n");
    if (headerEnd < 0) {
        sendResponse(socket, 400, QStringLiteral("{\"error\":\"Bad Request\"}"));
        socket->disconnectFromHost();
        return;
    }

    // 检查是否为 POST 请求
    const QString headerPart = request.left(headerEnd);
    if (!headerPart.startsWith("POST ")) {
        sendResponse(socket, 405, QStringLiteral("{\"error\":\"Only POST method allowed\"}"));
        socket->disconnectFromHost();
        return;
    }

    // 提取 JSON body
    const QString body = request.mid(headerEnd + 4);
    if (body.isEmpty()) {
        sendResponse(socket, 400, QStringLiteral("{\"error\":\"Empty body\"}"));
        socket->disconnectFromHost();
        return;
    }

    // 解析 JSON
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(body.toUtf8(), &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "GsiListener: JSON parse error:" << parseError.errorString();
        sendResponse(socket, 400, QStringLiteral("{\"error\":\"Invalid JSON payload\"}"));
        socket->disconnectFromHost();
        return;
    }

    const QJsonObject json = doc.object();

    // 验证基本结构
    if (json.isEmpty()) {
        sendResponse(socket, 400, QStringLiteral("{\"error\":\"Invalid GSI payload\"}"));
        socket->disconnectFromHost();
        return;
    }

    // 发射 GSI 更新信号
    emit gsiUpdateReceived(json);

    // 返回 200 OK
    sendResponse(socket, 200, QStringLiteral("{\"status\":\"OK\"}"));
    socket->disconnectFromHost();
}

void GsiListener::sendResponse(QTcpSocket *socket, int statusCode, const QString &body) {
    if (!socket || !socket->isOpen()) return;

    QString statusText;
    switch (statusCode) {
        case 200: statusText = QStringLiteral("OK"); break;
        case 400: statusText = QStringLiteral("Bad Request"); break;
        case 405: statusText = QStringLiteral("Method Not Allowed"); break;
        default:  statusText = QStringLiteral("Internal Server Error"); break;
    }

    const QByteArray bodyBytes = body.toUtf8();
    const QString response = QStringLiteral(
        "HTTP/1.1 %1 %2\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %3\r\n"
        "Connection: close\r\n"
        "\r\n"
        "%4"
    ).arg(statusCode).arg(statusText).arg(bodyBytes.size()).arg(body);

    socket->write(response.toUtf8());
    socket->flush();
}

} // namespace GSI
