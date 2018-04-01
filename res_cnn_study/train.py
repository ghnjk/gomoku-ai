#!/usr/bin/python
# -*- coding: UTF-8 -*- 
import os
from gomoku_chess import *
from gomoku_model import *
from mcts_alpha import *
import numpy as np
import pandas as pd  
from player import *

class TrainDataStore(object):
    """
    store and read trainning data
    """

    def __init__(self, hdf5FileName, rowCount = 15, colCount = 15, channelCount = 4):
        self.hdf5FileName = hdf5FileName
        self.rowCount = rowCount
        self.colCount = colCount
        self.channelCount = channelCount
        self.shape = [self.rowCount, self.colCount, self.channelCount]
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
        self.shape = [self.rowCount, self.colCount, self.channelCount]

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
        self.shape = np.array(store['shape'])
        self.rowCount = self.shape[0][0]
        self.colCount = self.shape[1][0]
        self.channelCount = self.shape[2][0]
        self.boardFeatureTable = []
        df = np.array(store['boardFeatureTable'])
        for x in df:
            boardFeature = np.array(x, dtype = 'int8').reshape(self.rowCount, self.colCount, self.channelCount)
            self.boardFeatureTable.append(boardFeature)
        self.winRateTable = []
        df = np.array(store['winRateTable'])
        for x in df:
            self.winRateTable.append(x[0])
        self.moveRateTable = []
        df = np.array(store['moveRateTable'])
        for x in df:
            self.moveRateTable.append(np.array(x))
        self.modelVersionTable = []
        df = np.array(store['modelVersionTable'])
        for x in df:
            self.modelVersionTable.append(x[0])
        store.close()

    def save(self):
        """
        save data to hdf5 file
        """
        store = pd.HDFStore(self.hdf5FileName)
        store.put('shape', pd.DataFrame(self.shape))
        cnt = len(self.boardFeatureTable)
        store.put('boardFeatureTable', pd.DataFrame(
            np.array(self.boardFeatureTable).reshape(cnt, self.rowCount * self.colCount * self.channelCount).tolist()
        ))
        store.put('winRateTable', pd.DataFrame(self.winRateTable))
        store.put('moveRateTable', pd.DataFrame(self.moveRateTable))
        store.put('modelVersionTable', pd.DataFrame(self.modelVersionTable))
        store.close()

    def append(self, boardFeature, winRate, moveRate, modelVersion):
        """
        append a train case data to store
        """
        self.boardFeatureTable.append(boardFeature)
        self.winRateTable.append(winRate)
        moveRate = np.array(moveRate).reshape(self.rowCount * self.colCount)
        self.moveRateTable.append(moveRate)
        self.modelVersionTable.append(modelVersion)

    def size(self):
        return len(self.boardFeatureTable)


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
        self.modelUpdateCnt = 0
        self.rowCount = rowCount
        self.colCount = colCount
        self.nInRow = nInRow
        self.trainBatchCnt = trainBatchCnt
        self.gameCntPerBatch = gameCntPerBatch
        self.maxRemainSize = 4096 # 训练时， 保留最大的历史棋局数
        self.modelPath = "data/model.json"
        self.weightPath = "data/weight.hdf5"
        self.trainStore = TrainDataStore(hdf5FileName, self.rowCount, self.colCount)
        if os.path.isfile(hdf5FileName):
            self.trainStore.load()
        self.policyModel = GomokuModel(rowCount, colCount)
        self.epochs = 5
        self.trainModelMinSize = 512
        self.estimateInterval = 10
        if os.path.isfile(self.modelPath) and os.path.isfile(self.weightPath):
            self.policyModel.load_model(self.modelPath, self.weightPath)
        else:
            self.policyModel.build_model()
        self.estimateHist = []

    def run(self):
        """
        启动训练
        """
        for i in range(0, self.trainBatchCnt):
            if(i % self.estimateInterval == 0):
                self.estimate()
            print "train index %d" % (i)
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
            , mctsPlayout = 200
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
        print "game over. winner : " + board.playerName[board.winColor]
        if board.winColor == TIDE:
            winnerZs = np.zeros(len(winnerZs))
        else:
            for i in range(0, len(winnerZs)):
                if winnerZs[i] == board.winColor:
                    winnerZs[i] = 1.0
                else:
                    winnerZs[i] = -1.0
                # print "winnerZs[i]: " + str(winnerZs[i])
                # print "outProbs: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.3lf " % (outProbs[i][r * board.rowCount + c])
                #     print s
                # print "**** self chess: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.0f " % (states[i][r][c][0])
                #     print s
                # print "**** optcal chess: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.0f " % (states[i][r][c][1])
                #     print s
                # print "**** last chess: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.0f " % (states[i][r][c][2])
                #     print s
                # print "**** is first: "
                # for r in range(0, board.rowCount):
                #     s = ""
                #     for c in range(0, board.colCount):
                #         s += "%1.0f " % (states[i][r][c][3])
                #     print s
        self.append_train_data(states, outProbs, winnerZs)

    def append_train_data(self, states, outProbs, winnerZs):
        """
        将数据添加到训练数据池中
        """
        for i in range(0, len(states)):
            self.trainStore.append(states[i], winnerZs[i], outProbs[i], self.modelUpdateCnt)

    def update_model(self):
        """
        更新策略模型
        """
        startIdx = 0
        if self.trainStore.size() > self.maxRemainSize:
            startIdx = self.trainStore.size() - self.maxRemainSize
        cnt = self.trainStore.size() - startIdx
        states = np.array(self.trainStore.boardFeatureTable[startIdx:])
        winRates = np.array(self.trainStore.winRateTable[startIdx:])
        outProbs = np.array(self.trainStore.moveRateTable[startIdx:])
        self.policyModel.fit(states, winRates, outProbs, epochs = self.epochs, batchSize = cnt)
        self.policyModel.save_model(self.modelPath, self.weightPath)
        self.modelUpdateCnt += 1

    def estimate(self):
        """
        评估训练结果
        """
        self.trainStore.save()
        print "start estimate: "
        winCnt = 0
        tideCnt = 0
        competeCnt = 10
        for i in range(competeCnt):
            z = self.estimate_once()
            print "[%d]: %d" % (i, z)
            if z > 0:
                winCnt += 1
            if z == 0:
                tideCnt += 1
        print "winRate: %0.1lf tideRate: %0.1lf" % (winCnt / float(competeCnt), tideCnt / float(competeCnt) )
        self.estimateHist.append(winCnt / float(competeCnt) )
        print str(self.estimateHist)
        if winCnt == competeCnt:
            exit(0)


    def estimate_once(self):
        """
        评估训练结果
        """
        board = GomokuBoard(self.rowCount, self.colCount, self.nInRow)
        pureMctsPlayer = PureMctsPlayer(2000)
        alphaPlayer = AlphaZeroPlayer(200)
        alphaPlayer.init_player(board, 0, self.policyModel)
        pureMctsPlayer.init_player(board, 1)
        players = [alphaPlayer, pureMctsPlayer]
        while True:
            board.check_game_over()
            if board.isGameOver:
                if board.winColor == TIDE:
                    return 0
                elif board.winColor == alphaPlayer.playerColor:
                    return 1
                else:
                    return -1
            player = players[board.curPlayer]
            (r, c) = player.gen_next_move(board)
            board.play(r, c, board.curPlayer)
        


if __name__ == '__main__':
    trainner = SelfPlayTranner(
        rowCount = 6
        , colCount = 6
        , nInRow = 4
        , trainBatchCnt = 1000
        , gameCntPerBatch = 16
        , hdf5FileName = "data/train_data.hdf5"
        )
    trainner.run()

