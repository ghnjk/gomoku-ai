#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
from gomoku_chess import *
from gomoku_model import *
from mcts_alpha import *
from train import *
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


def test_model_seriaze():
    model = GomokuModel(rowCount = 6, colCount = 6)
    model.build_model()
    model.save_model("model.json", "weight.hdf5")

def print_format_moverate(board, moveRates):
    a = np.array(moveRates).reshape(board.rowCount, board.colCount)
    for r in range(0, board.rowCount):
        s = ""
        for c in range(0, board.colCount):
            s += "%1.03lf " %(a[r][c])
        print s

def test_gen_policy():
    model = GomokuModel(rowCount = 6, colCount = 6)
    model.build_model()
    board = GomokuBoard(rowCount = 6, colCount = 6, nInRow = 4)
    (winRate, moveProbs) = model.gen_policy(board)
    print "winRate: " + str(winRate)
    print "moveProbs: "
    print str(moveProbs)

def test_alpha_zero_engine():
    engine = AlphaZeroEngine(rowCount = 6, colCount = 6)
    board = GomokuBoard(rowCount = 6, colCount = 6, nInRow = 4)
    (bestMoveX, moveRates) = engine.search_moves(board)
    (r, c) = board.location_to_move(bestMoveX)
    print "bestMoveX: %d %d" % ( r, c)
    print "moveRates: "
    print_format_moverate(board, moveRates)

def test_train_store():
    store = TrainDataStore("data/test.dat", rowCount = 6, colCount = 6)
    model = GomokuModel(rowCount = 6, colCount = 6)
    model.build_model()
    board = GomokuBoard(rowCount = 6, colCount = 6, nInRow = 4)
    (winRate, moveProbs) = model.gen_policy(board)
    moveRates = np.zeros(board.rowCount * board.colCount)
    for (x, rate) in moveProbs:
        moveRates[x] = rate
    store.append(board_to_state(board), winRate, moveRates, "1")
    store.save()
    tmpStore = TrainDataStore("data/test.dat")
    tmpStore.load()
    print tmpStore.size()
    print tmpStore.shape
    print np.array(tmpStore.boardFeatureTable)
    print np.array(tmpStore.winRateTable)
    print np.array(tmpStore.moveRateTable)
    print np.array(tmpStore.modelVersionTable)

def test_trainner_one_play():
    trainner = SelfPlayTranner(
        rowCount = 6
        , colCount = 6
        , nInRow = 4
        , trainBatchCnt = 1500
        , gameCntPerBatch = 1
        , hdf5FileName = "data/test.dat"
        )
    trainner.run_one_self_play()
    trainner.update_model()

if __name__ == '__main__':
    test_trainner_one_play()

