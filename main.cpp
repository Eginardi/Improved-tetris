#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <time.h>
#include <iostream>
#include <tuple>

using namespace sf;

const int M = 15;
// Строки

const int N = 10;
// Столбцы

int field[M][N] = { 0 };
// Поле с блоками

int w = 34;
// Размер одной клетки

struct Point
{
  int x, y;
}a[4], b[4];
// a, b - предыдущее состояние, текущее состояние фигуры. Зачем оно - не понятно, но оно есть

int figures[7][4] = {
1,3,5,7,
2,4,5,7,
3,5,4,6,
3,5,4,7,
2,3,5,7,
3,5,7,6,
2,3,4,5,
};
// Фигуры создаются в поле 2 на 8, отсчет от нуля, точки означают координаты 

bool check() {
  for (int i = 0; i < 4; i++)
    if (a[i].x < 0 || a[i].x >= N || a[i].y >= M)
      return 0;
    else if (field[a[i].y][a[i].x])
      return 0;
  return 1;
}
// проверяет на выход за пределы поля и пересечения с уже существующими блоками 


class PLAYER {

public:
  float dx, dy;
  FloatRect rect;
  bool onGround;
  Sprite sprite;
  float currentFrame;

  PLAYER(Texture& image)
  {
    sprite.setTexture(image);
    rect = FloatRect(7 * 34, 9 * 34, 30, 34);

    dx = dy = 0.1;
    currentFrame = 0;
  }

  
  bool checkForWin()
  {
    if(rect.top<=0)
      return 1;
    return 0;
  }  

  void update(float time)
  {
    float player_x = rect.left; 
    float player_y = rect.top;
    
    // соприкосновение со стенками окна
    if(rect.left >= 0 && rect.left+30 <= N * w)
      rect.left += dx * 9;
    else{
      if(rect.left<0)
        rect.left=0;
      if(rect.left+30>N*w)
        rect.left=N * w - 30;
    }


  //Коллизия по оси y
      if( field[int(player_y/34)][int(player_x/34)] || (field[int(player_y/34)][int((player_x+30)/34)] & (player_x + 34 <340)))
      {
        if(player_y < int(player_y/34)*34 + 34 & player_y + 34 > int(player_y/34)*34 + 34)
        {
          rect.top = int(player_y/34)*34 + 34;
          dy = 0;
          std::cout<<"UP TRUE"<<std::endl;
        }
      }

    if(field[int((player_y+34)/34)][int(player_x/34)] || (field[int((player_y+34)/34)][int((player_x+34)/34)] & (player_x+34<340)))
    {
      if(player_y+34 > int((player_y+40)/34)*34 & player_y < int((player_y+34)/34)*34)
      {
        rect.top = int((player_y)/34)*34;
        onGround=true;
      }
    }
    else
    {
      onGround=false;
    }


  // Коллизия по оси x
    if( field[int(player_y/34)][int(player_x/34)] )//|| (field[int(player_y/34)+1][int(player_x/34)]) & (player_y+34 < 34*15) )
    {
      if( rect.left-34<int(player_x/34)*34)
      {
        rect.left = (int(player_x/34)+1)*34;
        std::cout<<"LEFT tRUE"<<std::endl;
      }
    }

    else if( (player_x + 34 < 340) & (field[int(player_y/34)][int(player_x/34)+1]))// || (field[int(player_y/34)+1][int(player_x/34)+1] & (player_y+34<34*15))))
    {
      if(player_x + 30 > int(player_x/34)*34 + 34)
      {
        rect.left = (int(player_x/34))*34;
        std::cout<<"RIGHT tRUE"<<std::endl;
      }
    }

  
    if (!onGround) dy = dy + 150 * time;
    rect.top += dy * time;
    //onGround = false;

    if (rect.top + 34 >= M * w){//ground)
      rect.top  = M * w - 34;  onGround = true;
    }

    currentFrame += 0.005 * time;
    if (currentFrame > 6) currentFrame -= 6;

    if (dx > 0) sprite.setTextureRect(IntRect(0 * int(currentFrame), 0, 30, 34));
    if (dx < 0) sprite.setTextureRect(IntRect(0 * int(currentFrame) + 30, 0, -30, 34));

    sprite.setPosition(rect.left, rect.top);

    dx = 0;
  }

};

