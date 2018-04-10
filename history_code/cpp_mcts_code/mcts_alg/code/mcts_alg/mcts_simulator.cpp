/**
 * @File mcts_alg/mcts_simulator.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-10 15:46:11
 */


#include "mcts_alg/mcts_simulator.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

using namespace gomoku;

namespace mcts_alg
{

/**
 * 判断最后谁赢了局面
 * 返回胜利的颜色
 * 如果是平局， 则返回COLOR_BLANK
 **/
TChessColor SimpleGomokuSimulator::simulateWinner(const ChessBoard & chessBoard)
{
    ChessBoard testBoard(chessBoard);
    ChessMove arrMove[MAX_MOVE_COUNT];
    TScore arrScore[MAX_MOVE_COUNT];
    size_t iRes;
    while(true)
    {
        iRes = m_moveGenerator.generateAllMoves(testBoard.m_nextPlayerColor, testBoard, arrMove, arrScore, MAX_MOVE_COUNT);
        if(iRes == 0)
        {
            //testBoard.printChessBord();
            return gomoku::COLOR_TIDE;
        }
        // int sum = 0;
        // for(size_t i = 0; i < iRes; i++)
        // {
        //     sum += (int) arrScore[i] * arrScore[i];
        // }
        // sum = sum / 2 + 1;
        // int rndSun = rand() % sum;
        size_t iSelect = 0;
        // for(size_t i = 0; i < iRes; i++)
        // {
        //     rndSun -= (int)arrScore[i] ;//* arrScore[i];
        //     if(rndSun <= 0)
        //     {
        //         iSelect = i;
        //         break;
        //     }
        // }
        //iSelect = rand() % iRes;
        testBoard.playChess(arrMove[iSelect]);
        if(testBoard.isGameOver(arrMove[iSelect]))
        {
            //testBoard.printChessBord(arrMove[iSelect]);
            return arrMove[iSelect].color;
        }
    }
}


/**
 * 判断最后谁赢了局面
 * 返回胜利的颜色
 * 如果是平局， 则返回COLOR_BLANK
 **/
TChessColor RandomGomokuSimulator::simulateWinner(const ChessBoard & chessBoard)
{
    ChessBoard testBoard(chessBoard);
    m_moveBitmap = testBoard;
    this->genAllMoves(testBoard);
    while(true)
    {
        if(m_iRes == 0)
        {
            return gomoku::COLOR_TIDE;
        }
        ChessMove move = this->play(testBoard);
        //printf("%d %d %c\n", move.row, move.col, move.color);
        //testBoard.printChessBord();
        if(testBoard.isGameOver(move))
        {
            //exit(-1);
            return move.color;
        }
        else
        {
            this->addPosibleMove(move);
        }
    }
}

ChessMove RandomGomokuSimulator::play(ChessBoard & board)
{
    size_t iSelect = rand() % m_iRes;
    ChessMove move = m_arrMove[iSelect];
    move.color = board.m_nextPlayerColor;
    board.playChess(move);
    if(iSelect == m_iRes -1)
    {
        m_iRes --;
    }
    else
    {
        m_arrMove[iSelect] = m_arrMove[m_iRes - 1];
        m_iRes --;
    }
    return move;
}
void RandomGomokuSimulator::addPosibleMove(const ChessMove & move)
{
    for(int i = -1; i <= 1; i++)
    {
        int nr = (int)move.row + i;
        if(nr < 0 || nr >= CHESS_BOARD_SIZE)
        {
            continue;
        }
        for(int j = -1; j <= 1; j++)
        {
            int nc = (int)move.col + j;
            if(nc < 0 || nc >= CHESS_BOARD_SIZE)
            {
                continue;
            }
            if(m_moveBitmap.m_board[nr][nc] != COLOR_BLANK)
            {
                continue;
            }
            m_arrMove[m_iRes].row = nr;
            m_arrMove[m_iRes].col = nc;
            m_moveBitmap.m_board[nr][nc] = 'X';
            m_iRes ++;
        }
    }
}
void RandomGomokuSimulator::genAllMoves(const ChessBoard & board)
{
    m_iRes = 0;
    for(TChessPos r = 0; r < CHESS_BOARD_SIZE; r++)
    {
        for(TChessPos c = 0; c < CHESS_BOARD_SIZE; c++)
        {
            if(board.m_board[r][c] != COLOR_BLANK)
            {
                continue;
            }
            bool hasChess = false;
            for(int i = -1; i <= 1; i++)
            {
                int nr = (int)r + i;
                if(nr < 0 || nr >= CHESS_BOARD_SIZE)
                {
                    continue;
                }
                for(int j = -1; j <= 1; j++)
                {
                    int nc = (int)c + j;
                    if(nc < 0 || nc >= CHESS_BOARD_SIZE)
                    {
                        continue;
                    }
                    if(board.m_board[nr][nc] != COLOR_BLANK)
                    {
                        hasChess = true;
                        break;
                    }
                }
                if(hasChess){
                    break;
                }
            }
            if(!hasChess && !(r == 7 && c == 7) ){
                continue;
            }
            m_arrMove[m_iRes].row = r;
            m_arrMove[m_iRes].col = c;
            m_moveBitmap.m_board[r][c] = 'X';
            m_iRes ++;
        }
    }
}

}//namespace mcts_alg
