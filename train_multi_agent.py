#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
# 由于gpu资源有限， 可以通过该选项设置agent使用cpu模式
#os.environ["CUDA_VISIBLE_DEVICES"] = ""
import sys
from gomoku_chess import *
from gomoku_model import *
from mcts_alpha import *
import numpy as np
import pandas as pd  
from player import *
from datetime import datetime
from collections import deque
import random
import pickle
import math
import multiprocessing as mp

class SelfPlayWorker(object):


    def __init__(self, workerId, rowCount, colCount, nInRow, weightPath, lr):
        self.workerId = workerId
        self.rowCount = rowCount
        self.colCount = colCount
        self.nInRow = nInRow
        self.expandTemerature = 1
        self.policyModel = GomokuModel(self.rowCount, self.colCount, learningRate = lr)
        self.policyModel.build_model(showSummary=False)
        self.weightPath = weightPath

    def run_one_self_play(self, blackPlayout, whitePlayout, resultFile):
        """
        执行一轮self play
        """
        board = GomokuBoard(self.rowCount, self.colCount, self.nInRow)
        alphaZeroEngine = AlphaZeroEngine(self.rowCount, self.colCount
            , mctsPlayout = 500
            , policyModel = self.policyModel
            , isSelfPlay = True
            , cPuct = 5)
        states = []
        outProbs = []
        winnerZs = []
        while True:
            boardState = board_to_state(board)
            if board.curPlayer == COLOR_BLACK:
                alphaZeroEngine.setMctsPlayout(blackPlayout)
            else:
                alphaZeroEngine.setMctsPlayout(whitePlayout)
            self.expandTemerature = 1 -  len(board.moveHistory) / 3 * 0.1
            if self.expandTemerature < 0.2:
                self.expandTemerature = 0.2
            (bestMoveX, moveRates) = alphaZeroEngine.search_moves(board, self.expandTemerature)
            states.append(boardState)
            outProbs.append(moveRates)
            winnerZs.append(board.curPlayer)
            (r, c) = board.location_to_move(bestMoveX)
            board.play(r, c, board.curPlayer)
            print str(board)
            if board.check_game_over():
                break
        print "game over. winner : " + board.playerName[board.winColor]
        if board.winColor == TIDE:
            winnerZs = np.zeros(len(winnerZs))
        else:
            for i in range(0, len(winnerZs)):
                if winnerZs[i] == board.winColor:
                    winnerZs[i] = 1
                else:
                    winnerZs[i] = -1
        result = (board, states, outProbs, winnerZs)
        with open(resultFile, "wb") as fp:
            pickle.dump(result, fp)

    def updateWeights(self):
        print("updateWeights from ", self.weightPath)
        self.policyModel.model.load_weights(self.weightPath)

class SelfPlayAgent(object):

    def __init__(self):
        workerId = sys.argv[1]
        rowCount = int(sys.argv[2])
        colCount = int(sys.argv[3])
        nInRow = int(sys.argv[4])
        weightPath = sys.argv[5]
        lr = float(sys.argv[6])
        self.worker = SelfPlayWorker(
                workerId = workerId
                , rowCount = rowCount
                , colCount = colCount
                , nInRow = nInRow
                , weightPath = weightPath
                , lr = lr
            )

    def serve(self):
        while True:
            try:
                line = sys.stdin.readline().strip()
                if len(line) == 0:
                    continue
            except:
                break
            fields = line.split(" ")
            if len(fields) != 3:
                print "invalid line: " + str(line)
                print "AGENT_RESPONE ERROR"
            else:
                blackPlayout = int(fields[0].strip())
                whitePlayout = int(fields[1].strip())
                resultFile = fields[2].strip()
                print(blackPlayout, whitePlayout, resultFile)
                self.worker.updateWeights()
                self.worker.run_one_self_play(
                    blackPlayout = blackPlayout
                    , whitePlayout = whitePlayout
                    , resultFile = resultFile
                )
                print "AGENT_RESPONE SUCCESS"
            sys.stdout.flush()


if __name__ == '__main__':
    agent = SelfPlayAgent()
    agent.serve()
