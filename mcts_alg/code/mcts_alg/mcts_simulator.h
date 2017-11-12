/**
 * @File mcts_alg/mcts_simulator.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-09 02:55:47
 */
#ifndef mcts_alg__MCTS_SIMULATOR_H__
#define mcts_alg__MCTS_SIMULATOR_H__

#include "gomoku/gomoku_simulator.h"
#include "gomoku/move_generator.h"

using gomoku::GomokuSimulatorIf;
using gomoku::TScore;
using gomoku::MoveGeneratorIf;
using gomoku::GomokuSimulatorIf;
using gomoku::ChessBoard;
using gomoku::ChessMove;
using gomoku::MAX_MOVE_COUNT;
using gomoku::SortedMoveGenerator;
using gomoku::TChessColor;
using gomoku::AllMoveNoScoreGenerator;

namespace mcts_alg
{

class SimpleGomokuSimulator: virtual public GomokuSimulatorIf
{
public:
    /**
     * 判断最后谁赢了局面
     * 返回胜利的颜色
     * 如果是平局， 则返回COLOR_BLANK
     **/
    virtual TChessColor simulateWinner(const ChessBoard & board);
    SortedMoveGenerator & getGenerator()
    {
        return m_moveGenerator;
    }
private:
    SortedMoveGenerator m_moveGenerator;
};

class RandomGomokuSimulator: virtual public GomokuSimulatorIf
{
public:
    /**
     * 判断最后谁赢了局面
     * 返回胜利的颜色
     * 如果是平局， 则返回COLOR_BLANK
     **/
    virtual TChessColor simulateWinner(const ChessBoard & board);
protected:
    void genAllMoves(const ChessBoard & board);
    ChessMove play(ChessBoard & board);
    void addPosibleMove(const ChessMove & move);
protected:
    ChessBoard m_moveBitmap;
    ChessMove m_arrMove[MAX_MOVE_COUNT];
    size_t m_iRes;
};

}//namespace mcts_alg

#endif //mcts_alg/__MCTS_SIMULATOR_H__
