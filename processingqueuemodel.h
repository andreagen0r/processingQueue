#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <QThread>

#include "ModelData.h"

class ProgressData : public ModelData {

    Q_OBJECT
    Q_PROPERTY(double progress READ getProgress WRITE setProgress NOTIFY progressChanged FINAL)
    Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged FINAL)

public:
    ProgressData(QString title, int processTime, bool selected = false, double progress = 0, bool paused = false, QObject* parent = nullptr)
        : ModelData(title, processTime, selected, parent)
        , progress(progress)
        , paused(paused){}
    double progress;
    bool paused;

    bool getPaused() const
    {
        return paused;
    }

    void setPaused(bool newPaused)
    {
        if (paused == newPaused)
            return;
        paused = newPaused;
        emit pausedChanged();
    }

    double getProgress() const
    {
        return progress;
    }

    void setProgress(double newProgress)
    {
        if (qFuzzyCompare(progress, newProgress))
            return;
        progress = newProgress;
        emit progressChanged();
    }

signals:
    void progressChanged();
    void pausedChanged();

protected:
    void run() override
    {
        for (auto i = 0; i <= 100; ++i) {
            setProgress(i);
            thread()->sleep(processTime/100);
        }
    }

};

class ProcessingQueueModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QList<ModelData*> dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged FINAL)

public:
    enum ProcessingRoles {
        Title = Qt::UserRole + 1,
        Progress,
        Paused,
    };

    explicit ProcessingQueueModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QList<ModelData*> dataSource() const;
    void setDataSource(const QList<ModelData*> &newDataSource);

    Q_INVOKABLE void clear();

Q_SIGNALS:
    void dataSourceChanged();

private:
    QList<ProgressData*> m_processingData;
    QList<ModelData*> m_dataSource;

    void heavyProcessingData();
};
