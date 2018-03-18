#!/usr/bin/python
# -*- coding: UTF-8 -*- 

from keras.model import Model
from keras.layers import Input, Output, Dense, Conv2D, BatchNormalization, Activation, Flatten
from keras.optimizers import Adam
from res_cnn import *
from keras.models import model_from_json

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
        input = Input(self.rowCount, self.colCount, self.channelCount)
        resCnn = ResCnn()
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
            , activation =' softmax'
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
            , activation = 'sigmoid'
            , name = 'WinRate.gen')(winRate)
        # 编译模型
        model = Model(input, [winRate, moveRate])
        model.compile(optimizer = Adam(lr=2e-2)
            , loss = ['categorical_crossentropy', 'binary_crossentropy']
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
        model.save_weights(weightPath)

    def load_model(self, modelPath, weightPath):
        """
        从文件中加载模型和权重
        """
        with open(modelPath, "r") as fp:
            modelJson = fp.read()
        model = model_from_json(modelJson)
        model.load_weights(weightPath)
        self.model = model
        return model