#!/usr/bin/python
# -*- coding: UTF-8 -*- 

from keras.model import Model
from keras.layers import Input, Output, Dense, Conv2D, BatchNormalization, Activation
from keras.layers.merge import add

# 采用（sample， row， col， channel）模式输入数据
DATA_FORMAT = "channels_last"
# 需要归一化的轴下标，和DATA_FORMAT相匹配
BATCH_NORMAL_AXIS = 3

class ResCnn(object):
    """
    Residual_CNN model
    """
    def __init__(self, filterCnt = 256, resLayerCnt = 19
        , kernelHeight = 3, KernelWidth = 3):
        self.filterCnt = filterCnt
        self.resLayerCnt = resLayerCnt
        self.kernelHeight = kernelHeight
        self.KernelWidth = KernelWidth

    def build_model(self, height, width, channelCount):
        """
        build res cnn net
        """
        input = Input(shape=(height, width, channelCount))
        x = self.add_convolution_layer(input, 'ResCnn.headConv2D', 'ResCnn.BnHead')
        for i in range(0, self.resLayerCnt):
            x = self.add_res_layer(i, x)
        return Model(inputs = input, outputs = x)

    def add_res_layer(self, layerIndex, inputTensor):
        """
        add a res layer after the input tensor
        """
        x = inputTensor
        cnName = 'ResCnn.layer_' + str(layerIndex + 1) + '_cv'
        bnName = 'ResCnn.layer_' + str(layerIndex + 1) + '_bn'
        x = self.add_convolution_layer(x, cnName + '_a', bnName + '_a')
        x = self.add_convolution_layer(x, cnName + '_b', bnName + '_b')
        return add([x, inputTensor])

    def add_convolution_layer(self, inputTensor, conv2DName, bnName):
        """
        add a conv2D BatchNormalize relu to inputTensor
        """
        x = inputTensor
        x = Conv2D(filters = self.filterCnt
            , kernel_size = (self.kernelHeight, self.KernelWidth)
            , padding = 'same', data_format = DATA_FORMAT
            , name = conv2DName)(x)
        x = BatchNormalization(axis = BATCH_NORMAL_AXIS
            , name = bnName)(x)
        x = Activation('relu')(x)
        return x


