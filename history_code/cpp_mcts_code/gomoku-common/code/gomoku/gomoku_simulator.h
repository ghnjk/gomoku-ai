/**
 * @File gomoku/gomoku_simulator.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-08 23:39:18
 */
#ifndef gomoku__GOMOKU_SIMULATOR_H__
#define gomoku__GOMOKU_SIMULATOR_H__
#include <map>
using std::map;

#include "gomoku/chess_board.h"

namespace gomoku
{


class GomokuSimulatorIf
{
public:
    /**
     * 判断最后谁赢了局面
     * 返回胜利的颜色
     * 如果是平局， 则返回COLOR_BLANK
     **/
    virtual TChessColor simulateWinner(const ChessBoard & board) = 0;
};

class MoveEvaluator
{
public:
    MoveEvaluator();
    //评估某步的价值
    TScore evaluateMove(const ChessBoard & board, const ChessMove & move);
#ifndef UNIT_TEST
protected:
#endif
    void initLineTypeMap();
    bool isMatchLine(int iPos, char szLine[9], const string & str);
    bool isValidLineType(int iPos, char szLine[9]);
    void getLineFromBoard(const ChessBoard & board, const ChessMove & move, TChessPos dr, TChessPos dc
        , int & iPos, char szLine[9] );
    int getLineType(int iPos, char szLine[9]);
#ifndef UNIT_TEST
protected:
#endif
    map<int, int> m_mpLineType;
};

}//bnamespace gomoku
#endif //gomoku__GOMOKU_SIMULATOR_H__
