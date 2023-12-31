﻿#include <iostream>
#include <vector>
#include <algorithm>

class TicTacToe {
    const char X;
    const char O;

    std::vector<char> field;

    enum class Turn { X, O, PLAYER, COMPUTER }; // для режима PvP: Turn::X и Turn::O - ход игрока Х и ход игрока О соответственно,
                                                // для режима PvЕ: Turn::Player и Turn::Computer - ход игрока и ход компьютера соответсвенно
    enum class Winner { NO_WINNER, X, O, DRAW };

    Turn turn;
    Winner winner;

public:
    TicTacToe() : X('X'), O('O'), turn(Turn::X), winner(Winner::NO_WINNER) { field.insert(field.end(), 9, ' '); }
    void start();

private:
    void showRulesPvP();
    void showRulesPvE();

    void PvP();
    void Xturn();
    void Oturn();
    
    void PvE();
    void computerTurn(char computerSymbol, char playerSymbol);
    int computerTurnExtra(char computerSymbol, char playerSymbol);
    void playerTurn(char playerSymbol);

    int checkEnter();
    bool checkFree(int choice);
    Winner checkWinner(char symbol);
    void announceWinner();
    void display();
};

void TicTacToe::start() {
    std::cout << "Игра \"Крестики-нолики\".\n\n";
    std::cout << "Выберите режим игры: \n\n";
    std::cout << "1. Игрок против игрока\n";
    std::cout << "2. Игрок против компьютера\n\n";

    int choiceOpp;
    std::cin >> choiceOpp;

    while (choiceOpp != 1 && choiceOpp != 2) {
        std::cout << "\nВведите цифру 1 или 2!\n\n";
        std::cin >> choiceOpp;
    }

    if (choiceOpp == 1) {
        showRulesPvP();
        PvP();
    }
    else {
        showRulesPvE();
        PvE();
    }
}

void TicTacToe::showRulesPvP() {
    std::cout << "\nИгроки ходят по очереди. Первыми начинают крестики\n";
    std::cout << "Для того, чтобы сделать ход, нужно указать клетку с помощью цифры от 1 до 9: \n\n";
    std::cout << " 1 | 2 | 3 \n";
    std::cout << " 4 | 5 | 6 \n";
    std::cout << " 7 | 8 | 9 \n\n";
    std::cout << "Для начала игры выберите клетку. Удачи!\n\n";
}

void TicTacToe::showRulesPvE() {
    std::cout << "\nИгрок и компьютер ходят по очереди. Первыми начинают крестики\n";
    std::cout << "Для того, чтобы сделать ход, нужно указать клетку с помощью цифры от 1 до 9: \n\n";
    std::cout << " 1 | 2 | 3 \n";
    std::cout << " 4 | 5 | 6 \n";
    std::cout << " 7 | 8 | 9 \n\n";
}

void TicTacToe::PvP() {
    while (true) {
        if (turn == Turn::X) {
            Xturn();
            winner = checkWinner(X);
            if (winner != Winner::NO_WINNER) {
                announceWinner();
                break;
            }
            turn = Turn::O;
        }
        if (turn == Turn::O) {
            Oturn();
            winner = checkWinner(O);
            if (winner != Winner::NO_WINNER) {
                announceWinner();
                break;
            }
            turn = Turn::X;
        }
    }
}

void TicTacToe::Xturn() {
    int choice = checkEnter();
    field[choice - 1] = X;
    display();
}

void TicTacToe::Oturn() {
    int choice = checkEnter();
    field[choice - 1] = O;
    display();
}

void TicTacToe::PvE() {
    char playerSymbol;
    std::cout << "\nВыберите символ (X или O): \n";
    std::cin >> playerSymbol;

    while (playerSymbol != X && playerSymbol != O) {
        std::cout << "\nВводите только X или O\n";
        std::cin >> playerSymbol;
    }

    char computerSymbol;
    if (playerSymbol == X) {
        computerSymbol = O;
        turn = Turn::PLAYER;
        std::cout << "\nДля начала игры выберите клетку. Удачи!\n\n";
    }
    else {
        computerSymbol = X;
        turn = Turn::COMPUTER;
    }

    while (true) {
        if (turn == Turn::PLAYER) {
            playerTurn(playerSymbol);
            winner = checkWinner(playerSymbol);
            if (winner != Winner::NO_WINNER) {
                announceWinner();
                break;
            }
            turn = Turn::COMPUTER;
        }
        if (turn == Turn::COMPUTER) {
            computerTurn(computerSymbol, playerSymbol);
            winner = checkWinner(computerSymbol);
            if (winner != Winner::NO_WINNER) {
                announceWinner();
                break;
            }
            turn = Turn::PLAYER;
        }
    }
}

