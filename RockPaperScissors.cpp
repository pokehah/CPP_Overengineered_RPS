#include <iostream>
#include <array>
#include <string>
#include <random>

enum RPS_MOVE : unsigned int;
enum class GAME_RESULT;
std::string to_string(RPS_MOVE);
std::string to_string (GAME_RESULT);

struct RPS_STATS;
void PrintGameStats(const RPS_STATS&, bool);

class RPS_AI;
GAME_RESULT Solve(RPS_MOVE, RPS_MOVE);
bool GetPlayerMove(RPS_MOVE&);

void PlayRPS();
void CalcRPS();

bool RUN = true;    // Using global for gameLoop, for now.

enum RPS_MOVE : unsigned int {
  Rock = 0,
  Paper = 1,
  Scissors = 2
};

enum class GAME_RESULT {
  Win,
  Lose,
  Tie
};

std::string to_string(RPS_MOVE move) {
  switch (move) {
    case RPS_MOVE::Rock:
      return "rock";
    case RPS_MOVE::Paper:
      return "paper";
    case RPS_MOVE::Scissors:
      return "scissors";
    default:
      return "Huh...? (to_string(RPS_MOVE))";
  }
}
std::string to_string(GAME_RESULT result) {
  switch (result) {
    case GAME_RESULT::Win:
      return "win";
    case GAME_RESULT::Lose:
      return "lose";
    case GAME_RESULT::Tie:
      return "tie";
    default:
      return "Huh...? (to_string(GAME_RESULT))";
  }
}

class RPS_AI {
  public:
    RPS_AI() : RNG(std::random_device{}()), dist(0, 2) {}

    RPS_MOVE Move() {
      switch ( dist( RNG ) )
      {
        case 0:
          return RPS_MOVE::Rock;
        case 1:
          return RPS_MOVE::Paper;
        case 2:
          return RPS_MOVE::Scissors;
        default:
          std::cout << "Huh...? (RPS_AI::Move())" << std::endl;
          return RPS_MOVE::Rock; // Won't ever occur unless class impl is changed
      }
    }
  private:
    std::mt19937 RNG;
    std::uniform_int_distribution<int> dist;
};

GAME_RESULT Solve(RPS_MOVE playerMove, RPS_MOVE cpuMove) {
  if ( playerMove == cpuMove) {
    return GAME_RESULT::Tie;
  } else if ( (playerMove + 1) % 3 == cpuMove ) { // Move after a move is always the one that beats it
    return GAME_RESULT::Lose;
  } else {                                        // Already solved for tie and loss, only option is win
    return GAME_RESULT::Win;
  }

}

bool GetPlayerMove(RPS_MOVE& moveInput) {
  char input;
  std::cout << "Enter R, P, or S. Enter Q to see stats and quit." << std::endl
            << "Move >> ";
  std::cin >> input;
  std::cin.ignore(256, '\n');

  switch (input) {
    case 'q':
    case 'Q':
      RUN = false;
      break;
    case 'r':
    case 'R':
      moveInput = RPS_MOVE::Rock;
      break;
    case 'p':
    case 'P':
      moveInput = RPS_MOVE::Paper;
      break;
    case 's':
    case 'S':
      moveInput = RPS_MOVE::Scissors;
      break;
    default:
      return false;
  }

  return true;
}

