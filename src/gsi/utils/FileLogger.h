#ifndef FILELOGGER_H
#define FILELOGGER_H

#include <QString>

namespace GSI {

/**
 * @brief 文件日志记录器
 * @details 安装自定义 Qt 消息处理器，将 qDebug/qWarning/qCritical 输出
 *          同时写入控制台和本地日志文件。
 *          日志文件保存在可执行文件目录下的 logs/ 文件夹中。
 */
class FileLogger {
public:
    /**
     * @brief 初始化日志系统
     * @param logDir 日志目录路径（默认: 可执行文件目录下的 logs/）
     * @return 是否初始化成功
     */
    static bool init(const QString &logDir = QString());

    /**
     * @brief 获取当前日志文件路径
     */
    static QString logFilePath();

private:
    static QString s_logFilePath;
};

} // namespace GSI

#endif // FILELOGGER_H
