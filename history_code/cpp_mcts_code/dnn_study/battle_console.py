import os
import json
import fileinput
from chess.sgf import sgf_to_board
from chess.chess_board import *
from dnn import dnn_evalutor
import sys

def play_chess(model, chess, color):
    """
    gen next step of color
    """
    arr_x = []
    arr_res = []
    chess.next_color = color
    recorder = dnn_evalutor.DbDataLoader()
    print >> sys.stderr, str(chess)
    for i in range(0, CHESS_BOARD_SIZE):
        for j in range(0, CHESS_BOARD_SIZE):
            if chess.board[i][j] != COLOR_BLANK:
                continue
            chess.board[i][j] = color
            (x, y) = recorder.transfer_db_record(chess, {})
            arr_res.append( (i,j) )
            arr_x.append(x)
            chess.board[i][j] = COLOR_BLANK
    arr_y = model.predict(arr_x, verbose = 0)
    # print "arr_y: "
    # print arr_y
    # print "arr_res:"
    # print arr_res
    res = None
    resValue = 0
    for i in range(0, len(arr_y)):
        y = arr_y[i]
        print >> sys.stderr, str(arr_res[i]) + " evalued: " + str(1 - y)
        if res is None or resValue > y:
            resValue = y
            res = arr_res[i]
    # print "resValue: "
    # print resValue
    # print "res:"
    # print res
    # print ""
    print >> sys.stderr, "play_chess " + str(res) + " evaluted: " + str(1 - resValue)
    return res
    
def main():
    """
    gomoku battle entrance
    """
    evaluator = dnn_evalutor.DnnEvaluator()
    model = evaluator.load_model("data/model_v1/model.json", "data/model_v1/model.h5")
    while True:
        try:
            line = raw_input()
        except:
            break
        line = line.strip()
        if len(line) == 0:
            continue
        print >> sys.stderr, "receive : " + line
        req = json.loads(line)
        resp = {}
        command = req.get("command", "")
        if command == "NEXT_BLACK":
            #print "NEXT_BLACK"
            board = sgf_to_board(req.get("chessboard", ""))
            (row, col) = play_chess(model, board, COLOR_BLACK)
            resp["rowIndex"] = row
            resp["columnIndex"] = col
        elif command == "NEXT_WHITE":
            #print "NEXT_WHITE"
            board = sgf_to_board(req.get("chessboard", ""))
            (row, col) = play_chess(model, board, COLOR_WHITE)
            resp["rowIndex"] = row
            resp["columnIndex"] = col
        else:
            resp["error"] = "invalid line: " + line
        print json.dumps(resp)
        print >> sys.stderr,json.dumps(resp)
        sys.stdout.flush()
        sys.stderr.flush()
if __name__ == '__main__':
    main()