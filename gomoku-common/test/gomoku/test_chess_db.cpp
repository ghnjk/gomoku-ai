/**
 * @File test_chess_db.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-06 23:33:16
 */
#include "gtest/gtest.h"

#define UNIT_TEST true
#include "gomoku/chess_db.h"

using namespace gomoku;


TEST(ChessDb, clear)
{
    ChessDb db("data/chess.db");
    ASSERT_TRUE(db.clear());
}

TEST(ChessDb, append)
{
    ChessDb db("data/chess.db");
    ASSERT_TRUE(db.clear());
    ChessBoard board;
    map<string, string> mpInfo;
    board.playChess(ChessMove(COLOR_BLACK, 7, 7));
    board.playChess(ChessMove(COLOR_WHITE, 6, 7));
    board.playChess(ChessMove(COLOR_BLACK, 7, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 5));
    board.playChess(ChessMove(COLOR_BLACK, 6, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 8));
    mpInfo["winRate"] = "0.99";
    mpInfo["nextPlayer"] = "B";
    ASSERT_TRUE(db.append(board, mpInfo));
    board.playChess(ChessMove(COLOR_BLACK, 8, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 9));
    board.playChess(ChessMove(COLOR_BLACK, 9, 6));  
    board.playChess(ChessMove(COLOR_WHITE, 5, 6));  
    board.playChess(ChessMove(COLOR_WHITE, 4, 7));  
    board.playChess(ChessMove(COLOR_WHITE, 3, 8));  
    board.playChess(ChessMove(COLOR_WHITE, 2, 9));  
    mpInfo["winRate"] = "0.19";
    mpInfo["nextPlayer"] = "W";
    ASSERT_TRUE(db.append(board, mpInfo));
}
TEST(ChessDb, readNext)
{
    ChessDb db("data/chess.db");
    ChessBoard board;
    map<string, string> mpInfo;
    ASSERT_TRUE(db.readNext(board, mpInfo));
    board.printChessBord();
    ASSERT_TRUE(db.readNext(board, mpInfo));
    board.printChessBord();
}
TEST(ChessDb, save)
{
    ChessDb db("data/chess.db");
    ChessBoard board;
    map<string, string> mpInfo;
    ASSERT_TRUE(db.readNext(board, mpInfo));
    board.printChessBord();
    board.playChess(ChessMove(COLOR_WHITE, 14, 8));  
    mpInfo["winRate"] = "1.00";
    mpInfo["nextPlayer"] = "W";
    ASSERT_TRUE(db.save(board, mpInfo));
}
