#include <iostream>
#include <cstdlib>
#include <allegro.h>
#include <fstream>
#include <algorithm>

using namespace std;

BITMAP *buffer;
BITMAP *muertebmp;
BITMAP *tanquebmp;
BITMAP *tanque;
BITMAP *ladrillo;
BITMAP *bloque_falso;
BITMAP *trofeo;
BITMAP *bala;

//pos inicial tanque
int direccion = 0; // tanque y disparo
int direccion_bala;
int pos_x = 30*14; // enemigo tanke bala
int pos_y = 30*15; // enemigo tanque bala

int anterior_pos_x; // enemigo
int anterior_pos_y; // enemigo

//puntaje
int score_1 = 0; // enemigo
int score_2 = 0;
int score_3 = 0;

//estado del jugador
bool vida = true; // enemigo

//mapa
int const ancho = 21; // juego
int const alto = 31; // juego

//bala

int pos_b_x; // enemigo  y bala
int pos_b_y; // enemigo  y bala
int origen_b_x;  // bala
int origen_b_y;  // bala

class Juego{

public:
    static Juego *getInstancia(){
            if(instancia == 0)
                instancia = new Juego();
            return instancia;
        }
    void crearMapa();
    bool finJuego();
    void imprimirSprite();
    void Inicializar();

protected:
    char mapa[ancho][alto]={
       "MMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
       "M            | |            M",
       "M                           M",
       "M      M    FFFFF    M      M",
       "M      M             M      M",
       "M      M             M      M",
       "M|     M             M     |M",
       "M    MMMMM         MMMMM    M",
       "M             M             M",
       "M             M             M",
       "M      |      M      |      M",
       "M         MMMMMMMMM         M",
       "M             M             M",
       "M        |    M    |        M",
       "MMMMMM        M        MMMMMM",
       "M    F                 F    M",
       "M    F                 F    M",
       "M    F      MMMMM      F    M",
       "M                           M",
       "M             o             M",
       "MMMMMMMMMMMMMMMMMMMMMMMMMMMMM",
    };
    private:
        static Juego *instancia;
};

void Juego::crearMapa()
{
   for(int fila = 0; fila < ancho; fila++)
    {
        for(int columna = 0; columna < alto; columna++)
        {
            if(mapa[fila][columna] == 'M')
            {
                draw_sprite(buffer, ladrillo, columna*30, fila*30);
            }
             else if(mapa[fila][columna] == 'F')
            {
                draw_sprite(buffer, bloque_falso, columna*30, fila*30);
            }
            else if(mapa[fila][columna] == 'o')
            {
                draw_sprite(buffer, trofeo, columna*30, fila*30);
            }
        }
   }
}


bool Juego::finJuego()
{
   for(int fila = 0; fila < ancho; fila++)
    {
        for(int columna = 0; columna < alto; columna++)
        {
            if(mapa[fila][columna] == 'o')
            {
                return true;
            }
        }
   }
   return false;
}


void Juego::imprimirSprite()
{
   blit(buffer, screen, 0,0,0,0,880,630);
}

void Juego::Inicializar()
{
    buffer = create_bitmap(880, 630);
    ladrillo = load_bitmap("bloque.bmp",NULL);
    tanquebmp = load_bitmap("tanque.bmp",NULL);
    bloque_falso = load_bitmap("bloque_falso.bmp",NULL);
    tanque = create_bitmap(33, 33);
    trofeo = load_bitmap("trofeo.bmp",NULL);
    bala = load_bitmap("bala.bmp",NULL);
    muertebmp = load_bitmap("muerte.bmp", NULL);

}

class Sprite
{
    public:
        virtual void CrearSprite() = 0;
};

class Enemigo: public Sprite, public Juego
{
    public:
        Enemigo();
        Enemigo(int, int);
        ~Enemigo();
        void CrearSprite();
        void Colision();
        void movimientoEnemigo();



    private:
        BITMAP *enemigobmp;
        BITMAP *enemigo;
        //pos enemigo
        int direccion_enemigo = 0;
        int pos_x_e = 30*14;
        int pos_y_e = 30*15;
        int anterior_pos_x_e;
        int anterior_pos_y_e;
};


Enemigo::Enemigo()
{
    pos_x_e = 30*14;
    pos_y_e = 30*8;
    direccion_enemigo = rand()%4;
    enemigo = create_bitmap(30, 30);
    enemigobmp = load_bitmap("enemigo1.bmp",NULL);
}


