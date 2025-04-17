#include<iostream>
#include <windows.h>

using namespace std;

void Draw8points(HDC hdc,int Xc,int Yc,int X,int Y, COLORREF c){

    SetPixel(hdc, Xc + X, Yc + Y, c);
    SetPixel(hdc, Xc + X, Yc - Y, c);
    SetPixel(hdc, Xc - X, Yc + Y, c);
    SetPixel(hdc, Xc - X, Yc - Y, c);

    SetPixel(hdc, Xc + Y, Yc + X, c);
    SetPixel(hdc, Xc - Y, Yc + X, c);
    SetPixel(hdc, Xc + Y, Yc - X, c);
    SetPixel(hdc, Xc - Y, Yc - X, c);

}

void Bresenhams_Circle(int Xc,int Yc,int r,COLORREF c){
    int x = 0, y = r, d = 1- r;
     while(x<y){
         if(d<0)
             d += 2 * x + 3;
          else
              d += 2 * (x - y) + 5,y--;

            x++;

            Draw8points();
     }
}