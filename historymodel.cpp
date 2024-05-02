#include "historymodel.h"
#include "mymodel.h"

#include <QAbstractItemModel>

HistoryModel::HistoryModel( QObject *parent )
    : QSortFilterProxyModel { parent } {
    this->sort( 0, Qt::SortOrder::AscendingOrder );
}

bool HistoryModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const {
    if ( !checkIndex( source_parent ) ) {
        return false;
    }

    const auto index { sourceModel()->index( source_row, 0, source_parent ) };

    const auto out = index.data( MyModel::MyRoles::Finished ).toBool();

    return out;
}

bool HistoryModel::lessThan( const QModelIndex &source_left, const QModelIndex &source_right ) const {
    const auto leftData = sourceModel()->data( source_left, MyModel::MyRoles::LastEdit ).toDateTime();
    const auto rightData = sourceModel()->data( source_right, MyModel::MyRoles::LastEdit ).toDateTime();

    return leftData < rightData;
}

void HistoryModel::setSourceModel( QAbstractItemModel *sourceModel ) {
    QSortFilterProxyModel::setSourceModel( sourceModel );
}

void HistoryModel::setSource( QAbstractItemModel *newSource ) {
    setSourceModel( newSource );
}
