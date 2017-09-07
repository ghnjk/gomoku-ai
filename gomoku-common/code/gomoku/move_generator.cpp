/**
 * @File gomoku/move_generator.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-07 23:39:39
 */

#include "gomoku/move_generator.h"


namespace gomoku
{

/**
 * 给定一个棋局， 产生所有下法
 * 输入：
 * player 下期颜色
 * chessBoard 棋局
 * arrMoves 输出的走法数组
 * arrScore 输出的走法评分的数组， 如果为NULL， 不设置
 * iMaxMoves 最大产生的走法数
 * 输出：
 * 产生的走法个数
 **/
size_t AllMoveNoScoreGenerator::generateAllMoves(TChessColor player, const ChessBoard & chessBoard
    , ChessMove * arrMoves
    , TScore * arrScore
    , size_t iMaxMoves)
{
    size_t iRes = 0;
    for(TChessPos r = 0; r < CHESS_BORD_SIZE; r++)
    {
        for(TChessPos c = 0; c < CHESS_BORD_SIZE; c++)
        {
            if(chessBoard.m_board[r][c] != COLOR_BLANK)
            {
                continue;
            }
            if(arrMoves != NULL)
            {
                arrMoves[iRes].row = r;
                arrMoves[iRes].col = c;
                arrMoves[iRes].color = player;
            }
            if(arrScore != NULL)
            {
                arrScore[iRes] = 0;
            }
            iRes ++;
            if(iRes >= iMaxMoves)
            {
                return iRes;
            }
        }
    }
    return iRes;
}



}//namespace gomoku