#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <QList>

#include "ModelData.h"

class MyModel : public QAbstractListModel {
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY( bool toProcessing READ toProcessing NOTIFY toProcessingChanged FINAL )
    Q_PROPERTY( int selectedCount READ selectedCount NOTIFY selectedCountChanged FINAL )
    Q_PROPERTY( int finishedCount READ finishedCount NOTIFY finishedCountChanged FINAL )

public:
    enum MyRoles {
        Title = Qt::UserRole + 1,
        Selected,
        Progress,
        ProcessTime,
        Processing,
        Finished,
        LastEdit,
    };
    explicit MyModel( QObject *parent = nullptr );

    int rowCount( const QModelIndex &parent ) const override;
    QVariant data( const QModelIndex &index, int role ) const override;
    bool setData( const QModelIndex &index, const QVariant &value, int role ) override;
    QHash<int, QByteArray> roleNames() const override;

    bool toProcessing() const;

    Q_INVOKABLE void processing();
    Q_INVOKABLE void unselectAll();
    Q_INVOKABLE void clearHistory();

    int selectedCount() const;

    int finishedCount() const;

Q_SIGNALS:
    void toProcessingChanged();

    void selectedCountChanged();

    void finishedCountChanged();

private:
    QList<ModelData *> m_data;
    bool m_toProcessing;
    int m_selectedCount;
    int m_finishedCount;
};
