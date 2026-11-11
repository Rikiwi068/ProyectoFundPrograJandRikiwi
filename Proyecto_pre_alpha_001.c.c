#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// TIPOS PARA ESTRUCTURAS
typedef enum{
    false=0,
    true=1
}bool;

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
}Entidad;

typedef struct {
    int ID;
    string nombre;
    int valor;
    string efecto;
} Objetos ;

typedef struct {
    int semilla, pisos, cuartos_base, HP_base, atck_base, def_base, n_mochila; 
    int piso_maximo, cuartos_explorados,enemigos_derrotados,objetos_consumidos;
    Atributos jugador;
    Objetos mochila[100];
    int items_en_mochila;
} EstadoJuego;

//ACIONES NOMINADAS
//Módulo 1: Lectura de datos en archivos y configuración
void cargarConfiguracion(EstadoJuego *estado);
void cargarBestiario(Entidad bestiario[], int *total_enemigos);
void cargarObjetos(Objetos catalogo[], int *total_objetos);
//Módulo 2: Navegación y Exploración
void avanzarCuarto(EstadoJuego *estado, Entidad bestiario[], int total_enemigos, Objetos catalogo[], int total_objetos);
Entidad seleccionarEnemigo(Entidad bestiario[], int total_enemigos, int es_cuarto_final);
//Módulo 3: Sistema de Combate y Progresión
void iniciarCombate(EstadoJuego *estado, Entidad *enemigo);
int calcularDano(int ataque_atacante, int defensa_defensor);
void otorgarRecompensa(EstadoJuego *estado, Objetos catalogo[], int total_objetos, char tipo_enemigo[]);
void verificarSubidaNivel(EstadoJuego *estado);
//Módulo 4: gestión de recursos
void usarObjeto(EstadoJuego *estado);
//Módulo 5: guardado y carga de partida
void registrarHighscore(EstadoJuego estado, int victoria);
//Módulo 6: acciones adicionales
void esperar_segundos(int segundos);

int main(){
  //VARIABLES
    EstadoJuego Estado_l;
    Entidad Enemies_v[100], Enemie_a; 
    Objetos Objects_v[100];
    int  i, opcion_ini, opcion_fue, opcion_com, Estado_juego = 0, enemigos_t = 0, total_objetos;
    bool juego_activo=true;

    cargarBestiario(Enemies_v, &enemigos_t);
    cargarConfiguracion(&Estado_l);
    cargarObjetos(Objects_v, &total_objetos);

    Estado_l.catalogo_global=Objects_v;
    Estado_l.total_objetos=objetos_t;
    
    srand(Estado_l.semilla);

    Estado_l.piso_maximo=1;
    Estado_l.cuartos_explorados=0;
    Estado_l.enemigos_derrotados=0;
    Estado_l.objetos_consumidos=0;
    Estado_l.jugador.HP=Estado_l.HP_base;
    Estado_l.jugador.ataque=Estado_l.atck_base;
    Estado_l.jugador.defensa=Estado_l.def_base;
    Estado_l.jugador.XP = 0;
    Estado_l.items_en_mochila=0;

    printf("\n======================\n");
    printf("  Seleccione una acción\n");
    printf("\n======================\n");

    scanf("%d", &opcion_fue);

    //las instrucciones en el switch son provisionales y se encuentran de esa forma para evitar errores por ahora
    while(opcion_fue == 1 ||opcion_fue == 3||opcion_fue == 2){
        switch  (opcion_fue) {
            case 1:
               opcion_fue=0;
            break;
            case 2:
                opcion_fue=0;
            break;
            case 3:
                opcion_fue=0;
            break;
            default:
                printf("Por favor, ingresa una opcion válida");
                scanf("%d", &opcion_fue);
        }
    }

    for(i=0; i<5; i++) {
        printf("\n%d\n", Estado_l.pisos);
    }


    
    return 0;
}

void cargarBestiario(Entidad bestiario[], int *total_enemigos) {
    FILE *arch_m = fopen("bestiario.txt","r");
    if(arch_m==NULL){
        printf("No se encontró el archivo del bestiario, verifique su existencia en su dispositivo o que el formato sea el adecuado e intente de nuevo");
        *total_enemigos=0;
        return;
    }

    int i=0;

    while(fscanf(arch_m,"%d %s %d %d %d %s", &bestiario[i].ID, bestiario[i].nombre, &bestiario[i].vida, &bestiario[i].dano, &bestiario[i].defensa, bestiario[i].tipo)!=EOF){
        i++;
    } 

    *total_enemigos=i;
    
    fclose(arch_m);
    
}

