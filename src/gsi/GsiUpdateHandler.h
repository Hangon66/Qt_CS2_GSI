#ifndef GSIUPDATEHANDLER_H
#define GSIUPDATEHANDLER_H

#include <QJsonObject>
#include "models/GameState.h"
#include "differs/DifferManager.h"

namespace GSI {

class GsiService;

/**
 * @brief GSI 更新处理器
 * @details 接收原始 JSON 更新，应用 delta 增量合并，
 *          构建新 GameState，并通过 DifferManager 检测变化
 */
class GsiUpdateHandler {
public:
    /**
     * @brief 构造更新处理器
     * @param differManager Differ 管理器引用
     * @param emitter 信号发射器（GsiService）
     */
    GsiUpdateHandler(DifferManager &differManager, GsiService *emitter);

    /**
     * @brief 处理一次 GSI 更新
     * @param rawUpdate 原始 JSON 数据
     */
    void handle(const QJsonObject &rawUpdate);

    /** @brief 获取最后一次状态 */
    const GameState &lastState() const { return m_lastState; }

private:
    DifferManager &m_differManager;
    GsiService *m_emitter;
    GameState m_lastState;
};

} // namespace GSI

#endif // GSIUPDATEHANDLER_H
