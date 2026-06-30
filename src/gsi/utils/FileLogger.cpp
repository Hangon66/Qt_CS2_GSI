#include "FileLogger.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QMutex>
#include <QTextStream>
#include <QtMessageHandler>

namespace GSI {

QString FileLogger::s_logFilePath;

static QFile *s_logFile = nullptr;
static QMutex s_logMutex;
static QtMessageHandler s_originalHandler = nullptr;

/// 日志级别转字符串
static const char *msgTypeToString(QtMsgType type) {
    switch (type) {
        case QtDebugMsg:    return "DEBUG";
        case QtInfoMsg:     return "INFO ";
        case QtWarningMsg:  return "WARN ";
        case QtCriticalMsg: return "ERROR";
        case QtFatalMsg:    return "FATAL";
    }
    return "?????";
}

/// 自定义消息处理器：写入日志文件，并调用原始处理器（Qt Creator 输出窗口）
static void fileMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QMutexLocker locker(&s_logMutex);

    // 1. 写入日志文件（带时间戳和级别）
    if (s_logFile && s_logFile->isOpen()) {
        const QString timestamp = QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd hh:mm:ss.zzz"));
        const QString level = QString::fromLatin1(msgTypeToString(type));
        QTextStream fileStream(s_logFile);
        fileStream << QStringLiteral("[%1] [%2] %3\n").arg(timestamp, level, msg);
        fileStream.flush();
    }

    // 2. 调用原始处理器（输出到 Qt Creator 调试窗口）
    if (s_originalHandler) {
        s_originalHandler(type, context, msg);
    } else {
        // 无原始处理器时，回退到 stderr
        QTextStream errStream(stderr, QIODevice::WriteOnly);
        errStream << msg << "\n";
        errStream.flush();
    }

    // Fatal 消息时刷新并终止
    if (type == QtFatalMsg) {
        if (s_logFile) s_logFile->flush();
        abort();
    }
}

bool FileLogger::init(const QString &logDir) {
    QMutexLocker locker(&s_logMutex);

    // 确定日志目录
    QString dir = logDir;
    if (dir.isEmpty()) {
        dir = QCoreApplication::applicationDirPath() + QStringLiteral("/logs");
    }

    // 确保目录存在
    QDir logDirectory(dir);
    if (!logDirectory.exists()) {
        if (!logDirectory.mkpath(QStringLiteral("."))) {
            qWarning() << "FileLogger: Failed to create log directory:" << dir;
            return false;
        }
    }

    // 生成日志文件名：QtGSI_yyyyMMdd_hhmmss.log
    const QString fileName = QStringLiteral("QtGSI_%1.log")
        .arg(QDateTime::currentDateTime().toString(QStringLiteral("yyyyMMdd_hhmmss")));
    s_logFilePath = dir + QStringLiteral("/") + fileName;

    // 打开日志文件（追加模式）
    s_logFile = new QFile(s_logFilePath);
    if (!s_logFile->open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        qWarning() << "FileLogger: Failed to open log file:" << s_logFilePath;
        delete s_logFile;
        s_logFile = nullptr;
        s_logFilePath.clear();
        return false;
    }

    // 安装自定义消息处理器，保存原始处理器
    s_originalHandler = qInstallMessageHandler(fileMessageHandler);

    return true;
}

QString FileLogger::logFilePath() {
    return s_logFilePath;
}

} // namespace GSI
