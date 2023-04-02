#include "alpha.hpp"


int alphaBlend(blend_t* blend)
{
    if (blend == NULL) return 1;

    for (int y = 0; y < blend->fr_hg; y++)
    {
        if (y + blend->y_pos >= blend->bk_hg) break;

        for (int x = 0; x < blend->fr_wd; x++)
        {
            if (x + blend->x_pos >= blend->bk_wd) break;
            
            for (int i = 0; i < 100; i++)
            {
                unsigned int fr_clr = blend->front[x + y * blend->fr_wd];
                unsigned int bk_clr = blend->back [x + blend->x_pos + 
                                                  (y + blend->y_pos) * blend->bk_wd];
                

                unsigned char fr_al = fr_clr >> 24;
                unsigned char bk_al = bk_clr >> 24;
                
                unsigned int new_clr =  bk_al << 24;
                
                for (int idx = 0; idx < 17; idx += 8)
                {
                    
                    unsigned char FR_CLR = ((0xFF << idx) & fr_clr) >> idx;
                    unsigned char BK_CLR = ((0xFF << idx) & bk_clr) >> idx;

                    new_clr += getColor(FR_CLR, BK_CLR, fr_al) << idx;
                }
                
                blend->new_img[x + blend->x_pos + (y + blend->y_pos) * blend->bk_wd] =
                    new_clr;
            }
        }
    }

    return 0;
}


unsigned char getColor(unsigned char fr_clr, unsigned char bk_clr, 
                       unsigned char transp)
{
    return ((unsigned int) fr_clr * transp + 
            (unsigned int) bk_clr * (255 - transp)) / 255;
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

    image->alpha_blend.create(blend->bk_wd, blend->bk_hg, (unsigned char*) blend->new_img);
    image->texture.create(blend->bk_wd, blend->bk_hg);
    image->texture.update(image->alpha_blend);
    image->sprite.setTexture(image->texture);

    return 0;
}   



