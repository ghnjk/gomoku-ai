/**
 * @File mcts_evaluator.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-05 14:50:43
 */
#include "utils/stringutils.h"
#include "utils/timeutils.h"
#include "gomoku/hash_table.h"
#include "gomoku/chess_db.h"
#include "tools/sgf_library.h"
#include "gomoku/move_generator.h"
#include "mcts_alg/mcts_search_engine.h"
#include "mcts_alg/mcts_simulator.h"
#include <string>
using namespace std;
using namespace gomoku;
using namespace utils;
using namespace tools;
using namespace mcts_alg;


void showHelp(int argc, char ** argv)
{
    printf("[usage] %s dbFile startIdx endIdx\n", argv[0]);
}

int mctsEvaluate(const string & dbFile, int startIdx, int endIdx)
{
    RandomGomokuSimulator randomSimulator;
    AllMoveNoScoreGenerator allMoveGenerator;
    SearchLimit searchLimit;
    searchLimit.iMaxSearchCount = 100000;
    searchLimit.iMaxSearchTimeSec = 60;
    MctsSearchEngine mctsEngine(2,  & allMoveGenerator /** (simulator.getGenerator()) **/, &randomSimulator, searchLimit);
    ChessDb db(dbFile);
    if(! db.seekChess(startIdx))
    {
        printf("seekChess to startIdx [%d] failed\n", startIdx);
        return -2;
    }
    for(int idx = startIdx; idx <= endIdx; idx ++)
    {
        ChessBoard board;
        map<string, string> mpInfo;
        if(!db.readNext(board, mpInfo))
        {
            printf("read %d failed\n", idx);
            return -3;
        }
        if(mpInfo["evaluateVersion"] == "1")
        {
            printf("skip board [%d]\n", idx);
            continue;
        }
        printf("%s start evalute for checc[%d]\n", TimeUtils::timeAsString(STANDARD_DATETIME_FMT).c_str(), idx);
        board.printChessBord();
        SearchResult result = mctsEngine.search(board);
        printf("check result:\n");
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
        board.printChessBord(result.nextMove);
        mpInfo["boardScore"] = StringUtils::toString(result.boardScore);
        mpInfo["winRate"] = StringUtils::toString(result.winRate);
        mpInfo["tideRate"] = StringUtils::toString(result.tideRate);
        mpInfo["nextMove.color"] = StringUtils::toString(result.nextMove.color);
        mpInfo["nextMove.row"] = StringUtils::toString(result.nextMove.row);
        mpInfo["nextMove.col"] = StringUtils::toString(result.nextMove.col);
        mpInfo["iSearchTimeSec"] = StringUtils::toString(result.iSearchTimeSec);
        mpInfo["iSearchCount"] = StringUtils::toString(result.iSearchCount);
        mpInfo["evaluateVersion"] = "1";
        db.save(board, mpInfo);
        printf("%s end evalute for checc[%d]\n", TimeUtils::timeAsString(STANDARD_DATETIME_FMT).c_str(), idx);
        fflush(stdout);
    }
    return 0;
}

int main(int argc, char ** argv)
{
    if(argc != 4)
    {
        printf("invalid argumens.\n");
        showHelp(argc, argv);
        return -1;
    }
    string dbFile = argv[1];
    int startIdx = StringUtils::parse<int>(argv[2]);
    int endIdx = StringUtils::parse<int>(argv[3]);
    return mctsEvaluate(dbFile, startIdx, endIdx);
}