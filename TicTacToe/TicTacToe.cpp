// TicTacToe.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>

std::string human = "O";
std::string robot = "X";

//std::vector<std::string> orignalBoard = {"O", "-", "X", "X", "-", "X", "-", "O", "O"};
//std::vector<std::string> orignalBoard = { "X", "O", "X", "O", "O", "X", "-", "-", "-" };
std::vector<std::string> orignalBoard = { "-", "-", "-", "-", "-", "-", "-", "-", "-" };

bool winning(std::vector<std::string> board, std::string player) {
    if (
        (board[0] == player && board[1] == player && board[2] == player) ||
        (board[3] == player && board[4] == player && board[5] == player) ||
        (board[6] == player && board[7] == player && board[8] == player) ||
        (board[0] == player && board[3] == player && board[6] == player) ||
        (board[1] == player && board[4] == player && board[7] == player) ||
        (board[2] == player && board[5] == player && board[8] == player) ||
        (board[0] == player && board[4] == player && board[8] == player) ||
        (board[2] == player && board[4] == player && board[6] == player)
        ) {
        return true;
    }
    else {
        return false;
    }
}

void calculateAvailableSlot(std::vector<std::string> &board, std::vector<int> &available_slots) {
    int idx = 0;
    for (auto& a : board) {
        if (a == "-") {
            available_slots.push_back(idx);
        }
        idx++;
    }
}

int minimax(std::vector<std::string> node, int depth, int alpha, int beta, bool maximixingPlayer) {
    std::vector<int> available_slots;
    calculateAvailableSlot(node, available_slots);

    // Check for terminating condition
    if (winning(node, human)) {
        return -1;
    }
    else if (winning(node, robot)) {
        return 1;
    }
    else if (depth == 0 || available_slots.empty()) {
        return 0;
    }

    int value;
    if (maximixingPlayer) {
        value = INT_MIN;
        for (auto& slot : available_slots) {
            node[slot] = robot;
            int result = minimax(node, depth - 1, alpha, beta, false);
            value = std::max(result, value);
            alpha = std::max(alpha, value);
            if (alpha >= beta) {
                break;
            }
            node[slot] = "-";
        }
    }
    else {
        value = INT_MAX;
        for (auto& slot : available_slots) {
            node[slot] = human;
            int result = minimax(node, depth - 1, alpha, beta, true);
            value = std::min(result, value);
            beta = std::min(beta, value);
            if (alpha >= beta) {
                break;
            }
            node[slot] = "-";
        }
    }
    return value;
}

int calculateNextMove(std::vector<std::string> &board) {
    std::cout << "Calculating next move..." << std::endl;
    
    std::vector<int> available_slots;
    calculateAvailableSlot(board, available_slots);

    int bestVal = INT_MIN, slotMove = 0;
    int alpha = INT_MIN, beta = INT_MAX;
    for (auto& slot : available_slots) {
        board[slot] = robot;
        int result = minimax(board, (int)available_slots.size(), alpha, beta, false);
        if (result > bestVal) {
            bestVal = result;
            slotMove = slot;
        }
        board[slot] = "-";
    }
    return slotMove;
}

bool evaluateWinnerAndPrintBoard(std::vector<std::string> &board) {
    int c = 1;
    for (auto& a : board) {
        std::cout << a;
        if (c % 3 == 0) {
            std::cout << std::endl;
        }
        else {
            std::cout << " ";
        }
        c++;
    }

    std::vector<int> available_slots;
    calculateAvailableSlot(board, available_slots);

    bool restart = false;
    if (winning(board, human)) {
        std::cout << "Human win ... " << std::endl;
        restart = true;
    }
    else if (winning(board, robot)) {
        std::cout << "Computer win ... " << std::endl;
        restart = true;
    }
    else if(available_slots.empty()) {
        std::cout << "Draw ... " << std::endl;
        restart = true;
    }

    return restart;
}

int main()
{
    bool restart_game = false;
    std::cout << "Starting game..." << std::endl;
    std::cout << "computer: " << robot << " human: " << human << std::endl;
    while (true) {
        int turn;
        if (!restart_game) {
            restart_game = evaluateWinnerAndPrintBoard(orignalBoard);
        }
        if (restart_game) {
            std::cout << "Restarting game..." << std::endl;
            for (auto& a : orignalBoard) {
                a = "-";
            }
            evaluateWinnerAndPrintBoard(orignalBoard);
            restart_game = false;
        }
        std::vector<int> available_slots;
        calculateAvailableSlot(orignalBoard, available_slots);
        // No one has won, it is last slot and human turn so fill slot automatically
        if (available_slots.size() == 1) {
            turn = available_slots[0] + 1;
            std::cout << "Enter your turn (-1 to exit): " << turn << std::endl;
        }
        else {
            std::cout << "Enter your turn (-1 to exit): ";
            std::cin >> turn;
        }
        if (turn == -1) {
            exit(0);
        }
        else if (turn < 1 || turn > 10 || orignalBoard[turn-1] != "-") {
            std::cout << "Wrong input..." << std::endl;
        }
        else {
            orignalBoard[turn - 1] = human;
            restart_game = evaluateWinnerAndPrintBoard(orignalBoard);
            if (!restart_game) {
                int slotMove = calculateNextMove(orignalBoard);
                std::cout << "Computer move: " << slotMove + 1 << std::endl;
                orignalBoard[slotMove] = robot;
            }
        }
    }
}