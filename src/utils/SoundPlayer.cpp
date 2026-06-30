#include "SoundPlayer.h"

#include <QApplication>
#include <QSoundEffect>
#include <QUrl>

QSoundEffect *SoundPlayer::deathSound() {
    static QSoundEffect *s = nullptr;
    if (!s) {
        s = new QSoundEffect();
        s->setSource(QUrl::fromLocalFile(
            QApplication::applicationDirPath() + "/resources/sounds/菜就多练.wav"));
        s->setVolume(0.8f);
    }
    return s;
}

QSoundEffect *SoundPlayer::bombSound() {
    static QSoundEffect *s = nullptr;
    if (!s) {
        s = new QSoundEffect();
        s->setSource(QUrl::fromLocalFile(
            QApplication::applicationDirPath() + "/resources/sounds/呦嘣.wav"));
        s->setVolume(0.8f);
    }
    return s;
}

QSoundEffect *SoundPlayer::respawnSound() {
    static QSoundEffect *s = nullptr;
    if (!s) {
        s = new QSoundEffect();
        s->setSource(QUrl::fromLocalFile(
            QApplication::applicationDirPath() + "/resources/sounds/来咯.wav"));
        s->setVolume(0.8f);
    }
    return s;
}

void SoundPlayer::playDeath() {
    deathSound()->play();
}

void SoundPlayer::playRespawn() {
    respawnSound()->play();
}

void SoundPlayer::playBombImminent() {
    bombSound()->play();
}
