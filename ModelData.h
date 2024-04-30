#pragma once

#include <QString>
#include <QObject>
#include <QThread>

class ModelData : public QObject {

    Q_OBJECT

    Q_PROPERTY(QString title READ getTitle WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(int processTime READ getProcessTime WRITE setProcessTime NOTIFY processTimeChanged FINAL)
    Q_PROPERTY(bool selected READ getSelected WRITE setSelected NOTIFY selectedChanged FINAL)

public:    
    ModelData(QString title, int processTime, bool selected = false, QObject* parent = nullptr)
        : QObject(parent)
        , title(std::move(title))
        , processTime(processTime)
        , selected(selected) {}

    QString getTitle() const
    {
        return title;
    }
    void setTitle(const QString &newTitle)
    {
        if (title == newTitle)
            return;
        title = newTitle;
        emit titleChanged();
    }
    int getProcessTime() const
    {
        return processTime;
    }
    void setProcessTime(int newProcessTime)
    {
        if (processTime == newProcessTime)
            return;
        processTime = newProcessTime;
        emit processTimeChanged();
    }

    bool getSelected() const
    {
        return selected;
    }
    void setSelected(bool newSelected)
    {
        if (selected == newSelected)
            return;
        selected = newSelected;
        emit selectedChanged();
    }

signals:
    void titleChanged();
    void processTimeChanged();
    void selectedChanged();

private:
    QString title;
    int processTime;
    bool selected;
};
