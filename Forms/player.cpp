//
// Created by Marcelo Dalcin on 12/08/2025.
//

#include "player.h"

#include <iostream>
#include <random>

#include "Icons.h"
#include "ui_player.h"

player::player(QWidget *parent) : QWidget(parent), ui(new Ui::player) {
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("Qt6 Media Player");

    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    audioOutput->setVolume(0.5f);
    ui->soundSlider->setValue(50);
    mediaPlayer->setAudioOutput(audioOutput);

    connect(ui->soundSlider, &QSlider::sliderMoved, this, &player::on_soundSlider_sliderMoved);
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &player::updateSongInfo);
    connect(mediaPlayer, &QMediaPlayer::playbackStateChanged, this, &player::updatePlayIcon);

    connect(mediaPlayer, &QMediaPlayer::durationChanged, this, [this](qint64 duration) {
        music_duration = duration;
        ui->progressSlider->setRange(0,duration);
    });

    connect(ui->progressSlider, &QSlider::sliderPressed, this, [this]() {
        music_isPressed = true;
    });

    connect(ui->progressSlider, &QSlider::sliderMoved, this, [this](int value) {
        mediaPlayer->setPosition(value);
    });

    connect(ui->progressSlider, &QSlider::sliderReleased, this, [this]() {
        music_isPressed = false;
        mediaPlayer->setPosition(ui->progressSlider->value());
    });

    connect(mediaPlayer, &QMediaPlayer::positionChanged, this, &player::updateProgress);
}

player::~player() {
    delete ui;
}

void player::on_stopPauseButton_clicked() {
    if (mediaPlayer->playbackState() == QMediaPlayer::PlayingState) {
        mediaPlayer->pause();
    }
    else {
        mediaPlayer->play();
    }
}

void player::on_nextButton_clicked() {
    if (playlist.isEmpty())
        return;

    std::random_device rd {};
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, playlist.size() - 1);

    int index {};
    do {
        index = dis(gen);
    }while (index == currentSongIndex);

    playIndex(index);
}

void player::on_lastButton_clicked() {
    if (queue.isEmpty() || (queue.top() < 0 || queue.top() > playlist.size()))
        return;

    queue.pop();
    if (!queue.isEmpty()) {
        mediaPlayer->setSource(QUrl::fromLocalFile(playlist.at(queue.top())));
        mediaPlayer->play();
    }
}

void player::on_soundSlider_sliderMoved(int position) {
    audioOutput->setVolume(position/100.0f);
}

void player::on_muteButton_clicked() {
    if (audioOutput->isMuted()) {
        audioOutput->setMuted(false);
        ui->muteButton->setIcon(Icons::getIcon(AppIcon::HIGH));
    } else {
        audioOutput->setMuted(true);
        ui->muteButton->setIcon(Icons::getIcon(AppIcon::MUTED));
    }
}

void player::on_searchButton_clicked() {
    QString folderPath {QFileDialog::getExistingDirectory(this,tr("Select Music Folder"), "", QFileDialog::ShowDirsOnly)};

    if (!folderPath.isEmpty()) {
        ui->listWidget->clear();
        playlist.clear();
        queue.clear();
        currentSongIndex = -1;

        QDir dir(folderPath);
        QStringList musics {dir.entryList(QStringList() << "*.mp3", QDir::Files)};


        for (const QString &musicName : musics) {
            QString filePath {dir.absoluteFilePath(musicName)};
            playlist << filePath;
            ui->listWidget->addItem(musicName);
        }
    }
}

void player::updateSongInfo() {
    if (mediaPlayer->mediaStatus() == QMediaPlayer::LoadedMedia) {
        QMediaMetaData meta {mediaPlayer->metaData()};
        QString song {meta.value(QMediaMetaData::Title).toString()};
        QImage img {meta.value(QMediaMetaData::ThumbnailImage).value<QImage>()};

        if (song.isEmpty()) {
            QFileInfo info {mediaPlayer->source().toLocalFile()};
            song = info.baseName();
        }

        if (!img.isNull() && !img.size().isEmpty()) {
            ui->coverLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->verticalLayoutWidget->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            ui->coverLabel->setPixmap(notfoundCover.scaled(ui->coverLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        }

        ui->songLabel->clear();
        ui->songLabel->setText(song);
    }
}

void player::updatePlayIcon(QMediaPlayer::PlaybackState state) {
    if (state == QMediaPlayer::PlayingState) {
        ui->stopPauseButton->setIcon(Icons::getIcon(AppIcon::PAUSE));
    } else {
        ui->stopPauseButton->setIcon(Icons::getIcon(AppIcon::PLAY));
    }
}

void player::updateProgress(qint64 position) {
    if (!music_isPressed)
        ui->progressSlider->setValue(position);

    QString text = QString("%1 / %2").arg(formatTime(position), formatTime(music_duration));
    ui->timeLabel->setText(text);
}



void player::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    int index {ui->listWidget->row(item)};

    if (index >= 0 && index < playlist.size()) {
        playIndex(index);
    }
}

void player::playIndex(int index) {
    if (index < 0 || index >= playlist.size())
        return;

    currentSongIndex = index;
    queue.push(currentSongIndex);
    mediaPlayer->setSource(QUrl::fromLocalFile(playlist.at(currentSongIndex)));
    mediaPlayer->play();
}

QString player::formatTime(qint64 milsec) {
    qint64 seconds = milsec / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}

