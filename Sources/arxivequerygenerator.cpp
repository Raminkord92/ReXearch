#include "Headers/arxivequerygenerator.h"

ArxiveQueryGenerator::ArxiveQueryGenerator(QString searchText, SearchFields field, SearchType searchType):
    m_searchText{searchText}, m_Field{field}, m_searchType{searchType}
{
    m_searchText = m_searchText.trimmed();
}

QString ArxiveQueryGenerator::getGeneratedQuery() const
{
    return m_generatedQuery;
}

void ArxiveQueryGenerator::ObtainQuery()
{
    if (m_searchType == SearchType::Simple)
    {
        QString queryField;
        switch (m_Field)
        {
        case SearchFields::Authors:
            queryField = "au:";
            m_searchText = "%22" + m_searchText + "%22";
            break;
        case SearchFields::Title:
            queryField = "ti:";
            m_searchText = "%22" + m_searchText + "%22";
            break;
        case SearchFields::All:
        default:
            queryField = "all:";
            m_searchText = "%22" + m_searchText + "%22";
        }
        m_generatedQuery += queryField + m_searchText + AND() + MaxResults(m_numOutput);

    }
    else
        m_generatedQuery = "";
}

QString ArxiveQueryGenerator::MaxResults(int num)
{
    return QString("max_results=") + QString::number(num);
}
