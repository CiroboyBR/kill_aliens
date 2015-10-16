#include <allegro.h>
#include <stdio.h>
#include "Nave2010.h"
#include "Nave_HP.h"

void init_allegro();
void carrega_multimidia();
void carrega_estruturas();
void colisao();
void colisao_tiro_inimigo();
void Colisao_Bonus();
void atirando();
void tiro_inimigo();
void mov_f();
void mov_jogador();
void mov_ini1();
void des_map();
void anim_esplosao();
void _LIFE();
void Eventos_Aleatorios();
void Reinicializa_Bonus();
int Sorteia_Hide(int n);
void Desenha_Eventos();

//----------------------------------------------------------------------------//
int main(int argc, char *argv[])
{
  srand(time(NULL));
  //inicia a alegro
  init_allegro();
  install_timer();
  //allegro_message("feito por:Ciro Martins Meneses");
  //allegro_message("aperte!\n[espaco] para atirar");

  carrega_estruturas();
  carrega_multimidia();


  //alinha o tiro com a linha da nave
  linTiro = pv;
  colNave = 200;

  set_hardware_volume(100, 50);//volimi
  //toca a música
  play_midi(m_hoo, 1);
  fade_in(black_palette, 10);
     //Ajusta volume

     // Fica em loop até pressionar a tecla ESC
	while( !key[KEY_ESC]) {
    mov_jogador();
    colisao();
    colisao_tiro_inimigo();
    mov_f();
    // Imprime Nave
    //draw_sprite(dbuffer, inimigo, colNave, linNave);
    mov_ini1();
    anim_esplosao();
    Eventos_Aleatorios();
    des_map();
    rest( velocidade );
    if ( matou_mesmo == 1 ) {
      remove_sound();
      allegro_message("YES! VOCE DESTRUIU A NAVE ALIEN!!!!!!!!!!!!!");
      break;
    }
    if (morreu_mesmo == 1) {
      remove_sound();
      allegro_message("A nave alien mandou uma onda eletromagnetica e ''Torrou'' o seu sistema!!!");
      break;
    }
    else;
  }

  stop_midi();

  fade_out(2);
	// Sai do programa
	allegro_exit();
}
// Esta macro converte o main para WinMain()
END_OF_MAIN();
//----------------------------------------------------------------------------//
//função que inicia a allegro
void init_allegro() {

	// Inicializa a biblioteca Allegro
	allegro_init();
    // Inicializa o manipulador de teclado
	install_keyboard();
	// Define a profundidade de cores para 16 bits (high color)
	set_color_depth(16);
	// Define a resolução de 640x480 no modo automatico e sem tela cheia
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
    //inicializa os sons
  if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, 0)) {
    allegro_message("Nao foi possivel iniciar o Som");
  }

}
//----------------------------------------------------------------------------//
//função que desenha o mapa

void des_map() {

  if( Bonus == 1){
    Desenha_Eventos();
  }

  if (morreu == 0){
     if (key[KEY_LEFT] || key[KEY_A]) {
        draw_sprite( dbuffer, n_e, ph, pv);
        area_colisao1 = 33;
        area_colisao2 = 28;
        if (linNave <= 0)
          linNave++;
             goto g_tiro;}
     else
     if (key[KEY_RIGHT] || key[KEY_D]) {
       draw_sprite( dbuffer, n_d, ph, pv);
       area_colisao1 = 33;
       area_colisao2 = 28;
       if ( linNave <= 0 )
         linNave++;

       goto g_tiro;
     }
     if (key[KEY_UP] || key[KEY_W]) {
       draw_sprite( dbuffer, n_t, ph, pv);
       if( linNave <= 0 )
         linNave++;

       goto g_tiro;
     }
     else; {
       draw_sprite( dbuffer, n_c, ph, pv);
       area_colisao1 = 48;
       area_colisao2 = 43;
       if( linNave <= 0 )
         linNave++;
     }
  }
  //criei um label
  g_tiro:

  atirando();
  tiro_inimigo();
  //Animação de Explosão na morte do Jogador
  if ( morreu == 1 ) {
    S_explo.px = ph;
    S_explo.py = pv;
    masked_blit (S_explo.img, dbuffer, HOR, 0, S_explo.px,S_explo.py, S_explo.height, S_explo.width);
    rest(100);
    passa_frame++;
    if ( passa_frame > 9 ) {
      morreu = 0;
      passa_frame = 0;
      morreu_mesmo = 1;
    }
  }
   //Animação de Explosão na morte do Inimigo
  if ( matou == 1 ) {
    S_explo.px = colNave;
    S_explo.py = linNave;
    masked_blit (S_explo.img, dbuffer, HOR, 0, S_explo.px,S_explo.py, S_explo.height, S_explo.width);
    rest(100);
    passa_frame++;
    if ( passa_frame > 9 ) {
      passa_frame = 0;
      matou_mesmo = 1;
    }
  }
  //Desenha a descida do Bonus na tela caso tenha ocorrido o evento


  //Desenha os HUB s
  blit (life.img, Menu_life, altura, largura, 0, 0, life.height, life.width);
  //Double Buffer do Menu_life para o buffer
  blit (Menu_life,dbuffer, 0, 0, life.px, life.py, 67, 12);


  textprintf_ex(dbuffer, font, 10, 10, makecol(100, 100, 200), -1, "Bonus: %d, Tipo: %d", Bonus, Tipo_Bonus);
    textprintf_ex(dbuffer, font, 10, 20, makecol(100, 100, 200), -1, "Bonus X: %d, Y: %d",Bonus_X, Bonus_Y);
  textprintf_ex(dbuffer, font, 10, 30, makecol(100, 100, 200), -1, "Player PH: %d, PV: %d", ph, pv );
  //double buffer
  masked_blit(dbuffer, screen, 0, 0, 0, 0, 640, 480);
  clear(dbuffer);
}
//----------------------------------------------------------------------------//

