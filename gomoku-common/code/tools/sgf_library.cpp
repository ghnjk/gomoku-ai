/**
 * @File tools/sgf_library.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-05 14:38:00
 */
#include "tools/sgf_library.h"

#include <stdio.,h>

namespace tools
{

void SgfLibrary::clear()
{
    m_vecBoard.clear();
}
bool SgfLibrary::load(const string & strSgfFilePath)
{
    FILE * fpData = fopen(strSgfFilePath.c_str(), "r");
    if(fpData == NULL)
    {
        return false;
    }
    char szBuffer[1024 * 1024];
    while(fgets(szBuffer, sizeof(szBuffer), fpData))
    {
        ChessBoard board;
        if(parse(szBuffer, board))
        {
            m_vecBoard.push_back(board);
        }
    }
    if(fpData)
    {
        fclose(fpData);
        fpData = NULL;
    }
    return true;
}
bool SgfLibrary::parse(const char * szBuffer, ChessBoard & board)
{
    
}
void SgfLibrary::add(const ChessBoard & board)
{
    m_vecBoard.push_back(board);
}
const vector<ChessBoard>& SgfLibrary::getAllBoard()const
{
    return m_vecBoard;
}

}//namespace tools