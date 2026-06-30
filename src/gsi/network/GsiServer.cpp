#include "GsiServer.h"
#include <QJsonDocument>
#include <QDebug>

namespace GSI {

GsiServer::GsiServer(QObject *parent)
    : QObject(parent)
    , m_wsServer(nullptr)
{}

GsiServer::~GsiServer() {
    stop();
}

bool GsiServer::start(quint16 port) {
    if (m_wsServer) {
        qWarning() << "GsiServer: Already running.";
        return true;
    }

    m_wsServer = new QWebSocketServer(
        QStringLiteral("QtGSI"),
        QWebSocketServer::NonSecureMode,
        this
    );

    if (!m_wsServer->listen(QHostAddress::Any, port)) {
        qWarning() << "GsiServer: Failed to start:" << m_wsServer->errorString();
        delete m_wsServer;
        m_wsServer = nullptr;
        return false;
    }

    connect(m_wsServer, &QWebSocketServer::newConnection, this, &GsiServer::onNewConnection);

    qDebug() << "GsiServer: WebSocket server listening on ws://localhost:" << port;
    emit started(port);
    return true;
}

void GsiServer::stop() {
    if (m_wsServer) {
        // 关闭所有客户端连接
        for (auto *client : m_clients) {
            client->close();
            client->deleteLater();
        }
        m_clients.clear();

        m_wsServer->close();
        m_wsServer->deleteLater();
        m_wsServer = nullptr;

        qDebug() << "GsiServer: Stopped.";
    }
}

void GsiServer::broadcast(const QString &type, const QJsonObject &payload) {
    if (m_clients.isEmpty()) return;

    QJsonObject msg;
    msg["type"] = type;
    msg["payload"] = payload;
    const QString jsonStr = QJsonDocument(msg).toJson(QJsonDocument::Compact);

    for (auto *client : m_clients) {
        client->sendTextMessage(jsonStr);
    }
}

int GsiServer::getClientCount() const {
    return m_clients.size();
}

bool GsiServer::isRunning() const {
    return m_wsServer && m_wsServer->isListening();
}

void GsiServer::onNewConnection() {
    while (m_wsServer->hasPendingConnections()) {
        QWebSocket *socket = m_wsServer->nextPendingConnection();
        if (!socket) continue;

        m_clients.insert(socket);
        connect(socket, &QWebSocket::disconnected, this, &GsiServer::onClientDisconnected);

        qDebug() << "GsiServer: Client connected. Total:" << m_clients.size();
        emit clientConnected();
    }
}

void GsiServer::onClientDisconnected() {
    QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
    if (socket) {
        m_clients.remove(socket);
        socket->deleteLater();
        qDebug() << "GsiServer: Client disconnected. Remaining:" << m_clients.size();
        emit clientDisconnected();
    }
}

} // namespace GSI
