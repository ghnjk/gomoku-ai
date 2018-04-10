/**
 * @File tools/sgf_library.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-05 14:38:00
 */
#ifndef tools__SGF_LIBRARY_H__
#define tools__SGF_LIBRARY_H__

#include <string>
#include <vector>

#include "gomoku/constant.h"
#include "gomoku/chess_board.h"

using std::string;
using std::vector;
using gomoku::ChessBoard;

namespace tools
{

class SgfLibrary
{
public:
    void clear();
    bool load(const string & strSgfFilePath);
    void add(const ChessBoard & board);
    const vector<ChessBoard>& getAllBoard()const;
    bool parse(const char * szBuffer, ChessBoard & board);
protected:
    vector<ChessBoard> m_vecBoard;
};


}//namespace tools

#endif