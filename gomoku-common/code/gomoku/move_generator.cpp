/**
 * @File gomoku/move_generator.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-07 23:39:39
 */

#include "gomoku/move_generator.h"

#include <algorithm>

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



/**
 * 给定一个棋局， 产生所有下法
 * 输入：
 * player 下棋颜色
 * chessBoard 棋局
 * arrMoves 输出的走法数组
 * arrScore 输出的走法评分的数组， 如果为NULL， 不设置
 * iMaxMoves 最大产生的走法数
 * 输出：
 * 产生的走法个数
 **/
size_t SortedMoveGenerator::generateAllMoves(TChessColor player, const ChessBoard & chessBoard
    , ChessMove * arrMoves
    , TScore * arrScore
    , size_t iMaxMoves)
{
    if(iMaxMoves <= 0)
    {
        return 0;
    }
    ScoredMove arrScoreMoves[MAX_MOVE_COUNT];
    size_t iRes = 0;
    for(TChessPos r = 0; r < CHESS_BORD_SIZE; r++)
    {
        for(TChessPos c = 0; c < CHESS_BORD_SIZE; c++)
        {
            if(chessBoard.m_board[r][c] != COLOR_BLANK)
            {
                continue;
            }
            arrScoreMoves[iRes].move.row = r;
            arrScoreMoves[iRes].move.col = c;
            arrScoreMoves[iRes].move.color = player;
            arrScoreMoves[iRes].score = m_moveEvaluator.evaluateMove(chessBoard, arrScoreMoves[iRes].move);
            // printf("move %d %d score %0.0lf\n"
            //     , arrScoreMoves[iRes].move.row
            //     , arrScoreMoves[iRes].move.col
            //     , arrScoreMoves[iRes].score);
            iRes ++;
        }
    }
    if(iRes == 0)
    {
        return 0;
    }
   // printf("** iRes %zu\n", iRes);
    std::sort(arrScoreMoves, arrScoreMoves + iRes);
    size_t i;
    for(i = 0; i < iRes; i++)
    {
        const ScoredMove & mv = arrScoreMoves[iRes - i - 1];
        if(mv.score == 0)
        {
            break;
        }
        if(arrMoves != NULL)
        {
            arrMoves[i] = mv.move;
        }
        if(arrScore != NULL)
        {
            arrScore[i] = mv.score;
        }
        if(i >= iMaxMoves)
        {
            return i;
        }
    }
    if(i == 0 && chessBoard.m_board[7][7] == COLOR_BLANK)
    {
        if(arrMoves != NULL)
        {
            arrMoves[i] = ChessMove(player, 7, 7);
        }
        if(arrScore != NULL)
        {
            arrScore[i] = 1;
        }
        i ++;
    }
    iRes = i;
    return iRes;
}



}//namespace gomoku