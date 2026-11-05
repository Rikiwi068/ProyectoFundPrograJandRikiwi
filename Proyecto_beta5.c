#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// TIPOS PARA ESTRUCTURAS
typedef char string[100];

typedef struct {
    string nombre;
    int HP, ataque, defensa, XP;
}Atributos;

typedef struct {
    int ID;
    string nombre;
    int vida, dano, defensa;
    string tipo;
}Enemigos;

typedef struct {
    int ID;
    string nombre;
    int valor;
    string efecto;
} Objetos ;

typedef struct {
    int semilla, pisos, cuartos_base, HP_base, atck_base, def_base, n_mochila; 
} EstadoJuego;

//ACIONES NOMINADAS
int Calculo_Daño(int def, int ata);
void Combate(int Vida);
//Módulo 1: Acciones de lectura de datos en archivos
void cargarConfiguracion(EstadoJuego config, FILE *arch_c);
void cargarBestiario(Enemigos bestiario[], int *total_enemigos, FILE *arch_m);
void cargarObjetos(Objetos catalogo[], int *total_objetos, FILE *arch_o);

int main(){
  //VARIABLES
    EstadoJuego Config_base;
    Enemigos Enemies_v[5] = {0}; 
    Objetos Objects_v[5];
    Atributos Jugador;
    int pisos, cuartos_base, mochila_n, i, semilla, cuarto_actual = 1, piso_actual = 1, opcion_ini, opcion_fue, opcion_com, Estado_juego = 0, enemigos_t = 5;
    FILE *in_obje, *in_config, *in_enem;
    Jugador.XP = 0;

    

    in_enem = fopen("bestiario.txt", "r");
    in_obje = fopen("objetos.txt", "r");
    in_config = fopen("config.txt", "r");

    cargarBestiario(Enemies_v, &enemigos_t, &in_enem);
    cargarConfiguracion(Config_base, &in_config);


    for(i=0; i<7; i++){
        fscanf(in_config, "%d %d %d %d %d %d %d", &semilla, &pisos, &cuartos_base, &Jugador.HP, &Jugador.ataque, &Jugador.defensa, &mochila_n);   
    }

    
    
    int mochila[mochila_n];

    while(Estado_juego == 1 ||opcion_fue == 3||Estado_juego == 2){
        scanf("%d", opcion_fue);
        switch  (opcion_fue) {
            case 1:
                switch (piso_actual) {
                    case 1:
                        if(cuarto_actual == cuartos_base-1){
                            
                        } else {
                            o
                        }
                    break;
                }
            break;
            case 2:

            break;
            case 3:
            break;
            default:
                printf("Por favor, ingresa una opcion válida");
        }
    }

    for(i=0; i<5; i++) {
        printf("\n%s\n", Objects_v[i].efecto);
    }


    
    fclose(in_obje);
    fclose(in_config);
    fclose(in_enem);
    return 0;
}

void cargarBestiario(Enemigos bestiario[], int *total_enemigos, FILE *arch_m) {
    int i;

    for(i=0; i<total_enemigos; i++){
        fscanf(arch_m, "%d %s %d %d %d %s", &bestiario[i].ID, bestiario[i].nombre, &bestiario[i].vida, &bestiario[i].dano, &bestiario[i].defensa, bestiario[i].tipo);   
    }    
    
}

void cargarConfiguracion(EstadoJuego config, FILE *arch_c) {

    int i;

    for(i=0; i<7; i++){
        fscanf(arch_c, "%d %d %d %d %d %d %d", &config.semilla, &config.pisos, &config.cuartos_base, &config.HP_base, &config.atck_base, &config.def_base, &config.n_mochila);   
    }   
    
}

void cargarObjetos(Objetos catalogo[], int *total_objetos, FILE *arch_o) {
