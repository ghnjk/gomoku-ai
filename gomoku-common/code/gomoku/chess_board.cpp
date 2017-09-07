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
    for(TChessPos i = 0; i < CHESS_BORD_SIZE; i++)
    {
        for(TChessPos j = 0; j < CHESS_BORD_SIZE; j++)
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
    for(TChessPos r = 0; r < CHESS_BORD_SIZE; r++)
    {
        for(TChessPos c = 0; c < CHESS_BORD_SIZE; c++)
        {
            if(m_board[r][c] == COLOR_BLANK)
            {
                continue;
            }
            if(isGameOver(ChessMove(m_board[r][c], r, c)))
            {
                return true;
            }
        }
    }
    return false;
}
/**
 * 根据最后一步判断棋局是否结束
 **/
bool ChessBoard::isGameOver(const ChessMove & lastStep)const
{
    static TChessPos dr[] = {0, -1, -1, -1};
    static TChessPos dc[] = {1, 1, 0, -1};
    for(size_t i = 0; i < sizeof(dr); i++)
    {
        int cnt = 1;
        TChessPos nr = lastStep.row;
        TChessPos nc = lastStep.col;
        for(TChessPos j = 0; j < 4; j ++)
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
        for(TChessPos j = 0; j < 4; j ++)
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