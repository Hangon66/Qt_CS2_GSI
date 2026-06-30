#include "GsiService.h"
#include "network/GsiListener.h"
#include "network/GsiServer.h"
#include "GsiUpdateHandler.h"
#include "differs/DifferManager.h"
#include "differs/PlayerDiffers.h"
#include "differs/MapDiffers.h"
#include "differs/BombDiffer.h"
#include "differs/GrenadesDiffer.h"
#include <QDebug>

namespace GSI {

// DifferManager 作为 GsiService 的内部成员
static DifferManager s_differManager;

GsiService::GsiService(QObject *parent)
    : QObject(parent)
    , m_listener(new GsiListener(this))
    , m_updateHandler(new GsiUpdateHandler(s_differManager, this))
    , m_wsServer(nullptr)
{
    // 注册所有默认 Differ
    s_differManager.registerDiffer(std::make_unique<ProviderDiffer>());
    s_differManager.registerDiffer(std::make_unique<MapDiffer>());
    s_differManager.registerDiffer(std::make_unique<RoundWinsDiffer>());
    s_differManager.registerDiffer(std::make_unique<RoundDiffer>());
    s_differManager.registerDiffer(std::make_unique<PlayerDiffer>());
    s_differManager.registerDiffer(std::make_unique<PlayerStateDiffer>());
    s_differManager.registerDiffer(std::make_unique<PlayerMatchStatsDiffer>());
    s_differManager.registerDiffer(std::make_unique<WeaponDiffer>());
    s_differManager.registerDiffer(std::make_unique<PhaseCountdownsDiffer>());
    s_differManager.registerDiffer(std::make_unique<BombDiffer>());
    s_differManager.registerDiffer(std::make_unique<GrenadesDiffer>());

    qDebug() << "GsiService: Initialized with all default differs.";
}

GsiService::~GsiService() {
    stop();
    delete m_updateHandler;
}

void GsiService::start(quint16 httpPort) {
    if (m_running) {
        qWarning() << "GsiService: Already running.";
        return;
    }

    qDebug() << "GsiService: Starting...";

    // 连接 GsiListener 信号
    connect(m_listener, &GsiListener::gsiUpdateReceived, this, [this](const QJsonObject &json) {
        m_updateHandler->handle(json);
    });

    connect(m_listener, &GsiListener::ready, this, [this](quint16 port) {
        qDebug() << "GsiService: HTTP listener ready on port" << port;
    });

    connect(m_listener, &GsiListener::errorOccurred, this, [this](const QString &err) {
        qWarning() << "GsiService: Listener error:" << err;
    });

    // 启动 HTTP 监听
    m_listener->start(httpPort);
    m_running = true;

    qDebug() << "GsiService: Started successfully.";
}

void GsiService::stop() {
    if (!m_running) return;

    qDebug() << "GsiService: Stopping...";
    m_listener->stop();

    if (m_wsServer) {
        m_wsServer->stop();
    }

    m_running = false;
    qDebug() << "GsiService: Stopped.";
}

bool GsiService::isRunning() const {
    return m_running;
}

const GameState &GsiService::lastState() const {
    return m_updateHandler->lastState();
}

} // namespace GSI
