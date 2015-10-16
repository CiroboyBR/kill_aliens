#include <allegro.h>
#include <stdio.h>

int altura = 1, largura = 407;//para o life

void _LIFE()
{
  if(tiro2 == 1) {
    largura -= 45;
    if(largura <= 1) {
      largura = 407;
      matou = 1;
    }
  }
  else
  largura -= 15;
  if(largura <= 1) {
    largura = 407;
    matou = 1;
  }
}
