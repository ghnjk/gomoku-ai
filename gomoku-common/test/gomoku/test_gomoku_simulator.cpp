/**
 * @File test_gomoku_simulator.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-10 10:15:39
 */


#include "gtest/gtest.h"
#define UNIT_TEST true
#include "gomoku/gomoku_simulator.h"

using namespace gomoku;
TEST(MoveEvaluator, init)
{
    MoveEvaluator tmp;
    printf("%d\n", tmp.m_mpLineType[364]);
}
TEST(MoveEvaluator, getLineFromBoard)
{
    MoveEvaluator evaluator;
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
    board.playChess(ChessMove(COLOR_WHITE, 14, 8));  

    int iPos;
    char szLine[9];
    ChessMove testMoves[] = {
        ChessMove(COLOR_BLACK, 4, 4),
        ChessMove(COLOR_WHITE, 4, 4),
        ChessMove(COLOR_BLACK, 0, 0),
        ChessMove(COLOR_BLACK, 7, 8),
        ChessMove(COLOR_BLACK, 0, 0),
        ChessMove(COLOR_BLACK, 10, 8),
        ChessMove(COLOR_WHITE, 10, 8),
        ChessMove(COLOR_WHITE, 6, 10)
    };
    TChessPos testDirect[][2] = {
        {1, 1},
        {1, 1},
        {1, 1},
        {-1, 1},
        {-1, 1},
        {1, 0},
        {1, 0},
        {0, -1}
    };
    int expPos[] = {
        4,
        4,
        0,
        4,
        0,
        3,
        4,
        4
    };
    char expLine[][9] = {
        {0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 0, 0, 0, 0, 0, 2, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 1, 0},
        {0, 0, 1, 0, 0, 2, 0, 0, 0},
        {0, 2, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 2, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 0, 1, 1, 1, 2}
    };
    for(size_t i = 0; i < sizeof(expPos) / sizeof(expPos[0]); i++)
    {
        evaluator.getLineFromBoard(board, testMoves[i], testDirect[i][0], testDirect[i][1]
            , iPos, szLine); 
        EXPECT_EQ(expPos[i], iPos);
        ASSERT_EQ(0, memcmp(szLine, expLine[i], sizeof(szLine)));
        printf("-----\n");
    }

    ChessMove move(COLOR_WHITE, 6, 10);
    board.printChessBord(move);
    evaluator.getLineFromBoard(board, move, 0, 1
        , iPos, szLine); 
    printf("iPos %d\n line type: ", iPos);
    for(int i = 0; i < 9; i++)
    {
        printf(", %d", szLine[i]);
    }
    printf("\n");
}

TEST(MoveEvaluator, isMatchLine)
{
    MoveEvaluator evaluator;
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
    board.playChess(ChessMove(COLOR_WHITE, 14, 8));  
    int iPos;
    char szLine[9];
    ChessMove move(COLOR_WHITE, 7, 8);
    board.printChessBord(move);
    evaluator.getLineFromBoard(board, move, 1, 1
        , iPos, szLine); 
    printf("iPos %d\n line type: ", iPos);
    for(int i = 0; i < 9; i++)
    {
        printf(", %d", szLine[i]);
    }
    printf("\n");
    ASSERT_TRUE(evaluator.isMatchLine(iPos, szLine, "_AAA_"));
    ASSERT_TRUE(evaluator.isMatchLine(iPos, szLine, "__AAA"));
    ASSERT_FALSE(evaluator.isMatchLine(iPos, szLine, "_A_AA"));
}

TEST(MoveEvaluator, getLineType)
{
    MoveEvaluator evaluator;
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
    board.playChess(ChessMove(COLOR_WHITE, 14, 8));  
    int iPos;
    char szLine[9];
    ChessMove move(COLOR_WHITE, 7, 8);
    board.printChessBord(move);
    evaluator.getLineFromBoard(board, move, 1, 1
        , iPos, szLine); 
    printf("iPos %d\n line type: ", iPos);
    for(int i = 0; i < 9; i++)
    {
        printf(", %d", szLine[i]);
    }
    printf("\n");
    EXPECT_EQ(2, evaluator.getLineType(iPos, szLine));

    move = ChessMove(COLOR_WHITE, 0, 11);
    board.printChessBord(move);
    evaluator.getLineFromBoard(board, move, 1, -1
        , iPos, szLine); 
    printf("iPos %d\n line type: ", iPos);
    for(int i = 0; i < 9; i++)
    {
        printf(", %d", szLine[i]);
    }
    printf("\n");
    EXPECT_EQ(4, evaluator.getLineType(iPos, szLine));

    board.playChess(ChessMove(COLOR_BLACK, 4, 4));  

    move = ChessMove(COLOR_BLACK, 5, 5);
    board.printChessBord(move);
    evaluator.getLineFromBoard(board, move, 1, 1
        , iPos, szLine); 
    printf("iPos %d\n line type: ", iPos);
    for(int i = 0; i < 9; i++)
    {
        printf(", %d", szLine[i]);
    }
    printf("\n");
    EXPECT_EQ(1, evaluator.getLineType(iPos, szLine));
}


TEST(MoveEvaluator, evaluateMove)
{
    MoveEvaluator evaluator;
    ChessBoard board;
    board.printChessBord(ChessMove(COLOR_BLACK, 7, 7));
    printf("%0.0lf\n", evaluator.evaluateMove(board, ChessMove(COLOR_BLACK, 7, 7)));
    board.playChess(ChessMove(COLOR_BLACK, 7, 7));
    board.playChess(ChessMove(COLOR_WHITE, 6, 7));
    board.playChess(ChessMove(COLOR_BLACK, 7, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 5));
    board.playChess(ChessMove(COLOR_BLACK, 6, 6));
    board.playChess(ChessMove(COLOR_WHITE, 6, 8));
    board.printChessBord(ChessMove(COLOR_BLACK, 8, 6));
    printf("%0.0lf\n", evaluator.evaluateMove(board, ChessMove(COLOR_BLACK, 8, 6)));
    board.playChess(ChessMove(COLOR_BLACK, 8, 6));
    board.printChessBord(ChessMove(COLOR_WHITE, 6, 9));
    printf("%0.0lf\n", evaluator.evaluateMove(board, ChessMove(COLOR_WHITE, 6, 9)));
    board.playChess(ChessMove(COLOR_WHITE, 6, 9));
    board.playChess(ChessMove(COLOR_BLACK, 9, 6));  
    board.playChess(ChessMove(COLOR_WHITE, 5, 6));  
    board.playChess(ChessMove(COLOR_WHITE, 4, 7));  
    board.playChess(ChessMove(COLOR_WHITE, 3, 8));  
    board.printChessBord(ChessMove(COLOR_WHITE, 2, 9));
    printf("%0.0lf\n", evaluator.evaluateMove(board, ChessMove(COLOR_WHITE, 2, 9)));
    board.playChess(ChessMove(COLOR_WHITE, 2, 9));   
}