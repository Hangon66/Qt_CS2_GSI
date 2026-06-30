#include "JsonUtils.h"

namespace GSI {

void JsonUtils::applyDelta(QJsonObject &target, const QJsonObject &source) {
    if (source.isEmpty()) return;
    deepMerge(target, source);
}

void JsonUtils::deepMerge(QJsonObject &target, const QJsonObject &source) {
    for (auto it = source.begin(); it != source.end(); ++it) {
        const QString &key = it.key();
        const QJsonValue &sourceValue = it.value();

        if (!target.contains(key)) {
            // target 中不存在此键：直接填充 previously 的旧值
            target[key] = sourceValue;
        } else if (sourceValue.isObject() && target.value(key).isObject()) {
            // 两边都是对象：递归合并，仅填充 target 中缺失的字段
            QJsonObject targetObj = target.value(key).toObject();
            QJsonObject sourceObj = sourceValue.toObject();
            deepMerge(targetObj, sourceObj);
            target[key] = targetObj;
        }
        // 否则：target 已有当前值，保留不覆盖
    }
}

} // namespace GSI
