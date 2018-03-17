import os
import json
import fileinput
from chess.sgf import sgf_to_board
from dnn import dnn_evalutor

def play_chess(model, chess, color):
    """
    gen next step of color
    """
    arr_x = []
    arr_res = []
    recorder = dnn_evalutor.DbDataLoader()
    for i in range(0, CHESS_BOARD_SIZE):
        for j in range(0, CHESS_BOARD_SIZE):
            if chess.board[i][j] != COLOR_BLANK:
                continue
            chess.board[i][j] = color
            (x, y) = recorder.transfer_db_record(chess.board, {})
            arr_res.append( (x, y) )
            arr_x.append(x)
    arr_y = model.predict_classes(arr_x, verbose = 1)
    res = None
    resValue = 0
    for i in range(0, len(arr_y)):
        y = arr_y[i]
        if res is None or resValue < y:
            resValue = y
            res = arr_res[i]
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
        req = json.loads(line)
        resp = {}
        command = req.get("command", "")
        if command == "NEXT_BLACK":
            print "NEXT_BLACK"
            board = sgf_to_board(req.get("chessboard", ""))
            (row, col) = play_chess(model, board, COLOR_BLACK)
            resp["rowIndex"] = row
            resp["columnIndex"] = col
        elif command == "NEXT_WHITE":
            print "NEXT_WHITE"
            board = sgf_to_board(req.get("chessboard", ""))
            (row, col) = play_chess(model, board, COLOR_WHITE)
            resp["rowIndex"] = row
            resp["columnIndex"] = col
        else:
            resp["error"] = "invalid line: " + line
        print json.dumps(resp)
if __name__ == '__main__':
    main()