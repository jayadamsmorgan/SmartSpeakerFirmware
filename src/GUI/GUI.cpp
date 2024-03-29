#include "GUI/Music/MusicPlayer.hpp"
#include <GUI/GUI.hpp>
#include <GUI/Music/MusicPlayerGUI.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <array>
#include <iostream>

constexpr std::array<std::string_view, 5> menuItems = {"Music", "Videos", "Equalizer", "Other", "Settings"};

constexpr std::array<std::string_view, 5> musicMenuItems = {"Play", "Pause", "Next", "Previous", "Stop"};
constexpr std::array<std::string_view, 3> videoMenuItems = {"Files", "YouTube", "Stream"};
constexpr std::array<std::string_view, 3> eqMenuItems = {"Bass", "Mid", "High"};
constexpr std::array<std::string_view, 3> otherMenuItems = {"Test", "Other", "Stuff"};
constexpr std::array<std::string_view, 5> settingsMenuItems = {"Wi-Fi", "Bluetooth", "Sound", "Speakers", "About"};

void GUIClass::clearScreen() {
// Clear screen
#ifdef TESTING
    setColor(color_black);
    SDL_RenderClear(renderer);

    setColor(color_purple);
    drawCircle(SCREEN_RADIUS, SCREEN_RADIUS, SCREEN_RADIUS);
#else   // PRODUCTION
    setColor(color_purple);
    SDL_RenderClear(renderer);
#endif  // TESTING
}

void GUIClass::animations() {
    if (animationProgress > 0 && animationProgress <= ANIMATION_FRAMES) {
        animationProgress++;
    } else if (animationProgress > ANIMATION_FRAMES) {
        lastMenuState = currentMenuState;
        lastSettingsMenuSelectedWord = settingsMenuSelectedWord;
        lastOtherMenuSelectedWord = settingsMenuSelectedWord;
        lastEqMenuSelectedWord = eqMenuSelectedWord;
        lastVideoMenuSelectedWord = videoMenuSelectedWord;
        lastMusicMenuSelectedWord = musicMenuSelectedWord;
        lastSelectedWord = selectedWord;
        animationProgress = 0;
        needRedraw = false;
    } else {
        needRedraw = false;
    }
}

