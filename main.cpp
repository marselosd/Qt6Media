//
// Created by Marcelo Dalcin on 12/08/2025.
//

#include <QApplication>
#include "Forms/player.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    player w {};
    w.show();

    return QApplication::exec();
}