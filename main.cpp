#include <SFML/Graphics.hpp>
#include <iostream>
#include "Laberinto.h"
int screenW=700;
int screenH=700;
int cellW = 100;
int cellH = 100;
int wallW = 100;
int wallH = 10;
int filas = 50;
int columnas = 50;
float scale = screenW/(filas*100.f);
float cellSpriteW = cellW*scale;
float cellSpriteH = cellH*scale;
float wallSpriteW = wallW*scale;
float wallSpriteH = wallH*scale;

int main()
{   
    srand(time(0)); 
    Laberinto maze(filas,columnas);
    maze.init();
    maze.generateMaze();

    std::cout<<scale;
    // create the window
    sf::RenderWindow window(sf::VideoMode(700, 700), "My window");
    sf::Texture cell,wall;
    if(!cell.loadFromFile("cell.png",sf::IntRect(0,0,100,100))){
        std::cout<< "error loading cell.png\n";
    }
    else{
        if(!wall.loadFromFile("wall.png",sf::IntRect(0,0,100,10)))
            std::cout<<"error loading wall.png\n";
    }

    sf::Sprite cellSprite,wallSpriteHorizontal,wallSpriteVertical;
    cellSprite.setTexture(cell);
    wallSpriteVertical.setTexture(wall);
    wallSpriteHorizontal.setTexture(wall);
    cellSprite.setScale(sf::Vector2f(scale,scale));
    wallSpriteHorizontal.setScale(sf::Vector2f(scale,scale));
    wallSpriteVertical.setScale(sf::Vector2f(scale,scale));
    cellSprite.setPosition(sf::Vector2f(0.f, 0.f)); // absolute position
    wallSpriteVertical.setRotation(90.f);
    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        // clear the window with black color
        window.clear(sf::Color::Black);
        for(int i=0;i<filas;++i){
            for(int j=0;j<columnas;++j){
                cellSprite.setPosition(sf::Vector2f(j*cellSpriteW,i*cellSpriteH));
                window.draw(cellSprite);                
            }
        }
        for(int i=0; i<filas;++i){
            for(int j=0;j<columnas -1 ;++j){
                if(maze.wv[(columnas -1)*i+j]->up){
                    wallSpriteVertical.setPosition(sf::Vector2f((j+1)*cellSpriteW + wallSpriteH,i*cellSpriteH));
                    window.draw(wallSpriteVertical);
                }
            }
        }   
        for(int i=0;i<filas-1;++i){
            for(int j=0;j<columnas;++j){
                if(maze.wh[columnas*i + j]->up){
                    wallSpriteHorizontal.setPosition(sf::Vector2f(j*cellSpriteW ,(i+1)*cellSpriteH));
                    window.draw(wallSpriteHorizontal);
                }
            }
        }
        for(int i=1;i<columnas;++i){
            wallSpriteHorizontal.setPosition(sf::Vector2f(i*cellSpriteW ,0*cellSpriteH));
            window.draw(wallSpriteHorizontal);
            wallSpriteHorizontal.setPosition(sf::Vector2f((i-1)*cellSpriteW ,filas*cellSpriteH - wallSpriteH));
            window.draw(wallSpriteHorizontal);
        }
        for(int i=0;i<filas;++i){
            wallSpriteVertical.setPosition(sf::Vector2f(0*cellSpriteW + wallSpriteH,i*cellSpriteH));
            window.draw(wallSpriteVertical);
            wallSpriteVertical.setPosition(sf::Vector2f(columnas*cellSpriteW ,i*cellSpriteH));
            window.draw(wallSpriteVertical);
        }
        window.display();
    }

    return 0;
}