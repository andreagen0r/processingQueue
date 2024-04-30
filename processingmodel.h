#pragma once

#include <QSortFilterProxyModel>
#include <QQmlEngine>

class ProcessingModel : public QSortFilterProxyModel {

    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY( QAbstractItemModel *source WRITE setSource NOTIFY sourceChanged FINAL )

public:
    explicit ProcessingModel( QObject *parent = nullptr );
    void setSourceModel( QAbstractItemModel *sourceModel ) override;
    void setSource( QAbstractItemModel *newSource );

Q_SIGNALS:
    void sourceChanged();

protected:
    bool filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const override;

private:
    QAbstractItemModel *m_source = nullptr;
};