#ifdef TESTING
void GUIClass::eventHandling() {
    if (animationProgress > 0) {
        return;
    }

    SDL_Event e;
    if (SDL_WaitEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = true;
        } else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    switch (currentMenuState) {
                        case 0:
                            lastMusicMenuSelectedWord = musicMenuSelectedWord;
                            if (musicMenuSelectedWord > 0) {
                                musicMenuSelectedWord--;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        case 1:
                            lastVideoMenuSelectedWord = videoMenuSelectedWord;
                            if (lastVideoMenuSelectedWord > 0) {
                                videoMenuSelectedWord--;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        case 2:
                            lastEqMenuSelectedWord = eqMenuSelectedWord;
                            if (eqMenuSelectedWord > 0) {
                                eqMenuSelectedWord--;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        case 3:
                            lastOtherMenuSelectedWord = otherMenuSelectedWord;
                            if (otherMenuSelectedWord > 0) {
                                otherMenuSelectedWord--;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        case 4:
                            lastSettingsMenuSelectedWord = settingsMenuSelectedWord;
                            if (settingsMenuSelectedWord > 0) {
                                settingsMenuSelectedWord--;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        default:
                            lastSelectedWord = selectedWord;
                            if (selectedWord > 0) {
                                selectedWord--;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                    }
                    break;
                case SDLK_DOWN:
                    switch (currentMenuState) {
                        case 0:
                            lastMusicMenuSelectedWord = musicMenuSelectedWord;
                            if (musicMenuSelectedWord < musicMenuItems.size() - 1) {
                                musicMenuSelectedWord++;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        case 1:
                            lastVideoMenuSelectedWord = videoMenuSelectedWord;
                            if (videoMenuSelectedWord < videoMenuItems.size() - 1) {
                                videoMenuSelectedWord++;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        case 2:
                            lastEqMenuSelectedWord = eqMenuSelectedWord;
                            if (eqMenuSelectedWord < eqMenuItems.size() - 1) {
                                eqMenuSelectedWord++;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        case 3:
                            lastOtherMenuSelectedWord = otherMenuSelectedWord;
                            if (otherMenuSelectedWord < otherMenuItems.size() - 1) {
                                otherMenuSelectedWord++;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        case 4:
                            lastSettingsMenuSelectedWord = settingsMenuSelectedWord;
                            if (settingsMenuSelectedWord < settingsMenuItems.size() - 1) {
                                settingsMenuSelectedWord++;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                        default:
                            lastSelectedWord = selectedWord;
                            if (selectedWord < menuItems.size() - 1) {
                                selectedWord++;
                                needRedraw = true;
                                animationProgress = 1;
                            }
                            break;
                    }
                    break;
                case SDLK_RIGHT:
                    if (currentMenuState == -1) {
                        needRedraw = true;
                        lastMenuState = -1;
                        currentMenuState = selectedWord;
                        animationProgress = 1;
                        break;
                    }
                    break;
                case SDLK_e:
                    if (selectedWord == 0 && currentMenuState == -1) {  // Music Main Menu
                        MusicPlayerGUI.mainThread(renderer);
                        needRedraw = true;
                        animationProgress = 1;
                        break;
                    }
                    if (currentMenuState == 0) {  // Music Menu selected
                        switch (musicMenuSelectedWord) {
                            case 0:  // Music -> Play
                                MP.play();
                                break;
                            case 1:  // Music -> Pause
                                MP.pause();
                                break;
                            case 2:  // Music -> Next
                                MP.next();
                                break;
                            case 3:  // Music -> Previous
                                MP.prev();
                                break;
                            case 4:  // Music -> Stop
                                MP.stop();
                                break;
                        }
                        MusicPlayerGUI.mainThread(renderer);
                        needRedraw = true;
                        animationProgress = 1;
                    }
                    break;
                case SDLK_LEFT:
                case SDLK_ESCAPE:
                    if (currentMenuState != -1) {
                        needRedraw = true;
                        lastMenuState = currentMenuState;
                        currentMenuState = -1;
                        animationProgress = 1;
                        break;
                    }
            }
        }
    }
}
#endif  // TESTING

GUIClass GUI = GUIClass();

double lerp(double start, double end, double t) {
    return start + t * (end - start);
}

void GUIClass::init() {
#ifdef PRODUCTION // Seems to be working, needs to be tested on the device
    if (setenv("SDL_VIDEODRIVER", "directfb", 1) != 0) {
        std::cerr << "Failed to set SDL_VIDEODRIVER to dummy" << '\n';
        exit(EXIT_FAILURE);
    }
#endif  // PRODUCTION

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << '\n';
        exit(EXIT_FAILURE);
    }

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << '\n';
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    window = SDL_CreateWindow("Text Circle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << '\n';
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    font = TTF_OpenFont(FONT_PATH, MAX_FONT_SIZE);
    if (font == nullptr) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << '\n';
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void GUIClass::renderMainMenu(double t) {
    for (int i = 0; i < menuItems.size(); ++i) {
        int lastDistance = std::abs(i - lastSelectedWord);
        int currentDistance = std::abs(i - selectedWord);

        double lastMinimization = lastMenuState == -1 ? 1 : -5;
        double currentMinimization = currentMenuState == -1 ? 1 : -5;
        double lastFontSize = MAX_FONT_SIZE - lastDistance * (MAX_FONT_SIZE - MIN_FONT_SIZE) / (menuItems.size() / 1.5) + lastMinimization;
        double currentFontSize = MAX_FONT_SIZE - currentDistance * (MAX_FONT_SIZE - MIN_FONT_SIZE) / (menuItems.size() / 1.5) + currentMinimization;

        double fontSize = lerp(lastFontSize, currentFontSize, t);
        TTF_Font* tempFont = TTF_OpenFont(FONT_PATH, static_cast<int>(fontSize));

        SDL_Color textColor = {0, 0, 0, 0xFF};
        SDL_Surface* textSurface = TTF_RenderText_Blended(tempFont, menuItems[i].data(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        int textWidth = textSurface->w;
        int textHeight = textSurface->h;

        double lastOffsetX = lastMenuState == -1 ? 0 : -85;
        double currentOffsetX = currentMenuState == -1 ? 0 : -85;
        double lastX = static_cast<double>(WINDOW_WIDTH - textWidth) / 2 - std::cos(lastDistance) * 60 + 50 + lastOffsetX;
        double currentX = static_cast<double>(WINDOW_WIDTH - textWidth) / 2 - std::cos(currentDistance) * 60 + 50 + currentOffsetX;
        double xPosition = lerp(lastX, currentX, t);

        double lastY = static_cast<double>(WINDOW_HEIGHT - textHeight) / 2 + (i - lastSelectedWord) * 80;
        double currentY = static_cast<double>(WINDOW_HEIGHT - textHeight) / 2 + (i - selectedWord) * 80;
        double yPosition = lerp(lastY, currentY, t);

        SDL_Rect renderQuad = {
            static_cast<int>(xPosition),
            static_cast<int>(yPosition),
            textWidth,
            textHeight};

        SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        TTF_CloseFont(tempFont);
    }
}

template<unsigned int i>
void GUIClass::renderSubMenu(std::array<std::string_view, i> array, double t) {
    SDL_Color textColor = {0, 0, 0, 0xFF};
    int arraySize = static_cast<int>(array.size());
    for (int j = 0; j < arraySize; j++) {
        double fontSize = 0;
        if (currentMenuState != -1) {
            double lastMinimization = lastMenuState == -1 ? 0 : 5;
            double currentMinimization = currentMenuState == -1 ? 0 : 5;
            double lastFontSize = MIN_FONT_SIZE + 4 + lastMinimization;
            double currentFontSize = MIN_FONT_SIZE + 4 + currentMinimization;
            fontSize = lerp(lastFontSize, currentFontSize, t);
        } else {
            fontSize = MIN_FONT_SIZE + 4;
        }
        TTF_Font* tempFont = TTF_OpenFont(FONT_PATH, static_cast<int>(fontSize));
        SDL_Surface* textSurface = TTF_RenderText_Blended(tempFont, array[j].data(), textColor);
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        int textWidth = textSurface->w;
        int textHeight = textSurface->h;

        double xPosition = static_cast<double>(WINDOW_WIDTH - textWidth) / 2 + 140 - cos(j - std::floor(arraySize / 2.0)) * 30;
        double lastOffsetX = lastMenuState == -1 ? 0 : -85;
        double currentOffsetX = currentMenuState == -1 ? 0 : -85;
        double lastXPosition = xPosition + lastOffsetX;
        double currentXPosition = xPosition + currentOffsetX;
        xPosition = lerp(lastXPosition, currentXPosition, t);

        double yPosition = static_cast<double>(WINDOW_HEIGHT - textHeight) / 2 + (j - std::floor(arraySize / 2.0)) * (textHeight + 20);

        if (currentMenuState != -1) {
            SDL_Rect highlightRect = {.x = static_cast<int>(xPosition - 10),
                                      .y = static_cast<int>(yPosition - 3),
                                      .w = textWidth + 20,
                                      .h = textHeight + 10};
            setColor(color_white);
            switch (currentMenuState) {
                case 0:
                    if (j == musicMenuSelectedWord) {
                        SDL_RenderDrawRect(renderer, &highlightRect);
                    }
                    break;
                case 1:
                    if (j == videoMenuSelectedWord) {
                        SDL_RenderDrawRect(renderer, &highlightRect);
                    }
                    break;
                case 2:
                    if (j == eqMenuSelectedWord) {
                        SDL_RenderDrawRect(renderer, &highlightRect);
                    }
                    break;
                case 3:
                    if (j == otherMenuSelectedWord) {
                        SDL_RenderDrawRect(renderer, &highlightRect);
                    }
                    break;
                case 4:
                    if (j == settingsMenuSelectedWord) {
                        SDL_RenderDrawRect(renderer, &highlightRect);
                    }
                    break;
            }
            setColor(color_black);
        }

        SDL_Rect renderQuad = {
            static_cast<int>(xPosition),
            static_cast<int>(yPosition),
            textWidth,
            textHeight};
        SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);
        TTF_CloseFont(tempFont);
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
}

void GUIClass::mainThread() {
    while (!quit) {
        frameStart = SDL_GetTicks();

#ifdef TESTING
        eventHandling();
        std::cout << "SCREEN UPDATE" << '\n';
#endif  // TESTING \
    // TODO(jayadamsmorgan): handle events of SmartKnob in production

        double t = static_cast<double>(animationProgress) / static_cast<double>(ANIMATION_FRAMES);

        clearScreen();

        renderMainMenu(t);

        if (selectedWord == 0) {  // MUSIC
            renderSubMenu<musicMenuItems.size()>(musicMenuItems, t);
        }
        if (selectedWord == 1) {  // VIDEO
            renderSubMenu<videoMenuItems.size()>(videoMenuItems, t);
        }
        if (selectedWord == 2) {  // EQ
            renderSubMenu<eqMenuItems.size()>(eqMenuItems, t);
        }
        if (selectedWord == 3) {  // OTHER
            renderSubMenu<otherMenuItems.size()>(otherMenuItems, t);
        }
        if (selectedWord == 4) {  // SETTINGS
            renderSubMenu<settingsMenuItems.size()>(settingsMenuItems, t);
        }

        animations();

        frameTime = SDL_GetTicks() - frameStart;
        if (FRAME_DELAY > frameTime) {
            SDL_RenderPresent(renderer);
            SDL_Delay(FRAME_DELAY - frameTime);
        }
    }
}

void GUIClass::close() {
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void GUIClass::setColor(SDLColor color) {
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);
}

void GUIClass::drawCircle(int32_t centerX, int32_t centerY, int32_t radius) {
    const int32_t diameter = (radius * 2);

    int32_t x = (radius - 1);
    int32_t y = 0;
    int32_t tx = 1;
    int32_t ty = 1;
    int32_t error = (tx - diameter);

    while (x >= y) {
        for (int i = centerX - x; i <= centerX + x; i++) {
            SDL_RenderDrawPoint(renderer, i, centerY + y);
            SDL_RenderDrawPoint(renderer, i, centerY - y);
        }
        for (int i = centerX - y; i <= centerX + y; i++) {
            SDL_RenderDrawPoint(renderer, i, centerY + x);
            SDL_RenderDrawPoint(renderer, i, centerY - x);
        }

        if (error <= 0) {
            ++y;
            error += ty;
            ty += 2;
        }

        if (error > 0) {
            --x;
            tx += 2;
            error += (tx - diameter);
        }
    }
}
