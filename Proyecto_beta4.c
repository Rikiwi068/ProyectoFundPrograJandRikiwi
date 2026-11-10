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
void cargarConfiguracion(EstadoJuego *estado);
void cargarBestiario(Enemigos bestiario[], int *total_enemigos);
void cargarObjetos(Objetos catalogo[], int *total_objetos);
//Módulo 2: Navegación y Exploración
void avanzarCuarto(EstadoJuego *estado, Enemigos bestiario[], int total_enemigos, Objetos catalogo[], int total_objetos);
Enemigos seleccionarEnemigo(Enemigos bestiario[], int total_enemigos, int es_cuarto_final);
// Módulo 3: Sistema de Combate y Progresión
void iniciarCombate(EstadoJuego *estado, Enemigos *enemigo);
int calcularDano(int ataque_atacante, int defensa_defensor);
void otorgarRecompensa(EstadoJuego *estado, Objetos catalogo[], int
total_objetos, char tipo_enemigo[]);
void verificarSubidaNivel(EstadoJuego *estado);

int main(){
  //VARIABLES
    EstadoJuego Estado_l;
    Enemigos Enemies_v[5], Enemie_a; 
    Objetos Objects_v[5];
    Atributos Jugador;
    int  i, cuarto_actual = 0, piso_actual = 1, opcion_ini, opcion_fue, opcion_com, Estado_juego = 0, enemigos_t = 5, objetos_t = 5;
    FILE *in_obje, *in_config, *in_enem;
    Jugador.XP = 0;
    
    
    

    cargarBestiario(Enemies_v, &enemigos_t);
    cargarConfiguracion(&Estado_l);
    cargarObjetos(Objects_v, &objetos_t);
    
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
    
    return 0;
}

void cargarBestiario(Enemigos bestiario[], int *total_enemigos) {
    int i;
    FILE *in_enem;

    in_enem = fopen("bestiario.txt", "r");
    for(i=0; i<*total_enemigos; i++){
        fscanf(in_enem, "%d %s %d %d %d %s", &bestiario[i].ID, bestiario[i].nombre, &bestiario[i].vida, &bestiario[i].dano, &bestiario[i].defensa, bestiario[i].tipo);   
    }    
    fclose(in_enem);
}

void cargarConfiguracion(EstadoJuego *estado) {
    FILE *in_config;
    
    in_config = fopen("config.txt", "r");
    fscanf(in_config, "%d %d %d %d %d %d %d", &(*estado).semilla, &(*estado).pisos, &(*estado).cuartos_base, &(*estado).HP_base, &(*estado).atck_base, &(*estado).def_base, &(*estado).n_mochila);   
    fclose(in_config);  
}

void cargarObjetos(Objetos catalogo[], int *total_objetos) {
    int i;
    FILE *in_obje;

    in_obje = fopen("objetos.txt", "r");
    for(i=0; i<*total_objetos; i++){
        fscanf(in_obje, "%d %s %d %s", &catalogo[i].ID, catalogo[i].nombre, &catalogo[i].valor, catalogo[i].efecto);   
    }
    fclose(in_obje);
}

Enemigos seleccionarEnemigo(Enemigos bestiario[], int total_enemigos, int es_cuarto_final){
    int resultado_dado;
    
    if(es_cuarto_final==0){
        do {
            resultado_dado = (rand() % total_enemigos);
        } while(strcmp(bestiario[resultado_dado].nombre, "comun" != 0));
        return bestiario[resultado_dado];
    } else{
        do {
            resultado_dado = (rand() % total_enemigos);
        } while(strcmp(bestiario[resultado_dado].nombre, "raro" != 0));
        return bestiario[resultado_dado];
    }
}

int Calculo_Dano(int ataque_atacante, int defensa_defensor){
    int damage;
    damage = ataque_atacante - defensa_defensor;
    if(damage<1){
        damage = 1;
    }
    return damage;
}