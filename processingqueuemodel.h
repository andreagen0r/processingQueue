#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <QThread>
#include <QtConcurrent>

#include "ModelData.h"

class ProgressData : public ModelData {

    Q_OBJECT
    Q_PROPERTY( double progress READ getProgress WRITE setProgress NOTIFY progressChanged FINAL )
    Q_PROPERTY( bool paused READ getPaused WRITE setPaused NOTIFY pausedChanged FINAL )
    Q_PROPERTY( bool finished READ finished WRITE setFinished NOTIFY finishedChanged FINAL )

public:
    ProgressData( QString title, int processTime, bool selected = false, double progress = 0, bool paused = false, QObject *parent = nullptr )
        : ModelData( title, processTime, selected, parent )
        , m_progress( progress )
        , m_paused( paused )
        , m_finished( false ) {

        connect( this, &ProgressData::progressChanged, this, [this] { qInfo() << "Progress:" << m_progress; } );
    }


    bool getPaused() const {
        return m_paused;
    }

    void setPaused( bool newPaused ) {
        if ( m_paused == newPaused )
            return;
        m_paused = newPaused;
        emit pausedChanged();
    }

    double getProgress() const {
        return m_progress;
    }

    void setProgress( double newProgress ) {
        if ( qFuzzyCompare( m_progress, newProgress ) )
            return;
        m_progress = newProgress;
        qInfo() << "Progress from data:" << m_progress;
        emit progressChanged();
    }

    // Q_INVOKABLE void processing()
    // {
    //     /*
    //      * Crio uma thread para executar o loop e emitir o signal de atualização
    //      */
    //     auto future = QtConcurrent::run([=]{

    //         const auto time = ceil(getProcessTime()  / 100.0);

    //         qInfo() << "Processing..." << getTitle();

    //         for (auto i = 1; i <= 100; ++i) {
    //             m_progress = i;
    //             emit progressChanged();
    //             qInfo() << "Sleep for" << time;
    //             QThread::msleep(time);
    //         }

    //         qInfo() << "End processing..." << getTitle();

    //         m_finished = true;
    //         emit finishedChanged();
    //     });
    // }


    bool finished() const {
        return m_finished;
    }
    void setFinished( bool newFinished ) {
        if ( m_finished == newFinished )
            return;
        m_finished = newFinished;
        emit finishedChanged();
    }

signals:
    void progressChanged();
    void pausedChanged();
    void finishedChanged();

private:
    double m_progress;
    bool m_paused;
    bool m_finished;
};

class ProcessingQueueModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY( QList<ModelData *> dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged FINAL )

public:
    enum ProcessingRoles {
        Title = Qt::UserRole + 1,
        Progress,
        Paused,
        Finished,
    };

    explicit ProcessingQueueModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;

    QList<ModelData *> dataSource() const;
    void setDataSource( const QList<ModelData *> &newDataSource );

    Q_INVOKABLE void clear();
    Q_INVOKABLE void processing();

Q_SIGNALS:
    void dataSourceChanged();

private:
    QList<ProgressData *> m_processingData;
    QList<ModelData *> m_dataSource;
};
