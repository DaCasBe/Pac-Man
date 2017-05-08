#include <allegro.h>
#include <cstdlib>

#define MAX_FILAS 20    //para el eje X
#define MAX_COLUMNAS 30 //para el eje Y

BITMAP * buffer;
BITMAP * muro;
BITMAP * pacbmp;
BITMAP * pacman;
BITMAP * comida;
BITMAP * muertebmp;
MIDI * musica_fondo;
int direccion=4;
int posicion_x=30*14;
int posicion_y=30*14;
int posicion_anterior_x;
int posicion_anterior_y;

char mapa[MAX_FILAS][MAX_COLUMNAS]{
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
	"Xo o |o o o XXXXX o o o| o oX",
	"X XXX XXXXX XXXXX XXXXX XXX X",
	"XoXXX XXXXX XXXXX XXXXX XXXoX",
	"X    | o|o o o o o o|o |    X",
	"XoXXXoXX XXXXXXXXXXX XXoXXXoX",
	"X|   |XX     XXX     XX|   |X",
	"XoXXXoXXXXXX XXX XXXXXXoXXXoX",
	"X XXXoXX ooo|ooo|ooo XXoXXX X",
	" o   |XX XXXXXXXXXXX XX|   o ",
	"X XXXoXX ooo|ooo|ooo XXoXXX X",
	"XoXXXoXXXXXX XXX XXXXXXoXXXoX",
	"X|   |XX     XXX     XX|   |X",
	"XoXXXoXX XXXXXXXXXXX XXoXXXoX",
	"X    | o|o o o o o o|o |    X",
	"XoXXX XXXXX XXXXX XXXXX XXXoX",
	"X XXX XXXXX XXXXX XXXXX XXX X",
	"Xo o |o o o XXXXX o o o| o oX",
	"XXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
};

void dibujar_mapa(){
	int fila;
	int columna;
	
	for(fila=0;fila<MAX_FILAS;fila++){
		for(columna=0;columna<MAX_COLUMNAS;columna++){
			if(mapa[fila][columna]=='X'){
				draw_sprite(buffer,muro,columna*30,fila*30);
			}
			else if(mapa[fila][columna]=='o'){
				draw_sprite(buffer,comida,columna*30,fila*30);
				
				if(posicion_y/30==fila && posicion_x/30==columna){
					mapa[fila][columna]=' ';
				}
			}
		}
	}
}

void pantalla(){
	blit(buffer,screen,0,0,0,0,880,570);
}

void dibujar_pacman(){
	blit(pacbmp,pacman,direccion*33,0,0,0,33,33);
	draw_sprite(buffer,pacman,posicion_x,posicion_y);
}

bool game_over(){
	int fila;
	int columna;
	
	for(fila=0;fila<MAX_FILAS;fila++){
		for(columna=0;columna<MAX_COLUMNAS;columna++){
			if(mapa[fila][columna]=='o'){
				return true;
			}
		}
	}
	
	return false;
}

class Enemigo{
	private:
		BITMAP * enemigobmp_;
		BITMAP * enemigo_;
		int direccion_enemigo_;
		int posicion_enemigo_x_;
		int posicion_enemigo_y_;
		
	public:
		Enemigo(int posicion_enemigo_x,int posicion_enemigo_y);
		void setX(int posicion_enemigo_x);
		int getX();
		void setY(int posicion_enemigo_y);
		int getY();
		void setDireccion(int direccion_enemigo);
		int getDireccion();
		const void dibujar_enemigo();
		void mover_enemigo();
		void muerte_pacman();
};

Enemigo::Enemigo(int posicion_enemigo_x,int posicion_enemigo_y){
	posicion_enemigo_x_=posicion_enemigo_x;
	posicion_enemigo_y_=posicion_enemigo_y;
	direccion_enemigo_=rand()%4;
	enemigobmp_=load_bitmap("enemigo.bmp",NULL);
	enemigo_=create_bitmap(30,30);
}

void Enemigo::setX(int posicion_enemigo_x){
	posicion_enemigo_x_=posicion_enemigo_x;
}

int Enemigo::getX(){
	return posicion_enemigo_x_;
}

void Enemigo::setY(int posicion_enemigo_y){
	posicion_enemigo_y_=posicion_enemigo_y;
}

int Enemigo::getY(){
	return posicion_enemigo_y_;
}

void Enemigo::setDireccion(int direccion_enemigo){
	direccion_enemigo_=direccion_enemigo;
}

int Enemigo::getDireccion(){
	return direccion_enemigo_;
}

const void Enemigo::dibujar_enemigo(){
	blit(enemigobmp_,enemigo_,0,0,0,0,30,30);
	draw_sprite(buffer,enemigo_,getX(),getY());
}

