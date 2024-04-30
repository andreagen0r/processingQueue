#include "historymodel.h"
#include "mymodel.h"

#include <QAbstractItemModel>

HistoryModel::HistoryModel( QObject *parent )
    : QSortFilterProxyModel { parent } {}

bool HistoryModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const {
    if ( !checkIndex( source_parent ) ) {
        return false;
    }

    const auto index { sourceModel()->index( source_row, 0, source_parent ) };

    const auto out = index.data( MyModel::MyRoles::Finished ).toBool();

    return out;
}

void HistoryModel::setSourceModel( QAbstractItemModel *sourceModel ) {
    QSortFilterProxyModel::setSourceModel( sourceModel );
}

void HistoryModel::setSource( QAbstractItemModel *newSource ) {
    setSourceModel( newSource );
}
