/**
 * @File merge_db_evalution.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-05 14:50:43
 */
#include "utils/stringutils.h"
#include "utils/timeutils.h"
#include "utils/path.h"
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
    printf("[usage] %s db_1 db_2 targetFile\n", argv[0]);
}
bool isSameBoard(const ChessBoard & a, const ChessBoard & b)
{
    for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i ++)
    {
        for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
        {
            if(a.m_board[i][j] != b.m_board[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
int mergeChessDb(string db1Path, string db2Path, string saveDbPath)
{
    ChessDb db1(db1Path);
    ChessDb db2(db2Path);
    ChessDb saveDb(saveDbPath);
    while(true)
    {
        ChessBoard board1, board2;
        map<string, string> mpInfo1, mpInfo2;
        bool ok1 = db1.readNext(board1, mpInfo1);
        bool ok2 = db2.readNext(board2, mpInfo2);
        if(ok1 != ok2)
        {
            if(ok1)
            {
                printf("load db 1 ok.\n");
            }
            if(ok2)
            {
                printf("load db2 ok.\n");
            }
            printf("two db chess count diff.\n");
            return -1;
        }
        if(!ok1)
        {
            break;
        }
        if(!isSameBoard(board1, board2))
        {
            printf("has diff chess board.\n");
            return -2;
        }

        int version1 = 0;
        if(mpInfo1["evaluateVersion"] != "")
        {
            version1 = StringUtils::parse<int>(mpInfo1["evaluateVersion"]); 
        }
        
        int version2 = 0;
        if(mpInfo2["evaluateVersion"] != "")
        {
            version2 = StringUtils::parse<int>(mpInfo2["evaluateVersion"]);
        }
        if(version1 < version2)
        {
            saveDb.append(board2, mpInfo2);
        }
        else
        {
            saveDb.append(board1, mpInfo1);
        }
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
    Path db1Path(argv[1]);
    Path db2Path(argv[2]);
    Path saveDbPath(argv[3]);
    if(!db1Path.exists())
    {
        printf("file [%s] not exists.\n", ((string)db1Path).c_str());
        return -2;
    }
    if(!db2Path.exists())
    {
        printf("file [%s] not exists.\n", ((string)db2Path).c_str());
        return -2;
    }
    if(saveDbPath.exists())
    {
        printf("targetFile [%s] is already exists.\n", ((string)saveDbPath).c_str());
        return -2;
    }
    int iRet = mergeChessDb(db1Path, db2Path, saveDbPath);
    if(iRet)
    {
        printf("merge failed.\n");
    }
    else
    {
        printf("merge success.\n");
    }
    return iRet;
}