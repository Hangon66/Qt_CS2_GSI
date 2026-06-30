#include "GsiService.h"
#include "network/GsiListener.h"
#include "network/GsiServer.h"
#include "network/GsiConfigWriter.h"
#include "GsiUpdateHandler.h"
#include "differs/DifferManager.h"
#include "differs/PlayerDiffers.h"
#include "differs/MapDiffers.h"
#include "differs/BombDiffer.h"
#include "differs/GrenadesDiffer.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QStandardPaths>

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

    // 自动检测并安装 CS2 GSI 配置文件
    QString cfgPath = ensureGsiConfig(httpPort);
    if (!cfgPath.isEmpty()) {
        qDebug() << "GsiService: GSI config installed at:" << cfgPath;
    } else {
        qWarning() << "GsiService: CS2 cfg directory not found. Please install GSI config manually.";
    }

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

// ---------------------------------------------------------------------------
// CS2 GSI 配置文件自动安装
// ---------------------------------------------------------------------------

static const QString CFG_FILENAME = QStringLiteral("gamestate_integration_QtGSI.cfg");

QStringList GsiService::findCs2CfgDirs() {
    QStringList result;

    // 从注册表读取 Steam 安装路径
    QSettings reg(QStringLiteral("HKEY_CURRENT_USER\\Software\\Valve\\Steam"),
                  QSettings::NativeFormat);
    QString steamPath = reg.value(QStringLiteral("SteamPath")).toString();
    if (steamPath.isEmpty()) {
        // 备用：常见默认路径
        steamPath = QStringLiteral("C:/Program Files (x86)/Steam");
    }
    // 统一分隔符
    steamPath.replace('\\', '/');

    // CS2 在 Steam 中的可能位置
    QStringList relativePaths = {
        QStringLiteral("/steamapps/common/Counter-Strike Global Offensive/game/csgo/cfg"),
        QStringLiteral("/SteamApps/common/Counter-Strike Global Offensive/game/csgo/cfg"),
    };

    for (const QString &rel : relativePaths) {
        QString full = steamPath + rel;
        if (QDir(full).exists()) {
            result.append(full);
        }
    }

    // 扫描 Steam LibraryFolders (libraryfolders.vdf)
    QString vdfPath = steamPath + QStringLiteral("/steamapps/libraryfolders.vdf");
    if (QFile::exists(vdfPath)) {
        QFile vdf(vdfPath);
        if (vdf.open(QIODevice::ReadOnly | QIODevice::Text)) {
            // 简单解析：提取 "path" 字段
            while (!vdf.atEnd()) {
                QString line = QString::fromUtf8(vdf.readLine()).trimmed();
                if (line.startsWith(QStringLiteral("\"path\""))) {
                    // "path"  "D:\\Games\\Steam"
                    QStringList parts = line.split(QStringLiteral("\""), Qt::SkipEmptyParts);
                    if (parts.size() >= 4) {
                        QString libPath = parts[3];
                        libPath.replace("\\\\", "/");
                        libPath.replace('\\', '/');
                        for (const QString &rel : relativePaths) {
                            QString full = libPath + rel;
                            if (QDir(full).exists() && !result.contains(full)) {
                                result.append(full);
                            }
                        }
                    }
                }
            }
        }
    }

    return result;
}

QString GsiService::ensureGsiConfig(quint16 port) {
    QStringList cfgDirs = findCs2CfgDirs();
    if (cfgDirs.isEmpty()) {
        return {};
    }

    QString uri = QStringLiteral("http://localhost:%1").arg(port);
    QString installedPath;

    for (const QString &dir : cfgDirs) {
        QString filePath = dir + QStringLiteral("/") + CFG_FILENAME;
        if (!QFile::exists(filePath)) {
            if (GsiConfigWriter::writeToFile(filePath, QStringLiteral("QtGSI"), uri)) {
                qDebug() << "GsiService: Wrote GSI config to:" << filePath;
                installedPath = filePath;
            } else {
                qWarning() << "GsiService: Failed to write GSI config to:" << filePath;
            }
        } else {
            qDebug() << "GsiService: GSI config already exists at:" << filePath;
            if (installedPath.isEmpty()) {
                installedPath = filePath;
            }
        }
    }

    return installedPath;
}

} // namespace GSI
