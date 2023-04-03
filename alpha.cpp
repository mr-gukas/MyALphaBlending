#include "alpha.hpp"

inline void calcPxl(blend_t* blend, int x, int y, __m128i* clr)
{
    __m128i fr_clr = _mm_load_si128((__m128i *) &blend->front[y * blend->fr_wd + x]);
    __m128i bk_clr = _mm_load_si128((__m128i *) &blend->back[(y + blend->y_pos) * blend->bk_wd + x + blend->x_pos]);

    __m128i upFr_clr = (__m128i) _mm_movehl_ps((__m128) _0, (__m128) fr_clr);
    __m128i upBk_clr = (__m128i) _mm_movehl_ps((__m128) _0, (__m128) bk_clr);

    fr_clr   = _mm_cvtepu8_epi16(fr_clr);
    upFr_clr = _mm_cvtepu8_epi16(upFr_clr);
    bk_clr   = _mm_cvtepu8_epi16(bk_clr);
    upBk_clr = _mm_cvtepu8_epi16(upBk_clr);

    static const __m128i moveA = _mm_set_epi8 (zero, 14, zero, 14, zero, 14, zero, 14,
                                               zero,  6, zero,  6, zero,  6, zero,  6);
    __m128i lowTransp = _mm_shuffle_epi8 (fr_clr, moveA);                           
    __m128i upTransp  = _mm_shuffle_epi8 (upFr_clr, moveA);

    fr_clr   = _mm_mullo_epi16 (fr_clr, lowTransp);                                            
    upFr_clr = _mm_mullo_epi16 (upFr_clr, upTransp);
    bk_clr   = _mm_mullo_epi16 (bk_clr, _mm_sub_epi16 (_255, lowTransp));                     
    upBk_clr = _mm_mullo_epi16 (upBk_clr, _mm_sub_epi16 (_255, upTransp));

    __m128i sumLow = _mm_add_epi16 (fr_clr, bk_clr);                                  
    __m128i sumUp  = _mm_add_epi16 (upFr_clr, upBk_clr);

    static const __m128i moveSum = _mm_set_epi8 ( zero,  zero,  zero, zero, zero, zero, zero, zero,
                                                    15,    13,    11,    9,    7,    5,    3,    1);
    sumLow = _mm_shuffle_epi8 (sumLow, moveSum);                                
    sumUp  = _mm_shuffle_epi8 (sumUp, moveSum);

    *clr = (__m128i) _mm_movelh_ps ((__m128) sumLow, (__m128) sumUp);
}


int alphaBlend(blend_t* blend)
{
    for (unsigned y = 0; y < blend->fr_hg; ++y)
    {
        if (y + blend->y_pos >= blend->bk_hg) break;

        for (unsigned x = 0; x < blend->fr_wd; x += 4)
        {
            if (x + blend->x_pos + 3 >= blend->bk_wd) break;
            
            __m128i clr = _mm_set1_epi8(0xFF);

            for (int i = 0; i < 100; i++)
            {
                calcPxl(blend, x, y, &clr);
            }

            _mm_store_si128 ((__m128i *) &blend->new_img[(y + blend->y_pos) * blend->bk_wd + x + blend->x_pos], clr);
       }
    }
    
    return 0;
}

int blendCtor(blend_t* blend, const char* front_name, const char* back_name, 
              int      x_pos, int         y_pos)
{
    if (!blend || !front_name || !back_name) return 1;

    if (!blend->fr_img.loadFromFile(front_name))
    {
        return 1;
    }

    if (!blend->bk_img.loadFromFile(back_name))
    {
        return 1;
    }
    
    blend->fr_wd = blend->fr_img.getSize().x;
    blend->fr_hg = blend->fr_img.getSize().y;
    blend->bk_wd = blend->bk_img.getSize().x;
    blend->bk_hg = blend->bk_img.getSize().y;
    blend->x_pos = x_pos;
    blend->y_pos = y_pos;

    blend->front = (unsigned int*) blend->fr_img.getPixelsPtr();
    blend->back  = (unsigned int*) blend->bk_img.getPixelsPtr();
    
    blend->new_img = (unsigned int*) blend->back;

    return 0;
}


int blendDtor(blend_t* blend)
{
    if (!blend) return 1;

    free(blend->new_img);
    blend->new_img = nullptr;

    return 0;
}

int isNumber(char* str)
{
    if (str == NULL) return 1;
    
    char frst = *str;
    
    if (!frst) return 1;

    if (frst == '-' || frst == '+')
        frst = *(++str);

    while (frst != '\0')
    {
        if (frst < '0' || frst > '9')
            return 1;

        frst = *(++str);
    }
    
    return 0;
}

 int windowCtor(window_t* win, blend_t* blend)
{
    if (!win || !blend) return 1;

    win->window.create(sf::VideoMode(blend->bk_wd, blend->bk_hg), "Alpha Blending");

    if (!win->font.loadFromFile("../src/font.ttf")) {
        return 1; 
    }
    
    win->fpsText.setFont(win->font);
    win->fpsText.setString("");
    win->fpsText.setCharacterSize(10);
    win->fpsText.setFillColor(sf::Color::White);
    win->fpsText.setPosition(sf::Vector2f(0, 0));

    return 0;
}

int setFps(window_t* win)
{
    if (!win) return 1;

    sf::Time elapsed = win->clock.restart();

    float fps = 1.0f / elapsed.asSeconds();

    std::ostringstream ss;
    ss << "FPS: " << fps;

    win->fpsText.setString(ss.str());

    return 0;
}

int imageCtor(image_t* image, blend_t* blend)
{
    if (!image || !blend) return 1;

    image->alpha_blend.create(blend->bk_wd, blend->bk_hg,(unsigned char*) blend->new_img);
    image->texture.create(blend->bk_wd, blend->bk_hg);
    image->texture.update(image->alpha_blend);
    image->sprite.setTexture(image->texture);

    return 0;
}   



