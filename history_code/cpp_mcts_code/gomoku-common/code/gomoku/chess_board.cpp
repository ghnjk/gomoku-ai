/**
 * @File gomoku/chess_board.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-06 22:23:44
 */

#include "gomoku/chess_board.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

namespace gomoku
{


ChessBoard::ChessBoard()
{
    for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
    {
        for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
        {
            m_board[i][j] = COLOR_BLANK;
        }
    }
    m_nextPlayerColor = COLOR_BLACK;
    m_iMoveCnt = 0;
}

/**
 判断两个棋局是否完全相等
 **/
bool ChessBoard::operator == (const ChessBoard & other) const
{
    if(m_iMoveCnt != other.m_iMoveCnt)
    {
        //printf("move cnt diff\n");
        return false;
    }
    if(m_nextPlayerColor != other.m_nextPlayerColor)
    {
       // printf("color diff\n");
        return false;
    }
    for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
    {
        for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
        {
            if(m_board[i][j] != other.m_board[i][j])
            {
                printf("%d %d diff\n", i, j);
                return false;
            }
        }
    }
    return true;
}
/**
 * 判断游戏是否结束
 **/
bool ChessBoard::isGameOver()const
{
    return getWinColor() != COLOR_BLANK;
}
TChessColor ChessBoard::getWinColor()const
{
    if(m_iMoveCnt == 0)
    {
        return COLOR_BLANK;
    }
    if(isGameOver(m_arrMoves[m_iMoveCnt - 1]))
    {
        return m_arrMoves[m_iMoveCnt - 1].color;
    }
    if(m_iMoveCnt >= CHESS_BOARD_SIZE * CHESS_BOARD_SIZE)
    {
        return COLOR_TIDE;
    }
    else
    {
        return COLOR_BLANK;
    }
}
/**
 * 根据最后一步判断棋局是否结束
 **/
bool ChessBoard::isGameOver(const ChessMove & lastStep)const
{
    static const TChessPos dr[] = {0, -1, -1, -1};
    static const TChessPos dc[] = {1, 1, 0, -1};
    for(int i = 0; i < 4; i++)
    {
        int cnt = 1;
        TChessPos nr = lastStep.row;
        TChessPos nc = lastStep.col;
        for(int j = 0; j < 4; j ++)
        {
            nr -= dr[i];
            nc -= dc[i];
            if(!IsValidPos(nr, nc))
            {
                break;
            }
            if(m_board[nr][nc] == lastStep.color)
            {
                cnt ++;
            }
            else
            {
                break;
            }
        }
        nr = lastStep.row;
        nc = lastStep.col;
        for(int j = 0; j < 4; j ++)
        {
            nr += dr[i];
            nc += dc[i];
            if(!IsValidPos(nr, nc))
            {
                break;
            }
            if(m_board[nr][nc] == lastStep.color)
            {
                cnt ++;
            }
            else
            {
                break;
            }
        }
        if(cnt >= 5)
        {
            // printf("game over last [%d %d] i %d\n"
            //     , lastStep.row
            //     , lastStep.col
            //     , i);
            return true;
        }
    }
    return false;
}

/**
 * 打印棋盘
 **/
void ChessBoard::printChessBord()const
{
   for(int i = 0; i < CHESS_BOARD_SIZE; i++)
    {
        for(int j = 0; j < CHESS_BOARD_SIZE; j++)
        {
            putchar(m_board[i][j]);
        }
        putchar('\n');
    }
}
void ChessBoard::printChessBord(const ChessMove & move)const
{
   for(int i = 0; i < CHESS_BOARD_SIZE; i++)
    {
        for(int j = 0; j < CHESS_BOARD_SIZE; j++)
        {
            if(i == move.row && j == move.col)
            {
                putchar(tolower(move.color));
            }
            else
            {
                putchar(m_board[i][j]);
            }
        }
        putchar('\n');
    }  
}
/**
 重新设置棋局
 **/
void ChessBoard::reset(char  szBoard[][CHESS_BOARD_SIZE])
{
    int whiteCnt = 0;
    int blackCnt = 0;
    m_iMoveCnt = 0;
    m_nextPlayerColor = COLOR_BLACK;

    for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
    {
        for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
        {
            m_board[i][j] = COLOR_BLANK;
        }
    }
    
    for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
    {
        for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
        {
            if(szBoard[i][j] == COLOR_BLACK)
            {
                blackCnt ++;
                this->playChess(ChessMove(szBoard[i][j], i, j));
            }
            else if(szBoard[i][j] == COLOR_WHITE)
            {
                whiteCnt ++;
                this->playChess(ChessMove(szBoard[i][j], i, j));
            }
        }
    }
    if(whiteCnt < blackCnt)
    {
        m_nextPlayerColor = COLOR_WHITE;
    }
    else
    {
        m_nextPlayerColor = COLOR_BLACK;
    }
}
}//namespace gomoku
