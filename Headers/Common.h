#ifndef COMMON_H
#define COMMON_H
#include "qcolor.h"
#include <QString>
#include <unordered_map>
#include <vector>
#include <QStringList>
#include <QMap>



struct Author
{
    QString name;
};


struct Paper
{
   QVector<Author> authors;
   QString summary;
   QString published;
   QString title;
   QString address;
};

enum class SearchTblColEnum
{
    Authors,
    Title,
    Summary,
    Published,
    Address,
    Max
};

static const std::unordered_map<SearchTblColEnum, QString> SearchTblHeaders = {{SearchTblColEnum::Authors, "Authors"},
 {SearchTblColEnum::Title, "Title"},
 {SearchTblColEnum::Summary, "Summary"},
 {SearchTblColEnum::Published, "Published"},
 {SearchTblColEnum::Address, "Address"}};

inline const QStringList SearchFieldsText = {"All", "Author(s)", "Title"};

enum class SearchFields
{
    All,
    Authors,
    Title
};

inline const std::unordered_map< QString, SearchFields> SearchFieldsMap  = \
{
 {SearchFieldsText[0], SearchFields::All},
 {SearchFieldsText[1], SearchFields::Authors},
 {SearchFieldsText[2], SearchFields::Title}
};

//using TableData = std::vector<std::unordered_map<QString, QString>>;

//namespace AXTables
//{
//    namespace SearchResultTbl
//    {
//        TableData data = {{"Authors", ""}, {"Title", ""}, {"Summary", ""}, {"Published Date", ""},
//                          {"Address", ""}};
//        const int numCols = data.size();
//    }
//}

#endif // COMMON_H
