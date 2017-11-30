/**
 * @File gomoku/chess_db.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-06 22:23:44
 */

#include "gomoku/chess_db.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <vector>
#include "utils/stringutils.h"
using namespace utils;
using namespace std;
namespace gomoku
{


ChessDb::ChessDb(const string & strDbFilePath)
{
    m_strDbFilePath = strDbFilePath;
    m_fp = fopen(strDbFilePath.c_str(), "r");
    if(m_fp == NULL)
    {
        m_fp = fopen(m_strDbFilePath.c_str(), "w");
    }
    fclose(m_fp);
    m_fp = fopen(m_strDbFilePath.c_str(), "r+");
}
ChessDb::~ChessDb()
{
    if(m_fp)
    {
        fclose(m_fp);
        m_fp = NULL;
    }
}
bool ChessDb::clear()
{
    if(m_fp == NULL){return false;}
    fclose(m_fp);
    m_fp = fopen(m_strDbFilePath.c_str(), "w");
    fclose(m_fp);
    m_fp = fopen(m_strDbFilePath.c_str(), "r+");
    return true;
}
bool ChessDb::seekChess(int idx)
{
    if(m_fp == NULL){return false;}
    size_t pos = (size_t)idx * sizeof(ChessDbRedord);
    if(fseek(m_fp, pos, SEEK_SET) != 0)
    {
        return false;
    }
    return true;
}
bool ChessDb::append(const ChessBoard & board, const map<string, string>& mpInfo)
{
    if(m_fp == NULL){return false;}
    fseek(m_fp, 0, SEEK_END);
    size_t size = ftell(m_fp);
    if(size % sizeof(ChessDbRedord) != 0)
    {
        //printf("current size %zu\n", size);
        return false;
    }
    char szBuffer[sizeof(ChessDbRedord)];
    this->serialize((int) (size / sizeof(ChessDbRedord)), board, mpInfo, szBuffer);
    fwrite(szBuffer, sizeof(char), sizeof(szBuffer), m_fp);
    fflush(m_fp);
    return true;
}
bool ChessDb::readNext(ChessBoard & board, map<string, string> & mpInfo)
{
    if(m_fp == NULL){return false;}
    if(feof(m_fp))
    {
        return false;
    }
    size_t size = ftell(m_fp);
    if(size % sizeof(ChessDbRedord) != 0)
    {
        //printf("size invalid\n");
        return false;
    }
    char szBuffer[sizeof(ChessDbRedord)];
    fread(szBuffer, 1, sizeof(szBuffer), m_fp);
    return this->parse(szBuffer, board, mpInfo);
}
bool ChessDb::save(const ChessBoard & board, const map<string, string> & mpInfo)
{
    if(m_fp == NULL){return false;}
    map<string, string>::const_iterator it = mpInfo.find("idx");
    if(it == mpInfo.end())
    {
        return false;
    }
    int idx = StringUtils::parse<int>(it->second);
    size_t pos = idx * sizeof(ChessDbRedord);
    if(fseek(m_fp, pos, SEEK_SET) != 0)
    {
        return false;
    }
    char szBuffer[sizeof(ChessDbRedord)];
    this->serialize(idx, board, mpInfo, szBuffer);
    fwrite(szBuffer, sizeof(char), sizeof(szBuffer), m_fp);
    fflush(m_fp);
    return true;
}
bool ChessDb::parse(const char * szBuffer, ChessBoard & board, map<string, string> & mpInfo)
{
    const ChessDbRedord * ptrRecord = (ChessDbRedord*) szBuffer;
    if(! StringUtils::startsWith(ptrRecord->szHead, "chessRecord "))
    {
        //printf("invalid head.\n");
        return false;
    }
    int idx = StringUtils::parse<int>(ptrRecord->szHead + strlen("chessRecord "));
    mpInfo.clear();
    mpInfo["idx"] = StringUtils::toString(idx);
    char szTmp[CHESS_BOARD_SIZE][CHESS_BOARD_SIZE];
    for(int i = 0; i < CHESS_BOARD_SIZE; i++)
    {
        strncpy(szTmp[i], ptrRecord->szChessMap[i], CHESS_BOARD_SIZE);
    }
    board.reset(szTmp);
    string strDetail = string(ptrRecord->szInfo, sizeof(ptrRecord->szInfo));
    strDetail = StringUtils::trim(strDetail);
    //printf("szBuffer [%s]", szBuffer);
    //printf("strDetail [%s]", strDetail.c_str());
    vector<string> vec = StringUtils::split(strDetail, ' ');
    for(size_t i = 0; i < vec.size(); i++)
    {
        if(vec[i].length() == 0)
        {
            continue;
        }
        vector<string> f = StringUtils::split(vec[i], '=');
        if(f.size() == 1)
        {
            continue;
        }
        if(f.size() != 2)
        {
            //printf("invalid [%s]\n", vec[i].c_str());
            return false;
        }
        mpInfo[f[0]] = f[1];
    }
    return true;
}
bool ChessDb::serialize(int idx, const ChessBoard & board, const map<string, string> & mpInfo, char * szBuffer)
{
    ChessDbRedord * ptrRecord = (ChessDbRedord*) szBuffer;
    memset(ptrRecord, 0, sizeof(ChessDbRedord));
    snprintf(ptrRecord->szHead, sizeof(ptrRecord->szHead), "chessRecord %d", idx);
    ptrRecord->szHead[sizeof(ptrRecord->szHead) - 1] = '\n';
    for(int i = 0; i < CHESS_BOARD_SIZE; i++)
    {
        strncpy(ptrRecord->szChessMap[i], (const char *)board.m_board[i], CHESS_BOARD_SIZE);
        ptrRecord->szChessMap[i][CHESS_BOARD_SIZE] = '\n';
    }
    vector<string> vec;
    for(map<string, string>::const_iterator it = mpInfo.begin(); it != mpInfo.end(); it ++)
    {
        if(it->first == "idx")
        {
            continue;
        }
        vec.push_back(it->first + "=" + it->second);
    }
    string strInfo = StringUtils::join(vec, " ");
    strncpy(ptrRecord->szInfo, strInfo.c_str(), sizeof(ptrRecord->szInfo));
    ptrRecord->szInfo[sizeof(ptrRecord->szInfo) - 1] = '\n';
    for(size_t j = 0; j < sizeof(ChessDbRedord); j++)
    {
        if(szBuffer[j] == 0)
        {
            szBuffer[j] = ' ';
        }
    }
    return true;
}
bool ChessDb::checkAvailable()const
{
    return m_fp != NULL;
}

}//namespace gomoku