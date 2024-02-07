//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html)
//Problem: Render 3D models to the screen
//Solution: 
//Extra Credit: 

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
  raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 1");
  window.SetState(FLAG_WINDOW_RESIZABLE);

  while (!window.ShouldClose())
  {
    int currentHeight = window.GetRenderHeight();
    int currentWidth = window.GetRenderWidth();

    //Begin Rendering
    window.BeginDrawing();
    window.ClearBackground(BLACK);
    window.EndDrawing();
  }

  CloseWindow();

  return 0;
}