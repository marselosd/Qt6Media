//
// Created by Marcelo Dalcin on 14/08/2025.
//

#ifndef QT6MEDIA_CLICKABLESLIDER_H
#define QT6MEDIA_CLICKABLESLIDER_H

#include <QSlider>
#include <QMouseEvent>
#include <QStyle>

class ClickableSlider : public QSlider {
    Q_OBJECT
public:
    using QSlider::QSlider;

protected:
    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::LeftButton && orientation() == Qt::Horizontal) {
            int val = QStyle::sliderValueFromPosition(
                minimum(), maximum(),
                event->pos().x(),width()
                );
            setValue(val);
            emit sliderMoved(val);
            emit sliderReleased();
        }
        QSlider::mousePressEvent(event);
    }
};

#endif //QT6MEDIA_CLICKABLESLIDER_H