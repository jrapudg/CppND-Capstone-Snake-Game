#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include <random>
#include <vector>

class Game {
public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void SetHighestScore(int score) { hScore = score; }
  int GetHighestScore(){ return hScore; }

private:
  Snake snake;
  SDL_Point food;
  std::vector<SDL_Point> obstacles;
  std::mutex _mtx;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int hScore{0};
  bool running{true};
  void PlaceFood();
  void AddObstacle();
  void ObstaclesThread();
  void Update();
};

#endif