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
int Calculo_Dano(int def, int ata);
void Combate(int Vida);
//Módulo 1: Lectura de datos en archivos y configuración
void cargarConfiguracion(EstadoJuego *config, FILE *arch_c);
void cargarBestiario(Enemigos bestiario[], int *total_enemigos, FILE *arch_m);
void cargarObjetos(Objetos catalogo[], int *total_objetos, FILE *arch_o);
//Módulo 2: Navegación y Exploración
void avanzarCuarto(EstadoJuego *estado, Enemigos bestiario[], int total_enemigos, Objetos catalogo[], int total_objetos);
Enemigos seleccionarEnemigo(Enemigos bestiario[], int total_enemigos, int es_cuarto_final);

int main(){
  //VARIABLES
    EstadoJuego Estado_l;
    Enemigos Enemies_v[5], Enemie_a; 
    Objetos Objects_v[5];
    Atributos Jugador;
    int  i, cuarto_actual = 0, piso_actual = 1, opcion_ini, opcion_fue, opcion_com, Estado_juego = 0, enemigos_t = 5, objetos_t = 5;
    FILE *in_obje, *in_config, *in_enem;
    Jugador.XP = 0;
    

    

    in_enem = fopen("bestiario.txt", "r");
    in_obje = fopen("objetos.txt", "r");
    in_config = fopen("config.txt", "r");

    cargarBestiario(Enemies_v, &enemigos_t, in_enem);
    cargarConfiguracion(&Estado_l, in_config);
    cargarObjetos(Objects_v, &objetos_t, in_obje);
    
    srand(Estado_l.semilla);

    Objetos mochila[Estado_l.n_mochila];

    while(Estado_juego == 1 ||opcion_fue == 3||Estado_juego == 2){
        scanf("%d", opcion_fue);
        switch  (opcion_fue) {
            case 1:
               
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
        printf("\n%d\n", Estado_l.pisos);
    }


    
    fclose(in_obje);
    fclose(in_config);
    fclose(in_enem);
    return 0;
}

void cargarBestiario(Enemigos bestiario[], int *total_enemigos, FILE *arch_m) {
    int i;

    for(i=0; i<*total_enemigos; i++){
        fscanf(arch_m, "%d %s %d %d %d %s", &bestiario[i].ID, bestiario[i].nombre, &bestiario[i].vida, &bestiario[i].dano, &bestiario[i].defensa, bestiario[i].tipo);   
    }    
    
}

void cargarConfiguracion(EstadoJuego *config, FILE *arch_c) {
        
    fscanf(arch_c, "%d %d %d %d %d %d %d", &(config->semilla), &(config->pisos), &(config->cuartos_base), &(config->HP_base), &(config->atck_base), &(config->def_base), &(config->n_mochila));   
      
}

void cargarObjetos(Objetos catalogo[], int *total_objetos, FILE *arch_o) {
    int i;

    for(i=0; i<*total_objetos; i++){
        fscanf(arch_o, "%d %s %d %s", &catalogo[i].ID, catalogo[i].nombre, &catalogo[i].valor, catalogo[i].efecto);   
    }

}

Enemigos seleccionarEnemigo(Enemigos bestiario[], int total_enemigos, int es_cuarto_final){
    if(es_cuarto_final==0){
        int resultado_dado = (rand() % total_enemigos);
        return bestiario[resultado_dado];
    }
}

int Calculo_Dano(int def, int ata){
    int damage;
    damage = ata - def;
    if(damage<1){
        damage = 1;
    }
    return damage;
}