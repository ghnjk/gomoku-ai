#!/usr/bin/python
# -*- coding: UTF-8 -*- 

# 棋盘大小
CHESS_BOARD_SIZE = 15
# 棋子颜色
# 空白
COLOR_BLANK = '.'
# 黑丝
COLOR_BLACK = 'B'
# 白色
COLOR_WHITE = 'W'

class ChessBoard(object):
    """
    棋局
    """

    def __init__(self):
        self.board =  []
        for i in range(0, CHESS_BOARD_SIZE):
            r = []
            for j in range(0, CHESS_BOARD_SIZE):
                r.append(COLOR_BLANK)
            self.board.append(r)
        self.next_color = COLOR_BLACK

    def reset(self, board):
        """
        根据输入的board map初始化
        """
        white_cnt = 0
        black_cnt = 0
        for i in range(0, CHESS_BOARD_SIZE):
            for j in range(0, CHESS_BOARD_SIZE):
                if board[i][j] == COLOR_BLACK:
                    black_cnt += 1
                    self.board[i][j] = COLOR_BLACK
                elif board[i][j] == COLOR_WHITE:
                    white_cnt += 1
                    self.board[i][j] = COLOR_WHITE
                else:
                    self.board[i][j] = COLOR_BLANK
        if black_cnt > white_cnt:
            self.next_color = COLOR_WHITE
        else:
            self.next_color = COLOR_BLACK

    def __str__(self):
        res = ""
        for i in range(0, CHESS_BOARD_SIZE):
            res += ''.join(self.board[i])
            res += '\n'
        return res


