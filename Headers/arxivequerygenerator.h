#ifndef ARXIVEQUERYGENERATOR_H
#define ARXIVEQUERYGENERATOR_H

#include "qobjectdefs.h"
#include <Headers/Common.h>
#include <QString>

class ArxiveQueryGenerator
{
public:
    enum class SearchType
    {
        Simple,
        Advanced
    };
    ArxiveQueryGenerator(QString searchText, SearchFields field, SearchType searchType);
    QString getGeneratedQuery() const;
    void ObtainQuery();
private:
    QString AND()
    {
        return "&";
    }
    QString MaxResults(int num);
    QString m_generatedQuery = "http://export.arxiv.org/api/query?search_query=";
    QString m_searchText;
    SearchFields m_Field;
    int m_numOutput = 100;
    SearchType m_searchType = SearchType::Simple;
};

#endif // ARXIVEQUERYGENERATOR_H
