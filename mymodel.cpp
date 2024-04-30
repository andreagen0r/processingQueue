#include "mymodel.h"

#include <QDebug>
#include <QRandomGenerator>

using namespace Qt::Literals;

MyModel::MyModel(QObject *parent)
    : QAbstractListModel{parent}
{
    for (auto i = 0; i < 30; ++i) {
        const quint32 time = QRandomGenerator::global()->bounded(1000, 5000);
        m_data << new ModelData(QString("Title from c++ %1").arg(i), time, false);
    }

    connect(this, &MyModel::dataChanged, this, [this]{
        auto it = std::find_if(m_data.cbegin(), m_data.cend(),[](ModelData* data){
            return data->getSelected() ==true;
        });
        m_toProcessing = {it != m_data.cend()};
        Q_EMIT toProcessingChanged();
    });
}

int MyModel::rowCount(const QModelIndex &parent) const
{
    return m_data.count();
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    if (!checkIndex(index)) {
        return {};
    }

    const auto row = index.row();

    switch (static_cast<MyRoles>(role)) {
    case Title:
        return m_data.at(row)->getTitle();
        break;
    case Selected:
        return m_data.at(row)->getSelected();

        break;
    case ProcessTime:
        return m_data.at(row)->getProcessTime();
        break;
    default:
        break;
    }

    return {};
}

bool MyModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!checkIndex(index)) {
        return {};
    }

    const auto row = index.row();

    bool out {false};
    switch (static_cast<MyRoles>(role)) {
    case Selected:
        m_data[row]->setSelected( value.toBool());
        out = true;
        break;
    default:
        break;
    }

    if (out) {
        Q_EMIT dataChanged(index, index);
        return out;
    }

    return {};
}

QHash<int, QByteArray> MyModel::roleNames() const
{
    static const QHash<int, QByteArray> out {
        {Title, "title"_ba},
        {Selected, "selected"_ba},
        {ProcessTime, "processTime"_ba},
        {Progress, "progress"_ba}
    };
    return out;
}

bool MyModel::toProcessing() const
{
    return m_toProcessing;
}

void MyModel::processing()
{
    qDeleteAll(m_outData);
    m_outData.clear();

    for (const auto i : m_data) {
        if (i->getSelected()) {
            m_outData << i;
        }
    }

    Q_EMIT outDataChanged();
}

QList<ModelData *> MyModel::outData() const
{
    return m_outData;
}
