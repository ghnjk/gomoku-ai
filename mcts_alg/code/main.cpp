/**
 * @File main.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-10 16:34:06
 */

#include "mcts_alg/mcts_search_engine.h"
#include "mcts_alg/mcts_simulator.h"

#include <string>
using namespace std;
using namespace gomoku;
using namespace mcts_alg;



void debugMcts()
{
    SimpleGomokuSimulator simulator;
    RandomGomokuSimulator randomSimulator;
    AllMoveNoScoreGenerator allMoveGenerator;
    SearchLimit searchLimit;
    searchLimit.iMaxSearchCount = 100000;
    searchLimit.iMaxSearchTimeSec = 10;
    MctsSearchEngine mctsEngine(2,  & allMoveGenerator /** (simulator.getGenerator()) **/, &randomSimulator, searchLimit);

    ChessBoard chessBoard;
    while(! chessBoard.isGameOver())
    {
        printf("mcts search start.\n");
        SearchResult result = mctsEngine.search(chessBoard);
        printf("mcsc search end\n");
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
        chessBoard.playChess(result.nextMove);
        if(chessBoard.isGameOver())
        {
            break;
        }
        while(true)
        {
            printf("input your move: row(0-14) col(0-14): ");
            ChessMove mv;
            int r, c;
            scanf("%d %d", &r, &c);
            mv.row = r;
            mv.col = c;
            mv.color = COLOR_WHITE;
            if(chessBoard.playChess(mv, true))
            {
                break;
            }
            else
            {
                printf("----- invalid step.\n");
               // chessBoard.printChessBord();
                //printf("invalid step %d %d\n", mv.row, mv.col);
            }
        }
    }
    printf("winner: %c\n", chessBoard.getWinColor());
}

void mctsAgent()
{
    FILE * fpLog = fopen("/tmp/mctsAgent.log", "a");
    SimpleGomokuSimulator simulator;
    RandomGomokuSimulator randomSimulator;
    AllMoveNoScoreGenerator allMoveGenerator;
    SearchLimit searchLimit;
    searchLimit.iMaxSearchCount = 100000;
    searchLimit.iMaxSearchTimeSec = 20;
    MctsSearchEngine mctsEngine(2,  & (simulator.getGenerator()), &simulator, searchLimit);
    ChessBoard chessBoard;
    char szCmd[128];
    while(scanf("%s", szCmd) != EOF)
    {
        string strCmd = szCmd;
        if(strCmd == "PLAY_WHITE")
        {
            int row, col;
            scanf("%d %d", &row, &col);
            fprintf(fpLog, "RECEIVE PLAY_WHITE %d %d\n", row, col);
            chessBoard.playChess(ChessMove(COLOR_WHITE, row, col));
        }
        else if(strCmd == "PLAY_BLACK")
        {
            int row, col;
            scanf("%d %d", &row, &col);
            fprintf(fpLog, "RECEIVE PLAY_BLACK %d %d\n", row, col);
            chessBoard.playChess(ChessMove(COLOR_BLACK, row, col));
        }
        else if(strCmd == "SHOW")
        {
            int row, col;
            scanf("%d %d", &row, &col);
            char szInput[256];
            while(row --)
            {
                scanf("%s", szInput);
            }
            fprintf(fpLog, "RECEIVE SHOW\n");
        }
        else if(strCmd == "NEXT_BLACK")
        {
            chessBoard.m_nextPlayerColor = COLOR_BLACK;
            fprintf(fpLog, "RECEIVE NEXT_BLACK\n");
            SearchResult result = mctsEngine.search(chessBoard);
            printf("PUT %d %d\n", result.nextMove.row, result.nextMove.col);
            chessBoard.playChess(result.nextMove);
            fprintf(fpLog, "MCTS_SEARCH boardScore [%0.12lf] winRate [%0.6lf] tideRate [%0.6lf] move [%c %d %d] moveScore [%0.2lf] search time [%d sec] count [%d]\n"
                , result.boardScore
                , result.winRate
                , result.tideRate
                , result.nextMove.color
                , result.nextMove.row
                , result.nextMove.col
                , result.nextMoveScore
                , result.iSearchTimeSec
                , result.iSearchCount );
        }
        else if(strCmd == "NEXT_WHITE")
        {
            chessBoard.m_nextPlayerColor = COLOR_WHITE;
            fprintf(fpLog, "RECEIVE NEXT_WHITE\n");
            SearchResult result = mctsEngine.search(chessBoard);
            printf("PUT %d %d\n", result.nextMove.row, result.nextMove.col);
            chessBoard.playChess(result.nextMove);
            fprintf(fpLog, "MCTS_SEARCH boardScore [%0.12lf] winRate [%0.6lf] tideRate [%0.6lf] move [%c %d %d] moveScore [%0.2lf] search time [%d sec] count [%d]\n"
                , result.boardScore
                , result.winRate
                , result.tideRate
                , result.nextMove.color
                , result.nextMove.row
                , result.nextMove.col
                , result.nextMoveScore
                , result.iSearchTimeSec
                , result.iSearchCount );
        }
        else if(strCmd == "CLEAR")
        {
            chessBoard = ChessBoard();
            fprintf(fpLog, "RECEIVE CLEAR\n");
        }
        else
        {
            fprintf(fpLog, "RECEIVE UNKNOWN CMD [%s]\n", szCmd);
        }
        fflush(fpLog);
        fflush(stdout);
    }
    fclose(fpLog);
}

int main(int argc, char ** argv)
{
    if(argc >= 2 && (string)argv[1] == "agent" )
    {
        mctsAgent();
    }
    else
    {
        debugMcts();
    }
    return 0;
}

