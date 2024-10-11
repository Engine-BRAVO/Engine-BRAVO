#include <SDL.h>
class SDLrender
{
 public:
  SDLrender();

 private:
  const int WINDOW_WIDTH = 800;
  const int WINDOW_HEIGHT = 600;

  SDL_Window* window;
  SDL_Renderer* renderer;
};
