#include "gtest/gtest.h"

#define UNIT_TEST true
#include "gomoku/chess_board.h"

using namespace gomoku;


#include <stdio.h>
#include "mcts_alg/mcts_search_engine.h"
#include "mcts_alg/mcts_simulator.h"

#include <string>
using namespace std;
using namespace gomoku;
using namespace mcts_alg;


TEST(RandomMcts, test)
{
    RandomGomokuSimulator randomSimulator;
    AllMoveNoScoreGenerator allMoveGenerator;
    SearchLimit searchLimit;
    searchLimit.iMaxSearchCount = 100000;
    searchLimit.iMaxSearchTimeSec = 20;
    MctsSearchEngine mctsEngine(2,  & allMoveGenerator /** (simulator.getGenerator()) **/, &randomSimulator, searchLimit);
    ChessBoard chessBoard;  

    FILE * fp = fopen("data/block_three.dat", "r");
    char szColor[1024];
    char szBoard[16][CHESS_BOARD_SIZE];
    while(fscanf(fp, "%s", szColor) != EOF )
    {
        printf("--------------------------------\n");
        for(int i = 0; i < 15; i++)
        {
            fscanf(fp, "%s", szBoard[i]);
        }
        for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
        {
            for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
            {
                printf("%c", szBoard[i][j]);
            }
            printf("\n");
        }
        chessBoard.reset(szBoard);
        if((string)"BLACK" == szColor)
        {
            chessBoard.m_nextPlayerColor = COLOR_BLACK;
        }
        else
        {
            chessBoard.m_nextPlayerColor = COLOR_WHITE;
        }
        chessBoard.printChessBord();
        printf("start mcts search engine\n");
        SearchResult result = mctsEngine.search(chessBoard);
        printf("MCTS_SEARCH boardScore [%0.12lf] winRate [%0.6lf] tideRate [%0.6lf] move [%c %d %d] moveScore [%0.2lf] search time [%d sec] count [%d]\n"
                , result.boardScore
                , result.winRate
                , result.tideRate
                , result.nextMove.color
                , result.nextMove.row
                , result.nextMove.col
                , result.nextMoveScore
                , result.iSearchTimeSec
                , result.iSearchCount );
        chessBoard.printChessBord(result.nextMove);
        if(szBoard[result.nextMove.row][result.nextMove.col] != '#')
        {
            printf("testing failed.\n");
            for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
            {
                for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
                {
                    printf("%c", szBoard[i][j]);
                    if(result.allMoveScore[i][j] == -1)
                    {
                        printf("##### ");
                    }
                    else
                    {
                        printf("%0.3lf ", result.allMoveScore[i][j]);
                    }
                }
                printf("\n");
            }
        }
        else
        {
            printf("testing success.\n");
        }
    }
    fclose(fp);
}


TEST(SimpleMcts, test)
{
    RandomGomokuSimulator randomSimulator;
    SortedMoveGenerator sortedMoveGenerator;
    SearchLimit searchLimit;
    searchLimit.iMaxSearchCount = 100000;
    searchLimit.iMaxSearchTimeSec = 20;
    MctsSearchEngine mctsEngine(2,  & sortedMoveGenerator /** (simulator.getGenerator()) **/, &randomSimulator, searchLimit);
    ChessBoard chessBoard;  

    FILE * fp = fopen("data/block_three.dat", "r");
    char szColor[1024];
    char szBoard[16][CHESS_BOARD_SIZE];
    while(fscanf(fp, "%s", szColor) != EOF )
    {
        printf("--------------------------------\n");
        for(int i = 0; i < 15; i++)
        {
            fscanf(fp, "%s", szBoard[i]);
        }
        for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
        {
            for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
            {
                printf("%c", szBoard[i][j]);
            }
            printf("\n");
        }
        chessBoard.reset(szBoard);
        if((string)"BLACK" == szColor)
        {
            chessBoard.m_nextPlayerColor = COLOR_BLACK;
        }
        else
        {
            chessBoard.m_nextPlayerColor = COLOR_WHITE;
        }
        chessBoard.printChessBord();
        printf("start mcts search engine\n");
        SearchResult result = mctsEngine.search(chessBoard);
        printf("MCTS_SEARCH boardScore [%0.12lf] winRate [%0.6lf] tideRate [%0.6lf] move [%c %d %d] moveScore [%0.2lf] search time [%d sec] count [%d]\n"
                , result.boardScore
                , result.winRate
                , result.tideRate
                , result.nextMove.color
                , result.nextMove.row
                , result.nextMove.col
                , result.nextMoveScore
                , result.iSearchTimeSec
                , result.iSearchCount );
        chessBoard.printChessBord(result.nextMove);
        if(szBoard[result.nextMove.row][result.nextMove.col] != '#')
        {
            printf("testing failed.\n");
            for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
            {
                for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
                {
                    printf("%c", szBoard[i][j]);
                    if(result.allMoveScore[i][j] == -1)
                    {
                        printf("##### ");
                    }
                    else
                    {
                        printf("%0.3lf ", result.allMoveScore[i][j]);
                    }
                }
                printf("\n");
            }
        }
        else
        {
            printf("testing success.\n");
        }
    }
    fclose(fp);
}


TEST(SimpleMcts_v2, test)
{
    SimpleGomokuSimulator simulator;
    SortedMoveGenerator sortedMoveGenerator;
    SearchLimit searchLimit;
    searchLimit.iMaxSearchCount = 100000;
    searchLimit.iMaxSearchTimeSec = 20;
    MctsSearchEngine mctsEngine(2,  & sortedMoveGenerator /** (simulator.getGenerator()) **/, &simulator, searchLimit);
    ChessBoard chessBoard;  

    FILE * fp = fopen("data/block_three.dat", "r");
    char szColor[1024];
    char szBoard[16][CHESS_BOARD_SIZE];
    while(fscanf(fp, "%s", szColor) != EOF )
    {
        printf("--------------------------------\n");
        for(int i = 0; i < 15; i++)
        {
            fscanf(fp, "%s", szBoard[i]);
        }
        for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
        {
            for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
            {
                printf("%c", szBoard[i][j]);
            }
            printf("\n");
        }
        chessBoard.reset(szBoard);
        if((string)"BLACK" == szColor)
        {
            chessBoard.m_nextPlayerColor = COLOR_BLACK;
        }
        else
        {
            chessBoard.m_nextPlayerColor = COLOR_WHITE;
        }
        chessBoard.printChessBord();
        printf("start mcts search engine\n");
        SearchResult result = mctsEngine.search(chessBoard);
        printf("MCTS_SEARCH boardScore [%0.12lf] winRate [%0.6lf] tideRate [%0.6lf] move [%c %d %d] moveScore [%0.2lf] search time [%d sec] count [%d]\n"
                , result.boardScore
                , result.winRate
                , result.tideRate
                , result.nextMove.color
                , result.nextMove.row
                , result.nextMove.col
                , result.nextMoveScore
                , result.iSearchTimeSec
                , result.iSearchCount );
        chessBoard.printChessBord(result.nextMove);
        if(szBoard[result.nextMove.row][result.nextMove.col] != '#')
        {
            printf("testing failed.\n");
            for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
            {
                for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
                {
                    printf("%c", szBoard[i][j]);
                    if(result.allMoveScore[i][j] == -1)
                    {
                        printf("##### ");
                    }
                    else
                    {
                        printf("%0.3lf ", result.allMoveScore[i][j]);
                    }
                }
                printf("\n");
            }
        }
        else
        {
            printf("testing success.\n");
        }
    }
    fclose(fp);
}
