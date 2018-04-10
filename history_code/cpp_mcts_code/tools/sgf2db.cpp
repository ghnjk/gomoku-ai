/**
 * @File sgf2db.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-05 14:50:43
 */
#include "utils/stringutils.h"
#include "gomoku/hash_table.h"
#include "gomoku/chess_db.h"
#include "tools/sgf_library.h"
#include "gomoku/move_generator.h"
#include <string>
using namespace std;
using namespace gomoku;
using namespace utils;
using namespace tools;

struct ChessBoardNode
{
    ChessBoard board;
    map<string, string> mpInfo;
};

void showHelp(int argc, char ** argv)
{
    printf("[usage] %s sgfFile dbFile [spanCount]\n", argv[0]);
}


void addMoveToDb(ChessBoard& board, ChessDb & db, HashTable<ChessBoardNode>& table)
{
    if(! table.contains(board))
    {
        ChessBoardNode node;
        node.mpInfo["nextPlayerColor"] = StringUtils::toString(board.m_nextPlayerColor);
        node.board = board;
        db.append(board, node.mpInfo);
        table.put(node.board, node);
    }
}
void addChssToDb(ChessBoard board, ChessDb & db, HashTable<ChessBoardNode>& table
    , SortedMoveGenerator& moveGenerator, int spanCount)
{
    ChessMove arrMove[300];
    TScore arrScores[300];
    do
    {
        if(! table.contains(board))
        {
            addMoveToDb(board, db, table);
        }
        size_t iMoveCnt = moveGenerator.generateAllMoves(board.m_nextPlayerColor, board
            , arrMove, arrScores, 300);
        for(size_t i = 0; i < (size_t)spanCount && i < iMoveCnt; i++)
        {
            if(!board.playChess(arrMove[i], true))
            {
                continue;
            }
            addMoveToDb(board, db, table);
            board.undoMove();
        }
    }
    while(board.undoMove());
}
int sgf2db(const string & sgfFile, const string & dbFile, int spanCount)
{
    SortedMoveGenerator moveGenerator;
    ChessDb db(dbFile);
    HashTable<ChessBoardNode> table(5971849);
    if(! db.checkAvailable())
    {
        printf("cannot open db [%s]\n", dbFile.c_str());
        return -1;
    }
    while(true)
    {
        ChessBoardNode node;
        if(! db.readNext(node.board, node.mpInfo))
        {
            break;
        }
        table.put(node.board, node);
    }
    SgfLibrary lib;
    if(! lib.load(sgfFile))
    {
        printf("load SgfLibrary [%s] failed\n", sgfFile.c_str());
        return -1;
    }
    const vector<ChessBoard>& vecAllBoard = lib.getAllBoard();
    for(size_t i = 0; i < vecAllBoard.size(); i++)
    {
        addChssToDb(vecAllBoard[i], db, table, moveGenerator, spanCount);
    }
    return 0;
}
int main(int argc, char ** argv)
{
    if(argc != 3 && argc != 4)
    {
        printf("invalid argumens.\n");
        showHelp(argc, argv);
        return -1;
    }
    string sgfFile = argv[1];
    string dbFile = argv[2];
    int spanCount = 0;
    if(argc == 4)
    {
        spanCount = StringUtils::parse<int>(argv[3]);
    }
    printf("spanCount = %d\n", spanCount);
    return sgf2db(sgfFile, dbFile, spanCount);
}