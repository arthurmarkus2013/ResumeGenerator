#ifndef ITEMTEMPLATEINFO_H
#define ITEMTEMPLATEINFO_H

#include <QColor>
#include <QPoint>
#include <QString>

enum class ItemType
{
  IMAGE,
  TEXT,
  SHAPE,
  PAGE
};

enum class ShapeType
{
  RECTANGLE,
  CIRCLE,
  ELLIPSE
};

enum class TextType
{
    FULL_NAME,
    JOB_TITLE,
    EDUCATION_HEADER,
    WORK_EXPERIENCE_HEADER,
    SCHOOL_NAME,
    COMPANY_NAME,
    EDUCATION_START_DATE,
    EDUCATION_END_DATE,
    JOB_START_DATE,
    JOB_END_DATE,
    EMAIL_ADDRESS,
    ADDRESS,
    BIRTH_DATE,
    PHONE_NUMBER,
    SCHOOL_LOCATION,
    COMPANY_LOCATION,
    CURSE_NAME,
    TECHNOLOGIES_USED,
    DUTIES,
    NATIONALITY
};

struct ShapeInfo
{
  ShapeType type;
  QColor fill_color;
  QPoint pos;
  QPoint size;

  bool isValid()
  {
    bool validType = ((type == ShapeType::CIRCLE) || (type == ShapeType::ELLIPSE) || (type == ShapeType::RECTANGLE));
    bool validRect = ((pos.x() >= 0) && (pos.y() >= 0) && (size.x() != 0) && (size.y() != 0));

    return (validType && validRect && fill_color.isValid());
  }

  QString toString()
  {
    QString shapeName;

    switch (type)
    {
    case ShapeType::RECTANGLE:
      shapeName = "Rectangle";
      break;
    case ShapeType::CIRCLE:
      shapeName = "Circle";
      break;
    case ShapeType::ELLIPSE:
      shapeName = "Ellipse";
      break;
    default:
      shapeName = "Unknown";
      break;
    }

    return "ShapeInfo {\n\ttype: \"" + shapeName + "\",\n\tfill_color: \"" +
           fill_color.name() +
           "\",\n\tpos: {\n\t\tx: " + QString::number(pos.x()) +
           ",\n\t\ty: " + QString::number(pos.y()) +
           "\n\t},\n\tsize: {\n\t\tx: " + QString::number(size.x()) +
           ",\n\t\ty: " + QString::number(size.y()) + "\n\t}\n}";
  }
};

struct TextInfo
{
  // QString contents;
  int font_size = 8;
  float line_spacing = 1.3f;
  QString font_name = "Arial.ttf";
  QColor color;
  TextType type;
  int index = -1;


  bool isValid()
  {
    bool validType = ((type == TextType::FULL_NAME) || (type == TextType::NATIONALITY) || (type == TextType::BIRTH_DATE) ||
                      (type == TextType::EMAIL_ADDRESS) || (type == TextType::ADDRESS) || (type == TextType::PHONE_NUMBER) ||
                      (type == TextType::SCHOOL_NAME) || (type == TextType::SCHOOL_LOCATION) || (type == TextType::CURSE_NAME) ||
                      (type == TextType::EDUCATION_START_DATE) || (type == TextType::EDUCATION_END_DATE) || (type == TextType::EDUCATION_HEADER) ||
                      (type == TextType::COMPANY_NAME) || (type == TextType::COMPANY_LOCATION) || (type == TextType::JOB_TITLE) ||
                      (type == TextType::JOB_START_DATE) || (type == TextType::JOB_END_DATE) || (type == TextType::WORK_EXPERIENCE_HEADER) ||
                      (type == TextType::TECHNOLOGIES_USED) || (type == TextType::DUTIES));

    return ((font_size > 0) && (line_spacing > 0) && !font_name.isEmpty() && validType);
  }