void TicTacToe::computerTurn(char computerSymbol, char playerSymbol) {
    int choice = computerTurnExtra(computerSymbol, playerSymbol); // первым делом проверяется, может ли победить компьютер следующим ходом или проиграть

    if (choice != 0) field[choice - 1] = computerSymbol;
    else { // если ни одно из условий не выполняется, то компьютер ходит на свободную клетку в определенном порядке
        std::vector<int> order{ 5,1,3,7,9,2,4,6,8 };

        for (auto& i : order) {
            if (checkFree(i) == true) {
                choice = i;
                field[choice - 1] = computerSymbol;
                break;
            }
        }
    }

    std::cout << "\nКомпьютер ходит на клетку " << choice << "\n\n";
    display();
}

int TicTacToe::computerTurnExtra(char computerSymbol, char playerSymbol) {
    for (int i = 0; i < field.size(); i++) { // если компьютер может победить следующим ходом, он ставит символ так, чтобы победить
        if (checkFree(i + 1) == true) {
            field[i] = computerSymbol;
            if (checkWinner(computerSymbol) != Winner::NO_WINNER) {
                return ++i;
            }
            else field[i] = ' ';
        }
    }
    // если компьютер не может победить следующим ходом, то проверяется, может ли он проиграть следующим ходом
    for (int i = 0; i < field.size(); i++) {
        if (checkFree(i + 1) == true) {
            field[i] = playerSymbol;
            if (checkWinner(playerSymbol) != Winner::NO_WINNER) {
                return ++i;
            }
            else field[i] = ' ';
        }
    }
    return 0;
}

void TicTacToe::playerTurn(char playerSymbol) {
    int choice = checkEnter();
    field[choice - 1] = playerSymbol;
    display();
}

int TicTacToe::checkEnter() {
    int choice;
    std::cin >> choice;

    while ((choice < 1 || choice > 9) || checkFree(choice) != true) {
        if (choice < 1 || choice > 9) {
            std::cout << "\nВводите числа только от 1 до 9!\n\n";
        }
        else if (checkFree(choice) == false) {
            std::cout << "\nДанная клетка уже занята!\n\n";
        }
        std::cin >> choice;
    }

    return choice;
}

bool TicTacToe::checkFree(int choice) { return field[choice - 1] == ' '; }

TicTacToe::Winner TicTacToe::checkWinner(char symbol) {
    if (
        (field[0] == field[1] && field[1] == field[2] && field[0] != ' ') ||
        (field[3] == field[4] && field[4] == field[5] && field[3] != ' ') ||
        (field[6] == field[7] && field[7] == field[8] && field[6] != ' ') ||
        (field[0] == field[3] && field[3] == field[6] && field[0] != ' ') ||
        (field[1] == field[4] && field[4] == field[7] && field[1] != ' ') ||
        (field[2] == field[5] && field[5] == field[8] && field[2] != ' ') ||
        (field[0] == field[4] && field[4] == field[8] && field[0] != ' ') ||
        (field[2] == field[4] && field[4] == field[6] && field[2] != ' ')
        ) {
        if (symbol == X)
            return Winner::X;
        else
            return Winner::O;
    }
    else if (find(field.begin(), field.end(), ' ') == field.end()) {
        return Winner::DRAW;
    }

    return Winner::NO_WINNER;
}

void TicTacToe::announceWinner() {
    if (winner == Winner::X)
        std::cout << "\nПобедил игрок, играющий за X" << std::endl << std::endl;
    else if (winner == Winner::O) std::cout << "\nПобедил игрок, играющий за O" << std::endl << std::endl;
    else std::cout << "\nНичья!" << std::endl << std::endl;

    std::cout << "Спасибо за игру!\n";
}

void TicTacToe::display() {
    for (int i = 0; i < field.size(); i++) {
        std::cout << " " << field[i];
        if ((1 + i) % 3 != 0) {
            std::cout << " |";
        }
        else {
            std::cout << std::endl;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    TicTacToe Game1;

    Game1.start();
    
    return 0;
}