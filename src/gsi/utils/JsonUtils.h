#ifndef JSONUTILS_H
#define JSONUTILS_H

#include <QJsonObject>

namespace GSI {

/**
 * @brief JSON 工具函数集
 */
class JsonUtils {
public:
    /**
     * @brief 将 previously 增量数据递归合并到 currentState 中
     * @details 深度合并：对于嵌套对象递归处理，对于叶子节点用 source 覆盖 target
     * @param target 目标对象（被更新的完整状态）
     * @param source 源对象（previously 增量数据）
     */
    static void applyDelta(QJsonObject &target, const QJsonObject &source);

private:
    /**
     * @brief 递归深度合并
     * @param target 目标对象
     * @param source 源对象
     */
    static void deepMerge(QJsonObject &target, const QJsonObject &source);
};

} // namespace GSI

#endif // JSONUTILS_H
