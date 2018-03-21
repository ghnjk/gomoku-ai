#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
from gomoku_chess import *
from gomoku_model import *
import numpy as np
import matplotlib  
import matplotlib.pyplot as plt


def test_board_convertion():
    """
    test for function board_to_state function
    """
    board = GomokuBoard(4, 5, 2)
    board.play(0, 0, board.curPlayer)
    board.play(2, 0, board.curPlayer)
    board.play(0, 2, board.curPlayer)
    board.play(3, 3, board.curPlayer)
    print str(board)
    res = board_to_state(board)
    for i in range(0, 4):
        print "channel: %d" % (i)
        for r in range(0, board.rowCount):
            line = ""
            for c in range(0, board.colCount):
                line += " %d" % (res[r][c][i])
            print line
    return res


if __name__ == '__main__':
    test_board_convertion()

