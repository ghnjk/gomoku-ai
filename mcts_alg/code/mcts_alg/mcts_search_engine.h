/**
 * @File mcts_alg/mcts_search_engine.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-08 22:52:13
 */
#ifndef mcts_alg__MCTS_SEARCH_ENGINE_H__
#define mcts_alg__MCTS_SEARCH_ENGINE_H__

#include <memory.h>

#include "gomoku/gomoku_simulator.h"
#include "gomoku/move_generator.h"
#include "gomoku/search_engine.h"

using gomoku::TScore;
using gomoku::MoveGeneratorIf;
using gomoku::GomokuSimulatorIf;
using gomoku::SearchResult;
using gomoku::ChessBoard;
using gomoku::ChessMove;
using gomoku::TChessColor;
using gomoku::MAX_MOVE_COUNT;

namespace mcts_alg
{

struct SearchLimit
{
    //最大搜索次数
    int iMaxSearchCount;
    //最大搜索时间 单位秒
    int iMaxSearchTimeSec;
};

struct MctsSearchNode
{
    ChessBoard board;
    size_t iWinCount;
    size_t iTideCount;
    size_t iSearchCount;
    MctsSearchNode * ptrChildNodes[gomoku::MAX_MOVE_COUNT];
    ChessMove moves[gomoku::MAX_MOVE_COUNT];
    TScore moveScores[gomoku::MAX_MOVE_COUNT];
    size_t iMoveCount;
    bool isGameOver;
    MctsSearchNode(const ChessBoard & _board)
        : board(_board)
    {
        iWinCount = iSearchCount = 0;
        iMoveCount = 0;
        iTideCount = 0;
        isGameOver = false;
        for(size_t i = 0; i < MAX_MOVE_COUNT; i++)
        {
            ptrChildNodes[i] = NULL;
        }
    }
    virtual ~MctsSearchNode()
    {
        for(size_t i = 0; i < MAX_MOVE_COUNT; i++)
        {
            if(ptrChildNodes[i] != NULL)
            {
                delete ptrChildNodes[i];
                ptrChildNodes[i] = NULL;
            }
        }
    }
    inline MctsSearchNode * expandChild(size_t iMoveIndex)
    {
        if(iMoveIndex >= iMoveCount)
        {
            return NULL;
        }
        if(ptrChildNodes[iMoveIndex] != NULL)
        {
            return ptrChildNodes[iMoveIndex];
        }
        ptrChildNodes[iMoveIndex] = new MctsSearchNode(board);
        ptrChildNodes[iMoveIndex]->board.playChess(moves[iMoveIndex]);
        return ptrChildNodes[iMoveIndex];
    }
    inline TScore getScore()const
    {
        if(iSearchCount == 0)
        {
            return 0;
        }
        return ( (TScore) iWinCount ) / iSearchCount 
            + ( (TScore) iTideCount / 2.0) / iSearchCount;
    }
};

class MctsSearchEngine: virtual public gomoku::SearchEngineIf
{
public:
    /**
     * utcConstArg UCB 估值时， vi + c * sqrt(logNp / ni)中c的参数
     * ptrMoveGenerator 走法产生器
     * GomokuSimulatorIf 评估棋局胜负
     * 搜索限制
     **/
    MctsSearchEngine(TScore ucbConstArg, MoveGeneratorIf * ptrMoveGenerator
        , GomokuSimulatorIf * ptrSimulator, SearchLimit limit);
    /**
     * 搜索下一步走法
     **/
    virtual SearchResult search(const ChessBoard & board);
protected:
    /**
     * dfs 进行 MCTS搜索，
     * 返回本次搜索的赢家
     **/
    TChessColor dfsMcts(MctsSearchNode & node);
    /**
     * root节点选择最优解
     **/
    size_t chooseBestMove(const MctsSearchNode & root);
protected:
    TScore m_ucbConstArg;
    MoveGeneratorIf * m_ptrMoveGenerator;
    GomokuSimulatorIf * m_ptrSimulator;
    SearchLimit  m_searchLimit;
};


}//namespace mcts_alg

#endif //mcts_alg__MCTS_SEARCH_ENGINE_H__
