#include "processingmodel.h"
#include "mymodel.h"

#include <QAbstractItemModel>

ProcessingModel::ProcessingModel( QObject *parent )
    : QSortFilterProxyModel { parent } {}

void ProcessingModel::setSourceModel( QAbstractItemModel *sourceModel ) {
    QSortFilterProxyModel::setSourceModel( sourceModel );
}

bool ProcessingModel::filterAcceptsRow( int source_row, const QModelIndex &source_parent ) const {
    if ( !checkIndex( source_parent ) ) {
        return false;
    }

    const auto index { sourceModel()->index( source_row, 0, source_parent ) };

    const auto out = index.data( MyModel::MyRoles::Processing ).toBool();

    return out;
}

void ProcessingModel::setSource( QAbstractItemModel *newSource ) {
    setSourceModel( newSource );
}
