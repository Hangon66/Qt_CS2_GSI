#include "GsiService.h"
#include "LocalPlayerService.h"
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
    s_differManager.registerDiffer(std::make_unique<AuthDiffer>());
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

    // 创建本地玩家服务（内部自动连接 stateUpdated 信号）
    m_localPlayer = new LocalPlayerService(this, this);
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

LocalPlayerService *GsiService::localPlayer() const {
    return m_localPlayer;
}

const GameState &GsiService::lastState() const {
    return m_updateHandler->lastState();
}

QString GsiService::localSteamid() const {
    return m_updateHandler->lastState().provider.steamid;
}

bool GsiService::isLocal(const QString &steamid) const {
    return !steamid.isEmpty() && steamid == localSteamid();
}

// ---------------------------------------------------------------------------
// CS2 GSI 配置文件自动安装
// ---------------------------------------------------------------------------

static const QString CFG_FILENAME = QStringLiteral("gamestate_integration_QtGSI.cfg");

/// CS2 的 Steam App ID
static constexpr int CS2_APP_ID = 730;

/// 从 Steam 注册表读取 Steam 安装路径（依次尝试 HKCU 和 HKLM）
static QString resolveSteamPath() {
    // 1. HKEY_CURRENT_USER（Steam 运行时写入）
    QSettings regCU(QStringLiteral("HKEY_CURRENT_USER\\Software\\Valve\\Steam"),
                    QSettings::NativeFormat);
    QString path = regCU.value(QStringLiteral("SteamPath")).toString();
    if (!path.isEmpty()) {
        path.replace('\\', '/');
        return path;
    }

    // 2. HKEY_LOCAL_MACHINE（Steam 安装时写入，参考 SteamAppPathProvider）
    QSettings regLM(QStringLiteral("HKEY_LOCAL_MACHINE\\SOFTWARE\\WOW6432Node\\Valve\\Steam"),
                    QSettings::NativeFormat);
    path = regLM.value(QStringLiteral("InstallPath")).toString();
    if (!path.isEmpty()) {
        path.replace('\\', '/');
        return path;
    }

    // 3. 常见默认路径
    QString defaultPath = QStringLiteral("C:/Program Files (x86)/Steam");
    if (QDir(defaultPath).exists()) {
        return defaultPath;
    }

    return {};
}

/// 从 libraryfolders.vdf 中解析所有 Steam 库路径
static QStringList resolveSteamLibraries(const QString &steamPath) {
    QStringList libraries;

    // 始终包含默认 steamapps 目录
    QString defaultApps = steamPath + QStringLiteral("/steamapps");
    if (QDir(defaultApps).exists()) {
        libraries.append(defaultApps);
    }

    QString vdfPath = steamPath + QStringLiteral("/steamapps/libraryfolders.vdf");
    if (!QFile::exists(vdfPath)) {
        return libraries;
    }

    QFile vdf(vdfPath);
    if (!vdf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return libraries;
    }

    // 简单解析：提取 "path" 字段值
    while (!vdf.atEnd()) {
        QString line = QString::fromUtf8(vdf.readLine()).trimmed();
        if (line.startsWith(QStringLiteral("\"path\""))) {
            QStringList parts = line.split(QStringLiteral("\""), Qt::SkipEmptyParts);
            // parts[0]="path", parts[1]=tabs, parts[2]=实际路径值
            if (parts.size() >= 3) {
                QString libPath = parts.last();  // 取最后一个非空部分作为路径
                libPath.replace("\\\\", "/");
                libPath.replace('\\', '/');
                QString steamApps = libPath + QStringLiteral("/steamapps");
                if (QDir(steamApps).exists() && !libraries.contains(steamApps)) {
                    libraries.append(steamApps);
                }
            }
        }
    }

    return libraries;
}

/// 解析 appmanifest_<appId>.acf 获取游戏的 installdir
/// @return 游戏安装目录的完整路径（如 .../steamapps/common/Counter-Strike Global Offensive），空表示未找到
static QString resolveGameInstallDir(const QString &steamAppsPath, int appId) {
    QString acfPath = steamAppsPath + QStringLiteral("/appmanifest_%1.acf").arg(appId);
    if (!QFile::exists(acfPath)) {
        return {};
    }

    QFile acf(acfPath);
    if (!acf.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {};
    }

    // 在 ACF 中查找 "installdir" 字段
    QString installDir;
    while (!acf.atEnd()) {
        QString line = QString::fromUtf8(acf.readLine()).trimmed();
        if (line.startsWith(QStringLiteral("\"installdir\""))) {
            QStringList parts = line.split(QStringLiteral("\""), Qt::SkipEmptyParts);
            // parts[0]="installdir", parts[1]=tabs, parts[2]=实际路径值
            if (parts.size() >= 3) {
                installDir = parts.last();  // 取最后一个非空部分
                break;
            }
        }
    }

    if (installDir.isEmpty()) {
        return {};
    }

    QString fullPath = steamAppsPath + QStringLiteral("/common/") + installDir;
    if (QDir(fullPath).exists()) {
        return fullPath;
    }

    return {};
}

QStringList GsiService::findCs2CfgDirs() {
    QStringList result;

    // 1. 解析 Steam 安装路径
    QString steamPath = resolveSteamPath();
    if (steamPath.isEmpty()) {
        qWarning() << "GsiService: Steam installation not found in registry.";
        return result;
    }
    qDebug() << "GsiService: Steam path:" << steamPath;

    // 2. 获取所有 Steam 库
    QStringList libraries = resolveSteamLibraries(steamPath);
    qDebug() << "GsiService: Found" << libraries.size() << "Steam libraries.";

    // 3. 在每个库中通过 appmanifest_730.acf 精确定位 CS2
    for (const QString &lib : libraries) {
        QString gameDir = resolveGameInstallDir(lib, CS2_APP_ID);
        if (!gameDir.isEmpty()) {
            // CS2 cfg 目录: <gameDir>/game/csgo/cfg
            QString cfgDir = gameDir + QStringLiteral("/game/csgo/cfg");
            if (QDir(cfgDir).exists() && !result.contains(cfgDir)) {
                result.append(cfgDir);
                qDebug() << "GsiService: Found CS2 cfg dir via ACF:" << cfgDir;
            }
        }
    }

    // 4. 兜底：如果 ACF 方式未找到，尝试常见硬编码路径
    if (result.isEmpty()) {
        QStringList fallbackPaths = {
            steamPath + QStringLiteral("/steamapps/common/Counter-Strike Global Offensive/game/csgo/cfg"),
            steamPath + QStringLiteral("/SteamApps/common/Counter-Strike Global Offensive/game/csgo/cfg"),
        };
        for (const QString &fallback : fallbackPaths) {
            if (QDir(fallback).exists() && !result.contains(fallback)) {
                result.append(fallback);
                qDebug() << "GsiService: Found CS2 cfg dir via fallback:" << fallback;
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