Enemigo::Enemigo(int x, int y)
{
    pos_x_e = x;
    pos_y_e = y;
    direccion_enemigo = rand()%4;
    enemigo = create_bitmap(30, 30);
    enemigobmp = load_bitmap("enemigo2.bmp",NULL);
}
Enemigo::~Enemigo(){
    delete enemigo;
}


void Enemigo::CrearSprite()
{
    blit(enemigobmp, enemigo, 0,0,0,0,30,30);
    draw_sprite(buffer, enemigo, pos_x_e, pos_y_e);
}


void Enemigo::Colision()
{
    if((pos_y == pos_y_e && pos_x == pos_x_e) || (pos_y_e == anterior_pos_y && pos_x_e == anterior_pos_x)){
        for(int i = 0; i <= 5; i++){
            clear(tanque);
            clear(buffer);
            crearMapa();

            blit(muertebmp, tanque, i*33,0,0,0,33,33);
            draw_sprite(buffer, tanque,pos_x, pos_y);

            imprimirSprite();
            rest(80);
            vida = false;
        }
    }

    if((pos_b_y == pos_y_e && pos_b_x == pos_x_e)){
        pos_x_e = 30*50;
        pos_y_e = 30*50;
        score_1 += 10;
    }

    if(pos_x_e == 30*14 && pos_y_e == 30*19){
        vida = false;
    }
}

void Enemigo::movimientoEnemigo()
{
   CrearSprite();
   Colision();

   if(mapa[pos_y_e/30][pos_x_e/30] == '|')
    {
           direccion_enemigo = rand()%4;
    }
   if(direccion_enemigo == 0)
    {
       if((mapa[pos_y_e/30][(pos_x_e-30)/30] != 'M' ) && (mapa[pos_y_e/30][(pos_x_e-30)/30] != 'F' ))
       {
           pos_x_e -= 30;
       }
       else direccion_enemigo = rand()%4;
   }
   if(direccion_enemigo == 1)
   {
       if((mapa[pos_y_e/30][(pos_x_e+30)/30] != 'M') && (mapa[pos_y_e/30][(pos_x_e+30)/30] != 'F' ))
       {
           pos_x_e += 30;
       }
       else direccion_enemigo = rand()%4;
   }
   if(direccion_enemigo == 2)
   {
       if((mapa[(pos_y_e-30)/30][pos_x_e/30] != 'M')&& (mapa[(pos_y_e-30)/30][(pos_x_e)/30] != 'F' ))
       {
           pos_y_e -= 30;
       }
       else direccion_enemigo = rand()%4;
   }
   if(direccion_enemigo == 3)
   {
       if((mapa[(pos_y_e+30)/30][pos_x_e/30] != 'M')&&(mapa[(pos_y_e+30)/30][(pos_x_e)/30] != 'F' ))
       {
           pos_y_e += 30;
       }
       else direccion_enemigo = rand()%4;
   }
}

class Tanque: public Sprite, public Juego
{
    public:
        bool activo;
        void CrearSprite();
        void direccionamiento(int &, int &, int &);
};

void Tanque::direccionamiento(int &direccion, int &pos_x, int &pos_y)
{

       if(key[KEY_LEFT]){
           direccion = direccion_bala = 0;
            activo = true;
        }
       else if(key[KEY_RIGHT]){
           direccion = direccion_bala = 1;
            activo = true;
        }
       else if(key[KEY_UP]){
           direccion = direccion_bala = 2;
            activo = true;
        }
       else if(key[KEY_DOWN]){
           direccion =direccion_bala =  3;
            activo = true;
        }
           if(direccion == 0 && activo == true){
                 if((mapa[pos_y/30][(pos_x-30)/30] != 'M') && (mapa[pos_y/30][(pos_x-30)/30] != 'F')){
                   pos_x -= 30;
                   activo = false;
                }
                else
                    direccion = 0;
           }

           if(direccion == 1 && activo == true){
                if((mapa[pos_y/30][(pos_x+30)/30] != 'M')&&(mapa[pos_y/30][(pos_x+30)/30] != 'F')){
                   pos_x += 30;
                    activo = false;
                }
                else
                    direccion = 1;
           }

           if(direccion == 2 && activo == true){
                if((mapa[(pos_y-30)/30][pos_x/30] != 'M')&&(mapa[(pos_y-30)/30][pos_x/30] != 'F')){
                  pos_y -= 30;
                  activo = false;
                }
                else
                    direccion = 2;
           }

           if(direccion == 3 && activo == true){
                if((mapa[(pos_y+30)/30][pos_x/30] != 'M')&&(mapa[(pos_y+30)/30][pos_x/30] != 'F') ){
                   pos_y += 30;
                   activo = false;
                }
                else
                    direccion = 3;
           }
}

