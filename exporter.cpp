#include "exporter.h"
#include "databaseoperations.h"
#include "hpdf.h"
#include "pathhelpers.h"

#include <QDateTime>
#include <QStandardPaths>
#include <QMessageBox>

Exporter::Exporter(QObject *parent)
    : QObject{parent}
{

}

void pdf_error_handler(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
    auto msg = Exporter::tr("ERROR") + ": error_no = " + QString::number((unsigned int) error_no)
               + " detail_no = " + QString::number((int) detail_no);
    throw std::exception(msg.toUtf8()); /* throw exception on error */
}

void mapData(QList<ItemTemplateInfo> &infos, QMap<int, QList<ItemTemplateInfo>> &map, int index = 0, int max_nesting = 5)
{
    if(max_nesting < 0)
        return;

    for(auto &info : infos)
    {
        auto tmp = info;
        tmp.children.clear();

        map[index].append(tmp);

        if(!info.children.empty())
            mapData(info.children, map, index + 1, max_nesting - 1);
    }
}

QList<ItemTemplateInfo> flattenData(QMap<int, QList<ItemTemplateInfo>> &map)
{
    QList<ItemTemplateInfo> retVal;

    int count = map.count();

    for(int i = 0; i < count; i++)
    {
        retVal.append(map[i]);
    }

    return retVal;
}

QList<ItemTemplateInfo> prepareData(QList<ItemTemplateInfo> &infos)
{
    QMap<int, QList<ItemTemplateInfo>> map;

    mapData(infos, map);
    return flattenData(map);
}

