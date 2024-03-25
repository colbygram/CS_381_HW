//Author: Colby Gramelspacher
//Reference: raylib cheatsheet (https://www.raylib.com/cheatsheet/cheatsheet.html), 
//Problem: Implement an audio gui where buttons can be pressed to play soundsf
//Solution:  
//Extra Credit: 

#include "iostream"
#include "raylib-cpp.hpp"
#include "raygui.h"

int main(){
    //Consts
    const int WIDTH = 1920/2;
    const int HEIGHT = 1080/2;

    //Window and camera objects
    raylib::Window window(WIDTH, HEIGHT, "CS 381 - Assignment 5");

    while (!window.ShouldClose())
    { 
        window.BeginDrawing();
            ClearBackground(GRAY);
        window.EndDrawing();
    }
    return 0;
}