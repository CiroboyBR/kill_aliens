#include<allegro.h>
#include<stdio.h>

#define VEL_NAVE    2
#define VEL_INI     5
#define VEL_TIRO    3
#define velocidade  5
//bitmaps
BITMAP *dbuffer, *fundo, *inimigo, *inimigo_invisivel, *tiro, *n_c, *n_d, *n_e, *n_t;
//para animação
BITMAP *explosao, *Menu_life = NULL;

MIDI *m_hoo;
SAMPLE *s_explo, *s_tiro, *s_turbo, *Bonus_Som;

//variáveis
int mov_fundo = -480;            //variável do movimento do fundo
int pv = 410, ph = 240;           //pv=posição verticao e ph=horizontal
//Notifica 0=Não 1=Sim
int atirou = 0,matou = 0, matou_mesmo = 0, morreu = 0, morreu_mesmo = 0, turbo = 0, TIRO_CERTO = 0;
int tiro2 = 0, tiro3 = 0;           //se for =1 o tiro é ativado

int colTiro, colNave = 0;       // Coluna do Tiro
int linTiro, N_rand = 0, Rand_Bonus = 0, Bonus = 0, Tipo_Bonus = 0, Bonus_X = 0, Bonus_Y = 0, Bonus_Status = 0;
int direcaoNave = 1;           // Direção da nave (0=esquerda, 1=direita)

int linNave = 0, invisivel = 0;

int HOR = 1, passa_frame = 0;
int area_colisao1 = 48, area_colisao2 = 43 ;//Área de colisao da nave do player e 33 ou 58 para quando a nave estiver dobrando
//estrutura para animação
struct P {
  int px, py; // posição na tela
  int height,width; // largura e altura da imagem
  float pv; // pontos de vida
  BITMAP *img;
};

struct Tiro {
  int ph, pv; // posição na tela
  int tiro;
  BITMAP *img;
};

struct Bonus_Aleatorio {
  BITMAP *img;
  SAMPLE *som;
};

struct Status {
 int sts;
};

struct P S_explo,life;//cria uma estrutura do tipo P
struct Tiro T_inimigo;//cria uma estrutura do tipo Tiro
struct Bonus_Aleatorio Reparo, Luz;//Estrutura Para os Bonus Aleatórios
struct Status Condicao_Jogador;//Estrutura Para os status do jogador

void carrega_estruturas() {
  S_explo.px = 200; //inicia o sprite na posição 400 do eixo x
  S_explo.py = 200; //inicia o sprite na posição 300 do eixo y
  S_explo.img = load_bitmap("nave_v4/explosao.bmp",NULL);//carrega bitmap
  S_explo.height = S_explo.img->w/10; //divide a sprite por seis no sentido da largura
  S_explo.width = S_explo.img->h/1;

  life.px = 570; //inicia o sprite na posição 400 do eixo x
  life.py = 465; //inicia o sprite na posição 300 do eixo y
  life.img = load_bitmap("nave_v4/Life2.bmp",NULL);//carrega bitmap
  life.height = life.img->w/1; //divide a sprite por seis no sentido da largura
  life.width = life.img->h/28;

  T_inimigo.pv = 570; //inicia o sprite na posição 400 do eixo x
  T_inimigo.ph = 465; //inicia o sprite na posição 300 do eixo y
  T_inimigo.img = load_bitmap("nave_v4/tiro_ini.bmp",NULL);

 }

void carrega_multimidia() {

  dbuffer = create_bitmap(640,960);//double buffer
  Menu_life = create_bitmap (68, 415);// cria um buffer de tela para o double buffer

  //carregando bitmaps
  n_c = load_bitmap("nave_v4/n_c.bmp", NULL);
  n_e = load_bitmap("nave_v4/n_e.bmp", NULL);
  n_d = load_bitmap("nave_v4/n_d.bmp", NULL);
  n_t = load_bitmap("nave_v4/n_turbo2.bmp", NULL);
  inimigo = load_bitmap("nave_v4/inimigo225.bmp", NULL);
  inimigo_invisivel = load_bitmap("nave_v4/inimigo_invisivel.bmp", NULL);
  tiro = load_bitmap("nave_v4/tiro.bmp", NULL);
  fundo = load_bitmap("nave_v4/fstar.bmp", NULL);
  Luz.img = load_bitmap("nave_v4/Bonus/Luz.bmp", NULL);
  Reparo.img = load_bitmap("nave_v4/Bonus/Reparo.bmp", NULL);

  //carrega os sons
  m_hoo = load_midi("nave_v4/m1.mid");
  s_explo = load_sample("nave_v4/s_explosao.wav");
  s_tiro = load_sample("nave_v4/s_tiro.wav");
  s_turbo = load_sample("nave_v4/s_turbo.wav");
  Reparo.som = load_sample("nave_v4/Bonus/Reparo_bel.wav");
  Luz.som = load_sample("nave_v4/Bonus/Luz.wav");
}
