#include <iostream>
#include <cstdint>

using namespace std;

struct Board {
    uint16_t x = 0;
    uint16_t o = 0;

    void print() const {
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                uint16_t offset = col + (row * 3);
                uint16_t mask = 1u << offset;
                char c = offset + 1 + '0';

                if (x & mask) {
                    c = 'X';
                } else if (o & mask) {
                    c = 'O';
                }

                cout << c << (col == 2 ? "\n" : " | ");
            }
        }
        cout << endl;
    }

    void play(int pos, bool xTurn) {
        uint16_t mask = 1u << pos;

        if ((x | o) & mask) return;

        if (xTurn) {
            x |= mask;
        } else {
            o |= mask;
        }
    }

    void undo(int pos, bool xTurn) {
        uint16_t mask = 1u << pos;

        if (!((x | o) & mask)) return;

        if (xTurn) {
            x ^= mask;
        } else {
            o ^= mask;
        }
    }

    int check_winner() const {
        static constexpr uint16_t winning_positions[] = {
            0b000000111,
            0b000111000,
            0b111000000,
            0b100100100,
            0b010010010,
            0b001001001,
            0b100010001,
            0b001010100
        };

        for (const uint16_t& win_mask : winning_positions) {
            if ((x & win_mask) == win_mask) return 1;
            if ((o & win_mask) == win_mask) return -1;
        }

        return 0;
    }

    bool check_occupied(int pos) const {
        uint16_t mask = 1u << pos;

        if ((x | o) & mask) return true;

        return false;
    }

    bool is_full() const {
        static constexpr uint16_t mask = 0b111111111;
        return ((x | o) == mask);
    }
};

int minimax(Board& b, bool maximizer) {
    int winner = b.check_winner();
    if (winner != 0) {
        return winner;
    } 
    if (b.is_full()) {
        return 0;
    }
    int best = maximizer ? -1 : 1;
    for (int i = 0; i < 9; ++i) {
        if (!b.check_occupied(i)) {
            b.play(i, maximizer);
            int score = minimax(b, !maximizer);
            b.undo(i, maximizer);

            if (maximizer) {
                best = max(best, score);
                if (best == 1) return best;
            } else {
                best = min(best, score);
                if (best == -1) return best;
            }
        }
    }
    return best;
}

void find_best(Board& b, bool maximizer) {
    int best_index = -1;
    int best_score = maximizer ? -2 : 2;
    for (int i : {4,0,2,6,8,1,3,5,7}) {
        if (!b.check_occupied(i)) {
            b.play(i, maximizer);
            int score = minimax(b, !maximizer);
            if ((score > best_score && maximizer) or (score < best_score && !maximizer)) {
                best_index = i;
                best_score = score;
            }
            b.undo(i, maximizer);
            if ((score == 1 && maximizer) or (score == -1 && !maximizer)) {
                break;
            }
        }
    }

    cout << (maximizer ? 'X' : 'O') << "'s best move is: " << to_string(best_index + 1) << endl;
    b.play(best_index, maximizer);
}

int main() {
    Board b;

    bool maximizer;
    char response;
    cout << "Are you going first? (y or n): ";
    cin >> response;
    if (response == 'y') {
        maximizer = true;
    } else if (response == 'n') {
        maximizer = false;
    }

    if (!maximizer) {
        cout << "\nCurrent board: " << endl;
        b.print();
        cout << "What position did your opponent play?: ";
        int op_move;
        cin >> op_move;
        --op_move;
        b.play(op_move, true);
    }

    while (!(b.check_winner() or b.is_full())) {
        cout << "\nCurrent board: " << endl;
        b.print();
        find_best(b, maximizer);
        if (b.check_winner() or b.is_full()) break;

        maximizer = !maximizer;
        cout << "\nCurrent board: " << endl;
        b.print();
        cout << "What position did your opponent play?: ";
        int op_move;
        cin >> op_move;
        --op_move;
        b.play(op_move, maximizer);
        maximizer = !maximizer;
    }
}