void Tanque::CrearSprite()
{
    blit(tanquebmp, tanque, direccion*33,0,0,0,33,33);
    draw_sprite(buffer, tanque, pos_x, pos_y);
}

class Bala: public  Sprite, public Juego{
    public:
        void disparo(int &, int&,int );
        void CrearSprite();

};


void Bala::disparo(int &pos_b_x, int &pos_b_y,int direccion)
{
    origen_b_x = pos_x;
    origen_b_y = pos_y;

    if(direccion == 0){
        if(key[KEY_SPACE]){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
        }
        pos_b_x -= 30;
        if(mapa[pos_b_y/30][pos_b_x/30] == 'M'  || mapa[pos_b_y/30][pos_b_x/30] == 'F' )
            pos_b_x = 30*100;
    }

    if(direccion == 1){
        if(key[KEY_SPACE]){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
        }
        pos_b_x += 30;
        if(mapa[pos_b_y/30][pos_b_x/30] == 'M'  ||  mapa[pos_b_y/30][pos_b_x/30] == 'F'  ){
            pos_b_x = 30*100;
    }
    }
    if(direccion == 2){
        if(key[KEY_SPACE]){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
        }
        pos_b_y -= 30;
        if(mapa[pos_b_y/30][pos_b_x/30] == 'M'   ||  mapa[pos_b_y/30][pos_b_x/30] == 'F'){
            pos_b_x = 30*100;
      }
    }

    if(direccion == 3){
        if(key[KEY_SPACE]){
            pos_b_x = origen_b_x;
            pos_b_y = origen_b_y;
        }
        pos_b_y += 30;
        if(mapa[pos_b_y/30][pos_b_x/30] == 'M'  ||  mapa[pos_b_y/30][pos_b_x/30] == 'F'){
            pos_b_x = 30*100;
       }
    }
}

void Bala::CrearSprite()
{
    draw_sprite(buffer, bala, pos_b_x, pos_b_y);
}

Juego *Juego::instancia = 0;


template<typename T>
void quicksort(T *arr,int primero, int ultimo,bool (*pf)(T,T))
{
    int i=primero,j = ultimo;
    T auxi;
    T pivote  = arr[(primero + ultimo)/2];
    do
    {
        while((*pf)(pivote,arr[i]))i++;
            while((*pf)(arr[j],pivote))j--;
                if(i<=j)
                {
                    auxi = arr[i];
                    arr[i] = arr[j],
                    arr[j]=auxi;
                    i++;
                    j--;
                }
    }
        while(i<=j);
            if(primero<j)
                quicksort(arr,primero,j,pf);
            if(ultimo>i)
                quicksort(arr,i,ultimo,pf);
}

bool ascendente(char *a, char *b)
{
return *a > *b;
}

bool descendente(char *a, char *b)
{
return *a < *b;
}

void mostrar(int arr[], int len){
for(int i=0; i<len; i++)
        cout<< len-i << "º " << arr[i] << endl;
}

