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
    ChessBoard rootBoard(board);
    int i = 0;
    time_t tStart = time(NULL);
    MctsSearchNode root;
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
        dfsMcts(root, rootBoard);
    }
    SearchResult res = chooseBestMove(root);
    res.iSearchTimeSec = (int) ( time(NULL) - tStart );
    res.iSearchCount = i;
    return res;
}

/**
 * dfs 进行 MCTS搜索，
 * 返回本次搜索的赢家
 **/
TChessColor MctsSearchEngine::dfsMcts(MctsSearchNode & node, ChessBoard & board)
{
    TChessColor winColor;
    bool isNewNode = false;
    if(node.isNewNode())
    {
        isNewNode = true;
        //expansion
        node.expand(board, m_ptrMoveGenerator);
    }
    if(node.isGameOver)
    {
        winColor = node.winColor;
    }
    else if(isNewNode)
    {
        // simulation
        winColor = m_ptrSimulator->simulateWinner(board);
    }
    else
    {
        TScore selectedUcb = 0;
        size_t selectIdx = 0;
        //selection
        for(size_t i = 0; i < node.iMoveCnt; i++)
        {
            TScore ucb = node.getMoveUcb(m_ucbConstArg, i);
            if(i == 0 || selectedUcb < ucb)
            {
                selectedUcb = ucb;
                selectIdx = i;
            }
        }
        board.playChess(node.arrMoves[selectIdx]);
        winColor = dfsMcts(* node.ptrChildNode[selectIdx], board);
        board.undoMove();
    }
    // backpropagation
    node.iSearchCnt ++;
    if(winColor == COLOR_TIDE)
    {
        node.iTideCnt ++;
    }
    else if(winColor == board.getLastPlayerColor())
    {
        node.iWinCnt ++;
    }
    return winColor;
}

/**
* root节点选择最优解
**/
SearchResult MctsSearchEngine::chooseBestMove(const MctsSearchNode & root)
{
    size_t iSelectIndex = 0;
    TScore selectScore = 0;
    SearchResult res;
    for(TChessPos i = 0; i < CHESS_BOARD_SIZE; i++)
    {
        for(TChessPos j = 0; j < CHESS_BOARD_SIZE; j++)
        {
            res.allMoveScore[i][j] = -1;
        }
    }
    for(size_t i = 0; i < root.iMoveCnt; i++)
    {
        TScore childScore = root.ptrChildNode[i]->getScore();
        const ChessMove & move = root.arrMoves[i];
        res.allMoveScore[move.row][move.col] = childScore;
        if(selectScore  < childScore)
        {
            iSelectIndex = i;
            selectScore = childScore;   
        }
    }
    const MctsSearchNode & child = *(root.ptrChildNode[iSelectIndex]);
    res.boardScore = child.getScore();
    res.winRate = child.iWinCnt * 100.0 / (double) child.iSearchCnt;
    res.tideRate = child.iTideCnt * 100.0 / (double) child.iSearchCnt;
    res.nextMove = root.arrMoves[iSelectIndex];
    res.nextMoveScore = root.arrMoveScores[iSelectIndex];
    return res;
}


}//namespace mcts_alg
