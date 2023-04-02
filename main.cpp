#include "alpha.hpp"

int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        printf("expected: front name, back name, x position, y position\n");
        return 1;
    }

    if (isNumber(argv[3]) || isNumber(argv[4]))
    {
        printf("please, write correct coordinates\n");
        return 1;
    }
    
    blend_t blend = {};
    if (blendCtor(&blend, argv[1], argv[2], atoi(argv[3]), atoi(argv[4])))
    {
        printf("incorrect input data\n");
        return 1;
    }

    window_t win = {};
    windowCtor(&win,&blend);
    
    int first = 1;
    while (win.window.isOpen())
    {

        sf::Event event;
        while (win.window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                win.window.close();

        }
        
        alphaBlend(&blend);
        
        image_t image = {};
        imageCtor(&image, &blend); 
        
        if (first)
        {
            image.alpha_blend.saveToFile("../img/1.png");
            first = 0;
        }

        setFps(&win);

        win.window.clear();
        image.texture.update(image.alpha_blend);
        win.window.draw (image.sprite);
        win.window.draw(win.fpsText);
        win.window.display();
        
    }
 
    return 0;
}
