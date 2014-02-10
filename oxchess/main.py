#!/usr/bin/env python3

chesses = [0] * 9
chess_matrix = [' ', 'o', 'x']
input_matrix = [0, 6, 7, 8, 3, 4, 5, 0, 1, 2]

def draw_chessboard(chesses):
    chessboard = []
    for i in range(3):
        chessboard += [chess_matrix[chesses[i*3]], '|', chess_matrix[chesses[i*3+1]], '|', chess_matrix[chesses[i*3+2]]]
        chessboard += ['-'] * 5
    chessboard = chessboard[:-5]

    for i in range(25):
        print(chessboard[i], end = '')
        if (i + 1) % 5 == 0:
            print()

def get_input(player):
    _input = int(input())
    if player == 1:
        chesses[input_matrix[_input]] = 1
    elif player == 2:
        chesses[input_matrix[_input]] = 2
    else:
        raise


def test():
    draw_chessboard(chesses)
    while True:
        get_input(1)
        draw_chessboard(chesses)
        get_input(2)
        draw_chessboard(chesses)



test()
