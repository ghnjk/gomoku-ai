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
            return gomoku::COLOR_BLANK;
        }
        int sum = 0;
        for(size_t i = 0; i < iRes; i++)
        {
            sum += (int) arrScore[i];// * arrScore[i];
        }
        int rndSun = rand() % sum;
        size_t iSelect = 0;
        for(size_t i = 0; i < iRes; i++)
        {
            rndSun -= (int)arrScore[i] ;//* arrScore[i];
            if(rndSun <= 0)
            {
                iSelect = i;
                break;
            }
        }
        //iSelect = rand() % iRes;
        testBoard.playChess(arrMove[iSelect]);
        if(testBoard.isGameOver(arrMove[iSelect]))
        {
            //testBoard.printChessBord(arrMove[iSelect]);
            return arrMove[iSelect].color;
        }
    }
}

}//namespace mcts_alg