int main()
{

    allegro_init();
	install_keyboard();
	install_mouse();

	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 880, 630, 0, 0);


	Juego *configuracion = Juego::getInstancia();
    configuracion->Inicializar();

    Enemigo *A = new Enemigo(30*13, 30*5);
    Enemigo *B = new Enemigo(30*14, 30*5);
    Enemigo *C = new Enemigo(30*15, 30*5);
    Enemigo *D = new Enemigo(30*16, 30*5);
    Enemigo *E = new Enemigo;
    Enemigo *F = new Enemigo;
    Enemigo *G = new Enemigo;
    Enemigo *H = new Enemigo;
    Enemigo *I = new Enemigo;
    Enemigo *J = new Enemigo;


    Tanque *tan = new Tanque;
    Bala *balin = new Bala;
    bool nivel1 = true;
    bool nivel2 = false;


	//define variables del menu
	BITMAP *buffer2 = create_bitmap(880, 630);
	BITMAP *fondo1;
	BITMAP *fondo2;
	BITMAP *fondo3;
	BITMAP *cursor;

	// inicializa las variables del menu
	fondo1 = load_bitmap("Menu1.bmp",NULL);
	fondo2 = load_bitmap("Menu2.bmp",NULL);
	fondo3 = load_bitmap("Menu3.bmp",NULL);
	cursor = load_bitmap("cursor.bmp",NULL);

    bool salida = false;

	while ( !salida )
	{
         // posicion del boton
         if ( mouse_x > 377 && mouse_x < 530 &&
              mouse_y > 387 && mouse_y < 428)
         {
            blit(fondo2, buffer2, 0, 0, 0, 0, 880, 630);

            // se ha pulsado el boton del raton
            if ( mouse_b & 1 )
            {
                salida=true;
            }
         }

         else if ( mouse_x > 377 && mouse_x < 530 &&
              mouse_y > 430 && mouse_y < 471)
         {
            blit(fondo3, buffer2, 0, 0, 0, 0, 880, 630);

            // se ha pulsado el boton del raton
            if ( mouse_b & 1 )
            {
                allegro_exit();
            }
         }else{
            blit(fondo1, buffer2, 0, 0, 0, 0, 880, 630);
         }


         // pinta el cursor
         masked_blit(cursor, buffer2, 0, 0, mouse_x, mouse_y, 13,22);

         // se muestra todo por pantalla
	     blit(buffer2, screen, 0, 0, 0, 0, 880, 630);

    }
    // libera memoria quitando las imagenes

	clear(buffer2);
	clear(fondo1);
	clear(fondo2);
	clear(fondo3);
	clear(cursor);

    while(!key[KEY_ESC] && configuracion->finJuego() && vida && nivel1){

       anterior_pos_x = pos_x;
       anterior_pos_y = pos_y;

       tan->direccionamiento(direccion, pos_x, pos_y);
       balin->disparo(pos_b_x, pos_b_y,direccion_bala);


       clear(buffer);
	   configuracion->crearMapa();
	   tan->CrearSprite();
	   balin->CrearSprite();


	   A->movimientoEnemigo();
	   B->movimientoEnemigo();
	   C->movimientoEnemigo();
	   D->movimientoEnemigo();

	   configuracion->imprimirSprite();
	   rest(20);

	   clear(tanque);
       configuracion->imprimirSprite();
       rest(70);

       if(score_1 == 40){
        nivel1 = false;
       }

       ofstream guardar_puntaje;
       guardar_puntaje.open("Puntuacion_obtenida_1.txt");

       guardar_puntaje << "Tu puntuacion fue:" << score_1;
       guardar_puntaje.close();
    }

    if(nivel1 == false)
        nivel2 = true;

    while(!key[KEY_ESC] && configuracion->finJuego() && vida && nivel2){

       anterior_pos_x = pos_x;
       anterior_pos_y = pos_y;

       tan->direccionamiento(direccion, pos_x, pos_y);
       balin->disparo(pos_b_x, pos_b_y,direccion_bala);


       clear(buffer);
	   configuracion->crearMapa();
	   tan->CrearSprite();
	   balin->CrearSprite();


	   E->movimientoEnemigo();
	   F->movimientoEnemigo();
	   G->movimientoEnemigo();
	   H->movimientoEnemigo();
	   I->movimientoEnemigo();
	   J->movimientoEnemigo();

	   configuracion->imprimirSprite();
	   rest(20);

	   clear(tanque);
       configuracion->imprimirSprite();
       rest(60);

       score_2 = score_1-40;

       if(score_1 == 100){
        nivel1 = false;
       }

       ofstream guardar_puntaje;
       guardar_puntaje.open("Puntuacion_obtenida_2.txt");

       guardar_puntaje << "Tu puntuacion fue:" << score_2;
       guardar_puntaje.close();
    }

    allegro_exit();

    char *nombres[4] = {"rodrigo","sebastian","yhon"};
    int  puntajes[4] = {60,40,30};

    string nombre;
    cout<<"\t\t*   GAME OVER  *"<<endl;
    cout<<"\t\t***"<<endl;
    cout<<"\t\t*INGRESE SU NOMBRE  *"<<endl;
    cout<<"\t\t***"<<endl;
    cout<<">>";
    cin>>nombre;

    cout << "Jugadores anteriores: " << endl;
    quicksort<char*>(nombres,0,2,descendente);
    cout<<endl<<endl;
    cout << "descendente :" << endl;
    for(int i=0; i<3; i++)
        cout<< nombres[i] << endl;

    quicksort<char*>(nombres,0,2,ascendente);
    cout<<endl<<endl;
    cout << "ascendente :" << endl;
    for(int i=0; i<3; i++)
        cout<< nombres[i] << endl;

    cout << endl;

    cout << "Puntuaciones mas altas: " << endl;

    sort(puntajes,puntajes+3);
    mostrar(puntajes, 3);

    cout << endl;

    cout << "Tu puntuacion: " << endl;
    cout << nombre << " " << score_1;



	return 0;
}
END_OF_MAIN();
