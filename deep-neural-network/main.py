import os
import sys
import argparse
import pandas as pd
import numpy as np

from chess.chess_board import *
CHANNEL_SIZE = 1

def db2csv(args):
    if args.input is None or len(args.input) == 0:
        print "input file is empty"
        return -1
    if args.x_file is None or len(args.x_file) == 0:
        print "x_file is empty"
        return -1
    if args.y_file is None or len(args.y_file) == 0:
        print "y_file is empty"
        return -1
    if args.evaluate_version is None:
        print "evaluate_version is needed"
        return -1
    if not os.path.isfile(args.input):
        print "input file %s is not a file." % (args.input)
        return -1
    from dnn import dnn_evalutor
    loader = dnn_evalutor.DbDataLoader()
    x, y = loader.load_data(args.input, str(args.evaluate_version))
    x = x.reshape(x.shape[0], x.shape[1] * x.shape[2] * x.shape[3])
    features = []
    for i in range(0, x.shape[1]):
        features.append(x[:, i].tolist())
    mp = {
        "chess": range(0, len(x))
    }
    for i in range(0, len(features)):
        mp["chessFeature_%04d" % (i)] = features[i]
    pd.DataFrame(mp).to_csv(args.x_file, index=False, header=True)
    pd.DataFrame({"chess": range(0, len(y)), "chessScore": y}).to_csv(args.y_file, index=False, header=True)

def train(args):
    if args.x_file is None or len(args.x_file) == 0:
        print "x_file is empty"
        return -1
    if args.y_file is None or len(args.y_file) == 0:
        print "y_file is empty"
        return -1
    if args.model_path is None or len(args.model_path) == 0:
        print "model_path is empty"
        return -1
    if not os.path.isfile(args.x_file):
        print "x_file is not a file"
        return -1
    if not os.path.isfile(args.y_file):
        print "y_file is not a file"
        return -1
    # build
    from dnn import dnn_evalutor
    evaluator = dnn_evalutor.DnnEvaluator()
    model = evaluator.build_model()
    # load
    data_x = pd.read_csv(args.x_file)
    data_y = pd.read_csv(args.y_file)
    x = data_x.values[:, 1:].astype("int8")
    y = data_y.values[:, 1:].astype("float32") / 10000.0
    x = x.reshape(x.shape[0], CHESS_BOARD_SIZE, CHESS_BOARD_SIZE, CHANNEL_SIZE)
    cnt = int(len(x) * 0.8)
    train_x = x[:cnt,:,:,:]
    train_y = y[:cnt,:]
    test_x = x[cnt:,:,:,:]
    test_y = y[cnt:,:]
    # fit
    model.fit(train_x, train_y, epochs=64, batch_size=32, verbose=1)
    evaluator.save_model(model, args.model_path)
    loss, accu = model.evaluate(test_x,test_y, verbose = 1)
    print("evaluate: loss is %f accu is %f"%(loss,accu))
    predict_y = model.predict_classes(test_x, verbose = 1)
    evaluator.calc_prediction_acc(test_y, predict_y)


if __name__ == '__main__':
    np.random.seed(1337)
    parser = argparse.ArgumentParser(description='dnn_evalutor')
    parser.add_argument("--cmd", type=str, help="commad name", choices=["db2csv", "train"])
    parser.add_argument("--input", type=str, help="input file path", default = None)
    parser.add_argument("--evaluate_version", type=int, help="chess db evaluateVersion", default = None)
    parser.add_argument("--x_file", type=str, help="x_file file path", default = None)
    parser.add_argument("--y_file", type=str, help="y_file file path", default = None)
    parser.add_argument("--model_path", type=str, help="model_path file path", default = None)
    args = parser.parse_args()
    if args.cmd == "db2csv":
        exit(db2csv(args))
    elif args.cmd == "train":
        exit(train(args))