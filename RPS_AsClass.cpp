#include <iostream>
#include <memory>
#include <array>
#include <string>
#include <random>

class RPS {
    public:
        enum Move {
            Rock = 0,
            Paper = 1,
            Scissors = 2
            // None = 4? for detecting when no move wants to be made in a MoveMethod function ,since they can only return RPS::Move? Or they could return an optional. Or could be bool with param ref
        };

        enum class GAME_RESULT {
            Win,
            Lose,
            Tie
        };
        
        struct Strategy {
            Strategy(); // Handles setting initial move
            virtual RPS::Move onWin() = 0;
            virtual RPS::Move onLose() = 0;
            virtual RPS::Move onTie() = 0;
            const RPS::Move InitialMove;
        };

        // Players should be told if they win,lose,tie. They should be the ones that call onWin, etc in their Strategy
        struct Player {
            std::string Name{""};
            int ID;
            RPS::Strategy* MoveMethod;
        };

        struct Match {
            const Player* Winner;
            const Player* Loser;
            const Move WinMove;
            const Move LoseMove;
        };

        struct RPS_STATS {
            // Pair<PlayerMoves, AI_Moves>
            std::array<std::pair<Move, Move>, 65535> GameMoves;
            // 0:Win / 1:Lose / 2:Tie
            std::array<int, 3> GameResults = { 0, 0, 0 };
            int GamesPlayed() { return std::accumulate(GameResults.begin(), GameResults.end(), 0); }
        };
    public:
        void SetPlayer(size_t n, RPS::Player* pPlay) {
            // Players[n] = pPlay;
            if (n == 0 || n == 1)
                Players[n](pPlay);
        }
        // Tries to play a game using both players MoveMethods
        GAME_RESULT Solve() {
            Solve(Players[0].MoveMethod(), Players[1].MoveMethod);
        }
        // Return the results for Player1's move.
        GAME_RESULT Solve(Move Player1M, Move Player2M) {
            if ( Player1M == Player2M) {
                return GAME_RESULT::Tie;
            } else if ( (Player1M + 1) % 3 == Player2M ) { // Move after a move is always the one that beats it
                return GAME_RESULT::Lose;
            } else {                                        // Already solved for tie and loss, only option is win
                return GAME_RESULT::Win;
            }
        }
    private:
        // Thought about overengineering it to allow more than 2 players, but
        // that quickly grows to absurdity, since it becomes so mathematically unlikely that
        // One move doesn't get played, meaning that most matches are just Mut. Assur. Destruction
        std::array<std::weak_ptr<RPS::Player>, 2> Players;
};

std::string to_string(RPS::Move move) {
    switch (move) {
        case RPS::Move::Rock:
            return "rock";
        case RPS::Move::Paper:
            return "paper";
        case RPS::Move::Scissors:
            return "scissors";
        default:
            return "what the fuck...?";
    }
}

std::string to_string(RPS::GAME_RESULT result) {
    switch (result) {
        case RPS::GAME_RESULT::Win:
            return "win";
        case RPS::GAME_RESULT::Lose:
            return "lose";
        case RPS::GAME_RESULT::Tie:
            return "tie";
        default:
            return "what the fuck...?";
    }
}




int main() {
    RPS Engine;
    RPS::RPS_STATS gameStats;

    std::cout << "Rock Paper Scissors CLI Game!" << std::endl;
    std::cout << "Ctrl-C to quit." << std::endl;
    while (true) {
        cpuMove = cpu.Move();
        while (!GetPlayerMove(playerMove)) {
            std::cout << "\nInvalid move input. Enter Ctrl-C if you are trying to quit.\n" << std::endl;
        }
        if (!RUN) { break; };

        std::cout << "You threw " << to_string(playerMove) << " against the CPUs " << to_string(cpuMove) << "..." << std::endl;

        switch ( Solve(playerMove, cpuMove) )
        {
            case GAME_RESULT::Win:
                std::cout << "You won!" << std::endl;
                ++(gameStats.GameResults[0]);
                break;
            case GAME_RESULT::Lose:
                std::cout << "You lost..." << std::endl;
                ++(gameStats.GameResults[1]);
                break;
            case GAME_RESULT::Tie:
                std::cout << "You tied." << std::endl;
                ++(gameStats.GameResults[2]);
                break;
        }
        std::cout << std::endl;

        gameStats.GameMoves[gameStats.NumGames] = { playerMove, cpuMove };
        ++(gameStats.NumGames);
    }

    PrintGameStats(gameStats);

    return 0;
}
