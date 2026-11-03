#include <stdio.h>
#include <string.h>

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


//ACIONES NOMINADAS
int Calculo_Daño(int def, int ata);
void Combate(int Vida);

int main(){
  //VARIABLES
    Enemigos Enemies_v[5] = {0};
    int pisos, cuartos_base, mochila[10], objetos[20], i;
    FILE *in_enem;

    in_enem = fopen("bestiario.txt", "r");

    if (in_enem == NULL){
        printf("no se abre el archivo");
        return 1;
    }

    for(i=0; i<5; i++){
        fscanf(in_enem, "%d %s %d %d %d %s", &Enemies_v[i].ID, Enemies_v[i].nombre, &Enemies_v[i].vida, &Enemies_v[i].dano, &Enemies_v[i].defensa, Enemies_v[i].tipo);   
    }
    
    for(i=0; i<5; i++) {
        printf("\n%s\n", Enemies_v[i].nombre);
    }



  return 0;
}