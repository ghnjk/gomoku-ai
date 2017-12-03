import pandas
import numpy as np
from keras.models import Sequential
from keras.layers.core import Dropout, Dense, Activation, Flatten
from keras.layers.convolutional import Convolution2D, MaxPooling2D, AveragePooling2D
from keras.layers.normalization import BatchNormalization
from keras.optimizers import Adam
from keras.utils import np_utils
from keras.models import model_from_json
import matplotlib  
import matplotlib.pyplot as plt  

figure_size = 28

def load_mnist_data(data_path):
    data = pandas.read_csv(data_path)
    x = data.values[:, 1:].astype("float32")
    x = x.reshape(x.shape[0], 1, figure_size, figure_size)
    #x = x / 255.0
    y = np_utils.to_categorical(data.ix[:, 0].values.astype("int8"))
    train_size = int(x.shape[0] * 0.7)
    test_size = x.shape[0] - train_size
    train_x = x[:train_size,:,:,:]
    train_y = y[:train_size,:]
    test_x = x[train_size:,:,:,:]
    test_y = y[train_size:,:]
    return (train_x, train_y, test_x, test_y)

def load_mnist_train_data(data_path):
    data = pandas.read_csv(data_path)
    x = data.values[:, 1:].astype("float32")
    x = x.reshape(x.shape[0], 1, figure_size, figure_size)
    #x = x / 255.0
    y = np_utils.to_categorical(data.ix[:, 0].values.astype("int8"))
    return (x, y)

def load_test_input_data(data_path):
    data = pandas.read_csv(data_path)
    x = data.values[:, :].astype("float32")
    x = x.reshape(x.shape[0], 1, figure_size, figure_size)
    return x

def show_figure(data):
    for y in range(0, data.shape[0]):
        for x in range(0, data.shape[1]):
            v = data[y][x]
            if v > 0:
                plt.plot(x, y, marker = "*")
    plt.show()

def save_model(model):
    # serialize model to JSON
    model_json = model.to_json()
    with open("model.json", "w") as json_file:
        json_file.write(model_json)
    # serialize weights to HDF5
    model.save_weights("model.h5")
    print("Saved model to disk")

def load_model():
    loaded_model_json = None
    with open("model.json", "r") as json_file:
        loaded_model_json = json_file.read()
    loaded_model = model_from_json(loaded_model_json)
    # load weights into new model
    loaded_model.load_weights("model.h5")
    print("Loaded model from disk")
    loaded_model.compile(loss='categorical_crossentropy', optimizer=Adam(), metrics=["accuracy"])
    return loaded_model

def build_model():
    model = Sequential()

    model.add(
        Convolution2D(16, (3, 3), input_shape=(1, figure_size, figure_size)
            , padding="same", data_format="channels_first", use_bias=True)
    )
    model.add(
        Convolution2D(16, (3, 3), input_shape=(1, figure_size, figure_size)
            , padding="same", data_format="channels_first", activation='relu')
    )
    model.add(BatchNormalization(axis=1))
    model.add(AveragePooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.1))

    model.add(
        Convolution2D(16, (3, 3), input_shape=(1, figure_size, figure_size)
            , padding="same", data_format="channels_first", use_bias=True)
    )
    model.add(
        Convolution2D(16, (3, 3), input_shape=(1, figure_size, figure_size)
            , padding="same", data_format="channels_first", activation='relu')
    )
    model.add(BatchNormalization(axis=1))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))

    model.add(Flatten())
    model.add(Dense(512, activation='relu'))
    model.add(Dropout(0.25))
    model.add(Dense(256, activation='relu'))
    model.add(Dropout(0.25))
    model.add(Dense(64, activation='relu'))
    model.add(Dense(10, activation='softmax'))

    model.compile(loss='categorical_crossentropy', optimizer=Adam(), metrics=["accuracy"])

    return model

if __name__ == '__main__':
    train_x, train_y= load_mnist_train_data('train.csv')
    test_x = load_test_input_data("test.csv")
    
    model = build_model()
    model.fit(train_x, train_y, epochs=64, batch_size=100, verbose=1)
    #loss, accu = model.evaluate(test_x,test_y, verbose = 1)
    #print("evaluate: loss is %f accu is %f"%(loss,accu))

    save_model(model)
    test_y = model.predict_classes(test_x, verbose = 1)

    pandas.DataFrame({"ImageId": range(1, len(test_y) + 1), "Label": test_y}).to_csv('./predict_result.csv', index=False, header=True)




