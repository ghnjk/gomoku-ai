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
#include <math.h>

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
using gomoku::TChessPos;
using gomoku::COLOR_BLANK;

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
    // 节点是否已经结束
    bool isGameOver;
    // 如果isGameOver=true， 则表示最后一方赢得颜色
    TChessColor winColor;
    // 子局面个数
    size_t iMoveCnt;
    // 子局面步骤信息
    ChessMove arrMoves[MAX_MOVE_COUNT];
    // 子局面节点
    MctsSearchNode * ptrChildNode[MAX_MOVE_COUNT];
    // 子局面走法分数
    TScore arrMoveScores[MAX_MOVE_COUNT];
    // 下到该局面的那方赢得次数
    size_t iWinCnt;
    // 平局次数
    size_t iTideCnt;
    // 访问次数
    size_t iSearchCnt;
    MctsSearchNode()
    {
        isGameOver = false;
        iMoveCnt = 0;
        iWinCnt = 0;
        iTideCnt = 0;
        iSearchCnt = 0;
    }
    virtual ~MctsSearchNode()
    {
        for(TChessPos i = 0; i < (int)iMoveCnt; i++)
        {
            delete ptrChildNode[i];
        }
    }
    inline void expand(ChessBoard & board, MoveGeneratorIf * ptrMoveGenerator)
    {
        winColor = board.getWinColor();
        if(winColor == COLOR_BLANK)
        {
            iMoveCnt = ptrMoveGenerator->generateAllMoves(board.m_nextPlayerColor, board
                , arrMoves, arrMoveScores, MAX_MOVE_COUNT);
            for(TChessPos i = 0; i < (int)iMoveCnt; i++ )
            {
                ptrChildNode[i] = new MctsSearchNode();
            }
        }
        else
        {
            isGameOver = true;
        }
    }
    inline TScore getScore() const
    {
        return  ((TScore)iWinCnt) / iSearchCnt;
    }
    inline TScore getMoveUcb(const TScore & c, size_t i){
        if(i >= iMoveCnt)
        {
            return -1;
        }
        if(ptrChildNode[i]->iSearchCnt == 0 && iSearchCnt == 0)
        {
            return 0;
        }
        if(ptrChildNode[i]->iSearchCnt == 0)
        {
            return sqrt(c * log(iSearchCnt));
        }
        else
        {
            return ptrChildNode[i]->getScore()
              + sqrt(c * log(iSearchCnt) / ptrChildNode[i]->iSearchCnt );
        }
    }
    inline bool isNewNode()
    {
        return iSearchCnt == 0;
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
    TChessColor dfsMcts(MctsSearchNode & node, ChessBoard & board);
    /**
     * root节点选择最优解
     **/
    SearchResult chooseBestMove(const MctsSearchNode & root);
protected:
    TScore m_ucbConstArg;
    MoveGeneratorIf * m_ptrMoveGenerator;
    GomokuSimulatorIf * m_ptrSimulator;
    SearchLimit  m_searchLimit;
};


}//namespace mcts_alg

#endif //mcts_alg__MCTS_SEARCH_ENGINE_H__
