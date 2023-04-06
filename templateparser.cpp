#include "templateparser.h"

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>

using namespace xercesc;

#include <QMap>

#include "pathhelpers.h"

TemplateParser::TemplateParser(QObject *parent)
    : QThread{parent}
{

}

bool mapPageAttributes(ItemTemplateInfo &info, QString attr_name, QString attr_value)
{
    if(attr_name == "pageMargin")
    {
        info.page_margin = attr_value.toInt();
    } else if(attr_name == "orientation")
    {
        if(attr_value == "Portrait")
        {
            info.orientation = PageOrientation::PORTRAIT;
        } else if(attr_value == "Landscape")
        {
            info.orientation = PageOrientation::LANDSCAPE;
        } else
        {
            return false;
        }
    } else
    {
        return false;
    }

    return true;
}

bool mapRectangleAndEllipseAttributes(ItemTemplateInfo &info, QString attr_name, QString attr_value)
{
    if(attr_name == "x")
    {
        info.shape.pos.setX(attr_value.toInt());
    } else if(attr_name == "y")
    {
        info.shape.pos.setY(attr_value.toInt());
    } else if(attr_name == "width")
    {
        info.shape.size.setX(attr_value.toInt());
    } else if(attr_name == "height")
    {
        info.shape.size.setY(attr_value.toInt());
    } else if(attr_name == "color")
    {
        info.shape.fill_color = QColor(attr_value);
    } else
    {
        return false;
    }

    return true;
}

bool mapCircleAttributes(ItemTemplateInfo &info, QString attr_name, QString attr_value)
{
    if(attr_name == "x")
    {
        info.shape.pos.setX(attr_value.toInt());
    } else if(attr_name == "y")
    {
        info.shape.pos.setY(attr_value.toInt());
    } else if(attr_name == "radius")
    {
        info.shape.size.setX(attr_value.toInt());
        info.shape.size.setY(attr_value.toInt());
    } else if(attr_name == "color")
    {
        info.shape.fill_color = QColor(attr_value);
    } else
    {
        return false;
    }

    return true;
}

QString base_path = "";

bool mapImageAttributes(ItemTemplateInfo &info, QString attr_name, QString attr_value)
{
    if(attr_name == "x")
    {
        info.pos.setX(attr_value.toInt());
    } else if(attr_name == "y")
    {
        info.pos.setY(attr_value.toInt());
    } else if(attr_name == "width")
    {
        info.size.setX(attr_value.toInt());
    } else if(attr_name == "height")
    {
        info.size.setY(attr_value.toInt());
    } else if(attr_name == "src")
    {
        if((PathHelpers::getFileExtension(attr_value) == "jpeg") || (PathHelpers::getFileExtension(attr_value) == "jpg"))
            info.image.file_type = ImageFileType::JPEG;
        else if(PathHelpers::getFileExtension(attr_value) == "png")
            info.image.file_type = ImageFileType::PNG;
        else
            return false;

        if(PathHelpers::isAbsolutePath(attr_value))
            info.image.file_path = attr_value;
        else
            info.image.file_path = PathHelpers::combine({base_path, attr_value});
    } else if(attr_name == "mask")
    {
        info.image.mask_color = QColor(attr_value);
    } else if(attr_name == "type")
    {
        if(attr_value == "ProfilePhoto")
        {
            info.image.type = ImageType::PROFILE_PHOTO;
        } else if(attr_value == "Decoration")
        {
            info.image.type = ImageType::DECORATION;
        } else
        {
            return false;
        }
    } else
    {
        return false;
    }

    return true;
}

