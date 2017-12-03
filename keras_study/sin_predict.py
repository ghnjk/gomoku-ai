import numpy as np
from keras.models import Sequential
from keras.layers import Dense, Activation, TimeDistributed
import matplotlib
import matplotlib.pyplot as plt

def test_sin_prediction():
    train_x = np.sort(np.random.random(size=10000) * 100)
    train_y = (np.sin(train_x))
    test_x = np.sort(np.random.random(size=2000) * 30)
    test_y = (np.sin(test_x))
    model = Sequential()

    model.add(Dense(64, input_dim=1))
    model.add(Activation("tanh"))
    model.add(Dense(32, activation = "relu"))
    model.add(Dense(1, activation = "tanh"))

    model.compile(loss="msle", optimizer="adam", metrics=["mae"])
    model.fit(train_x, train_y, epochs=1000, batch_size=500, verbose=2)
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
