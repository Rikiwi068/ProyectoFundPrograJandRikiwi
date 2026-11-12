#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// TIPOS PARA ESTRUCTURAS
// tipo booleano para validar
typedef enum {
    F = 0,
    V = 1
} T_bool;

// tipo cadena de caracteres para facilitar uso de variables con texto
typedef char string[100];

// tipo de registro de atributos del jugador para facilitar su uso
typedef struct {
    int HP, ataque, defensa, XP, nivel;
}Atributos;

// tipo de registro de atributos del enemigo para facilitar acceso por parametros y validacion
typedef struct {
    int ID, vida, dano, defensa;
    string nombre, tipo;
}Entidad;

// tipo de registro de atributos de objetos para guardarlos y accederlos con facilidad a traves de un vector
typedef struct {
    int ID, valor;
    string nombre, efecto;
} Objetos ;

// tipo de registro con gran parte de la informacion del programa 
typedef struct {
    int semilla, pisos_base, cuartos_base, HP_base, atck_base, def_base, n_mochila, items_en_mochila; 
    int piso_maximo, cuartos_explorados,enemigos_derrotados,objetos_consumidos;
    Atributos jugador;
    Objetos mochila[100]; 
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
//Módulo 4: Gestión de recursos
void usarObjeto(EstadoJuego *estado);
//Módulo 5: Guardado y carga de partida
void guardarPartida(EstadoJuego estado);
int cargarPartida(EstadoJuego *estado);
void registrarHighscore(EstadoJuego estado, T_bool victoria);
//Módulo 6: Acciones adicionales implementadas
void esperar_segundos(int segundos);
void MostrarHighscores();
void Pelea_final(EstadoJuego *estado);
void BorrarPartidaGuardada();

int main(){
  //VARIABLES
    EstadoJuego Estado_l; //Variable con los valores generales
    Entidad Enemies_v[100]; // Vector con toda la base de datos de los enemigos
    Objetos Objects_v[100]; // Vector con toda la base de datos de los objetos 
    int opcion_in, opcion_fue = 0, tot_enemigos , tot_objetos, cargado_exitoso = 1; // Variables de opciones tomadas, numero de enemigos y objetos y variable para validar la lectura correcta del guardado
    T_bool juego_activo=V; // Bandera para seguir en el juego o salir de él

    cargarBestiario(Enemies_v, &tot_enemigos); //Procedimiento para llenar vector de enemigos desde el archivo
    cargarObjetos(Objects_v, &tot_objetos); //Procedimiento para llenar vector de objetos desde el archivo
    cargarConfiguracion(&Estado_l); //Lectura del archivo de configuracion
    
    srand(Estado_l.semilla); //Inicialización de la semilla

    if(tot_enemigos==0){ // Validacion de que existan enemigos en el archivo
        printf("Error al cargar los enemigos");
        juego_activo = F;
    }

    if(tot_objetos == 0){ //Validaion de que existan objetos en el archivo
        printf("Error al cargar los objetos");
        juego_activo = F;
    }

    //Menú al abrir juego para seleccionar opciones generales 
    while(opcion_fue != 1 && opcion_fue != 2 && opcion_fue != 4 && juego_activo == V){
        printf("\n========================================================================================\n");
        printf("\n                         BIENVENIDO A DUNGEON CRAWLER \n");
        printf("\n========================================================================================\n");
        printf("\n========================================================================================\n");
        printf("\nSeleccione una accion\n");
        printf("\n========================================================================================\n");
        printf("\nEmpezar Nueva partida(1) | Continuar Partida(2) | Highscores(3)| Salir(4)\n");
        printf("\n========================================================================================\n\n");
        scanf("%d", &opcion_fue);
        switch  (opcion_fue) {
            case 1: //Se le asigna el valor inicial a todos los atributos y a la ubicacion
                Estado_l.piso_maximo = 1;
                Estado_l.cuartos_explorados=0;
                Estado_l.enemigos_derrotados = 0;
                Estado_l.objetos_consumidos = 0;
                Estado_l.jugador.HP = Estado_l.HP_base;
                Estado_l.jugador.ataque = Estado_l.atck_base;
                Estado_l.jugador.defensa =Estado_l.def_base;
                Estado_l.jugador.XP = 0;
                Estado_l.items_en_mochila = 0;
                Estado_l.jugador.nivel = 1;
                printf("\n========================================================================================\n");
                printf("\n                         TU AVENTURA HA COMENZADO \n");
                printf("\n========================================================================================\n");
            break;
            case 2: //Se lee la ubicacion y los atributos del archivo de guardado, y se valida que se cargue con éxito 
                cargado_exitoso = cargarPartida(&Estado_l);
                if(cargado_exitoso == 0){
                    printf("No existe ninguna partida cargada");
                    opcion_fue = 0;
                } else {
                    printf("\n========================================================================================\n");
                    printf("\n                           TU AVENTURA CONTNUA \n");
                    printf("\n========================================================================================\n");
                }
            break;
            case 3: //Muestra las ultimas 5 puntuaciones
                MostrarHighscores();
            break;
            case 4: //Sale del Juego
                printf("\n========================================================================================\n");
                printf("\nHasta luego :(\n");
                printf("\n========================================================================================\n");
                juego_activo = F;
            break;
            default:
                printf("Por favor, ingresa una opcion válida");
        }
    }

    while(juego_activo == V){ //Ciclo que controla el flujo una vez iniciado el juego
        printf("\n========================================================================================\n");
        printf("\nEstas en una mazmorra con multiples cuartos e incertidumbre por delante, Que deseas hacer?\n");
        printf("\n========================================================================================\n");
        printf("\nIr al siguiente cuarto(1) | Revisar la mochila(2) | Guardar y salir(3)\n");
        printf("\n========================================================================================\n\n");
        scanf("%d\n", &opcion_in);
        switch (opcion_in) {
            case 1: //Se llama al procedimiento para continuar en el juego y se valida que no termine
                avanzarCuarto(&Estado_l, Enemies_v, tot_enemigos, Objects_v, tot_objetos);
            if(Estado_l.jugador.HP<=0){ // Sale del programa porque el jugador ha perdido
                juego_activo = F; //Se cambia la bandera a F para salir del ciclo
            } else if(Estado_l.cuartos_explorados == Estado_l.cuartos_base+1){ //Se valida si está en el cuarto final ya sabiendo que sobrevivió
                if(opcion_fue==2){ //Si se selecciono la partida guardada se borra ya que se terminó el juego desde esta
                    BorrarPartidaGuardada(); 
                }
                juego_activo = F; //Se cambia la bandera a F para salir del ciclo
            }
            break;
            case 2: //Se llama el procedimiento asociado a la gestion de objetos
                usarObjeto(&Estado_l);
            break;
            case 3: //Se guardan los atributos y se cierra el programa
                guardarPartida(Estado_l);
                juego_activo = F;
            break;
            default: //si se ingresa cualquier otro valor que no es válido, simplemente se repite el ciclo
                printf("\nPor favor, ingrese una opcion valida ;)\n");
        }
    }
    
    return 0;
}

void cargarBestiario(Entidad bestiario[], int *total_enemigos) { //Procedimiento para cargar la base de datos de los enemigos
    FILE *arch_m = fopen("bestiario.txt","r"); 
    int i=0; //Variable para contar la cantidad de enemigos

    if(arch_m==NULL){ //Se valida si el archivo no este vacio
        printf("No se encontró el archivo del bestiario, verifique su existencia en su dispositivo o que el formato sea el adecuado e intente de nuevo");
        *total_enemigos=0;
        return;
    }

    while(fscanf(arch_m,"%d %s %d %d %d %s", &bestiario[i].ID, bestiario[i].nombre, &bestiario[i].vida, &bestiario[i].dano, &bestiario[i].defensa, bestiario[i].tipo)!=EOF){ //Ciclo para leer los atributos de cada enemigo y se cuenta cuantos hay en total 
        i++;
    } 

    *total_enemigos=i;
    
    fclose(arch_m);
    
}

void cargarConfiguracion(EstadoJuego *config) { //Procedimiento para cargar los valores base del archivo de configuracion
    FILE *arch_c = fopen("config.txt", "r");

    if (arch_c==NULL){ //Se valida si el archivo no este vacio
        printf("No se encontró el archivo de configuración, verifique si el mismo se encuentra en su dispositivo y con el formato adecuado e intente de nuevo\n");
        return;
    }
        
    fscanf(arch_c, "%d %d %d %d %d %d %d", &((*config).semilla), &((*config).pisos_base), &((*config).cuartos_base), &((*config).HP_base), &((*config).atck_base), &((*config).def_base), &((*config).n_mochila)); //se leen todos los valores del archivo
    fclose(arch_c);
}

void cargarObjetos(Objetos catalogo[], int *total_objetos) { //Procedimiento para cargar la base de datos de los enemigos
    FILE *arch_o = fopen("objetos.txt","r");
    int i=0; //contador del total de objetos

    if(arch_o==NULL){ //Se valida si el archivo no este vacio
        printf("El archivo de objetos no se encuentra o tiene un formato equivocado, verifique y vuelva a intentar");
        return;
    }
    
    while(fscanf(arch_o, "%d %s %d %s", &catalogo[i].ID, catalogo[i].nombre, &catalogo[i].valor, catalogo[i].efecto)!=EOF){ //Ciclo para leer los atributos de cada objeto y se cuenta cuantos hay en total 
        i++;
    }  
    
    *total_objetos=i;

    fclose(arch_o);
}

Entidad seleccionarEnemigo(Entidad bestiario[], int total_enemigos, int es_cuarto_final){ //Funcion para seleccionar un enemigo para el combate
    int i, j;
    
    if(es_cuarto_final == 0){ //Si no es el cuarto final
        int resultado_dado = (rand() % total_enemigos); //Indice de enemigo aleatorio
        return bestiario[resultado_dado];
    } else {
        int cant_raras = 0;
        for(j = 0; j < total_enemigos; j++){ //Contador de enemigos raros
            if(strcmp(bestiario[j].tipo, "raro") == 0){
                cant_raras++;
            }
        }
        if(cant_raras > 0){ // Valida que exista al menos uno raro
            int objetivo = rand() % cant_raras; 
            int contador_raras = 0;

            for(i = 0; i < total_enemigos; i++){ //Se recorre todo el vector de enemigos para conseguir uno raro que 
                if(strcmp(bestiario[i].tipo, "raro") == 0){
                    if(contador_raras == objetivo){
                        return bestiario[i];
                    }
                    contador_raras++;
                }
            }
        }
        return bestiario[rand() % total_enemigos];
    }
}

int calcularDano(int ataque_atacante, int defensa_defensor){ //Funcion para calcular el daño inflingido
    int damage;
    damage = ataque_atacante - defensa_defensor; 
    if(damage<1){ //El daño mínimo es 1, por lo que se setea a 1 en caso de ser menor
        damage = 1;
    }
    return damage;
}

void usarObjeto(EstadoJuego *estado){ //Procedimiento para Mostrar los objetos y usar el deseado especificamente
    int i, selección;

    if((*estado).items_en_mochila<=0){ //Validando si tiene bobjetos en la mochila
        printf("Tu espalda esta ligera, tu mochila, vacia\n");
        return;
    }else{
        printf("\n===mochila %d / %d===\n", (*estado).items_en_mochila, (*estado).n_mochila); //Imprimiendo las casillas ocupadas
        for(i=0;i<(*estado).items_en_mochila;i++){ //Imprime todos los objetos en la mochila
            printf("\n%d %s (Valor: %d | Efecto: %s)\n", i+1, (*estado).mochila[i].nombre, (*estado).mochila[i].valor, (*estado).mochila[i].efecto);
        }
        
        printf("Seleccione el objeto a usar o ingrese 0 para salir de la mochila\n"); //Alternativa en caso de no querer usar objeto
        scanf("%d", &selección);

        if(selección<=0 || selección>(*estado).items_en_mochila){ //Valida que sea un indice de la mochila
            return;
        }else{
            int indice=selección-1;
            Objetos objeto_usado= (*estado).mochila[indice]; // se guarda el objeto seleccionado en una variable auxiliar

            if(strcmp(objeto_usado.efecto, "Cura_Vida")==0){ //Se valida si el objeto tiene el efecto de curar
                (*estado).jugador.HP += objeto_usado.valor; // Se suma a la vida del jugador
                if((*estado).jugador.HP > (*estado).HP_base){ //Si la vida que se sumó es mayor a la vida maxima se setea a la vida maxima y se indica que se curó en su totalidad
                    (*estado).jugador.HP = (*estado).HP_base; 
                    printf("Has consumido tu %s, Tu salud se ha restaurado\n", objeto_usado.nombre);
                } else {
                    printf("Has consumido tu %s, Te has curado parcialmente\n", objeto_usado.nombre);
                }
            }else if (strcmp(objeto_usado.efecto, "Buff_Dano")==0){ //Se valida si el objeto tiene el efecto de subir el ataque
                (*estado).jugador.ataque += objeto_usado.valor; //Se suma el valor de objeto al campo de ataque del jugador
                printf("Has consumido tu %s, La furia recorre tus venas y tu ataque aumenta\n", objeto_usado.nombre);
            }else if (strcmp(objeto_usado.efecto, "Buff_Defensa")==0){ //Se valida si el objeto tiene el efecto de subir la defensa
                (*estado).jugador.defensa += objeto_usado.valor; //Se suma el valor de objeto al campo de defensa del jugador
                printf("Has consumido tu %s, tu cuerpo se fortalece y tus músculos endurecen, tu defensa ha aumentado\n", objeto_usado.nombre);
            } else { //Al no tener ninguno de los efectos anteriores se asume que tiene el efecto de aumentar la vida maxima
                (*estado).HP_base += objeto_usado.valor; //Se aumenta el campo de vida maxima del jugador
                printf("Has consumido tu %s, las capacidades de tu cuerpo han sido potenciadas, tu vida máxima ha aumentado\n", objeto_usado.nombre);
            }

            (*estado).objetos_consumidos++; //Contador para la estadistica final de objetos 
            
            for(i=indice;i<(*estado).items_en_mochila-1;i++){ //Algoritmo de eliminacion de la casilla usada
                (*estado).mochila[i]=(*estado).mochila[i+1];
            }
            (*estado).items_en_mochila--; //Se ajustan los objetos totales a la cantidad tras usar uno
        }
    }
}

void esperar_segundos(int segundos){ //Procedimiento para contar una cantidad de segundos antes de seguir con el programa
    clock_t inicio=clock(), milisegundos=segundos*1000;

    while((clock()-inicio)*1000/CLOCKS_PER_SEC<milisegundos){

    }
}

void registrarHighscore(EstadoJuego estado, T_bool victoria){ //Procedimiento para guardar los datos de una partida terminada en el archivo de highscores
    char nombre_jugador[50];
    int puntaje_final=estado.jugador.XP*estado.jugador.nivel, bonus=rand()%60, penalización=rand()%50;

    printf("\n========================================================================================\n");

    if(victoria==V){ //Valida si culminó la partida porque ganó el juego
        puntaje_final=puntaje_final+bonus; //Funcion para el puntaje
        printf("Considerate maestro de la mazmorra, pues tus logros han demostrado ser insuperables a lo largo de tu travesia\n");
    }else{
        int base_derrota=estado.pisos_base/4;
        puntaje_final=(base_derrota+bonus-penalización);
        printf("Mientras tus ojos se cierran, la mazmorra reclama tu alma para retorcerla y usarla contra cualquier nuevo intruso que decida desafiarla\n");
    }

    printf("Tu puntuacion fue: %d\n", puntaje_final);

    printf("Ingresa el nombre de tu insensato aventurero\n");
    scanf("%s",nombre_jugador);
    printf("\nCargando pantalla final\n");

    esperar_segundos(3);

    printf("\nResumen de la aventura\n");
    printf("Piso maximo alcanzado %d/%d\n", estado.piso_maximo, estado.pisos_base);
    printf("Cuartos explorados %d\n", estado.cuartos_explorados);
    printf("Enemigos derrotados %d\n", estado.enemigos_derrotados);
    printf("Objetos consumidos %d\n", estado.objetos_consumidos);
    printf("Atributos finales: HP %d | Ataque %d | Defensa %d | XP %d \n", estado.jugador.HP, estado.jugador.ataque, estado.jugador.defensa, estado.jugador.XP);
    printf("\n========================================================================================\n");
    FILE *arch_h=fopen("highscore.txt","a");
    if(arch_h==NULL){
        printf("Error al abrir el archivo highscore, verifique la existencia y el formato del nisno y vuelva a intentarlo\n");
        return;
    }

    fprintf(arch_h,"%s %d %d %d %d %d %s\n", nombre_jugador, puntaje_final, estado.piso_maximo, estado.cuartos_explorados, estado.enemigos_derrotados, estado.objetos_consumidos, (victoria==V)? "Victoria":"Derrota");
    fclose(arch_h);
}

void iniciarCombate(EstadoJuego *estado, Entidad *enemigo){
    int opcion_com;
    T_bool combate_activo=V;

    printf("Te desafia %s!\n",(*enemigo).nombre);

    while ((*estado).jugador.HP>0 && (*enemigo).vida>0 && combate_activo==V){
        T_bool turno_consumido=F;
        
        printf("Tu: HP %d | Ataque %d | Defensa %d|\n", (*estado).jugador.HP, (*estado).jugador.ataque,(*estado).jugador.defensa);
        printf("%s: HP %d | Ataque %d | Defensa %d|\n", (*enemigo).nombre, (*enemigo).vida, (*enemigo).dano, (*enemigo).defensa);
        printf("\nTurno del jugador: Atacar(1) | Revisar la mochila(2) | Intentar huir(3)|\n\n");
        scanf("%d", &opcion_com);

        
        switch (opcion_com){
            case 1:{
                int dano_jugador=calcularDano((*estado).jugador.ataque, (*enemigo).defensa);
                (*enemigo).vida-=dano_jugador;

                printf("\n%s recibe %d de dano!\n", (*enemigo).nombre, dano_jugador);

                turno_consumido=V;
                break;
            }
            case 2:{
                printf("\nRevisas tu mochila...\n");
                usarObjeto(estado);
                turno_consumido=V;
                break;
            }
            case 3:{
                if(strcmp((*enemigo).tipo, "Boss")==1){
                    int prob_escape=((*estado).jugador.defensa*100)/((*estado).jugador.defensa+(*enemigo).dano),dado=rand()%100;

                    printf("Intentas huir del enemigo...\n");
                    if (dado<prob_escape){
                        printf("Has escapado\n");
                        combate_activo=F;
                    }else{
                        printf("Fallas al intentar escapar, y quedas expuesto al atque enemigo\n");
                        turno_consumido=V;
                    }
                    break;
                } else {
                    printf("\nLa risa de Sir Violeta hace retumbar las paredes, no podrás huir de esta pelea, las espadas moradas incrustadas a tu alrededor bloquean tu salida\n");
                }  
            
            }
            default: 
                printf("Valor invalido, intente de nuevo\n");
                break;
        }

        if ((*enemigo).vida<=0){
            combate_activo=F;
            break;
        }

        if (turno_consumido == V && combate_activo == V && (*enemigo).vida > 0) {
            int dano_enemigo = calcularDano((*enemigo).dano, (*estado).jugador.defensa);
            (*estado).jugador.HP -= dano_enemigo;
            
            printf(" %s te ataca e inflige %d de dano!\n", (*enemigo).nombre, dano_enemigo);
            if ((*estado).jugador.HP < 0) (*estado).jugador.HP = 0;
            printf("Tu HP restante: %d\n", (*estado).jugador.HP);

            if ((*estado).jugador.HP <= 0) {
                printf("\n Has muerto, tus restos ahora le pertenecen a la mazmorra y a sus habitantes... \n");
                registrarHighscore((*estado), F);
                combate_activo = F;
                break;
            }
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
        printf("\nEntre los restos del enemigo encuentras un objeto\n");
    }else{
        if((rand()%100)<40){
            entregar_objeto=1;
            printf("\nEntre los destrozos de la pelea encuentras un objeto\n");
        }else{
        printf("Entre los escombros de la batalla no hay mas que polvo, no has encontrado nada util\n");
        }
    }

    if(entregar_objeto==1 && total_objetos>0){
        if((*estado).items_en_mochila>= (*estado).n_mochila || (*estado).items_en_mochila>=100){
            printf("\nLa mochila se encuentra llena\n ");
            return;
        }
    }

    if(total_objetos>0){
        int indice_rand=rand()%total_objetos;
        Objetos item_encontrado=catalogo[indice_rand];

        (*estado).mochila[(*estado).items_en_mochila]=item_encontrado;
        (*estado).items_en_mochila++;

        printf("\nHas encontrado '%s'! Efecto: %s | Valor: %d\n", item_encontrado.nombre, item_encontrado.efecto, item_encontrado.valor);
    }
    
}

void verificarSubidaNivel(EstadoJuego *estado){
    int xp_por_nv=100;

    while((*estado).jugador.XP>=xp_por_nv){
        (*estado).jugador.XP-=xp_por_nv;

        (*estado).jugador.nivel++;
        (*estado).HP_base+=20;
        (*estado).jugador.HP=(*estado).HP_base;
        (*estado).jugador.defensa+=10;
        (*estado).jugador.ataque+=5;

        printf("\n=================================Has subido de nivel====================================\n");
        printf("                            Hp: %d, Def: %d, Atk: %d, NVL: %d\n", (*estado).jugador.HP,(*estado).jugador.defensa,(*estado).jugador.ataque), (*estado).jugador.nivel;
        printf("\n========================================================================================\n");
    }
}

void Pelea_final(EstadoJuego *estado){
    Entidad Jefe = {0, 500, 300, 200, "Sir Violeta", "Boss"};
    iniciarCombate(estado, &Jefe);
    if((*estado).jugador.HP <= 0){
        printf("\nEl vigor del veterano morado es mayor que tu voluntad, has sido derrotado...\ny contigo, se van todas las esperanzas de salvar al mundo de Sir Violeta\n");
    } else {
        printf("\nRecorres el ultimo cuarto de la mazmorra mientras el ambiente, antes lleno de criaturas hostiles y monstruosas...\nVuelve a su fin, tu alma se siente en paz mientras te adentras en las cavernas que yacen bajo este calabozo infernal\n");
        printf("Has derrotado a Sir Violeta y puedes sentir el porvenir de un Mundo de paz sin ese manto violeta cubriendo su hermosa luz...\n");
    }
}

void avanzarCuarto(EstadoJuego *estado, Entidad bestiario[], int total_enemigos, Objetos catalogo[], int total_objetos){
    int es_cuarto_final = 0, piso_actual = (*estado).piso_maximo, cuarto_actual = (*estado).cuartos_explorados, es_cuarto_jefe = 0;

    if((cuarto_actual==(*estado).cuartos_base-1 && piso_actual == 1) || (cuarto_actual==(*estado).cuartos_base && piso_actual != 1 && piso_actual != (*estado).pisos_base)){
        (*estado).cuartos_explorados = 1;
        (*estado).piso_maximo++;
        es_cuarto_final=1;
        printf("Desciendes en un nuevo piso de la mazmorra oscura, humeda y putrefacta\n");
    }else if(piso_actual == (*estado).piso_maximo && (cuarto_actual == (*estado).cuartos_base)) {
        (*estado).cuartos_explorados++;
        es_cuarto_jefe=1;
        printf("Has ingresado al ultimo cuarto de la mazmorra, preparate para el desafío que se presenta frente a tus ojos\n");
    } else {
        (*estado).cuartos_explorados++;
        printf("Avanzas por el piso hacia el proximo cuarto");
    }

    printf("\n========================================================================================\n");
    printf("\n                                PISO %d  CUARTO %d/%d\n", (*estado).piso_maximo, (*estado).cuartos_explorados,(*estado).cuartos_base);
    printf("\n========================================================================================\n");

    
    
    if(es_cuarto_jefe==1){
        printf("\nAparece una gran llama violeta que se disipa para mostrar a la maldad personificada en un gran caballero de armadura del mismo color del fuego\n");
        Pelea_final(estado);
        if((*estado).jugador.HP>0){
            BorrarPartidaGuardada();
            registrarHighscore((*estado),V);
            return;
        }
    } else {
        Entidad enemigo_actual = seleccionarEnemigo(bestiario,total_enemigos,es_cuarto_final);
        printf("%s se muestra frente a ti\n", enemigo_actual.nombre);
        iniciarCombate(estado, &enemigo_actual);
        if (enemigo_actual.vida<=0){
            printf("has derrotado a %s!\n", enemigo_actual.nombre);
            (*estado).enemigos_derrotados++;
            otorgarRecompensa(estado, catalogo, total_objetos, enemigo_actual.tipo);
            verificarSubidaNivel(estado);
        }
        if((*estado).jugador.HP<=0){
            return;
        }
    }

    
}

void MostrarHighscores(){
    int i, puntaje, piso_maximo, cuartos_explorados, enemigos_derrotados, objetos_consumidos;
    FILE *arch_h = fopen("highscore.txt","r");
    string nombre_jugador, resultado;

    if(arch_h == NULL){
        printf("Error al abrir el archivo highscore, verifique la existencia y el formato del nisno y vuelva a intentarlo\n");
        return;
    } else {
        printf("\n================================================================================================================================\n");
        for (i=0; i<5; i++){
            fscanf(arch_h,"%s %d %d %d %d %d %s\n", nombre_jugador, &puntaje, &piso_maximo, &cuartos_explorados, &enemigos_derrotados, &objetos_consumidos, resultado);
            printf("\n%s | Puntaje:%d | Piso maximo:%d | Cuarto alcanzado:%d | Enemigos derrotados:%d | Objetos consumidos:%d | Resultado:%s\n", nombre_jugador, puntaje, piso_maximo, cuartos_explorados, enemigos_derrotados, objetos_consumidos, resultado);
        }
        printf("\n================================================================================================================================\n");
    }

    fclose(arch_h);
}

void BorrarPartidaGuardada(){
    FILE *arch_save;
    arch_save = fopen("partida_guardada.txt", "w");
    fclose(arch_save);
}

void guardarPartida(EstadoJuego estado){
   // atributos, nivel actual, experiencia, piso, cuarto e inventario
    FILE *arch_save;
    BorrarPartidaGuardada();

    arch_save = fopen("partida_guardada.txt", "w");
    fprintf(arch_save, "%d %d %d %d %d %d %d %d %d %d %d", estado.cuartos_explorados, estado.piso_maximo, estado.enemigos_derrotados, estado.objetos_consumidos, estado.jugador.HP, estado.HP_base, estado.jugador.defensa, estado.jugador.ataque, estado.jugador.nivel, estado.jugador.XP, estado.items_en_mochila);
    fclose(arch_save);
}

int cargarPartida(EstadoJuego *estado){
    FILE *arch_save;
    arch_save = fopen("partida_guardada.txt", "r");
    
    if(arch_save == NULL){
        return 0;
    } 

    fscanf(arch_save, "%d %d %d %d %d %d %d %d %d %d %d", &(*estado).cuartos_explorados, &(*estado).piso_maximo, &(*estado).enemigos_derrotados, &(*estado).objetos_consumidos, &(*estado).jugador.HP, &(*estado).HP_base, &(*estado).jugador.defensa, &(*estado).jugador.ataque, &(*estado).jugador.nivel, &(*estado).jugador.XP, &(*estado).items_en_mochila);
    fclose(arch_save);
    return 1;
}