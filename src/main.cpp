#include "controller.h"
#include "game.h"
#include "renderer.h"
#include <iostream>
#include <fstream>

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};
  std::fstream highestScore;
  
  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);

  highestScore.open("highestScore.txt");
  if(highestScore){ 
      int score;
      std::string line;

      std::getline(highestScore, line);
      score = std::stoi(line);
      if (score > 0)
        game.SetHighestScore(score);
        highestScore.close();
        std::cout << "Highest Score = " << score << "\n";
    }
  
  game.Run(controller, renderer, kMsPerFrame);
  
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";

  if(game.GetScore() > game.GetHighestScore()){
      std::cout << "Congratulations! New Highest Score = " 
                << game.GetScore() << std::endl;
      highestScore.open("highestScore.txt", std::ios::out);
      if (highestScore){
          highestScore << game.GetScore();
          highestScore.close();
      }
  }
  
  return 0;
}