
#include "textnode.hpp"

#include <sstream>
#include <stdexcept>

#include <QRegExp>

#include "../world/columns.hpp"
#include "../world/idtable.hpp"

CSMFilter::TextNode::TextNode (int columnId, const std::string& text)
: mColumnId (columnId), mText (text)
{}

bool CSMFilter::TextNode::test (const CSMWorld::IdTable& table, int row,
    const std::map<int, int>& columns) const
{
    const std::map<int, int>::const_iterator iter = columns.find (mColumnId);

    if (iter==columns.end())
        throw std::logic_error ("invalid column in test node test");

    if (iter->second==-1)
        return true;

    QModelIndex index = table.index (row, iter->second);

    QVariant data = table.data (index);

    if (data.type()!=QVariant::String)
        return false;

    QRegExp regExp(QString::fromUtf8 (mText.c_str())); /// \todo make pattern syntax configurable

    return regExp.exactMatch (data.toString());
}

std::vector<int> CSMFilter::TextNode::getReferencedColumns() const
{
    return std::vector<int> (1, mColumnId);
}

std::string CSMFilter::TextNode::toString (bool numericColumns) const
{
    std::ostringstream stream;

    stream << "text (";

    if (numericColumns)
        stream << mColumnId;
    else
        stream
            << "\""
            << CSMWorld::Columns::getName (static_cast<CSMWorld::Columns::ColumnId> (mColumnId))
            << "\"";

    stream << ", \"" << mText << "\")";

    return stream.str();
}