QString findText(TextType type, int index)
{
    DatabaseOperations db;
    QString retVal = "";
    QList<DatabaseOperations::Entry> data;

    switch(type)
    {
        case TextType::FULL_NAME:
            data = db.getEntries(EntryType::PERSONAL_INFO_ENTRY);

            if(data.empty())
                return QString();

            retVal = std::get<PersonalInfoEntry>(data.at(0)).first_name;
            if(!std::get<PersonalInfoEntry>(data.at(0)).middle_name.isEmpty())
                retVal += " " + std::get<PersonalInfoEntry>(data.at(0)).middle_name;
            retVal += " " + std::get<PersonalInfoEntry>(data.at(0)).last_name;
            break;
        case TextType::JOB_TITLE:
            data = db.getEntries(EntryType::WORK_EXPERIENCE_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<WorkExperienceEntry>(data.at(index)).job_title;
            break;
        case TextType::EDUCATION_HEADER:
            retVal = QObject::tr("Education");
            break;
        case TextType::WORK_EXPERIENCE_HEADER:
            retVal = QObject::tr("Work experience");
            break;
        case TextType::SCHOOL_NAME:
            data = db.getEntries(EntryType::EDUCATION_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<EducationEntry>(data.at(index)).school_name;
            break;
        case TextType::COMPANY_NAME:
            data = db.getEntries(EntryType::WORK_EXPERIENCE_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<WorkExperienceEntry>(data.at(index)).company_name;
            break;
        case TextType::EDUCATION_START_DATE:
            data = db.getEntries(EntryType::EDUCATION_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<EducationEntry>(data.at(index)).start_date.toString();
            break;
        case TextType::EDUCATION_END_DATE:
            data = db.getEntries(EntryType::EDUCATION_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<EducationEntry>(data.at(index)).end_date.toString();
            break;
        case TextType::JOB_START_DATE:
            data = db.getEntries(EntryType::WORK_EXPERIENCE_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<WorkExperienceEntry>(data.at(index)).start_date.toString();
            break;
        case TextType::JOB_END_DATE:
            data = db.getEntries(EntryType::WORK_EXPERIENCE_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<WorkExperienceEntry>(data.at(index)).end_date.toString();
            break;
        case TextType::EMAIL_ADDRESS:
            data = db.getEntries(EntryType::PERSONAL_INFO_ENTRY);

            if(data.empty())
                return QString();

            retVal = std::get<PersonalInfoEntry>(data.at(0)).email_address;
            break;
        case TextType::ADDRESS:
            data = db.getEntries(EntryType::PERSONAL_INFO_ENTRY);

            if(data.empty())
                return QString();

            retVal = std::get<PersonalInfoEntry>(data.at(0)).address;
            break;
        case TextType::BIRTH_DATE:
            data = db.getEntries(EntryType::PERSONAL_INFO_ENTRY);

            if(data.empty())
                return QString();

            retVal = std::get<PersonalInfoEntry>(data.at(0)).birth_date.toString();
            break;
        case TextType::PHONE_NUMBER:
            data = db.getEntries(EntryType::PERSONAL_INFO_ENTRY);

            if(data.empty())
                return QString();

            retVal = std::get<PersonalInfoEntry>(data.at(0)).phone_number;
            break;
        case TextType::SCHOOL_LOCATION:
            data = db.getEntries(EntryType::EDUCATION_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<EducationEntry>(data.at(index)).location;
            break;
        case TextType::COMPANY_LOCATION:
            data = db.getEntries(EntryType::WORK_EXPERIENCE_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<WorkExperienceEntry>(data.at(index)).location;
            break;
        case TextType::CURSE_NAME:
            data = db.getEntries(EntryType::EDUCATION_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<EducationEntry>(data.at(index)).curse_name;
            break;
        case TextType::TECHNOLOGIES_USED:
            data = db.getEntries(EntryType::WORK_EXPERIENCE_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<WorkExperienceEntry>(data.at(index)).technologies_used;
            break;
        case TextType::DUTIES:
            data = db.getEntries(EntryType::WORK_EXPERIENCE_ENTRY);

            if(index >= data.count())
                return QString();

            retVal = std::get<WorkExperienceEntry>(data.at(index)).duties;
            break;
        case TextType::NATIONALITY:
            data = db.getEntries(EntryType::PERSONAL_INFO_ENTRY);

            if(data.empty())
                return QString();

            retVal = std::get<PersonalInfoEntry>(data.at(0)).nationality;
            break;
        default:
            break;
    }

    return retVal;
}

QString findImageFilePath(ItemTemplateInfo &info)
{
    QString retVal = info.image.file_path;

    if(info.image.type == ImageType::PROFILE_PHOTO)
    {
        DatabaseOperations db;

        auto data = db.getEntries(EntryType::PERSONAL_INFO_ENTRY);

        if(data.empty())
            return QString();

        retVal = std::get<PersonalInfoEntry>(data.at(0)).profile_photo;
    }

    return PathHelpers::convertToNativeSeparators(retVal);
}

bool Exporter::exportAsPDF(QString path, QList<ItemTemplateInfo> infos)
{
    HPDF_Doc pdf = HPDF_New(pdf_error_handler, nullptr);

    if (!pdf) {
        QMessageBox::critical(static_cast<QWidget*>(parent()), tr("Fatal Error"), tr("ERROR: Can't create PDF object"));
        return false;
    }

    try
    {
        HPDF_UseUTFEncodings(pdf);
        HPDF_SetCompressionMode(pdf, HPDF_COMP_ALL);
        HPDF_SetPageMode(pdf, HPDF_PAGE_MODE_USE_NONE);
        HPDF_SetCurrentEncoder(pdf, "UTF-8");
        HPDF_SetInfoAttr(pdf, HPDF_INFO_TITLE, tr("Resume").toUtf8());

        HPDF_Date info {0};
        auto now = QDateTime::currentDateTime();
        info.seconds = now.time().second();
        info.minutes = now.time().minute();
        info.hour = now.time().hour();
        info.day = now.date().day();
        info.month = now.date().month();
        info.year = now.date().year();
        info.ind = 'Z';
        HPDF_SetInfoDateAttr(pdf, HPDF_INFO_CREATION_DATE, info);

        for(auto &page_info : infos)
        {
            if(!page_info.isValid() || (page_info.type != ItemType::PAGE))
                continue;

            HPDF_Page page = HPDF_AddPage(pdf);

            auto page_Children = prepareData(page_info.children);

            for(auto &info : page_Children)
            {
                if((HPDF_Page_GetHeight(page) - (info.size.y() + info.pos.y())) < info.page_margin)
                    page = HPDF_AddPage(pdf);

                switch(info.type)
                {
                    case ItemType::IMAGE:
                        HPDF_Image image;

                        if(info.image.file_type == ImageFileType::JPEG)
                            image = HPDF_LoadJpegImageFromFile(pdf, findImageFilePath(info).toUtf8());
                        else if(info.image.file_type == ImageFileType::PNG)
                            image = HPDF_LoadPngImageFromFile(pdf, findImageFilePath(info).toUtf8());
                        else
                            continue;

                        if(image)
                        {
                            if(info.image.mask_color.isValid())
                            {
                                HPDF_Image_SetColorMask(image, info.image.mask_color.redF(), info.image.mask_color.redF(),
                                                        info.image.mask_color.greenF(), info.image.mask_color.greenF(),
                                                        info.image.mask_color.blueF(), info.image.mask_color.blueF());
                            }

                            HPDF_Page_DrawImage(page, image, info.page_margin + info.pos.x(),
                                                HPDF_Page_GetHeight(page) - (info.page_margin + info.pos.y() + info.size.y()),
                                                info.size.x(), info.size.y());

                            if(info.image.border > 0)
                            {
                                HPDF_Page_SetLineWidth(page, info.image.border);
                                HPDF_Page_SetRGBStroke(page, info.image.border_color.redF(), info.image.border_color.greenF(),
                                                       info.image.border_color.blueF());
                                HPDF_Page_Rectangle(page, info.page_margin + info.pos.x(),
                                                    HPDF_Page_GetHeight(page) - (info.page_margin + info.pos.y() + info.size.y()),
                                                    info.size.x(), info.size.y());
                                HPDF_Page_Stroke(page);
                            }
                        }

                        continue;
                    case ItemType::TEXT:
                        HPDF_Font font;
                        font = HPDF_GetFont(pdf, HPDF_LoadTTFontFromFile(pdf,
                                                                         QStandardPaths::locate(QStandardPaths::FontsLocation, info.text.font_name)
                                                                             .toUtf8(), true), "UTF-8");
                        HPDF_Page_SetFontAndSize(page, font, info.text.font_size);
                        HPDF_Page_BeginText(page);
                        HPDF_Page_SetTextLeading(page, info.text.font_size * info.text.line_spacing);
                        HPDF_Page_SetRGBFill(page, info.text.color.redF(), info.text.color.greenF(), info.text.color.blueF());
                        HPDF_Page_TextRect(page, info.page_margin + info.pos.x(), HPDF_Page_GetHeight(page) - (info.page_margin + info.pos.y()),
                                           info.pos.x() + info.size.x(), info.pos.y() + info.size.y(),
                                           findText(info.text.type, info.text.index).toUtf8(), HPDF_TALIGN_LEFT, nullptr);
                        HPDF_Page_EndText(page);
                        continue;
                    case ItemType::SHAPE:
                        switch(info.shape.type)
                        {
                            case ShapeType::RECTANGLE:
                                HPDF_Page_SetLineWidth(page, info.shape.border);
                                HPDF_Page_SetRGBFill(page, info.shape.fill_color.redF(), info.shape.fill_color.greenF(),
                                                        info.shape.fill_color.blueF());
                                HPDF_Page_SetRGBStroke(page, info.shape.border_color.redF(), info.shape.border_color.greenF(),
                                                        info.shape.border_color.blueF());
                                HPDF_Page_Rectangle(page, info.page_margin + info.shape.pos.x(),
                                                    HPDF_Page_GetHeight(page) - (info.page_margin + info.shape.pos.y()),
                                                    info.shape.size.x(), info.shape.size.y());
                                if(info.shape.border > 0)
                                    HPDF_Page_FillStroke(page);
                                else
                                    HPDF_Page_Fill(page);
                                continue;
                            case ShapeType::CIRCLE:
                                HPDF_Page_SetLineWidth(page, info.shape.border);
                                HPDF_Page_SetRGBFill(page, info.shape.fill_color.redF(), info.shape.fill_color.greenF(),
                                                     info.shape.fill_color.blueF());
                                HPDF_Page_SetRGBStroke(page, info.shape.border_color.redF(), info.shape.border_color.greenF(),
                                                       info.shape.border_color.blueF());
                                HPDF_Page_Circle(page, info.page_margin + info.shape.pos.x(),
                                                 HPDF_Page_GetHeight(page) - (info.page_margin + info.shape.pos.y()), info.shape.size.x());
                                if(info.shape.border > 0)
                                    HPDF_Page_FillStroke(page);
                                else
                                    HPDF_Page_Fill(page);
                                continue;
                            case ShapeType::ELLIPSE:
                                HPDF_Page_SetLineWidth(page, info.shape.border);
                                HPDF_Page_SetRGBFill(page, info.shape.fill_color.redF(), info.shape.fill_color.greenF(),
                                                     info.shape.fill_color.blueF());
                                HPDF_Page_SetRGBStroke(page, info.shape.border_color.redF(), info.shape.border_color.greenF(),
                                                       info.shape.border_color.blueF());
                                HPDF_Page_Ellipse(page, info.page_margin + info.shape.pos.x(),
                                                  HPDF_Page_GetHeight(page) - (info.page_margin + info.shape.pos.y()),
                                                  info.shape.size.x(), info.shape.size.y());
                                if(info.shape.border > 0)
                                    HPDF_Page_FillStroke(page);
                                else
                                    HPDF_Page_Fill(page);
                                continue;
                            default:
                                continue;
                        }
                        continue;
                    case ItemType::LINE:
                        HPDF_Page_SetLineWidth(page, info.line.thickness);
                        HPDF_Page_MoveTo(page, info.page_margin + info.line.startPos.x(),
                                         HPDF_Page_GetHeight(page) - (info.page_margin + info.line.startPos.y()));
                        HPDF_Page_LineTo(page, info.line.endPos.x(), info.line.endPos.y());
                        HPDF_Page_Stroke(page);
                        continue;
                    default:
                        // TODO: Add a warning message???
                        continue;
                }
            }
        }

        HPDF_SaveToFile(pdf, path.toUtf8());
    }
    catch(std::exception ex)
    {
        QMessageBox::critical(static_cast<QWidget*>(parent()), tr("Failed To Export PDF"), ex.what());

        HPDF_Free(pdf);
        return false;
    }

    HPDF_Free(pdf);

    return true;
}