bool mapTextTypeAttribute(ItemTemplateInfo &info, QString value)
{
    if(value == "FullName")
    {
        info.text.type = TextType::FULL_NAME;
    } else if(value == "Nationality")
    {
        info.text.type = TextType::NATIONALITY;
    } else if(value == "BirthDate")
    {
        info.text.type = TextType::BIRTH_DATE;
    } else if(value == "EmailAddress")
    {
        info.text.type = TextType::EMAIL_ADDRESS;
    } else if(value == "Address")
    {
        info.text.type = TextType::ADDRESS;
    } else if(value == "WorkExperienceHeader")
    {
        info.text.type = TextType::WORK_EXPERIENCE_HEADER;
    } else if(value == "EducationHeader")
    {
        info.text.type = TextType::EDUCATION_HEADER;
    } else if(value == "CompanyName")
    {
        info.text.type = TextType::COMPANY_NAME;
    } else if(value == "CompanyLocation")
    {
        info.text.type = TextType::COMPANY_LOCATION;
    } else if(value == "JobTitle")
    {
        info.text.type = TextType::JOB_TITLE;
    } else if(value == "JobStartDate")
    {
        info.text.type = TextType::JOB_START_DATE;
    } else if(value == "JobEndDate")
    {
        info.text.type = TextType::JOB_END_DATE;
    } else if(value == "TechnologiesUsed")
    {
        info.text.type = TextType::TECHNOLOGIES_USED;
    } else if(value == "Duties")
    {
        info.text.type = TextType::DUTIES;
    } else if(value == "SchoolName")
    {
        info.text.type = TextType::SCHOOL_NAME;
    } else if(value == "SchoolLocation")
    {
        info.text.type = TextType::SCHOOL_LOCATION;
    } else if(value == "EducationStartDate")
    {
        info.text.type = TextType::EDUCATION_START_DATE;
    } else if(value == "EducationEndDate")
    {
        info.text.type = TextType::EDUCATION_END_DATE;
    } else if(value == "CurseName")
    {
        info.text.type = TextType::CURSE_NAME;
    } else if(value == "PhoneNumber")
    {
        info.text.type = TextType::PHONE_NUMBER;
    } else
    {
        return false;
    }

    return true;
}

bool mapTextAttributes(ItemTemplateInfo &info, QString attr_name, QString attr_value)
{
    if(attr_name == "x")
    {
        info.pos.setX(attr_value.toInt());
    } else if(attr_name == "y")
    {
        info.pos.setY(attr_value.toInt());
    } else if(attr_name == "width")
    {
        info.size.setX(attr_value.toInt());
    } else if(attr_name == "height")
    {
        info.size.setY(attr_value.toInt());
    } else if(attr_name == "fontSize")
    {
        info.text.font_size = attr_value.toInt();
    } else if(attr_name == "fontFamily")
    {
        info.text.font_name = attr_value;
    } else if(attr_name == "lineSpacing")
    {
        info.text.line_spacing = attr_value.toFloat();
    } else if(attr_name == "color")
    {
        info.text.color = QColor(attr_value);
    } else if(attr_name == "type")
    {
        return mapTextTypeAttribute(info, attr_value);
    } else
    {
        return false;
    }

    return true;
}

bool mapLineAttributes(ItemTemplateInfo &info, QString attr_name, QString attr_value)
{
    if(attr_name == "fromX")
    {
        info.line.startPos.setX(attr_value.toInt());
    } else if(attr_name == "fromY")
    {
        info.line.startPos.setY(attr_value.toInt());
    } else if(attr_name == "toX")
    {
        info.line.endPos.setX(attr_value.toInt());
    } else if(attr_name == "toY")
    {
        info.line.endPos.setY(attr_value.toInt());
    } else if(attr_name == "color")
    {
        info.text.color = QColor(attr_value);
    } else
    {
        return false;
    }

    return true;
}

