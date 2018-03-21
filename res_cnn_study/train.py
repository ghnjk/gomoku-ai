#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
from gomoku_chess import *
from gomoku_model import *
import numpy as np
import pandas as pd
import matplotlib  
import matplotlib.pyplot as plt

class TrainDataStore(object):
    """
    store and read trainning data
    """

    def __init__(self, hdf5FileName, rowCount = 15, colCount = 15, channelCount = 4):
        self.hdf5FileName = hdf5FileName
        self.rowCount = rowCount
        self.colCount = colCount
        self.channelCount = channelCount
        self.shape = (self.rowCount, self.colCount, self.channelCount)
        self.boardFeatureTable = []
        self.winRateTable = []
        self.moveRateTable = []
        self.modelVersionTable = []

    def set_shape(self, rowCount, colCount, channelCount):
        """
        reset the data dimension
        """
        self.rowCount = rowCount
        self.colCount = colCount
        self.channelCount = channelCount
        self.shape = (self.rowCount, self.colCount, self.channelCount)

    def load(self):
        """
        load data from file:
        store['shape'] = [1, 3]
        store['boardFeatureTable'] = [dataIndex, rowCount * colCount * channelCount]
        store['winRateTable'] = [dataIndex, 1]
        store['moveRateTable'] = [dataIndex, rowCount * colCount]
        store['modelVersionTable'] = [dataIndex, 1]
        """
        store = pd.HDFStore(self.hdf5FileName)
        keys = store.keys()
        if 'shape' not in keys or 'boardFeatureTable' not in keys or 'winRateTable' not in keys or 'moveRateTable' not in keys or 'modelVersionTable' not in keys:
            return False
        self.shape = np.array(store['shape'])
        self.rowCount = self.shape[0]
        self.colCount = self.shape[1]
        self.channelCount = self.shape[2]
        self.boardFeatureTable = []
        df = np.array(store['boardFeatureTable'])
        for x in df:
            self.boardFeatureTable.append(np.array(x, dtype = 'int8'))
        self.winRateTable = []
        df = np.array(store['winRateTable'])
        for x in df:
            self.winRateTable.append(x)
        self.moveRateTable = []
        df = np.array(store['moveRateTable'])
        for x in df:
            self.moveRateTable.append(np.array(x))
        self.modelVersionTable = []
        df = np.array(store['modelVersionTable'])
        for x in df:
            self.modelVersionTable.append(x)
        store.close()

    def save(self):
        """
        save data to hdf5 file
        """
        store = pd.HDFStore(self.hdf5FileName)
        store.put('shape', pd.DataFrame(self.shape))
        store.put('boardFeatureTable', pd.DataFrame(self.boardFeatureTable))
        store.put('winRateTable', pd.DataFrame(self.winRateTable))
        store.put('moveRateTable', pd.DataFrame(self.moveRateTable))
        store.put('modelVersionTable', pd.DataFrame(self.modelVersionTable))
        store.close()

    def append(self, boardFeature, winRate, moveRate, modelVersion):
        """
        append a train case data to store
        """
        boardFeature = np.array(boardFeature).reshape(self.rowCount * self.colCount * self.channelCount)
        self.boardFeatureTable.append(boardFeature)
        self.winRateTable.append(winRate)
        moveRate = np.array(moveRate).reshape(self.rowCount * self.colCount)
        self.moveRateTable.append(moveRate)
        self.modelVersionTable.append(modelVersion)

    def size(self):
        return len(self.boardFeature)


class SelfPlayTranner(object):
    """
    通过self-play训练模型
    """

    def __init__(self, rowCount, colCount, nInRow, trainBatchCnt, gameCntPerBatch
        , hdf5FileName):
        """
        trainBatchCnt: 训练批次数
        gameCntPerBatch: 每个批次中， 需要完成self-play的棋局数
        """
        self.rowCount = rowCount
        self.colCount = colCount
        self.nInRow = nInRow
        self.trainBatchCnt = trainBatchCnt
        self.gameCntPerBatch = gameCntPerBatch
        self.maxRemainSize = 500 # 训练时， 保留最大的历史棋局数
        self.modelPath = "data/model.json"
        self.weightPath = "data/weight.hdf5"
        self.trainStore = TrainDataStore(hdf5FileName, self.rowCount, self.colCount)
        self.policyModel = GomokuModel(rowCount, colCount)
        if os.path.isfile(self.modelPath) and os.path.isfile(self.weightPath):
            self.policyModel.load_model(self.modelPath, self.weightPath)
        else:
            self.policyModel.build_model()

    def run(self):
        """
        启动训练
        """
        for i in range(0, self.trainBatchCnt):
            for j in range(0, self.gameCntPerBatch):
                self.run_one_self_play()
            if self.trainStore.size() >= self.trainModelMinSize:
                self.update_model()

    def run_one_self_play(self):
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
            (bestMoveX, moveRates) = alphaZeroEngine.search_moves(board)
            states.append(boardState)
            outProbs.append(moveRates)
            winnerZs.append(board.curPlayer)
            (r, c) = board.location_to_move(bestMoveX)
            board.play(r, c, board.curPlayer)
            print str(board)
            if board.check_game_over():
                break
        print "game over. winner : " + board.winColor
        if board.winColor == TIDE:
            winnerZs = np.zeros(len(winnerZs))
        else:
            for i in range(0, len(winnerZs)):
                if winnerZs[i] == board.winColor:
                    winnerZs[i] = 1.0
                else:
                    winnerZs[i] = -1.0
        self.append_train_data(states, outProbs, winnerZs)

    def append_train_data(self, states, outProbs, winnerZs):
        """
        将数据添加到训练数据池中
        """
        for i in range(0, len(states)):
            self.trainStore.append(states[i], winRate[i], outProbs[i], "1")

    def update_model(self):
        """
        更新策略模型
        """
        startIdx = 0
        if self.trainStore.size() > self.maxRemainSize:
            startIdx = self.trainStore.size() - self.maxRemainSize
        states = self.trainStore.boardFeatureTable[startIdx:]
        winRates = self.trainStore.winRateTable[startIdx:]
        outProbs = self.trainStore.moveRateTable[startIdx:]
        self.policyModel.fit(states, winRates, outProbs)
        self.policyModel.save_model(self.modelPath, self.weightPath)

if __name__ == '__main__':
    trainner = SelfPlayTranner(
        rowCount = 6
        , colCount = 6
        , nInRow = 4
        , trainBatchCnt = 1500
        , gameCntPerBatch = 1
        , hdf5FileName = "data/train_data.hdf5"
        )
    trainner.run()

