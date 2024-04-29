#pragma once

#include <QString>
#include <QObject>
#include <QThread>

class ModelData :public QThread{
    Q_OBJECT
public:

    ModelData(QString title, int processTime, bool selected = false, QObject* parent = nullptr)
        : QThread(parent), title(std::move(title)), processTime(processTime), selected(selected) {}

    QString title;
    int processTime;
    bool selected;
};
