#include <array>
#include <random>

using namespace std;

class ZobristHash {
public:
    static constexpr int NUM_META = 9;
    static constexpr int NUM_INNER = 9;
    static constexpr int NUM_MOVES = 3;


    ZobristHash() {
        mt19937_64 rng(random_device{}());

        for (int meta = 0; meta < NUM_META; ++meta) {
            for (int inner = 0; inner < NUM_INNER; ++inner) {
                for (int move = 0; move < NUM_MOVES; ++move) {
                    table[meta][inner][move] = rng();
                }
            }
        }

        for (int i = 0; i < 10; ++i) {
            forcedBoard[i] = rng();
        }
        sideToMove = rng();
    }

    uint64_t compute(const array<string, 9>& board, int last_forced) {
        uint64_t hash = 0;

        for (int meta = 0; meta < NUM_META; ++meta) {
            for (int inner = 0; inner < NUM_INNER; ++inner) {
                hash ^= table[meta][inner][moveIndex(board[meta][inner])];
            }
        }

        hash ^= sideToMove;
        hash ^= forcedBoard[last_forced];
        return hash;
    }

    void update(uint64_t& hash, int meta, int inner, char newMove, int old_forced, int new_forced) const {
        hash ^= forcedBoard[old_forced];
        hash ^= table[meta][inner][moveIndex(newMove)];
        hash ^= sideToMove;
        hash ^= forcedBoard[new_forced];
    }
private:
    uint64_t table[NUM_META][NUM_INNER][NUM_MOVES];
    uint64_t sideToMove;
    uint64_t forcedBoard[10];

    static int moveIndex(char c) {
        switch (c) {
            case '.': return 0;
            case 'X': return 1;
            case 'O': return 2;
            default: return 0;
        }
    }
};