/**
 * @File test_chess_board.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-06 23:33:16
 */
#include "gtest/gtest.h"

#include "gomoku/chess_board.h"

using namespace gomoku;


TEST(ChessBoard, isGameOver)
{
    ChessBoard board;
    ASSERT_FALSE(board.isGameOver());
}

TEST(ChessBoard, printChessBord)
{
    ChessBoard board;
    board.playChess(ChessMove(COLOR_BLACK, 7, 7));
    board.playChess(ChessMove(COLOR_WHITE, 6, 7));
    board.playChess(ChessMove(COLOR_BLACK, 7, 6));
    board.printChessBord();
}