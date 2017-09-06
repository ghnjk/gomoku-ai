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
    ASSERT_TRUE(board.isGameOver());
}

