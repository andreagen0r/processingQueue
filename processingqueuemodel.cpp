#include "processingqueuemodel.h"


using namespace Qt::Literals;

ProcessingQueueModel::ProcessingQueueModel(QObject *parent)
    : QAbstractListModel{parent} {

    connect(this, &ProcessingQueueModel::dataSourceChanged, this, &ProcessingQueueModel::processing);

}

int ProcessingQueueModel::rowCount(const QModelIndex &parent) const
{
    return m_processingData.count();
}

QVariant ProcessingQueueModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index)) {
        return {};
    }

    const auto row {index.row()};

    switch (static_cast<ProcessingRoles>(role)) {
    case Title:
        return m_processingData.at(row)->getTitle();
        break;
    case Progress:
        return m_processingData.at(row)->getProgress();
        break;
    case Paused:
        return m_processingData.at(row)->getPaused();
        break;
    case Finished:
        return m_processingData.at(row)->finished();
        break;
    default:
        break;
    }

    return {};
}

QHash<int, QByteArray> ProcessingQueueModel::roleNames() const
{
    static const QHash<int, QByteArray> out {
                                            {Title, "title"_ba},
                                            {Progress, "progress"_ba},
                                            {Paused, "paused"_ba},
                                            {Finished, "finished"_ba},
                                            };

    return out;
}

bool ProcessingQueueModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!checkIndex(index)) {
        return {};
    }

    const auto row {index.row()};

    switch (static_cast<ProcessingRoles>(role)) {
    case Paused:
        m_processingData[row]->setPaused( value.toBool());
        Q_EMIT dataChanged(index, index, {static_cast<int>(ProcessingRoles::Paused)});
        return true;
        break;
    default:
        break;
    }

    return {};
}

QList<ModelData *> ProcessingQueueModel::dataSource() const
{
    return m_dataSource;
}

void ProcessingQueueModel::setDataSource(const QList<ModelData *> &newDataSource)
{
    beginResetModel();
    m_dataSource = newDataSource;
    qDeleteAll(m_processingData);
    m_processingData.clear();

    for ( auto i : qAsConst(m_dataSource)) {
        auto p = new ProgressData (i->getTitle(), i->getProcessTime(), i->getSelected());
        m_processingData.append(p);
    }
    endResetModel();
    Q_EMIT dataSourceChanged();    
}

void ProcessingQueueModel::clear()
{
    beginResetModel();
    qDeleteAll(m_processingData);
    m_processingData.clear();
    endResetModel();
    Q_EMIT dataSourceChanged();
}

void ProcessingQueueModel::processing()
{
    for(int i = 0; i < m_processingData.count(); ++i) {
        disconnect(m_processingData[i]);

        connect(m_processingData[i], &ProgressData::progressChanged, this, [this, i]{
                beginResetModel();
                qInfo() << "Progress Update..." << m_processingData[i]->getProgress();
                emit dataChanged(QModelIndex{}, QModelIndex{}, {ProcessingRoles::Progress});
                endResetModel();
            }, Qt::QueuedConnection);

        connect(m_processingData[i], &ProgressData::finishedChanged, this, [this, i]{
                beginResetModel();
                qInfo() << "Progress Finished...";
                emit dataChanged(QModelIndex{}, QModelIndex{}, {ProcessingRoles::Finished});
                endResetModel();
            }, Qt::QueuedConnection);

        m_processingData[i]->processing();
    }
}




