/**
 * @File gomoku/chess_board.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-06 22:23:44
 */

#include "gomoku/chess_board.h"

namespace gomoku
{


ChessBoard::ChessBoard()
{
    for(int i = 0; i < CHESS_BORD_SIZE; i++)
    {
        for(int j = 0; j < CHESS_BORD_SIZE; j++)
        {
            m_board[i][j] = COLOR_BLANK;
        }
    }
    m_nextPlayerColor = COLOR_BLACK;
}

/**
 * 判断游戏是否结束
 **/
bool ChessBoard::isGameOver()const
{
    return false;
}
/**
 * 根据最后一步判断棋局是否结束
 **/
bool ChessBoard::isGameOver(const ChessMove & lastStep)const
{
    return false;
}
/**
 * 打印棋盘
 **/
void ChessBoard::printChessBord()const
{
   for(int i = 0; i < CHESS_BORD_SIZE; i++)
    {
        for(int j = 0; j < CHESS_BORD_SIZE; j++)
        {
            putchar(m_board[i][j]);
        }
        putchar('\n');
    }
}


}//namespace gomoku