#include "educationdatamodel.h"
#include "databaseoperations.h"

EducationDataModel::EducationDataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    db = new DatabaseOperations(parent);

    invalidateCache();
}

EducationDataModel::~EducationDataModel()
{
    delete db;
}

void EducationDataModel::invalidateCache()
{
    auto items = db->getEntries(EntryType::EDUCATION_ENTRY);

    for(auto &item : items)
    {
        m_entriesCache.append(std::get<EducationEntry>(item));
    }
}

void EducationDataModel::persistValidChanges()
{
    for(auto &item : m_entriesCache)
    {
        if(item.isValid())
            if(!db->updateEntry(item, EntryType::EDUCATION_ENTRY))
                item.education_id = db->addEntry(item, EntryType::EDUCATION_ENTRY);
    }

    for(int i = 0; i < m_DeletionQueue.count(); i++)
    {
        auto item = m_DeletionQueue.at(i);

        if(item.isValid())
            if(db->removeEntry(item, EntryType::EDUCATION_ENTRY))
                m_DeletionQueue.removeAt(i);
    }
}

bool EducationDataModel::setItemAtPosition(int row, int column, QVariant value)
{
    auto item = m_entriesCache.at(row);

    switch (column) {
        case 0:
            item.curse_name = value.toString();
            break;
        case 1:
            item.school_name = value.toString();
            break;
        case 2:
            item.location = value.toString();
            break;
        case 3:
            item.start_date = value.toDate();
            break;
        case 4:
            item.end_date = value.toDate();
            break;
        default:
            return false;
    }

    m_entriesCache.replace(row, item);
    return true;
}

QVariant EducationDataModel::getItemAtPosition(int row, int column) const
{
    switch (column) {
        case 0:
            return m_entriesCache.at(row).curse_name;
        case 1:
            return m_entriesCache.at(row).school_name;
        case 2:
            return m_entriesCache.at(row).location;
        case 3:
            return m_entriesCache.at(row).start_date;
        case 4:
            return m_entriesCache.at(row).end_date;
        default:
            return QVariant();
    }
}

QVariant EducationDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((role == Qt::DisplayRole) && (orientation == Qt::Horizontal))
    {
        switch(section)
        {
            case 0:
                return "Curse Name";
            case 1:
                return "School Name";
            case 2:
                return "Location";
            case 3:
                return "Start Date";
            case 4:
                return "End Date";
            default:
                return QVariant();
        }
    }

    return QVariant();
}

int EducationDataModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_entriesCache.count();
}

int EducationDataModel::columnCount(const QModelIndex &parent) const
{
    return 5;
}

bool EducationDataModel::hasChildren(const QModelIndex &parent) const
{
    return !m_entriesCache.empty();
}

QVariant EducationDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch(role)
    {
        case Qt::DisplayRole:
        case Qt::EditRole:
            return getItemAtPosition(index.row(), index.column());
        default:
            return QVariant();
    }
}

bool EducationDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        setItemAtPosition(index.row(), index.column(), value);
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags EducationDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

bool EducationDataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for(int i = 0; i < count; i++)
    {
        m_entriesCache.insert(row + i, EducationEntry());
    }
    endInsertRows();
    return true;
}

bool EducationDataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i = 0; i < count; i++)
    {
        auto item = m_entriesCache.at(i);
        m_entriesCache.removeAt(i);
        m_DeletionQueue.append(item);
    }
    endRemoveRows();
    return true;
}
