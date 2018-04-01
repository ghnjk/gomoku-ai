#!/usr/bin/python
# -*- coding: UTF-8 -*- 
from gomoku_chess import *

from keras.models import Model
from keras.layers import Input, Dense, Conv2D, BatchNormalization, Activation, Flatten
from keras.optimizers import Adam
from res_cnn import *
import numpy as np
from keras.models import model_from_json



def board_to_state(board):
    """
    convert chess board to input matrix
    return 
            rowCount, colCount, 4
            第一维： 本方棋子，
            第二维： 对方棋子
            第三维： 上一个棋子的下子位置
            第四维： 当前颜色是否是先手玩家
    """
    rowCount = board.rowCount
    colCount = board.colCount
    channelCount = 4
    res = np.zeros((rowCount, colCount, channelCount - 1), dtype = 'int8')
    for (x, color) in board.moveHistory:
        (r, c) = board.location_to_move(x)
        if color == board.curPlayer:
            res[r][c][0] = 1
        else:
            res[r][c][1] = 1
    if len(board.moveHistory) > 0:
        (x, color) = board.moveHistory[-1]
        (r, c) = board.location_to_move(x)
        res[r][c][2] = 1
    if board.curPlayer == 0:
        a = np.ones((rowCount, colCount, 1))
    else:
        a = np.zeros((rowCount, colCount, 1))
    res = np.concatenate( (res, a), axis = 2)
    return res

class GomokuModel(object):
    """
    用于训练五子棋的模型
    """
    def __init__(self, rowCount = 15, colCount = 15):
        self.rowCount = rowCount
        self.colCount = colCount
        self.channelCount = 4
        self.model = None

    def build_model(self):
        """
        build trainning model
        生成的训练模型如下：
        输入模型为：
            rowCount, colCount, 4
            第一维： 本方棋子，
            第二维： 对方棋子
            第三维： 上一个棋子的下子位置
            第四维： 当前颜色是否是先手玩家
        输出模型为：
            胜率，各棋子的下子概率
        """
        input = Input((self.rowCount, self.colCount, self.channelCount))
        resCnn = ResCnn(filterCnt = 64, resLayerCnt = 3)
        x = resCnn.build_model(self.rowCount, self.colCount, self.channelCount)(input)
        # 输出下子概率分布图
        moveRate = Conv2D(filters = 2
            , kernel_size = (1,1)
            , padding = 'same', data_format = DATA_FORMAT
            , name = 'MoveRate.conv')(x)
        moveRate = BatchNormalization(axis = BATCH_NORMAL_AXIS
            , name = 'MoveRate.bn')(moveRate)
        moveRate = Activation('relu')(moveRate)
        moveRate = Flatten()(moveRate)
        moveRate = Dense(self.rowCount * self.colCount
            , activation ='softmax'
            , name = 'MoveRate.gen')(moveRate)
        # 输出胜率
        winRate = Conv2D(filters = 1
            , kernel_size = (1,1)
            , padding = 'same', data_format = DATA_FORMAT
            , name = 'WinRate.conv')(x)
        winRate = BatchNormalization(axis = BATCH_NORMAL_AXIS
            , name = 'WinRate.bn')(winRate)
        winRate = Activation('relu')(winRate)
        winRate = Flatten()(winRate)
        winRate = Dense(256
            , activation = 'relu'
            , name = 'WinRate.dense')(winRate)
        winRate = Dense(1
            , activation = 'tanh'
            , name = 'WinRate.gen')(winRate)
        # 编译模型
        model = Model(input, [winRate, moveRate])
        model.compile(optimizer = Adam(lr=2e-2)
            , loss = ['mean_squared_error', 'categorical_crossentropy']
            )
        model.summary()
        self.model = model
        return model

    def save_model(self, modelPath, weightPath):
        """
        保存模型和训练权重
        """
        # serialize model to JSON
        modelJson = self.model.to_json()
        with open(modelPath, "w") as fp:
            fp.write(modelJson)
        # serialize weights to HDF5
        self.model.save_weights(weightPath)

    def gen_policy(self, board, showDetail = 0):
        """
        评估当前棋局
        返回当前棋子胜率和所有的走法概率
        return (winRate, [(moveX, prioP)])
        """
        state = board_to_state(board)
        (winRate, moveRate) = self.model.predict(np.array([state]), verbose = showDetail)
        winRate = winRate[0]
        moveRate = moveRate[0]
        moveProbs = []
        for x in board.availables:
            moveProbs.append((x, moveRate[x]))
        return (winRate, moveProbs)

    def fit(self, state, winRate, moveRate, epochs = 10, batchSize = 32):
        """
        训练模型
        """
        self.model.fit(state, [winRate, moveRate], epochs = epochs, batch_size = batchSize, verbose=1)


    def load_model(self, modelPath, weightPath):
        """
        从文件中加载模型和权重
        """
        with open(modelPath, "r") as fp:
            modelJson = fp.read()
        model = model_from_json(modelJson)
        model.compile(optimizer = Adam(lr=2e-2)
            , loss = ['mean_squared_error', 'categorical_crossentropy']
            )
        model.summary()
        model.load_weights(weightPath)
        self.model = model
        return model
