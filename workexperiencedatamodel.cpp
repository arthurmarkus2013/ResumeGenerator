#include "workexperiencedatamodel.h"
#include "databaseoperations.h"

WorkExperienceDataModel::WorkExperienceDataModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    db = new DatabaseOperations(parent);

    invalidateCache();
}

WorkExperienceDataModel::~WorkExperienceDataModel()
{
    delete db;
}

void WorkExperienceDataModel::invalidateCache()
{
    auto items = db->getEntries(EntryType::WORK_EXPERIENCE_ENTRY);

    for(auto &item : items)
    {
        m_entriesCache.append(std::get<WorkExperienceEntry>(item));
    }
}

void WorkExperienceDataModel::persistValidChanges()
{
    m_hasInvalidData = false;

    for(auto &item : m_entriesCache)
    {
        if(item.isValid())
        {
            if(!db->updateEntry(item, EntryType::WORK_EXPERIENCE_ENTRY))
                item.work_Experience_id = db->addEntry(item, EntryType::WORK_EXPERIENCE_ENTRY);
        }
        else
            m_hasInvalidData = true;
    }

    int count = m_DeletionQueue.count();

    for(int i = 0; i < count; i++)
    {
        if(m_DeletionQueue.empty())
            break;

        auto item = m_DeletionQueue.takeFirst();

        if(item.isValid())
            if(!db->removeEntry(item, EntryType::WORK_EXPERIENCE_ENTRY))
                m_DeletionQueue.append(item);
    }
}

bool WorkExperienceDataModel::setItemAtPosition(int row, int column, QVariant value)
{
    auto item = m_entriesCache.at(row);

    switch (column) {
        case 0:
            item.company_name = value.toString();
            break;
        case 1:
            item.job_title = value.toString();
            break;
        case 2:
            item.start_date = value.toDate();
            break;
        case 3:
            item.end_date = value.toDate();
            break;
        case 4:
            item.technologies_used = value.toString();
            break;
        case 5:
            item.duties = value.toString();
            break;
        case 6:
            item.location = value.toString();
            break;
        default:
            return false;
    }

    m_entriesCache.replace(row, item);
    return true;
}

QVariant WorkExperienceDataModel::getItemAtPosition(int row, int column) const
{
    switch (column) {
        case 0:
            return m_entriesCache.at(row).company_name;
        case 1:
            return m_entriesCache.at(row).job_title;
        case 2:
            return m_entriesCache.at(row).start_date;
        case 3:
            return m_entriesCache.at(row).end_date;
        case 4:
            return m_entriesCache.at(row).technologies_used;
        case 5:
            return m_entriesCache.at(row).duties;
        case 6:
            return m_entriesCache.at(row).location;
        default:
            return QVariant();
    }
}

QVariant WorkExperienceDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if((role == Qt::DisplayRole) && (orientation == Qt::Horizontal))
    {
        switch(section)
        {
            case 0:
                return "Company Name";
            case 1:
                return "Job Title";
            case 2:
                return "Start Date";
            case 3:
                return "End Date";
            case 4:
                return "Technologies Used";
            case 5:
                return "Duties";
            case 6:
                return "Location";
            default:
                return QVariant();
        }
    }

    return QVariant();
}

int WorkExperienceDataModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid())
        return 0;

    return m_entriesCache.count();
}

int WorkExperienceDataModel::columnCount(const QModelIndex &parent) const
{
    return 7;
}

bool WorkExperienceDataModel::hasChildren(const QModelIndex &parent) const
{
    return !m_entriesCache.empty();
}

QVariant WorkExperienceDataModel::data(const QModelIndex &index, int role) const
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

bool WorkExperienceDataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        setItemAtPosition(index.row(), index.column(), value);
        emit dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags WorkExperienceDataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsSelectable;
}

bool WorkExperienceDataModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for(int i = 0; i < count; i++)
    {
        m_entriesCache.insert(row + i, WorkExperienceEntry());
    }
    endInsertRows();
    return true;
}

bool WorkExperienceDataModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for(int i = 0; i < count; i++)
    {
        auto item = m_entriesCache.at(row + i);
        m_entriesCache.removeAt(row + i);
        m_DeletionQueue.append(item);
    }
    endRemoveRows();
    return true;
}