struct RPS_STATS {
  // Pair<PlayerMoves, AI_Moves>
  std::array<std::pair<RPS_MOVE, RPS_MOVE>, 65535> GameMoves;
  // 0:Win / 1:Lose / 2:Tie
  std::array<int, 3> GameResults = { 0, 0, 0 };
  int NumGames = 0; // Could technically just be gotten by a sum of GameResults. Would offload the slight slight slight time spent incr. for a sum at PrintStats
                    // even more ideally, if you didn't want to offload the time but also wanted to cut the extra line incr. numgames, make a member function that takes GameResult
                    // and automatically increments which one. This also avoids having to know the order of GameResults, which is really just good practice.
                    // Yeah. Probably do that.
};
void PrintGameStats(const RPS_STATS& gameStats, bool PrintReplay = true) {
  std::cout << "\tGame History" << std::endl
            << "You played " << gameStats.NumGames << " games." << std::endl;
  
  // Replay shows the move history and result history.
  if (PrintReplay)
    for (int i = 0; i < gameStats.NumGames; ++i)
      std::cout << '#' << i + 1 << ' '
                << to_string(gameStats.GameMoves[i].first)[0] << " <-> " << to_string(gameStats.GameMoves[i].second)[0]
                << ' ' << to_string(Solve(gameStats.GameMoves[i].first, gameStats.GameMoves[i].second)) << std::endl;
  
  // Percentage of total wins/losses/ties.
  std::cout << "  Wins: "   << gameStats.GameResults[0] << " ( " << static_cast<float>(gameStats.GameResults[0]) / gameStats.NumGames * 100 << "% )" << std::endl
            << "Losses: "   << gameStats.GameResults[1] << " ( " << static_cast<float>(gameStats.GameResults[1]) / gameStats.NumGames * 100 << "% )" << std::endl
            << "  Ties: "   << gameStats.GameResults[2] << " ( " << static_cast<float>(gameStats.GameResults[2]) / gameStats.NumGames * 100 << "% )" << std::endl;
}

int main() {
  PlayRPS();
  //for (int i = 0; i < 2; ++i) { CalcRPS(); }
  //CalcRPS();

  return 0;
}

// BELOW TEMPORARY IMPLEMENTATION //

void PlayRPS() {
  RPS_STATS gameStats;
  RPS_AI cpu;
  RPS_MOVE cpuMove;
  RPS_MOVE playerMove;
  
  std::cout << "Rock Paper Scissors CLI Game!" << std::endl
            << std::endl;
  while (true) {
    cpuMove = cpu.Move();
    while (!GetPlayerMove(playerMove)) {
      std::cout << "\nInvalid move input. Enter Q if you are trying to quit.\n" << std::endl;
    }
    if (!RUN) { break; };
    
    std::cout << std::endl
              << "You threw " << to_string(playerMove) << " against the CPUs " << to_string(cpuMove) << "..." << std::endl;
    
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
      default:
        std::cout << "Huh...? (PlayRPS() :: switch(Solve(m1, m2)))" << std::endl;
    }
    std::cout << std::endl;
    
    gameStats.GameMoves[gameStats.NumGames] = { playerMove, cpuMove };
    ++(gameStats.NumGames);
  }

  PrintGameStats(gameStats);
}

/* For designing an "RPS_STRATEGY" class which can be used to define modular behavior for CPU.
    RPS Strategy:
     Initial Move
     {
        Response to Win:  A
        Response to Lose: B
        Response to Tie:  C
     }
        ( Data Required for Responses: )
            Move History (For both players)
            Game Result History (in case  a response is only warranted for multiple losses, etc)

*/
void CalcRPS() {
  // 0 = rotate, 1 = play until lose, rotate, 2 = play until lose, copy
  int method = 2;
  RPS_STATS gameStats;
  static RPS_AI cpu;
  RPS_MOVE cpuMove = cpu.Move();
  RPS_MOVE playerMove = RPS_MOVE::Rock;

  const int ITERATIONS = 1000;  // Iterations default = 100
  for (int i = 0; i < ITERATIONS; ++i) {
    //cpuMove = cpu.Move();
    cpuMove = static_cast<RPS_MOVE>((cpuMove + 2) % 3);
    gameStats.GameMoves[gameStats.NumGames] = { playerMove, cpuMove };
    switch ( Solve(playerMove, cpuMove) )
    {
      case GAME_RESULT::Win:
          ++(gameStats.GameResults[0]);
          break;
      case GAME_RESULT::Lose:
          ++(gameStats.GameResults[1]);
          if (method == 1) { playerMove = static_cast<RPS_MOVE>((playerMove + 1) % 3); }
          else if (method == 2) { playerMove = cpuMove; }
          break;
      case GAME_RESULT::Tie:
          ++(gameStats.GameResults[2]);
          break;
    }

    ++(gameStats.NumGames);

    if (method == 0) { playerMove = static_cast<RPS_MOVE>((playerMove + 1) % 3); }
  }

  PrintGameStats(gameStats, true);
}

