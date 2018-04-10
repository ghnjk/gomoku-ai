import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Activation, TimeDistributed, Conv1D,AveragePooling1D, Reshape, Flatten
import matplotlib
import matplotlib.pyplot as plt

def test_sin_prediction():
    train_x = np.sort(np.random.random(size=10000) * 20)
    train_y = (np.sin(train_x))
    test_x = np.array(range(0, 4000)) / 100.0
    test_y = (np.sin(test_x))
    model = Sequential()

    model.add(Dense(64, input_dim=1))
    model.add(Activation("tanh"))
    model.add(Reshape((8,8)))
    model.add(Conv1D(64, 3, border_mode='same'))
    model.add(Activation("tanh"))
    model.add(Conv1D(32, 3, border_mode='same'))
    model.add(Flatten())
    model.add(Dense(16, activation = "tanh"))
    model.add(Activation("sigmoid"))
    model.add(Dense(1, activation = "tanh"))

    model.compile(loss="mse", optimizer="adam", metrics=["mae"])
    model.fit(train_x, train_y, epochs=4000, batch_size=50, verbose=2)
    loss, accu = model.evaluate(test_x,test_y, verbose = 2)
    print("loss is %f accu is %f"%(loss,accu))

    predictions = model.predict(test_x, verbose = 2)
    plt.plot(test_x, test_y)
    plt.plot(test_x, predictions)
    plt.show()

def matplotlib_test():
    train_x = np.sort(np.random.random(100000) * 10)
    train_y = np.sin(train_x)
    (train_x, train_y) = [train_x, train_y]
    plt.plot(train_x, train_y)
    plt.show()


if __name__ == '__main__':
    np.random.seed(7)
    test_sin_prediction()
