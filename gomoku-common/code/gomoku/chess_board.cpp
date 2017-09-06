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




/**
 * 判断游戏是否结束
 **/
bool ChessBoard::isGameOver()const
{
    return true;
}
/**
 * 根据最后一步判断棋局是否结束
 **/
bool ChessBoard::isGameOver(const ChessMove & lastStep)const
{
    return true;
}
/**
 * 打印棋盘
 **/
void ChessBoard::printChessBord()const
{
    return;
}


}//namespace gomoku