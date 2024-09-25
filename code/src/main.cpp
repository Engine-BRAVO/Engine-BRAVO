#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_render.h>
#include <SDL_video.h>

#include <fstream>
#include <iostream>
#include <map>
// #include <nlohmann/json.hpp>
#include <string>
#include <vector>

#include "Animation.h"
#include "Rectangle.h"
#include "Renderer.h"
#include "SpriteAtlas.h"
#include "Window.h"

const int MOVE_SPEED = 10;

void run() {
    Window myWindow;
    Renderer* myRenderer = new Renderer(myWindow);

    bool quit = false;
    SDL_Event event;

    // Variables for animation timing and sprite movement
    Uint32 lastTime = 0;         // Time of the last frame change
    int frame = 0;               // Current frame (sprite in the sprite sheet)
    const int spriteWidth = 16;  // Width of each sprite
    const int spriteHeight = 25; // Height of each sprite
    const int frameCount = 6;    // Total number of frames in the sprite sheet

    // SDL_Texture* spriteSheetTexture = loadTexture(renderer, "enter_the_gungeon_spritesheet.png");
    SpriteAtlas spriteAtlas(myRenderer, "/home/seger/projects/engine/code/enter_the_gungeon_spritesheet.png");

    Rectangle startOfAnimation;
    startOfAnimation.x = 22;          // Move horizontally in the sprite sheet
    startOfAnimation.y = 187;         // Keep the vertical position constant (you can change this for vertical movement)
    startOfAnimation.w = spriteWidth; // The width of the sprite
    startOfAnimation.h = spriteHeight; // The height of the sprite

    Animation& animation = spriteAtlas.getAnimation(startOfAnimation, frameCount);

    // Define the destination rect where the image will be drawn
    Rectangle destRect;
    destRect.x = 100;    // The x position on the screen
    destRect.y = 100;    // The y position on the screen
    destRect.w = 18 * 4; // The width of the drawn image (scaling)
    destRect.h = 26 * 4; // The height of the drawn image (scaling)

    while (!quit) {
        // Event handling
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
            // if (event.type == SDL_KEYDOWN) {
            //     switch (event.key.keysym.sym) {
            //     case SDLK_w: // Move up
            //         destRect.y -= MOVE_SPEED;
            //         break;
            //     case SDLK_s: // Move down
            //         destRect.y += MOVE_SPEED;
            //         break;
            //     case SDLK_a: // Move left
            //         destRect.x -= MOVE_SPEED;
            //         break;
            //     case SDLK_d: // Move right
            // destRect.x += MOVE_SPEED;
            //         break;
            //     }
            // }
        }
        // Get the state of all keys
        const Uint8* keyState = SDL_GetKeyboardState(NULL);

        // Check if specific keys are held down (e.g., Left arrow, Space, or Escape)
        if (keyState[SDL_SCANCODE_W]) {
            destRect.y -= MOVE_SPEED;
        }
        if (keyState[SDL_SCANCODE_A]) {
            destRect.x -= MOVE_SPEED;
        }
        if (keyState[SDL_SCANCODE_S]) {
            destRect.y += MOVE_SPEED;
        }
        if (keyState[SDL_SCANCODE_D]) {
            destRect.x += MOVE_SPEED;
        }
        if (keyState[SDL_SCANCODE_SPACE]) {
            std::cout << "Space bar is being held down" << std::endl;
        }

        // Clear screen
        // SDL_RenderClear(renderer);
        myRenderer->clear();

        // Texture* spriteTexture = animation.getTexture();

        // Copy part of the sprite sheet to the renderer
        // SDL_RenderCopy(renderer, spriteTexture->getSDL_Texture(), &srcRect, &destRect);
        myRenderer->renderAnimation(animation, destRect);

        // Update the screen
        // SDL_RenderPresent(renderer);
        myRenderer->present();

        SDL_Delay(16);
    }

    // Clean up
    // deInitSDL(window, renderer);
    return;
}

// // Alias for convenience
// using json = nlohmann::json;
//
// // Function to read JSON data into map
// std::map<std::string, std::vector<std::string>> loadKeyMappings(const std::string& filename) {
//     // Create a map to store the data
//     std::map<std::string, std::vector<std::string>> keyMap;
//
//     // Open the JSON file
//     std::ifstream file(filename);
//     if (!file.is_open()) {
//         throw std::runtime_error("Could not open file");
//     }
//
//     // Parse the JSON file
//     json jsonData;
//     file >> jsonData;
//
//     // Check if the "KeyMappings" section exists
//     if (jsonData.contains("KeyMappings")) {
//         json keyMappings = jsonData["KeyMappings"];
//
//         // Iterate over each key-value pair in the "KeyMappings" section
//         for (auto& [key, value] : keyMappings.items()) {
//             // Ensure that the value is an array of strings
//             if (value.is_array()) {
//                 std::vector<std::string> strings;
//                 for (const auto& item : value) {
//                     if (item.is_string()) {
//                         strings.push_back(item);
//                     }
//                 }
//                 // Insert the array of strings into the map
//                 keyMap[key] = strings;
//             }
//         }
//     }
//
//     // Return the populated map
//     return keyMap;
// }

int main() {
    run();
    // try {
    //     // Load key mappings from the JSON file
    //     std::map<std::string, std::vector<std::string>> keyMappings = loadKeyMappings("key_mappings.json");
    //
    //     // Display the key mappings
    //     for (const auto& [id, values] : keyMappings) {
    //         std::cout << id << ": ";
    //         for (const std::string& val : values) {
    //             std::cout << val << " ";
    //         }
    //         std::cout << std::endl;
    //     }
    // } catch (const std::exception& ex) {
    //     std::cerr << "Error: " << ex.what() << std::endl;
    // }

    return 0;
}
