#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <QList>

#include "ModelData.h"

class MyModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool toProcessing READ toProcessing NOTIFY toProcessingChanged FINAL)
    Q_PROPERTY(QList<ModelData*> outData READ outData NOTIFY outDataChanged FINAL)

public:
    enum MyRoles {
        Title = Qt::UserRole + 1,
        Selected,
        ProcessTime,
        Progress,
    };
    explicit MyModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;

    bool toProcessing() const;

    Q_INVOKABLE void processing();

    QList<ModelData*> outData() const;

Q_SIGNALS:
    void toProcessingChanged();

    void outDataChanged();

private:
    QList<ModelData*> m_data;
    bool m_toProcessing;
    QList<ModelData*> m_outData;
};
