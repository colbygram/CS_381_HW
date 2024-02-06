//Author: Colby Gramelspacher
//Reference: raylib github example (https://github.com/raysan5/raylib) & raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html)
//Problem: Create a resizable window with text that stays centered as window is resized
//Solution: Keep track of windows current width and height after every window loop. Use tracked width and height to reposition the text according to the current dimensions
//Extra Credit: Added multicolor fading on the text

#include <iostream>
#include "../raylib-cpp/include/raylib-cpp.hpp"

int main(void)
{
  //Constants
  const int WIDTH = 960;
  const int HEIGHT = 540; 
  const char* CENTER_TEXT = "THIS IS THE CENTER TEXT";
  const int FONT_SIZE = 20;

  //Raylib Objects
  raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 0");
  window.SetState(FLAG_WINDOW_RESIZABLE);
  raylib::Text text(CENTER_TEXT, FONT_SIZE, WHITE, GetFontDefault(), 1.5f);
  int TEXT_WIDTH = text.Measure();
  raylib::Color text_color(0,0,0,255);

  //Multicolor Variables
  bool isReverse = false; //Determines whether the colors need to increment or decrement
  int r=0,g=0,b=0;

  while (!window.ShouldClose())
  {
    //Multicolor happens due to values being incremented individually until all 3 are at the max 255, which they then reverse and decrement until all are zero again
    if(!isReverse){
      if(r != 255){
        r++;
        text_color.SetR(r);
      } else if(g != 255){
        g++;
        text_color.SetG(g);
      } else if(b != 255){
        b++;
        text_color.SetB(b);
      } else{
        isReverse = true;
      }
    } else{
      if(r != 0){
        r--;
        text_color.SetR(r);
      } else if(g != 0){
        g--;
        text_color.SetG(g);
      } else if(b != 0){
        b--;
        text_color.SetB(b);
      } else{
        isReverse = false;
      }
    }
    text.SetColor(text_color);

    int currentHeight = window.GetRenderHeight();
    int currentWidth = window.GetRenderWidth();

    //Begin Rendering
    window.BeginDrawing();
    window.ClearBackground(GRAY);
    text.Draw((currentWidth/2) - (TEXT_WIDTH/2), (currentHeight / 2));
    window.EndDrawing();
  }

  CloseWindow();

  return 0;
}