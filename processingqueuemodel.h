#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <QThread>
#include <QtConcurrent>

#include "ModelData.h"

class ProgressData : public ModelData {

    Q_OBJECT
    Q_PROPERTY(double progress READ getProgress WRITE setProgress NOTIFY progressChanged FINAL)
    Q_PROPERTY(bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged FINAL)
    Q_PROPERTY(bool processed READ processed NOTIFY processedChanged FINAL)

public:
    ProgressData(QString title, int processTime, bool selected = false, double progress = 0, bool paused = false, QObject* parent = nullptr)
        : ModelData(title, processTime, selected, parent)
        , m_progress(progress)
        , m_paused(paused){

        connect(this, &ProgressData::progressChanged, this, [this]{
            qInfo() << "Progress:" << m_progress;
        });
    }


    bool getPaused() const
    {
        return m_paused;
    }

    void setPaused(bool newPaused)
    {
        if (m_paused == newPaused)
            return;
        m_paused = newPaused;
        emit pausedChanged();
    }

    double getProgress() const
    {
        return m_progress;
    }

    void setProgress(double newProgress)
    {
        if (qFuzzyCompare(m_progress, newProgress))
            return;
        m_progress = newProgress;
        qInfo() << "Progress from data:" << m_progress;
        emit progressChanged();
    }

    Q_INVOKABLE void processing()
    {
        /*
         * Crio uma thread para executar o loop e emitir o signal de atualização
         */
        auto future = QtConcurrent::run([this]{

            const auto time = ceil(getProcessTime()  / 100.0);

            qInfo() << "Processing..." << getTitle();

            for (auto i = 1; i <= 100; ++i) {
                setProgress(i);
                qInfo() << "Sleep for" << time;
                QThread::sleep(time);
            }

            qInfo() << "End processing..." << getTitle();

            emit processFinished();
        });
    }

    bool processed() const { return m_processed; }

signals:
    void progressChanged();
    void pausedChanged();
    void processFinished();
    void processedChanged();
private:
    double m_progress;
    bool m_paused;
    bool m_processed;
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
    Q_INVOKABLE void processing();

Q_SIGNALS:
    void dataSourceChanged();

private:
    QList<ProgressData*> m_processingData;
    QList<ModelData*> m_dataSource;
};
