/**
 * @File gomoku/search_engine.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-08 22:58:21
 */
#ifndef gomoku__SEARCH_ENGINE_H__
#define gomoku__SEARCH_ENGINE_H__

#include "gomoku/chess_board.h"
#include "gomoku/move_generator.h"

namespace gomoku
{

struct SearchResult
{
    //当前局面分数
    TScore boardScore;
    //下一步走法
    ChessMove nextMove;
    //下一步走法的分数
    TScore nextMoveScore;
    //胜的概率
    double winRate;
    //平局的概率 
    double tideRate;
    //搜索时间单位秒
    int iSearchTimeSec;
    //搜索次数
    int iSearchCount;
    //每个棋子的胜率
    TScore allMoveScore[CHESS_BOARD_SIZE][CHESS_BOARD_SIZE];
};

class SearchEngineIf
{
public:
    /**
     * 搜索下一步走法
     **/
    virtual SearchResult search(const ChessBoard & board) = 0;
};

}//namespace gomoku

#endif //gomoku__SEARCH_ENGINE_H__
