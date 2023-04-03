#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream> // for std::ostringstream
#include <stdlib.h>
#include <emmintrin.h>                                                          
#include <smmintrin.h>  

const int x0 = 300;
const int y0 = 225;

struct blend_t 
{
    sf::Image fr_img;
    sf::Image bk_img;

    const unsigned int* front; 
    const unsigned int* back; 

    unsigned int* new_img;
    
    int fr_wd;
    int fr_hg;
    int bk_wd;
    int bk_hg;
    
    int x_pos;
    int y_pos;
};

struct image_t
{
    sf::Image   alpha_blend;
    sf::Texture texture; 
    sf::Sprite  sprite;
};

struct window_t
{
    sf::RenderWindow window;
    sf::Font         font;
    sf::Text         fpsText;
    sf::Clock        clock;
};

int blendCtor(blend_t* blend, const char* front_name, const char* back_name, int x_pos, int y_pos);
int blendDtor(blend_t* blend);

int isNumber(char* str);
int alphaBlend(blend_t* blend);

int setFps(window_t* win);
int windowCtor(window_t* win,  blend_t* blend);
int imageCtor (image_t* image, blend_t* blend);