int main()
{
  srand(time(0));
  RenderWindow window(VideoMode(N * w, M * w), "Tetris!");

  Texture t;
  t.loadFromFile("/home/miros/ code/Tiles.png");
  Sprite tiles(t);
  tiles.scale(1.f,1.f);

  Texture t1;
  t1.loadFromFile("/home/miros/ code/BackgroundSciFi.png");
  Sprite pic(t1);
  pic.scale(1.7f, 1.6f);

  Texture m;
  m.loadFromFile("/home/miros/ code/pacGhostSmall.png");
  float currentFrame = 0;

  Texture wi, lo;

  Music music;
  music.openFromFile("/home/miros/ code/audio_2023-04-24_11-54-21.ogg");
  music.play();

  // int time_left_sec{30};
  // Text time_left;
    
  PLAYER p(m);

  int dx = 0, colorNum = 1;

  bool rotate = false;
  bool game_over = false;
  bool win = false;
  float timer = 0, delay = 0.3;

  Clock clock;

  bool ad = true;

  while (window.isOpen() && !Keyboard::isKeyPressed(Keyboard::Escape))
  {
    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;

    Event event;

    while (window.pollEvent(event))
    {
      if (event.type == Event::Closed)
        window.close();
      if (event.type == Event::KeyPressed)
        if (event.key.code == Keyboard::W)
          rotate = true;
        if (event.key.code == Keyboard::D)
          dx = 1;
        if (event.key.code == Keyboard::A)
          dx = -1; 

        if (event.key.code == Keyboard::I)
          if (p.onGround) { p.dy = -150;} //p.onGround = false; }

        if (event.key.code == Keyboard::L) {
          p.dx = 1;
                  }
        if (event.key.code == Keyboard::J) {
          p.dx = -1;
                  }
    }

    if (Keyboard::isKeyPressed(Keyboard::S))
      delay = 0.05;
    for (int i = 0; i < 4; i++) {
      b[i] = a[i];
      a[i].x += dx;
    }
    if (!check()) {

      for (int i = 0; i < 4; i++)

        a[i] = b[i];
    }
    if (rotate) {
      Point p = a[1];
      for (int i = 0; i < 4; i++) {
        int x = a[i].y - p.y;
        int y = a[i].x - p.x;
        a[i].x = p.x - x;
        a[i].y = p.y + y;
      }
      if (!check()) {
        for (int i = 0; i < 4; i++)
          a[i] = b[i];
      }
    }
    if (timer > delay) {
      for (int i = 0; i < 4; i++) {
        b[i] = a[i];
        a[i].y += 1;
      }
      if (!check()) {
        for (int i = 0; i < 4; i++)
          field[b[i].y][b[i].x] = colorNum;
        colorNum = 1 + rand() % 7;
        int n = rand() % 7;
        for (int i = 0; i < 4; i++) {
          a[i].x = figures[n][i] % 2;
          a[i].y = figures[n][i] / 2;
        }
      }
      timer = 0;
    }

    if (field[1][0] || field[1][1])
    {
      lo.loadFromFile("/home/miros/ code/peakpx.png");
      Sprite los(lo);
      los.scale(0.32f,0.3f);
      music.stop();
      
      SoundBuffer sad_music;
      Sound fail_sound;
      sad_music.loadFromFile("/home/miros/ code/trombone.ogg");
      fail_sound.setBuffer(sad_music);
      fail_sound.play();


      
      while(!Keyboard::isKeyPressed(Keyboard::Escape))
      {
        window.draw(los);
        window.display();
      }
      game_over = true;
    }

    if (ad) {

      int n = rand() % 7;
      if (a[0].x == 0)
        for (int i = 0; i < 4; i++) {
          a[i].x = figures[n][i] % 2;
          a[i].y = figures[n][i] / 2;
        }
      ad = false; //the first
    }

    int k = M - 1;
    for (int i = M - 1; i > 0; i--) {
      int count = 0;
      for (int j = 0; j < N; j++) {
        if (field[i][j])
          count++;
        field[k][j] = field[i][j];
      }
      if (count < N)
        k--;
    }
    
    if(p.checkForWin())
    {
      wi.loadFromFile("/home/miros/ code/you_win.png");
      Sprite winscr(wi);
      winscr.scale(0.8f,0.8f);
      while(!Keyboard::isKeyPressed(Keyboard::Escape))
      {
        window.draw(winscr);
        window.display();
      }
      game_over = true;
    }

    dx = 0;
    rotate = false;
    delay = 0.3;
    p.update(time);
    window.clear();
    window.draw(pic);
    window.draw(p.sprite);

    for (int i = 0; i < M; i++)
      for (int j = 0; j < N; j++) {
        if (field[i][j] == 0)
          continue;

        tiles.setTextureRect(IntRect(field[i][j] * w, 0, w, w));
        tiles.setPosition(j * w, i * w);
        window.draw(tiles);
      }

    for (int i = 0; i < 4; i++) {
      tiles.setTextureRect(IntRect(colorNum * w, 0, w, w));
      tiles.setPosition(a[i].x * w, a[i].y * w);
      window.draw(tiles);

    }
  
    window.display();

  }

  RenderWindow window1(VideoMode(N* w, M* w), "Tetris!");
  

  return 0;
}