#pragma once

#include <QString>
#include <QObject>
#include <QDateTime>

class ModelData : public QObject {

    Q_OBJECT

    Q_PROPERTY( QString title READ title WRITE setTitle NOTIFY titleChanged FINAL )
    Q_PROPERTY( int processTime READ processTime WRITE setProcessTime NOTIFY processTimeChanged FINAL )
    Q_PROPERTY( bool selected READ selected WRITE setSelected NOTIFY selectedChanged FINAL )
    Q_PROPERTY( bool processing READ isProcessing WRITE setProcessing NOTIFY isProcessingChanged FINAL )
    Q_PROPERTY( bool finished READ isFinished WRITE setFinished NOTIFY isFinishedChanged FINAL )
    Q_PROPERTY( double progress READ progress WRITE setProgress NOTIFY progressChanged FINAL )
    Q_PROPERTY( QDateTime lastEdit READ lastEdit WRITE setLastEdit NOTIFY lastEditChanged FINAL )

public:
    ModelData( QString title, int processTime, bool selected = false, QObject* parent = nullptr )
        : QObject( parent )
        , m_title( std::move( title ) )
        , m_processTime( processTime )
        , m_selected( selected )
        , m_processing( false )
        , m_finished( false ) {}

    QString title() const {
        return m_title;
    }

    void setTitle( const QString& newTitle ) {
        if ( m_title == newTitle )
            return;
        m_title = newTitle;
        Q_EMIT titleChanged();
    }

    int processTime() const {
        return m_processTime;
    }

    void setProcessTime( int newProcessTime ) {
        if ( m_processTime == newProcessTime ) {
            return;
        }

        m_processTime = newProcessTime;
        Q_EMIT processTimeChanged();
    }

    bool selected() const {
        return m_selected;
    }

    void setSelected( bool newSelected ) {
        if ( m_selected == newSelected ) {
            return;
        }

        m_selected = newSelected;
        Q_EMIT selectedChanged();
    }

    bool isProcessing() const {
        return m_processing;
    }

    void setProcessing( bool newProcessing ) {
        if ( m_processing == newProcessing ) {
            return;
        }
        m_processing = newProcessing;
        Q_EMIT isProcessingChanged();
    }


    bool isFinished() const {
        return m_finished;
    }

    void setFinished( bool newFinished ) {
        if ( m_finished == newFinished ) {
            return;
        }

        m_finished = newFinished;
        Q_EMIT isFinishedChanged();
    }

    double progress() const {
        return m_progress;
    }
    void setProgress( double newProgress ) {
        if ( qFuzzyCompare( m_progress, newProgress ) )
            return;
        m_progress = newProgress;
        emit progressChanged();
    }

    QDateTime lastEdit() const {
        return m_lastEdit;
    }
    void setLastEdit( const QDateTime& newLastEdit ) {
        if ( m_lastEdit == newLastEdit )
            return;
        m_lastEdit = newLastEdit;
        emit lastEditChanged();
    }

signals:
    void titleChanged();
    void processTimeChanged();
    void selectedChanged();
    void isProcessingChanged();
    void isFinishedChanged();
    void progressChanged();

    void lastEditChanged();

private:
    QString m_title;
    int m_processTime;
    bool m_selected;
    bool m_processing;
    bool m_finished;
    double m_progress;
    QDateTime m_lastEdit;
};
