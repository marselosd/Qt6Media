//
// Created by Marcelo Dalcin on 12/08/2025.
//

#ifndef QT6MEDIA_PLAYER_H
#define QT6MEDIA_PLAYER_H

#include <QWidget>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QListWidget>
#include <QDir>
#include <QFileDialog>
#include <QMediaMetaData>
#include <QStack>

QT_BEGIN_NAMESPACE

namespace Ui {
    class player;
}

QT_END_NAMESPACE

class player : public QWidget {
    Q_OBJECT

public:
    explicit player(QWidget *parent = nullptr);

    ~player() override;

private slots:
    void on_stopPauseButton_clicked();
    void on_nextButton_clicked();
    void on_lastButton_clicked();
    void on_soundSlider_sliderMoved(int position);
    void on_searchButton_clicked();
    void updateSongInfo();
    void updatePlayIcon(QMediaPlayer::PlaybackState state);
    QString formatTime(qint64);
    void updateProgress(qint64 position);
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void playIndex(int index);
    void on_muteButton_clicked();

private:
    Ui::player *ui;
    QMediaPlayer *mediaPlayer;
    QAudioOutput *audioOutput;
    QStringList playlist;
    int currentSongIndex {-1};
    QStack<int> queue;
    qint64 music_duration {0};
    bool music_isPressed {false};
    const QPixmap notfoundCover{":/icons/cover_notfound.jpg"};
};


#endif //QT6MEDIA_PLAYER_H