void cargarConfiguracion(EstadoJuego *config) {
    FILE *arch_c = fopen("config.txt", "r");

    if (arch_c==NULL){
        printf("No se encontró el archivo de configuración, verifique si el mismo se encuentra en su dispositivo y con el formato adecuado e intente de nuevo\n");
        return;
    }
        
    fscanf(arch_c, "%d %d %d %d %d %d %d", &((*config).semilla), &((*config).pisos), &((*config).cuartos_base), &((*config).HP_base), &((*config).atck_base), &((*config).def_base), &((*config).n_mochila));   
    fclose(arch_c);
}

void cargarObjetos(Objetos catalogo[], int *total_objetos) {
    FILE *arch_o = fopen("objetos.txt","r");

    if(arch_o==NULL){
        printf("El archivo de objetos no se encuentra o tiene un formato equivocado, verifique y vuelva a intentar");
        return;
    }
    int i=0;

    
    while(fscanf(arch_o, "%d %s %d %s", &catalogo[i].ID, catalogo[i].nombre, &catalogo[i].valor, catalogo[i].efecto)!=EOF){
        i++;
    }  
    
    *total_objetos=i;

    fclose(arch_o);
}

Entidad seleccionarEnemigo(Entidad bestiario[], int total_enemigos, int es_cuarto_final){
    int cant_raras=0, i, objetivo=rand()%cant_raras, contador_raras=0, j;

    
    if(es_cuarto_final==0){
        int resultado_dado = (rand() % total_enemigos);
        return bestiario[resultado_dado];
    }else{
        for(j=0; j<total_enemigos; j++){
            if(strcmp(bestiario[j].tipo,"raro")==0){
                cant_raras++;
            }
        }

        if(cant_raras>0){

            for(i=0;i<total_enemigos;i++){
                if(strcmp(bestiario[i].tipo,"raro")==0){
                    if(contador_raras==objetivo){
                        return bestiario[i];
                    }
                    contador_raras++;
                }
            }
        }
        return bestiario[rand()%total_enemigos];
    }
}

int calcularDano(int ataque_atacante, int defensa_defensor){
    int damage;
    damage = ataque_atacante - defensa_defensor;
    if(damage<1){
        damage = 1;
    }
    return damage;
}

void iniciarCombate(EstadoJuego *estado, Entidad *enemigo){
    int opcion_com;
    bool combate_activo=true;

    printf("Te desafía %s!\n",(*enemigo).nombre);

    while ((*estado).jugador.HP>0 && (*enemigo).vida>0 && combate_activo==true){
        bool turno_consumido=false;
        
        printf("Tu: HP %d | Ataque %d | Defensa %d|\n", (*estado).jugador.HP, (*estado).jugador.ataque,(*estado).jugador.defensa);
        printf("%s: HP %d | Ataque %d | Defensa %d|\n", (*enemigo).nombre, (*enemigo).vida, (*enemigo).dano, (*enemigo).defensa);
        printf("\nTurno del jugador: Atacar(1) | Revisar la mochila(2) | Intentar huir(3)|\n");
        scanf("%d", &opcion_com);

        

        switch (opcion_com){
            case 1:{
                int dano_jugador=calcularDano((*estado).jugador.ataque, (*enemigo).defensa);
                (*enemigo).vida-=dano_jugador;

                printf("\n %s recibe %d de dano!\n", (*enemigo).nombre, dano_jugador);

                turno_consumido=true;
                break;
            }
            case 2:{
                printf("\nRevisas tu mochila...\n");
                usarObjeto(estado);
                turno_consumido=true;
                break;
            }
            case 3:{
                int prob_escape=((*estado).jugador.defensa*100)/((*estado).jugador.defensa+(*enemigo).dano),dado=rand()%100;

                printf("Intentas huir del enemigo...\n");
                if (dado<prob_escape){
                    printf("Has escapado\n");
                    combate_activo=false;
                }else{
                    printf("Fallas al intentar escapar, y quedas expuesto al atque enemigo\n");
                    turno_consumido=true;
                }
                break;
            }   
            default: 
                printf("Valor invalido, intente de nuevo\n");
                break;
        }

        if ((*enemigo).vida<=0){
            combate_activo=false;
            break;
        }

        if (turno_consumido == true && combate_activo == true && (*enemigo).vida > 0) {
            int dano_enemigo = calcularDano((*enemigo).dano, (*estado).jugador.defensa);
            (*estado).jugador.HP -= dano_enemigo;
            
            printf(" %s te ataca e inflige %d de daño!\n", (*enemigo).nombre, dano_enemigo);
            if ((*estado).jugador.HP < 0) (*estado).jugador.HP = 0;
            printf("Tu HP restante: %d\n", (*estado).jugador.HP);

            if ((*estado).jugador.HP <= 0) {
                printf("\n Has muerto, tus restos ahora le pertenecen a la mazmorra y a sus habitantes... \n");
                registrarHighscore(*estado, false);
                combate_activo = false;
                break;
            }
        }
    }
}

