#ifndef DIFFERBASE_H
#define DIFFERBASE_H

namespace GSI {

struct GameState;
class GsiService;

/**
 * @brief Differ 抽象基类
 * @details 所有 Differ 继承此类，实现 diff() 方法比较前后游戏状态并发射信号
 */
class DifferBase {
public:
    virtual ~DifferBase() = default;

    /**
     * @brief 比较前后状态差异并发射对应信号
     * @param prev 前一帧游戏状态
     * @param curr 当前帧游戏状态
     * @param emitter 信号发射器（GsiService）
     */
    virtual void diff(const GameState &prev, const GameState &curr, GsiService *emitter) = 0;
};

} // namespace GSI

#endif // DIFFERBASE_H
