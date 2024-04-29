#include "processingqueuemodel.h"

using namespace Qt::Literals;

ProcessingQueueModel::ProcessingQueueModel(QObject *parent)
    : QAbstractListModel{parent} {

    connect(this, &ProcessingQueueModel::dataSourceChanged, this, &ProcessingQueueModel::heavyProcessingData);
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
        return m_processingData.at(row)->title;
        break;
    case Progress:
        return m_processingData.at(row)->progress;
        break;
    case Paused:
        return m_processingData.at(row)->paused;
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
                                            };

    return out;
}

bool ProcessingQueueModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!checkIndex(index)) {
        return {};
    }

    const auto row {index.row()};

    bool out {false};

    switch (static_cast<ProcessingRoles>(role)) {
    case Paused:
        m_processingData[row]->paused = value.toBool();
        qInfo() << m_processingData.at(row)->title << m_processingData.at(row)->paused;
        out = true;
        break;
    default:
        break;
    }

    if (out) {
        Q_EMIT dataChanged(index, index);
        return true;
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
        auto p = new ProgressData (i->title, i->processTime, i->selected);
        m_processingData.append(p);
    }
    endResetModel();
    Q_EMIT dataSourceChanged();
}

void ProcessingQueueModel::heavyProcessingData()
{
    qInfo() << "Processing data";
    for(auto i : m_processingData) {
        qInfo() << i->title << i->processTime;
        moveToThread(std::move(i));
    }
}

void ProgressData::run()
{
    for (auto i = 0; i <= 100; ++i) {
        progress += i;
    }
}