void avanzarCuarto(EstadoJuego *estado, Entidad bestiario[], int total_enemigos, Objetos catalogo[], int total_objetos){
    static int cuarto_actual=0, piso_actual=1;
    int es_cuarto_final=0;

    cuarto_actual++;
    (*estado).cuartos_explorados++;
    (*estado).piso_maximo=piso_actual;

    printf("\n====================================\n");
    printf("\n          PISO%d  CUARTO%d/%d\n",piso_actual,cuarto_actual,(*estado).cuartos_base);
    printf("\n====================================\n");

    if(cuarto_actual==(*estado).cuartos_base){
        es_cuarto_final=1;
        printf("Has ingresado al último cuarto de este piso, prepárate para el desafío que se presenta frente a tus ojos\n");
    }else{
        printf("desciendes en un nuevo piso de la mazmorra oscura, húmeda y putrefacta\n");
    }

    Entidad enemigo_actual = seleccionarEnemigo(bestiario,total_enemigos,es_cuarto_final);
    printf("%s se muestra frente a ti\n", enemigo_actual.nombre);
    iniciarCombate(estado, &enemigo_actual);

    if((*estado).jugador.HP<=0){
        return;
    }

    if (enemigo_actual.vida<=0){
            printf("has derrotado a %s!\n", enemigo_actual.nombre);
            (*estado).enemigos_derrotados++;
            otorgarRecompensa(estado, catalogo, total_objetos, enemigo_actual.tipo);
    }

    if(es_cuarto_final==1){
        printf("La mazmorra vuelve a engullirte mientras desciendes nuevamente\n");
        piso_actual++;
        cuarto_actual=0;

        if(piso_actual>(*estado).pisos){
            printf("\nrecorres el último cuarto de la mazmorra mientras el ambiente, antes lleno de criaturas hostiles y monstruosas, vuelve a su fin, tu alma se siente en paz mientras te adentras en las cavernas que yacen bajo este calabozo infernal\n");
            registrarHighscore(*estado,1);
        }
    }
}

void esperar_segundos(int segundos){
    clock_t inicio=clock(), milisegundos=segundos*1000;

    while((clock()-inicio)*1000/CLOCKS_PER_SEC<milisegundos){

    }
}

void registrarHighscore(EstadoJuego estado, int victoria){
    char nombre_jugador[50];
    int puntaje_final=estado.jugador.XP, bonus=rand()%60,penalización=rand()%50;

    printf("\n=========================\n");

    if(victoria==true){
        puntaje_final=puntaje_final+bonus;
        printf("Considérate maestro de la mazmorra, pues tus hazañas han demostrado ser insuperables a lo largo de tu travesía\n");
    }else{
        int base_derrota=estado.pisos/4;
        puntaje_final=(base_derrota+bonus-penalización);
        printf("Mientras tus ojos se cierran, la mazmorra reclama tu alma para retorcerla y usarla contra cualquier nuevo intruso que decida desafiarla\n");
    }

    printf("tu puntuación fue: %d\n", puntaje_final);

    printf("ingresa el nombre de tu insensato aventurero\n");
    scanf("%s",nombre_jugador);
    printf("\ncargando pantalla final\n");

    esperar_segundos(3);

    printf("\nResumen de la aventura\n");
    printf("piso máximo alcanzado %d/%d\n", estado.piso_maximo, estado.pisos);
    printf("cuartos explorados %d\n", estado.cuartos_explorados);
    printf("enemigos derrotados %d\n", estado.enemigos_derrotados);
    printf("objetos consumidos %d\n", estado.objetos_consumidos);
    printf("Atributos finales: HP %d | Ataque %d | Defensa %d | XP %d \n", estado.jugador.HP, estado.jugador.ataque, estado.jugador.defensa, estado.jugador.XP);

    FILE *arch_h=fopen("highscore.txt","a");
    if(arch_h==NULL){
        printf("Error al abrir el archivo highscore, verifique la existencia y el formato del nisno y vuelva a intentarlo\n");
        return;
    }
    fprintf(arch_h,"%s %d %d %d %d %d %s\n", nombre_jugador, puntaje_final, estado.piso_maximo, estado.cuartos_explorados, estado.enemigos_derrotados, estado.objetos_consumidos, (victoria==true)? "Victoria":"Derrota");
    fclose(arch_h);
}