void Enemigo::mover_enemigo(){
	dibujar_enemigo();
	
	muerte_pacman();
	
	if(mapa[getY()/30][getX()/30]=='|'){
		setDireccion(rand()%4);
	}
	
	if(getDireccion()==0){
		if(mapa[getY()/30][(getX()-30)/30]!='X'){
			setX(getX()-30);
		}
		else{
			setDireccion(rand()%4);
		}
	}
	
	if(getDireccion()==1){
		if(mapa[getY()/30][(getX()+30)/30]!='X'){
			setX(getX()+30);
		}
		else{
			setDireccion(rand()%4);
		}
	}
	
	if(getDireccion()==2){
		if(mapa[(getY()-30)/30][getX()/30]!='X'){
			setY(getY()-30);
		}
		else{
			setDireccion(rand()%4);
		}
	}
	
	if(getDireccion()==3){
		if(mapa[(getY()+30)/30][getX()/30]!='X'){
			setY(getY()+30);
		}
		else{
			setDireccion(rand()%4);
		}
	}
	
	if(getX()<=-30){
		setX(870);
	}
	else if(getX()>=870){
		setX(getX()-30);
	}
}

void Enemigo::muerte_pacman(){
	int i;
	
	if(posicion_y==getY() && posicion_x==getX() || posicion_anterior_y==getY() && posicion_anterior_x==getX()){
		for(i=0;i<=5;i++){
			clear(pacman);
			clear(buffer);
			dibujar_mapa();
			
			blit(muertebmp,pacman,i*33,0,0,0,33,33);
			draw_sprite(buffer,pacman,posicion_x,posicion_y);
			
			pantalla();
			rest(80);
		}
		
		posicion_x=30*14;
		posicion_y=30*14;
		direccion=4;
	}
}

int main(){
	allegro_init();
	install_keyboard();
	
	set_color_depth(32);
	set_gfx_mode(GFX_AUTODETECT_WINDOWED,880,570,0,0);
	
	if(install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL)!=0){
		allegro_message("Error: inicializando sistema de sonido\n%s\n",allegro_error);
		return 1;
	}
	
	set_volume(70,70);
	
	buffer=create_bitmap(880,600);
	muro=load_bitmap("muro.bmp",NULL);
	pacbmp=load_bitmap("pacman.bmp",NULL);
	pacman=create_bitmap(33,33);
	comida=load_bitmap("comida.bmp",NULL);
	muertebmp=load_bitmap("muerte.bmp",NULL);
	musica_fondo=load_midi("PacMan Original Theme.mid");
	
	Enemigo enemigo1(30*3,30*1);
	Enemigo enemigo2(30*3,30*17);
	Enemigo enemigo3(30*27,30*15);
	Enemigo enemigo4(30*27,30*1);
	Enemigo enemigo5(30*14,30*8);
	
	play_midi(musica_fondo,1);
	
	while(!key[KEY_ESC] && game_over()){
		posicion_anterior_x=posicion_x;
		posicion_anterior_y=posicion_y;
		
		if(key[KEY_LEFT]){
			direccion=0;
		}
		else if(key[KEY_RIGHT]){
			direccion=1;
		}
		else if(key[KEY_UP]){
			direccion=2;
		}
		else if(key[KEY_DOWN]){
			direccion=3;
		}
		
		if(direccion==0){
			if(mapa[posicion_y/30][(posicion_x-30)/30]!='X'){
				posicion_x=posicion_x-30;
			}
			else{
				direccion=4;
			}
		}
		if(direccion==1){
			if(mapa[posicion_y/30][(posicion_x+30)/30]!='X'){
				posicion_x=posicion_x+30;
			}
			else{
				direccion=4;
			}
		}
		if(direccion==2){
			if(mapa[(posicion_y-30)/30][posicion_x/30]!='X'){
				posicion_y=posicion_y-30;
			}
			else{
				direccion=4;
			}
		}
		if(direccion==3){
			if(mapa[(posicion_y+30)/30][posicion_x/30]!='X'){
				posicion_y=posicion_y+30;
			}
			else{
				direccion=4;
			}
		}
		
		if(posicion_x<=-30){
			posicion_x=870;
		}
		else if(posicion_x>=870){
			posicion_x=-30;
		}
		
		clear(buffer);
		dibujar_mapa();
		dibujar_pacman();
		enemigo1.mover_enemigo();
		enemigo2.mover_enemigo();
		enemigo3.mover_enemigo();
		enemigo4.mover_enemigo();
		enemigo5.mover_enemigo();
		pantalla();
		rest(70);
		
		clear(pacman);
		blit(pacbmp,pacman,4*33,0,0,0,33,33);
		draw_sprite(buffer,pacman,posicion_x,posicion_y);
		pantalla();
		rest(90);
	}
}
END_OF_MAIN();
