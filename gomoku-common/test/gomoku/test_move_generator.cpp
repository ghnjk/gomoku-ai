/**
 * @File test_move_generator.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-07 23:51:05
 */
#include <stdlib.h>
#include <time.h>

#include "gtest/gtest.h"

#include "gomoku/move_generator.h"

using namespace gomoku;

TEST(AllMoveNoScoreGenerator, generate)
{
    srand(time(NULL));
    ChessBoard board;
    ChessMove arrMove[300];
    size_t iRes;
    AllMoveNoScoreGenerator moveGenerator;
    while(! board.isGameOver())
    {
        iRes = moveGenerator.generateAllMoves(board.m_nextPlayerColor, board, arrMove, NULL, 300);
        iRes = rand() % iRes;
        board.playChess(arrMove[iRes]);
    }
    board.printChessBord();
    printf("last move %c %d %d\n", arrMove[iRes].color, arrMove[iRes].row, arrMove[iRes].col);
}
TEST(SortedMoveGenerator, generate)
{
    ChessBoard board;
    ChessMove arrMove[300];
    TScore arrScores[300];
    size_t iRes;
    SortedMoveGenerator moveGenerator;
    while(! board.isGameOver())
    {
        iRes = moveGenerator.generateAllMoves(board.m_nextPlayerColor, board, arrMove, arrScores, 300);
        if(iRes <= 0)
        {
            printf("############\n");
            board.printChessBord();
            printf("------\n");
        }
        ASSERT_TRUE(iRes > 0);
        board.printChessBord(arrMove[0]);
        printf("last move %c %d %d score [%0.2lf]\n"
            , arrMove[0].color
            , arrMove[0].row
            , arrMove[0].col
            , arrScores[0]);
        board.playChess(arrMove[0]);
    }
}