/**
 * @File gomoku/chess_db.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-05 14:38:00
 */
#ifndef gomoku__CHESS_DB_H__
#define gomoku__CHESS_DB_H__

#include <stdio.h>
#include <string>
#include <map>
#include "gomoku/chess_board.h"

using std::string;
using std::map;

namespace gomoku
{

struct ChessDbRedord
{
    char szHead[64];
    char szChessMap[CHESS_BOARD_SIZE][CHESS_BOARD_SIZE + 1];
    char szInfo[512];
};

class ChessDb
{
public:
    ChessDb(const string & strDbFilePath);
    virtual ~ChessDb();
    bool append(const ChessBoard & board, const map<string, string>& mpInfo);
    bool readNext(ChessBoard & board, map<string, string> & mpInfo);
    bool save(const ChessBoard & board, const map<string, string> & mpInfo);
    bool clear();
protected:
    bool parse(const char * szBuffer, ChessBoard & board, map<string, string> & mpInfo);
    bool serialize(int idx, const ChessBoard & board, const map<string, string> & mpInfo, char * szBuffer);
    FILE * m_fp;
    string m_strDbFilePath;
};

}//namespace gomoku

#endif