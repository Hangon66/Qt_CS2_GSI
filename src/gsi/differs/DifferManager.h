#ifndef DIFFERMANAGER_H
#define DIFFERMANAGER_H

#include <vector>
#include <memory>
#include "DifferBase.h"

namespace GSI {

struct GameState;
class GsiService;

/**
 * @brief Differ 管理器，持有并执行所有 Differ
 */
class DifferManager {
public:
    DifferManager() = default;

    /**
     * @brief 注册一个 Differ
     * @param differ Differ 实例（所有权转移）
     */
    void registerDiffer(std::unique_ptr<DifferBase> differ);

    /**
     * @brief 执行所有已注册的 Differ
     * @param prev 前一帧状态
     * @param curr 当前帧状态
     * @param emitter 信号发射器
     */
    void diff(const GameState &prev, const GameState &curr, GsiService *emitter);

private:
    std::vector<std::unique_ptr<DifferBase>> m_differs;
};

} // namespace GSI

#endif // DIFFERMANAGER_H
