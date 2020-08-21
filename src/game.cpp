#include "game.h"
#include "SDL.h"
#include <future>
#include <iostream>
#include <thread>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height), engine(dev()),
      random_w(0, static_cast<int>(grid_width) - 1),
      random_h(0, static_cast<int>(grid_height) - 1) {
  PlaceFood();
  // AddObstacle();
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  std::thread t1(&Game::ObstaclesThread, this);

  while (running) {

    frame_start = SDL_GetTicks();
    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, obstacles);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  t1.join();
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::ObstaclesThread() {
  int i = 0;
  while (snake.alive) {
    if (i <= 50) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      // std::cout << i << std::endl;
      i++;
    } else {
      Game::AddObstacle();
      i = 0;
    }
  }
}

void Game::AddObstacle() {
  SDL_Point obstacle;
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);

    if (!snake.SnakeCell(x, y)) {
      obstacle.x = x;
      obstacle.y = y;
      std::vector<SDL_Point>::iterator it;

      it = std::find_if(obstacles.begin(), obstacles.end(),
                        [&obs = obstacle](const SDL_Point &i_obs) -> bool {
                          return obs.x == i_obs.x && obs.y == i_obs.y;
                        });

      if (it == obstacles.end()) {
        obstacles.emplace_back(obstacle);
        return;
      }
    }
  }
}

void Game::Update() {
  for (auto &obstacle : obstacles) {
    if (static_cast<int>(snake.head_x) == obstacle.x &&
        static_cast<int>(snake.head_y) == obstacle.y)
      snake.alive = false;
  }

  if (!snake.alive)
    return;

  snake.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  // Check if there's food over here
  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.02;
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }