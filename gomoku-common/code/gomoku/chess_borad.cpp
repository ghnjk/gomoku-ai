/**
 * @File gomoku/chess_borad.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-06 22:23:44
 */

#include "gomoku/chess_borad.h"

namespace gomoku
{




/**
 * 判断游戏是否结束
 **/
bool ChessBord::isGameOver()const
{
    return true;
}
/**
 * 根据最后一步判断棋局是否结束
 **/
bool ChessBord::isGameOver(const ChessMove & lastStep)const
{
    return true;
}
/**
 * 打印棋盘
 **/
void ChessBord::printChessBord()const
{
    return;
}


}//namespace gomoku