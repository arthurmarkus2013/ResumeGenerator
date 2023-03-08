#ifndef EDUCATIONDATAMODEL_H
#define EDUCATIONDATAMODEL_H

#include <QAbstractTableModel>

#include "EducationEntry.h"
#include "databaseoperations.h"

class EducationDataModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit EducationDataModel(QObject *parent = nullptr);
    ~EducationDataModel();

    void persistValidChanges();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    void invalidateCache();
    bool setItemAtPosition(int row, int column, QVariant value);
    QVariant getItemAtPosition(int row, int column) const;
    QList<EducationEntry> m_entriesCache;
    QList<EducationEntry> m_DeletionQueue;
    DatabaseOperations *db;
};

#endif // EDUCATIONDATAMODEL_H