bool mapAttributes(ItemTemplateInfo &info, DOMElement *node)
{
    auto attrs = node->getAttributes();
    auto count = attrs->getLength();

    for(int i = 0; i < count; i++)
    {
        if(attrs->item(i)->getNodeType() == DOMNode::ATTRIBUTE_NODE)
        {
            auto attr = dynamic_cast<DOMAttr*>(attrs->item(i));

            QString attr_name(XMLString::transcode(attr->getName()));
            QString attr_value(XMLString::transcode(attr->getValue()));

            if(info.type == ItemType::PAGE)
            {
                if(!mapPageAttributes(info, attr_name, attr_value))
                    return false;
            } else if(((info.type == ItemType::SHAPE) && (info.shape.type == ShapeType::RECTANGLE)) ||
                ((info.type == ItemType::SHAPE) && (info.shape.type == ShapeType::ELLIPSE)))
            {
                if(!mapRectangleAndEllipseAttributes(info, attr_name, attr_value))
                    return false;
            } else if((info.type == ItemType::SHAPE) && (info.shape.type == ShapeType::CIRCLE))
            {
                if(!mapCircleAttributes(info, attr_name, attr_value))
                    return false;
            } else if(info.type == ItemType::IMAGE)
            {
                if(!mapImageAttributes(info, attr_name, attr_value))
                    return false;
            } else if(info.type == ItemType::TEXT)
            {
                if(!mapTextAttributes(info, attr_name, attr_value))
                    return false;
            } else if(info.type == ItemType::LINE)
            {
                if(!mapLineAttributes(info, attr_name, attr_value))
                    return false;
            } else
            {
                return false;
            }
        }
    }

    return true;
}

bool determineChildType(ItemTemplateInfo &info, QString tagName)
{
    if(tagName == "page")
    {
        info.type = ItemType::PAGE;
    } else if(tagName == "image")
    {
        info.type = ItemType::IMAGE;
    } else if(tagName == "text")
    {
        info.type = ItemType::TEXT;
    } else if(tagName == "rectangle")
    {
        info.type = ItemType::SHAPE;
        info.shape.type = ShapeType::RECTANGLE;
    } else if(tagName == "circle")
    {
        info.type = ItemType::SHAPE;
        info.shape.type = ShapeType::CIRCLE;
    } else if(tagName == "ellipse")
    {
        info.type = ItemType::SHAPE;
        info.shape.type = ShapeType::ELLIPSE;
    } else if(tagName == "line")
    {
        info.type = ItemType::LINE;
    } else
    {
        return false;
    }

    return true;
}

QString getInnerText(DOMNodeList *children, size_t count)
{
    for(int i = 0; i < count; i++)
    {
        auto child = children->item(i);

        if(child->getNodeType() && (child->getNodeType() == DOMNode::TEXT_NODE))
        {
            auto text = dynamic_cast<DOMText*>(child);

            QString plain_text(XMLString::transcode(text->getWholeText()));
            plain_text = plain_text.trimmed().remove("\n", Qt::CaseInsensitive);

            if(!plain_text.isEmpty())
                return plain_text;
        }
    }

    return QString();
}

QMap<TextType, int> textTypeIndexes;

void determineTextTypeIndex(ItemTemplateInfo &info)
{
    int index = textTypeIndexes.value(info.text.type, -1) + 1;
    info.text.index = index;
    textTypeIndexes[info.text.type] = index;
}

bool handleChild(ItemTemplateInfo &info, DOMNode *child)
{
    if(!child->getNodeType())
        return false;

    if(child->getNodeType() == DOMNode::ELEMENT_NODE)
    {
        auto elem = dynamic_cast<DOMElement*>(child);

        if(!determineChildType(info, QString(XMLString::transcode(elem->getTagName()))))
            return false;

        /* if((info.type == ItemType::TEXT) && elem->hasChildNodes())
        {
            auto children = elem->getChildNodes();
            auto count = children->getLength();

            info.text.contents = getInnerText(children, count);
        } */

        if(info.type == ItemType::TEXT)
            determineTextTypeIndex(info);

        if(elem->hasAttributes())
            if(mapAttributes(info, elem))
                return true;
    }

    return false;
}

