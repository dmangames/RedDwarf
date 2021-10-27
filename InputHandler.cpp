#include "InputHandler.h"

// PRIVATE HELPER FUNCTIONS

bool InputHandler::check_for_quit(SDL_Event event) {

    if (event.type == SDL_QUIT) {
        return true;
    }

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            return true;
            break;
        default:
            break;
        }
    }

    return false;

}

// PUBLIC FUNCTIONS

InputHandler::InputHandler() {
    quit = false;
}

bool InputHandler::get_quit() {
    return quit;
}

void InputHandler::update() {

    key_states = SDL_GetKeyboardState(NULL);

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0) {

        // ESC or X button close the game
        if (check_for_quit(event)) {
            quit = true;
        }

        // Record mouse position when it moves
        if (event.type == SDL_MOUSEMOTION) {
            mouse_x = event.motion.x;
            mouse_y = event.motion.y;
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            //handle_mouse_down(event);
        }
    }

}

bool InputHandler::is_key_down(int key) {
    //if(key_states[key])
    //    printf("Key pressed: %d\n", key);
    return key_states[key];
}