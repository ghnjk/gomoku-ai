/**
 * @File mcts_alg/mcts_search_engine.cpp
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-09 00:32:39
 */
#include "mcts_alg/mcts_search_engine.h"

#include <math.h>
#include <time.h>

using namespace gomoku;

namespace mcts_alg
{


/**
 * utcConstArg UCB 估值时， vi + c * sqrt(logNp / ni)中c的参数
 * ptrMoveGenerator 走法产生器
 * GomokuSimulatorIf 评估棋局胜负
 **/
MctsSearchEngine::MctsSearchEngine(TScore ucbConstArg, MoveGeneratorIf * ptrMoveGenerator
        , GomokuSimulatorIf * ptrSimulator, SearchLimit limit)
{
    m_ucbConstArg = ucbConstArg;
    m_ptrMoveGenerator = ptrMoveGenerator;
    m_ptrSimulator = ptrSimulator;
    m_searchLimit = limit;
}
/**
 * 搜索下一步走法
 **/
SearchResult MctsSearchEngine::search(const ChessBoard & board)
{
    MctsSearchNode root(board);
    root.isGameOver = board.isGameOver();
    root.iMoveCount = m_ptrMoveGenerator->generateAllMoves(
        board.m_nextPlayerColor, board
        , root.moves, root.moveScores, MAX_MOVE_COUNT
        );
    int i = 0;
    time_t tStart = time(NULL);
    for(i = 0; m_searchLimit.iMaxSearchCount <= 0 || i < m_searchLimit.iMaxSearchCount; i++)
    {
        if(i != 0 && m_searchLimit.iMaxSearchTimeSec > 0 && (i + 1) % 100 == 0)
        {
            time_t tEnd = time(NULL);
            if( (int) (tEnd - tStart) >= m_searchLimit.iMaxSearchTimeSec)
            {
                break;
            }
        }
        dfsMcts(root);
    }
    SearchResult res;
    res.boardScore = root.getScore();
    res.winRate = root.iWinCount * 100.0 / (double) root.iSearchCount;
    res.tideRate = root.iTideCount * 100.0 / (double) root.iSearchCount;
    size_t iMoveIndex = chooseBestMove(root);
    res.nextMove = root.moves[iMoveIndex];
    res.nextMoveScore = root.moveScores[iMoveIndex];
    res.iSearchTimeSec = (int) ( time(NULL) - tStart );
    res.iSearchCount = i;
    return res;
}

/**
 * dfs 进行 MCTS搜索，
 * 返回本次搜索的赢家
 **/
TChessColor MctsSearchEngine::dfsMcts(MctsSearchNode & node)
{
    TChessColor winColor;
    if(node.isGameOver)
    {
        winColor = node.board.getLastPlayerColor();
    }
    else if(node.iMoveCount == 0)
    {
        winColor = COLOR_BLANK;
    }
    else
    {
        size_t iSelectIndex = 0;
        TScore selectUcb = 0;
        TScore selectMoveScore = 0;
        for(size_t i = 0; i < node.iMoveCount; i++)
        {
            //selection
            TScore ucb = 0;
            if(node.ptrChildNodes[i] != NULL)
            {
                ucb = node.ptrChildNodes[i]->getScore()
                    + m_ucbConstArg * sqrt(log(node.iSearchCount) / (node.ptrChildNodes[i]->iSearchCount));
            }
            else
            {
                ucb = 0.5 + m_ucbConstArg * sqrt(log(node.iSearchCount));
            }
            if(i == 0 || selectUcb < ucb 
                || (selectUcb == ucb && selectMoveScore < node.moveScores[i]))
            {
                iSelectIndex = i;
                selectUcb = ucb;
                selectMoveScore = node.moveScores[i];
            }
        }
        if(node.ptrChildNodes[iSelectIndex] == NULL)
        {
            //expansion
            MctsSearchNode* ptrChild = node.expandChild(iSelectIndex);
            ptrChild->isGameOver = ptrChild->board.isGameOver(node.moves[iSelectIndex]);
            if(! ptrChild->isGameOver)
            {
                ptrChild->iMoveCount = m_ptrMoveGenerator->generateAllMoves(
                    ptrChild->board.m_nextPlayerColor, ptrChild->board
                    , ptrChild->moves, ptrChild->moveScores, MAX_MOVE_COUNT
                    );
                //simulate
                if(ptrChild->iMoveCount == 0)
                {
                    winColor = COLOR_BLANK;
                }
                else
                {
                    winColor = m_ptrSimulator->simulateWinner(ptrChild->board);
                }
            }
            else
            {
                winColor = ptrChild->board.getLastPlayerColor();
            }
            
        }
        else
        {
            winColor = dfsMcts(* (node.ptrChildNodes[iSelectIndex]));
        }
    }
    //Backpropagation
    if(winColor == node.board.m_nextPlayerColor)
    {
        node.iWinCount ++;
        node.iSearchCount ++;
    }
    else if(winColor == COLOR_BLANK)
    {
        node.iTideCount ++;
        node.iSearchCount ++;
    }
    else
    {
        node.iSearchCount ++;
    }
    return winColor;
}


/**
* root节点选择最优解
**/
size_t MctsSearchEngine::chooseBestMove(const MctsSearchNode & root)
{
    size_t iSelectIndex = 0;
    TScore selectMoveScore = 0;
    TScore selectScore = 0;
    for(size_t i = 0; i < root.iMoveCount; i++)
    {
        if(root.ptrChildNodes[i] == NULL)
        {
            continue;
        }
        if(root.ptrChildNodes[i]->iSearchCount  * 20 < root.iSearchCount)
        {
            continue;
        }

        TScore childScore = root.ptrChildNodes[i]->getScore();
        if(
            (selectScore + 0.05 < childScore )
            || ( fabs(selectScore - childScore) < 0.05 && selectMoveScore < root.moveScores[i] )
            )
        {
            iSelectIndex = i;
            selectScore = childScore;   
            selectMoveScore = root.moveScores[i];
        }
    }
    return iSelectIndex;
}


}//namespace mcts_alg