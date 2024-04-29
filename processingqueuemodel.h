#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <QQmlEngine>
#include <QThread>

#include "ModelData.h"

class ProgressData : public ModelData {

    Q_OBJECT
public:
    ProgressData(QString title, int processTime, bool selected = false, double progress = 0, bool paused = false, QObject* parent = nullptr)
        : ModelData(title, processTime, selected, parent)
        , progress(progress)
        , paused(paused){}
    double progress;
    bool paused;

    // QThread interface
protected:
    void run() override;
};

class ProcessingQueueModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QList<ModelData*> dataSource READ dataSource WRITE setDataSource NOTIFY dataSourceChanged FINAL)

public:
    enum ProcessingRoles {
        Title = Qt::UserRole + 1,
        Progress,
        Paused,
    };

    explicit ProcessingQueueModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;

    QList<ModelData*> dataSource() const;
    void setDataSource(const QList<ModelData*> &newDataSource);

Q_SIGNALS:
    void dataSourceChanged();

private:
    QList<ProgressData*> m_processingData;
    QList<ModelData*> m_dataSource;

    void heavyProcessingData();
};
