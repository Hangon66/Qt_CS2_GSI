#ifndef GSICONFIGWRITER_H
#define GSICONFIGWRITER_H

#include <QString>

namespace GSI {

/**
 * @brief GSI 配置文件生成器
 * @details 生成 CS2 所需的 GSI .cfg 配置文件内容
 */
class GsiConfigWriter {
public:
    /**
     * @brief 生成 GSI 配置文件内容
     * @param name 配置名称（默认 "QtGSI"）
     * @param uri GSI 服务器 URI（默认 "http://localhost:3000"）
     * @return .cfg 文件的完整文本内容
     */
    static QString generate(const QString &name = QStringLiteral("QtGSI"),
                            const QString &uri = QStringLiteral("http://localhost:3000"));

    /**
     * @brief 将配置文件写入指定路径
     * @param filePath 目标文件路径
     * @param name 配置名称
     * @param uri GSI 服务器 URI
     * @return 是否写入成功
     */
    static bool writeToFile(const QString &filePath,
                            const QString &name = QStringLiteral("QtGSI"),
                            const QString &uri = QStringLiteral("http://localhost:3000"));
};

} // namespace GSI

#endif // GSICONFIGWRITER_H