//função que controla o jogador
void mov_jogador() {

  if (key[KEY_LEFT] || key[KEY_A])
    ph -= VEL_NAVE;
  else
  if (key[KEY_RIGHT] || key[KEY_D])
    ph += VEL_NAVE;
  else
  if (key[KEY_UP] || key[KEY_W]) {
    pv -= VEL_NAVE;
    mov_fundo += 1;
  }
  else
  if (key[KEY_DOWN] || key[KEY_S]) {
    pv += VEL_NAVE;
    mov_fundo --;
  }

  if ( key[KEY_T] ) {
    if (tiro2 == 1)
      tiro2 = 0;
    else
    if (tiro2 == 0)
      tiro2 = 1;
  }
}
//detecta a colisão com as paredes e tiros
void colisao() {

  if (ph <= 0)
    ph = 0;

  if (ph >= 580)
    ph = 580;

  if (pv <= 60)
    pv = 60;

  if (pv >= 420)
    pv = 420;

  if(linTiro <= 40 && linTiro > 0) {
    //Colisão do Tiro disparado contra a Nave Inimiga
    if(tiro2 == 1) {
      if(colTiro >= colNave-10 && colTiro <= colNave+88 ) {
        TIRO_CERTO = 1;
        play_sample(s_explo, 255, 128, 1000, 0);
        _LIFE();
        atirou = 0;
        linTiro = pv;
      }
    }
    else;
    if(colTiro >= colNave+2 && colTiro <= colNave+75 ) {
    TIRO_CERTO = 1;
    play_sample(s_explo, 255, 128, 1000, 0);
    _LIFE();
    atirou = 0;
    linTiro = pv;
    }
  }
}

void colisao_tiro_inimigo()
{
  //Aqui a detecção de colisão do tiro do inimigo com a nave
  if (T_inimigo.pv >= pv-10 && T_inimigo.pv <= pv+20 ) {
    if (T_inimigo.ph + area_colisao1 >= ph && T_inimigo.ph - area_colisao2 <= ph ) {
      play_sample(s_explo, 255, 128, 1000, 0);
      morreu = 1;
      T_inimigo.tiro = 0;
      T_inimigo.pv = linNave;
    }
  }

   //---------da parte de cima da nave--------------
  if (T_inimigo.pv >= pv-45 && T_inimigo.pv <= pv-20 ) {
    if (T_inimigo.ph+25  >=ph && T_inimigo.ph-15  <= ph ) {
      play_sample(s_explo, 255, 128, 1000, 0);
      morreu = 1;
      T_inimigo.tiro = 0;
      T_inimigo.pv = linNave;
    }
  }
}

void atirando() {
       // Verifica se deve atirar
  if ( morreu == 0 && atirou == 0 && key[KEY_SPACE] ) {
    play_sample(s_tiro, 100, 128, 1000, 0);
    atirou = 1;
    colTiro = ph+28;
    linTiro = pv;
  }
  // Verifica se existe um tiro disparado
  if ( atirou == 1 ) {
    if (tiro2 == 1) {
        // Imprime tiro triplo na tela
      draw_sprite(dbuffer, tiro, colTiro, linTiro);
      draw_sprite(dbuffer, tiro, colTiro-12, linTiro);
      draw_sprite(dbuffer, tiro, colTiro+12, linTiro);
      // Movimenta o tiro
      linTiro = linTiro - VEL_TIRO;
    }
    else {
      // Imprime tiro na tela
      draw_sprite(dbuffer, tiro, colTiro, linTiro);
      // Movimenta o tiro
      linTiro = linTiro - VEL_TIRO;
    }
  }
  // Verifica se o tiro morreu saindo da tela
  if ( linTiro <= -0 ) {
    atirou = 0;
    linTiro = pv;
  }
}

