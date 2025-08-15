//
// Created by Marcelo Dalcin on 15/08/2025.
//

#ifndef QT6MEDIA_ICONS_H
#define QT6MEDIA_ICONS_H

#include <QIcon>
#include <QMap>

enum class AppIcon {
    PLAY,
    PAUSE,
    HIGH,
    MUTED
};

class Icons {
public:
    static const QIcon &getIcon(AppIcon icon) {
        static QMap<AppIcon, QIcon> iMap = {
            {AppIcon::PLAY, QIcon::fromTheme("media-playback-start")},
            {AppIcon::PAUSE, QIcon::fromTheme("media-playback-pause")},
            {AppIcon::HIGH, QIcon::fromTheme("audio-volume-high")},
            {AppIcon::MUTED, QIcon::fromTheme("audio-volume-muted")}
        };
        return iMap[icon];
    }
};

#endif //QT6MEDIA_ICONS_H