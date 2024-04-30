#pragma once

#include <QSortFilterProxyModel>
#include <QQmlEngine>

class QAbstractItemModel;

class HistoryModel : public QSortFilterProxyModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QAbstractItemModel* source WRITE setSource NOTIFY sourceChanged FINAL)

public:
    explicit HistoryModel(QObject *parent = nullptr);

    Q_INVOKABLE void setSourceModel(QAbstractItemModel *sourceModel) override;

    void setSource(QAbstractItemModel *newSource);

signals:
    void sourceChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    QAbstractItemModel *m_source = nullptr;
};