bool handleChildren(QList<ItemTemplateInfo> &infos, DOMNodeList *children, size_t count, int max_nesting = 5)
{
    if(max_nesting < 0)
        return false;

    for(int i = 0; i < count; i++)
    {
        auto child = children->item(i);
        ItemTemplateInfo info;

        if(!child->getNodeType())
            return false;

        handleChild(info, child);

        if(child->hasChildNodes())
        {
            auto elem_children = child->getChildNodes();
            auto child_count = elem_children->getLength();

            QList<ItemTemplateInfo> node_children;

            if(!handleChildren(node_children, elem_children, child_count, max_nesting - 1))
                return false;

            info.children = node_children;
        }

        if(info.isValid())
            infos.append(info);
    }

    return true;
}

bool handlePages(QList<ItemTemplateInfo> &infos, DOMNodeList *children, size_t count)
{
    for(int i = 0; i < count; i++)
    {
        auto child = children->item(i);

        if(child->getNodeType() && child->getNodeType() == DOMNode::ELEMENT_NODE)
        {
            auto elem = dynamic_cast<DOMElement*>(child);

            if(QString(XMLString::transcode(elem->getTagName())) != "page")
                return false;

            ItemTemplateInfo info;
            info.type = ItemType::PAGE;

            if(elem->hasAttributes())
                if(!mapAttributes(info, elem))
                    return false;

            if(elem->hasChildNodes())
            {
                auto sub_elem = elem->getChildNodes();
                auto sub_count = sub_elem->getLength();

                QList<ItemTemplateInfo> page_children;

                if(!handleChildren(page_children, sub_elem, sub_count))
                    return false;

                info.children = page_children;
            }

            infos.append(info);
        }
    }

    return true;
}

void TemplateParser::parseImpl()
{
    QList<ItemTemplateInfo> infos;
    bool result = false;

    try
    {
        XMLPlatformUtils::Initialize();
    }
    catch(const XMLException& toCatch)
    {
        char* msg = XMLString::transcode(toCatch.getMessage());
        emit parsingFailed(tr("Failed to initialize parser: ") + msg);
        XMLString::release(&msg);

        return;
    }

    auto parser = new XercesDOMParser();
    auto errHandler = (ErrorHandler*) new HandlerBase();

    parser->setValidationScheme(XercesDOMParser::Val_Always);
    parser->setDoNamespaces(true);
    parser->setErrorHandler(errHandler);
    parser->setExternalNoNamespaceSchemaLocation("./TemplateSchema.xsd");
    parser->setDoSchema(true);
    parser->setValidationSchemaFullChecking(true);

    DOMDocument *doc = nullptr;

    try
    {
        parser->parse(tmpl_path.toUtf8());
        parser->setIncludeIgnorableWhitespace(false);
        doc = parser->getDocument();
        doc->normalize();
    }
    catch(const XMLException& toCatch)
    {
        char* msg = XMLString::transcode(toCatch.getMessage());
        emit parsingFailed(tr("Failed to initialize parser: ") + msg);
        XMLString::release(&msg);

        return;
    }
    catch(const DOMException& toCatch)
    {
        char* msg = XMLString::transcode(toCatch.getMessage());
        emit parsingFailed(tr("Failed to parse template file: ") + msg);
        XMLString::release(&msg);

        return;
    }
    catch(...)
    {
        emit parsingFailed(tr("Unknown issue"));

        return;
    }

    if(doc)
    {
        auto root = doc->getDocumentElement();

        if(root != nullptr)
        {
            auto root_children = root->getChildNodes();
            auto count = root_children->getLength();

            result = handlePages(infos, root_children, count);
        }
    }

    if(result)
        emit parsingSucceeded(infos);

    delete parser;
    delete errHandler;

    try
    {
        XMLPlatformUtils::Terminate();
    }
    catch(const XMLException& toCatch)
    {
        char* msg = XMLString::transcode(toCatch.getMessage());
        emit parsingFailed(tr("Failed to uninitialize parser: ") + msg);
        XMLString::release(&msg);

        return;
    }
}

void TemplateParser::run()
{
    parseImpl();
}

void TemplateParser::parse(QString file_path)
{
    base_path = PathHelpers::getBasePath(file_path);
    this->tmpl_path = file_path;
    this->start();
}
