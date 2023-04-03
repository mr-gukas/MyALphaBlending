#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <sstream> // for ostringstream
#include <stdlib.h>
#include <emmintrin.h>                                                          
#include <smmintrin.h>  

const __m128i   _0 = _mm_set_epi8  (0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0);
const __m128i _255 = _mm_set1_epi16(0x00FF);    

const unsigned char zero = 0x80;

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





