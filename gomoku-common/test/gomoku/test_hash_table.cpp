/**
 * @File test_hash_table.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-10 10:15:39
 */


#include "gtest/gtest.h"
#define UNIT_TEST true
#include "gomoku/hash_table.h"

using namespace gomoku;
TEST(HashTable, test)
{
    HashTable<int> hashTable(97);
    ChessBoard board;
    ChessBoard b0 = board;
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 0);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_BLACK, 7, 7));
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 1);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_WHITE, 6, 7));
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 2);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_BLACK, 7, 6));
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 3);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_WHITE, 6, 5));
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 4);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_BLACK, 6, 6));
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 5);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_WHITE, 6, 8));
        ChessBoard b6 = board;
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 6);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_BLACK, 8, 6));
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 7);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_WHITE, 6, 9));
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 8);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_BLACK, 9, 6));  
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 9);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_WHITE, 5, 6));  
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 10);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_WHITE, 4, 7));  
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 11);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_WHITE, 3, 8));  
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 12);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_WHITE, 2, 9));  
    ChessBoard b13 = board;
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 13);
    ASSERT_TRUE(hashTable.contains(board));
    board.playChess(ChessMove(COLOR_WHITE, 14, 8));
    ASSERT_FALSE(hashTable.contains(board));
    EXPECT_EQ(NULL, hashTable.get(board));
    hashTable.put(board, 14);
    ASSERT_TRUE(hashTable.contains(board));
    EXPECT_EQ(0, * (hashTable.get(b0) ) );
    EXPECT_EQ(6, * (hashTable.get(b6) ) );
    EXPECT_EQ(13, * (hashTable.get(b13) ) );
}