/**
 * @File test_chess_board.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-06 23:33:16
 */
#include "gtest/gtest.h"

#define UNIT_TEST true
#include "gomoku/chess_board.h"

using namespace gomoku;


TEST(ChessBoard, isGameOver)
{
    ChessBoard board;
    board.playChess(ChessMove(COLOR_BLACK, 7, 7));
    board.playChess(ChessMove(COLOR_WHITE, 6, 7));
    board.playChess(ChessMove(COLOR_BLACK, 7, 6));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_WHITE, 6, 5));
    board.playChess(ChessMove(COLOR_BLACK, 6, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 8));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_BLACK, 8, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 9));
    board.playChess(ChessMove(COLOR_BLACK, 9, 6));  
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_BLACK, 5, 6));  
    ASSERT_TRUE(board.isGameOver());
}
TEST(ChessBoard, isGameOver_2)
{
    ChessBoard board;
    board.playChess(ChessMove(COLOR_BLACK, 7, 7));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_WHITE, 6, 7));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_BLACK, 7, 6));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_WHITE, 6, 5));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_BLACK, 6, 6));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_WHITE, 6, 8));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_BLACK, 8, 6));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_WHITE, 6, 9));
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_BLACK, 9, 6));  
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_WHITE, 5, 6));  
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_WHITE, 4, 7));  
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_WHITE, 3, 8));  
    ASSERT_FALSE(board.isGameOver());
    board.playChess(ChessMove(COLOR_WHITE, 2, 9));  
    ASSERT_TRUE(board.isGameOver());
}
TEST(ChessBoard, printChessBord)
{
    ChessBoard board;
    board.playChess(ChessMove(COLOR_BLACK, 7, 7));
    board.playChess(ChessMove(COLOR_WHITE, 6, 7));
    board.playChess(ChessMove(COLOR_BLACK, 7, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 5));
    board.playChess(ChessMove(COLOR_BLACK, 6, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 8));
    board.playChess(ChessMove(COLOR_BLACK, 8, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 9));
    board.playChess(ChessMove(COLOR_BLACK, 9, 6));  
    board.playChess(ChessMove(COLOR_WHITE, 5, 6));  
    board.playChess(ChessMove(COLOR_WHITE, 4, 7));  
    board.playChess(ChessMove(COLOR_WHITE, 3, 8));  
    board.playChess(ChessMove(COLOR_WHITE, 2, 9));  
    board.printChessBord();
}

