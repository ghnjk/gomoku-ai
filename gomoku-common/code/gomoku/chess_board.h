/**
 * @File gomoku/chess_borad.h
 * @Brief file_description
 * Details:
 * @Author guohainan
 * @Version v0.0.1
 * @date 2017-09-05 14:38:00
 */
#ifndef gomoku__CHESS_BORAD_H__
#define gomoku__CHESS_BORAD_H__

#include "gomoku/constant.h"

namespace gomoku
{

/**
 * 走法
 **/
struct ChessMove
{
    TChessPos row;
    TChessPos col;
    TChessColor color;
    ChessMove()
    {
        row = col = color = (char) 0;
    }
    ChessMove(TChessColor player, TChessPos r, TChessPos c)
        : row(r), col(c), color(player)
    {
    }
    inline string toString() const
    {
        char szTmp[32] = {0};
        snprintf(szTmp, sizeof(szTmp) - 1, "%c put [%d, %d]", color, (int) row, (int) col);
        return szTmp;
    }
};

/**
 * 棋盘
 **/
struct ChessBoard
{
public:
    /**
     * 判断游戏是否结束
     **/
    bool isGameOver()const;
    /**
     * 根据最后一步判断棋局是否结束
     **/
    bool isGameOver(const ChessMove & lastStep)const;
    /**
     * 打印棋盘
     **/
    void printChessBord()const;
public:
    //inline public function
    /**
     * 下棋， isCheckRule 是否校验该步是否合法
     **/
    inline bool payChess(const ChessMove & step, bool isCheckRule = false);
    /**
     * 判断是否合法
     **/
    inline bool isValidStep(const ChessMove & step) const;
public:
    //棋盘点阵信息
    TChessColor m_bord[CHESS_BORD_SIZE][CHESS_BORD_SIZE];
    //下一个玩家颜色
    TChessColor m_nextPlayerColor;
};


}//namespace gomoku

#endif //gomoku__CHESS_BORAD_H__