void usarObjeto(EstadoJuego *estado){
    int i, selección;

    if((*estado).items_en_mochila<=0){
        printf("tu espalda está ligera, tu mochila, vacía\n");
        return;
    }else{
        printf("\n===mochila %d / %d===\n", (*estado).items_en_mochila, (*estado).n_mochila);
        for(i=0;i<(*estado).items_en_mochila;i++){
            printf("\n%d %s (Valor: %d | Efecto: %s)\n", i+1, (*estado).mochila[i].nombre, (*estado).mochila[i].valor, (*estado).mochila[i].efecto);
        }
        
        printf("seleccione el objeto a usar o ingrese 0 para salir de la mochila\n");
        scanf("%d", &selección);

        if(selección<=0 || selección>(*estado).items_en_mochila){
            return;
        }else{
            int indice=selección-1;
            Objetos objeto_usado= (*estado).mochila[indice];

            if(strcmp(objeto_usado.efecto, "Cura_Vida")==0){
                (*estado).jugador.HP+=objeto_usado.valor;
                if((*estado).jugador.HP>(*estado).HP_base){
                    (*estado).jugador.HP=(*estado).HP_base;
                    printf("Tu salud se ha restaurado\n");
                }
            }else if (strcmp(objeto_usado.efecto, "Buff_Dano")==0){
                (*estado).jugador.ataque+=objeto_usado.valor;
                printf("la furia recorre tus venas y tu ataque aumenta\n");
            }else if (strcmp(objeto_usado.efecto, "Buff_Defensa")==0){
                (*estado).jugador.defensa+=objeto_usado.valor;
                printf("tu cuerpo se fortalece y tus músculos endurecen, tu defensa ha aumentado\n");
            }

            (*estado).objetos_consumidos++;
            
            for(i=indice;i<(*estado).items_en_mochila-1;i++){
                (*estado).mochila[i]=(*estado).mochila[i+1];
            }
            (*estado).items_en_mochila--;
        }
    }
}

void otorgarRecompensa(EstadoJuego *estado, Objetos catalogo[], int total_objetos, char tipo_enemigo[]){
    int xp_base=50;

    (*estado).jugador.XP+=xp_base;
    printf("Has ganado %d puntos de experiencia\n", xp_base);

    int entregar_objeto=0;

    if(strcmp(tipo_enemigo, "raro")==0){
        entregar_objeto=1;
        printf("\nentre los restos del enemigo encuentras un objeto\n");
    }else{
        if((rand()%100)<40){
            entregar_objeto=1;
            printf("\nentre los destrozos de la pelea encuentras un objeto\n");
        }else{
        printf("entre los escombros de la batalla no hay más que polvo\n");
        }
    }

    if(entregar_objeto==1 && total_objetos>0){
        if((*estado).items_en_mochila>= (*estado).n_mochila || (*estado).items_en_mochila>=100){
            printf("\nla mochila se encuentra llena\n ");
            return;
        }
    }

    if(total_objetos>0){
        int indice_rand=rand()%total_objetos;
        Objetos item_encontrado=catalogo[indice_rand];

        (*estado).mochila[(*estado).items_en_mochila]=item_encontrado;
        (*estado).items_en_mochila++;

        printf("\nHas encontrado '%s'! (Efecto: %s | Valor: %d\n", item_encontrado.nombre, item_encontrado.efecto, item_encontrado.valor);
    }
    
}

void verificarSubidaNivel(EstadoJuego *estado){
    int xp_por_nv=100;

    while((*estado).jugador.XP>=xp_por_nv){
        (*estado).jugador.XP-=xp_por_nv;

        (*estado).HP_base+=20;
        (*estado).atck_base+=5;
        (*estado).def_base+=10;

        (*estado).jugador.HP=(*estado).HP_base;
        (*estado).jugador.defensa=(*estado).def_base;
        (*estado).jugador.ataque=(*estado).atck_base;

        printf("\n==========Has subido de nivel==========\n");
        printf("Hp: %d, Def: %d, Atk: %d\n", (*estado).jugador.HP,(*estado).jugador.defensa,(*estado).jugador.ataque);
        printf("\n=======================================\n");
    }
}