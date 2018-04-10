# model_v1

## train result

mse: 0.0122

## data

将db记录转成输入和输出
(
    x // 棋局信息 [15][15][1]
        １　当前颜色棋子
        ２　对方颜色棋子
        ３　空白
    , y // 当前局面的胜率，针对上一个棋子的颜色
)

## model

```
def build_model(self):
    model = Sequential()
    model.add(
        ZeroPadding2D(padding = (1, 1)
            , input_shape = (CHESS_BOARD_SIZE, CHESS_BOARD_SIZE, CHANNEL_SIZE), data_format=DATA_FORMAT)
    )
    model.add(
        Convolution2D(32, (3, 3), padding="same", data_format=DATA_FORMAT
            , activation="tanh")
    )
    model.add(
        Convolution2D(32, (3, 3), padding="same", data_format=DATA_FORMAT
            , activation="tanh")
    )
    model.add(MaxPooling2D(pool_size=(2, 2), padding="same"))
    model.add(Dropout(0.1))

    model.add(
        Convolution2D(64, (3, 3), padding="same", data_format=DATA_FORMAT
            , activation="tanh")
    )
    model.add(
        Convolution2D(64, (3, 3), padding="same", data_format=DATA_FORMAT
            , activation="tanh")
    )
    model.add(MaxPooling2D(pool_size=(2, 2), padding="same"))
    model.add(Dropout(0.2))

    model.add(Flatten())
    model.add(Dense(512, activation='tanh'))
    model.add(Dropout(0.2))
    model.add(Dense(256, activation='tanh'))
    model.add(Dense(128, activation='tanh'))
    model.add(Dropout(0.2))
    model.add(Dense(64, activation='tanh'))
    model.add(Dense(1, activation='sigmoid'))

    model.compile(loss='mean_squared_error', optimizer="adadelta"
        , metrics=["mse"]
    )

    return model
```