//fução que movimenta o fundo da tela
void mov_f() {


  //movimenta o fundo de cima para baixo
  mov_fundo += 2;
  //desenha o fundo
  blit(fundo, dbuffer, 0, 0, 0, mov_fundo, 640, 960);
  //verifica se o fundo chegou no 0 e volta ele para a posição -960
  if(mov_fundo >= 0)
    mov_fundo =- 480;
}

void mov_ini1() {
  // Imprime Nave
  if( matou != 1 && largura >= 50 || largura <= 50 && Sorteia_Hide(invisivel) > 1990 || Condicao_Jogador.sts == 2) {
    draw_sprite(dbuffer, inimigo, colNave, linNave);
  }

  // Momvimenta Nave
  if ( direcaoNave == 1 )
    colNave += 1;
  else
    colNave -= 1;

  //Evento que define o numero aleatório para o Inimigo sair do Invisível
  if (atirou == 1) {
    //manobra evasiva I.A
    if ( colNave + 40 <= colTiro && linTiro <= 200 && colNave >= 10)
      direcaoNave = 0;
    else
    if ( colNave + 40 > colTiro && linTiro <= 200 && colNave <= 550)
      direcaoNave = 1;
  }
  else {
    // Verifica limites da tela para nave
    if ( colNave <= 0)
      direcaoNave = 1;
    else
    if ( colNave >= 560)
      direcaoNave = 0;
  }
}

//Divide o Tile da explosão em 10 partes de 64
void anim_esplosao() {
  HOR += 64;
  if(HOR > 640) {
    HOR = 0;
    TIRO_CERTO = 0;
  }
}

void tiro_inimigo()
{
  // Verifica se deve atirar
  if ( T_inimigo.tiro == 0 && colNave+32 >= ph) {
    if ( colNave-32 <= ph) {
      T_inimigo.tiro = 1;
      play_sample(s_tiro, 100, 128, 1000, 0);
      T_inimigo.ph = colNave;
    }
  }
  if ( T_inimigo.tiro == 1){
    // Imprime tiro na tela
    draw_sprite_v_flip(dbuffer, T_inimigo.img, T_inimigo.ph+20, T_inimigo.pv+32);
    // Movimenta o tiro
    T_inimigo.pv = T_inimigo.pv + VEL_TIRO;
  }
  // Verifica se o tiro morreu saindo da tela
  if ( T_inimigo.pv >= 440 ) {
    T_inimigo.tiro = 0;
    T_inimigo.pv = linNave;
  }
}

int Sorteia_Hide(int n) {
  n = rand() % 2001;
  return n;
}

void Eventos_Aleatorios() {
  int n;
  n = rand() % 10000 + 1;

  if ( Bonus != 1 && n > 9990){
    Bonus = 1;
    Tipo_Bonus = rand() % 2 + 1;
    Bonus_X = rand() % 500 + 40;
  }
}

void Desenha_Eventos() {

  if ( Bonus == 1 ){
    if ( Tipo_Bonus == 1){
      draw_sprite(dbuffer, Reparo.img, Bonus_X, Bonus_Y);
      Bonus_Som = Reparo.som;


    }
    else{
      draw_sprite(dbuffer, Luz.img, Bonus_X, Bonus_Y);
      Bonus_Som = Luz.som;
    }

      Bonus_Status = Tipo_Bonus;
    //if (key[KEY_P] ) //Fins de teste
      Bonus_Y ++;//Movimenta os Bonus

    if ( Bonus_Y >= 470)//Limite para o Bonus desaparecer
      Reinicializa_Bonus();

    //Testa as Colisoes entre a nave do jogador e os bonus
    Colisao_Bonus();

  }
}
void Colisao_Bonus() {
  //Aqui a detecção de colisão do tiro do inimigo com a nave
  if ( Bonus_Y >= pv && Bonus_Y <= pv+50 ) {
    if ( Bonus_X + 29 >= ph && Bonus_X - 64 <= ph ) {
      play_sample(Bonus_Som, 255, 128, 1000, 0);
      Condicao_Jogador.sts = Bonus_Status;
      Reinicializa_Bonus();
    }
  }

  //---------da parte frontal da nave--------------
  if ( Bonus_Y >= pv-30 && Bonus_Y <= pv+10 ) {
    //play_sample(s_explo, 255, 128, 1000, 0);
    if ( Bonus_X+5  >= ph && Bonus_X  <= ph+40 ) {
      play_sample(Bonus_Som, 255, 128, 1000, 0);
      Condicao_Jogador.sts = Bonus_Status;
      Reinicializa_Bonus();
    }
  }
}

void Reinicializa_Bonus() {
   Bonus = 0;
   Tipo_Bonus = 0;
   Bonus_X = 0;
   Bonus_Y = 0;

}
