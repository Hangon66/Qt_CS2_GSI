#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

class QSoundEffect;

/**
 * @brief 音效播放工具类，管理游戏音效的加载与播放。
 *
 * 音效文件存放在 resources/sounds/ 目录下，运行时从 exe 同级目录加载。
 * 使用 QSoundEffect 实现零解码延迟的 WAV 播放。
 */
class SoundPlayer {
public:
    /**
     * @brief 播放本地玩家死亡音效（菜就多练.wav）。
     */
    static void playDeath();

    /**
     * @brief 播放新回合开始音效（来咯.wav）。
     */
    static void playRespawn();

    /**
     * @brief 播放炸弹即将爆炸音效（呦嘣.wav）。
     */
    static void playBombImminent();

private:
    static QSoundEffect *deathSound();
    static QSoundEffect *respawnSound();
    static QSoundEffect *bombSound();
};

#endif // SOUNDPLAYER_H