  QString toString()
  {
    QString textType;

    switch(type)
    {
        case TextType::FULL_NAME:
            textType = "Full Name";
            break;
        case TextType::JOB_TITLE:
            textType = "Job Title";
            break;
        case TextType::EDUCATION_HEADER:
            textType = "Education Header";
            break;
        case TextType::WORK_EXPERIENCE_HEADER:
            textType = "Work Experience Header";
            break;
        case TextType::SCHOOL_NAME:
            textType = "School Name";
            break;
        case TextType::COMPANY_NAME:
            textType = "Company Name";
            break;
        case TextType::EDUCATION_START_DATE:
            textType = "Education Start Date";
            break;
        case TextType::EDUCATION_END_DATE:
            textType = "Education End Date";
            break;
        case TextType::JOB_START_DATE:
            textType = "Job Start Date";
            break;
        case TextType::JOB_END_DATE:
            textType = "Job End Date";
            break;
        case TextType::EMAIL_ADDRESS:
            textType = "E-Mail Address";
            break;
        case TextType::ADDRESS:
            textType = "Address";
            break;
        case TextType::BIRTH_DATE:
            textType = "Birth Date";
            break;
        case TextType::PHONE_NUMBER:
            textType = "Phone Number";
            break;
        case TextType::SCHOOL_LOCATION:
            textType = "School Location";
            break;
        case TextType::COMPANY_LOCATION:
            textType = "Company Location";
            break;
        case TextType::CURSE_NAME:
            textType = "Curse Name";
            break;
        case TextType::TECHNOLOGIES_USED:
            textType = "Technologies Used";
            break;
        case TextType::DUTIES:
            textType = "Duties";
            break;
        case TextType::NATIONALITY:
            textType = "Nationality";
            break;
        default:
            textType = "Unknown";
            break;
    }

    return "TextInfo {\n\ttype: \"" + textType +
           "\",\n\tfont_size: " + QString::number(font_size) +
           ",\n\tline_Spacing: " + QString::number(line_spacing) +
           ",\n\tfont_name: \"" + font_name + "\",\n\tcolor: \"" +
           color.name() + "\",\n\tindex: " + QString::number(index) + "\n}";
  }
};

struct ImageInfo
{
  QString src_file;
  QString mask_file;

  bool isValid()
  {
    return !src_file.isEmpty();
  }

  QString toString()
  {
    return "ImageInfo {\n\tsrc_file: \"" + src_file + "\",\n\tmask_file: \"" + mask_file + "\"\n}";
  }
};

enum class PageOrientation
{
  PORTRAIT,
  LANDSCAPE
};

struct ItemTemplateInfo
{
  ItemType type;
  QPoint pos;
  QPoint size;
  int page_margin = 30;
  PageOrientation orientation = PageOrientation::PORTRAIT;
  TextInfo text;
  ShapeInfo shape;
  ImageInfo image;
  QList<ItemTemplateInfo> children;

  bool isValid()
  {
    switch (type)
    {
    case ItemType::PAGE:
      return true;
    case ItemType::IMAGE:
      return (image.isValid() && (pos.x() >= 0) && (pos.y() >= 0) &&
              (size.x() > 0) && (size.y() > 0));
    case ItemType::SHAPE:
      return shape.isValid();
    case ItemType::TEXT:
      return (text.isValid() && (pos.x() >= 0) && (pos.y() >= 0) &&
              (size.x() > 0) && (size.y() > 0));
    default:
      return false;
    }
  }

  QString toString()
  {
    QString itemName;

    switch (type)
    {
    case ItemType::IMAGE:
      itemName = "Image";
      break;
    case ItemType::TEXT:
      itemName = "Text";
      break;
    case ItemType::SHAPE:
      itemName = "Shape";
      break;
    case ItemType::PAGE:
      itemName = "Page";
      break;
    default:
      itemName = "Unknown";
      break;
    }

    QString stringified_children;

    for (ItemTemplateInfo info : children)
    {
      stringified_children += "{\n\t" + info.toString() + "\n}\n";
    }

    return "ItemTemplateInfo {\n\ttype: \"" + itemName + "\",\n\tpos: {\n\t\tx: " + QString::number(pos.x()) +
           ",\n\t\ty: " + QString::number(pos.y()) + "},\n\t\tsize: {x: " + QString::number(size.x()) +
           ",\n\t\theight: " + QString::number(size.y()) + "\n\t},\n\tpage_margin: " + QString::number(page_margin) +
           ",\n\ttext: " + text.toString() + ",\n\tshape: " + shape.toString() + ",\n\timage: " + image.toString() +
           ",\n\tPageOrientation: " + ((orientation == PageOrientation::PORTRAIT) ? "\"Portrait\"" : "\"Landscape\"") +
           ",\n\tchildren: [\n" + stringified_children + "]\n}";
  }
};

#endif // ITEMTEMPLATEINFO_H
