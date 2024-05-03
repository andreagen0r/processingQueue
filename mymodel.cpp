#include "mymodel.h"

#include <algorithm>
#include <QDebug>
#include <QRandomGenerator>
#include <QtConcurrent>

using namespace Qt::Literals;

MyModel::MyModel( QObject *parent )
    : QAbstractListModel { parent } {

    for ( auto i = 0; i < 20; ++i ) {
        const quint32 time = QRandomGenerator::global()->bounded( 1000, 5000 );
        auto item = new ModelData( QString( "Picture %1.%2" ).arg( i ).arg( ( i % 2 == 0 ) ? "jpg" : "png" ), time, false, this );
        item->setLastEdit( QDateTime::currentDateTime() );
        m_data << item;
    }

    // Indicates if there are an item is selected
    connect( this, &MyModel::dataChanged, this, [this] {
        auto it = std::find_if( m_data.cbegin(), m_data.cend(), []( ModelData *data ) { return data->selected() == true; } );
        m_toProcessing = { it != m_data.cend() };
        Q_EMIT toProcessingChanged();
    } );
}

int MyModel::rowCount( const QModelIndex &parent ) const {
    return m_data.count();
}

QVariant MyModel::data( const QModelIndex &index, int role ) const {
    if ( !checkIndex( index ) ) {
        return {};
    }

    const auto row = index.row();

    switch ( static_cast<MyRoles>( role ) ) {
        case Title:
            return m_data.at( row )->title();
            break;
        case Selected:
            return m_data.at( row )->selected();
            break;
        case Progress:
            return m_data.at( row )->progress();
            break;
        case ProcessTime:
            return m_data.at( row )->processTime();
            break;
        case Processing:
            return m_data.at( row )->isProcessing();
            break;
        case Finished:
            return m_data.at( row )->isFinished();
            break;
        case LastEdit:
            return m_data.at( row )->lastEdit();
            break;
        default:
            break;
    }

    return {};
}

bool MyModel::setData( const QModelIndex &index, const QVariant &value, int role ) {
    if ( !checkIndex( index ) ) {
        return {};
    }

    const auto row = index.row();

    bool out { false };
    switch ( static_cast<MyRoles>( role ) ) {
        case Selected:
            m_data[row]->setSelected( value.toBool() );
            out = true;
            break;
        default:
            break;
    }

    if ( out ) {
        Q_EMIT dataChanged( index, index );
        return out;
    }

    return {};
}

QHash<int, QByteArray> MyModel::roleNames() const {
    static const QHash<int, QByteArray> out {
        { Title, "title"_ba },           { Selected, "selected"_ba }, { Progress, "progress"_ba }, { ProcessTime, "processTime"_ba },
        { Processing, "processing"_ba }, { Finished, "finished"_ba }, { LastEdit, "lastEdit"_ba },
    };
    return out;
}

bool MyModel::toProcessing() const {
    return m_toProcessing;
}

void execute( ModelData *model ) {

    model->setProcessing( false );
    model->setProgress( 0.0 );

    if ( model->selected() ) {

        qInfo() << "EXECUTING" << model->title();

        model->setProcessing( true );

        const auto delay { ceil( model->processTime() / 100.0 ) };

        for ( auto i = 1; i <= 100; ++i ) {
            model->setProgress( i );
            QThread::msleep( delay );
        }

        model->setLastEdit( QDateTime::currentDateTime() );
        model->setProcessing( false );
        model->setFinished( true );
    }
}

void MyModel::processing() {

    qInfo() << __PRETTY_FUNCTION__;

    m_selectedCount = 0;
    m_finishedCount = 0;

    for ( auto &i : m_data ) {

        if ( i->selected() ) {
            m_selectedCount++;
            Q_EMIT selectedCountChanged();
        }
        i->setProcessing( false );
        i->setFinished( false );
        i->setProgress( 0.0 );
    }

    for ( auto &i : m_data ) {

        disconnect( i );

        if ( i->selected() ) {
            qInfo() << "STARTED:" << i->title();

            // clang-format off

            connect( i, &ModelData::isProcessingChanged, this, [&]() {
                    Q_EMIT layoutChanged();
                }, Qt::QueuedConnection );

            connect( i, &ModelData::progressChanged, this, [&]() {
                    Q_EMIT layoutChanged();
                }, Qt::QueuedConnection );

            connect( i, &ModelData::isFinishedChanged, this, [&]() {
                    m_finishedCount++;
                    Q_EMIT finishedCountChanged();
                    Q_EMIT layoutChanged();
                }, Qt::QueuedConnection );

            connect( i, &ModelData::lastEditChanged, this, [&]() {
                    Q_EMIT layoutChanged();
                }, Qt::QueuedConnection );

            // clang-format on
        }
    }

    auto future = QtConcurrent::map( m_data, execute );
    Q_EMIT layoutChanged();
}

void MyModel::unselectAll() {
    for ( auto &i : m_data ) {
        i->setSelected( false );
    }
    m_selectedCount = 0;
    Q_EMIT selectedCountChanged();
    Q_EMIT layoutChanged();
}

void MyModel::clearHistory() {
    for ( auto &i : m_data ) {
        i->setFinished( false );
    }
    Q_EMIT layoutChanged();
}

int MyModel::selectedCount() const {
    return m_selectedCount;
}

int MyModel::finishedCount() const {
    return m_finishedCount;
}
