//
// Created by Marcelo Dalcin on 12/08/2025.
//

#include "player.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include "ui_player.h"

player::player(QWidget *parent) : QWidget(parent), ui(new Ui::player) {
    ui->setupUi(this);
    mediaPlayer = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);
    mediaPlayer->setAudioOutput(audioOutput);
    srand(time(NULL));

    connect(ui->stopPauseButton, &QPushButton::click, this, &player::on_stopPauseButton_clicked);
    connect(ui->nextButton, &QPushButton::clicked, this, &player::on_nextButton_clicked);
    connect(ui->lastButton, &QPushButton::clicked, this, &player::on_lastButton_clicked);

    connect(ui->soundSlider, &QSlider::sliderMoved, this, &player::on_soundSlider_sliderMoved);

    connect(mediaPlayer, &QMediaPlayer::durationChanged, ui->progressBar, &QProgressBar::setMaximum);
    connect(mediaPlayer, &QMediaPlayer::positionChanged, ui->progressBar, &QProgressBar::setValue);

    connect(ui->searchButton, &QPushButton::click, this, &player::on_searchButton_clicked);
    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &player::on_listWidget_itemDoubleClicked);
    connect(mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, &player::updateSongInfo);
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
    mediaPlayer->play();
}

void player::on_lastButton_clicked() {

}

void player::on_soundSlider_sliderMoved(int position) {

}

void player::on_searchButton_clicked() {
    QString folderPath {QFileDialog::getExistingDirectory(this,tr("Select Music Folder"), "", QFileDialog::ShowDirsOnly)};

    if (!folderPath.isEmpty()) {
        ui->listWidget->clear();
        playlist.clear();

        QDir dir(folderPath);
        QStringList musics {dir.entryList(QStringList() << "*.mp3", QDir::Files)};


        foreach (QString musicName, musics) {
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
        QString artist {meta.value(QMediaMetaData::AlbumArtist).toString()};
        QString album {meta.value(QMediaMetaData::AlbumTitle).toString()};
        QImage img {meta.value(QMediaMetaData::ThumbnailImage).value<QImage>()};

        if (song.isEmpty()) {
            QFileInfo info {mediaPlayer->source().toLocalFile()};
            song = info.baseName();
        }

        if (!img.isNull()) {
            ui->coverLabel->setPixmap(QPixmap::fromImage(img).scaled(ui->coverLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            ui->coverLabel->clear();
        }

        ui->dataPlainText->clear();
        ui->dataPlainText->appendPlainText("Now Playing 🎶.. "+ song);
        ui->dataPlainText->appendPlainText("By 🎶.. " + artist);
        ui->dataPlainText->appendPlainText("Album 🎶.." + album);
    }
}

void player::on_listWidget_itemDoubleClicked(QListWidgetItem *item) {
    int index {ui->listWidget->row(item)};

    if (index >= 0 && index < playlist.size()) {
        mediaPlayer->setSource(QUrl::fromLocalFile(playlist.at(index)));
        mediaPlayer->play();
        currentSongIndex = index;
    }
}


