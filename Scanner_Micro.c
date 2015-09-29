/*
 ============================================================================
 N
ame        : Scanner_Micro.c
 Author      : Luciano
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define BUFMAX 33

typedef enum{
   INICIO, FIN, LEER, ESCRIBIR, ID, CONSTANTE, PARENIZQUIERDO,
   PARENDERECHO, PUNTOYCOMA, COMA, ASIGNACION, SUMA, RESTA, FDT
} TOKEN;

//int verifica(char s);
int columna(int car);
int automata (int car, int estado_actual);
void AgregarCaracter(int car);
void LimpiarBuffer(void);
TOKEN EsReservada(void);
void ImprimeToken(TOKEN a);
TOKEN Scanner(void);
int estado_actual=0;
int i=0;
char  buffer[BUFMAX];
char caracter;
int c;

FILE * in;

int main(int argc, char * argv[]) {

   if(argc==1)
   {
      printf("Debe Ingresar el Nombre del Archivo\n");
      return -1;
   }
   if(argc!=2)
   {
      printf("Nombre Incorrecto de Argumento\n");
      return -2;
   }
   if((in = fopen(argv[1],"r"))==NULL)
   {
      printf("No se puede Abrir el Archivo\n");
      return -3;
   }


   TOKEN a;

   while (((a=Scanner())!=13))
   {
      ImprimeToken(a);
      if(a==-4)
      {
         printf("ERROR LEXICO");
         break;
      }
   }

   fclose(in);
}

TOKEN Scanner(void)
{

   while(feof(in)==0)
   {
      caracter=getc(in);
      c = caracter;

      estado_actual=automata(columna(c),estado_actual);

      switch(estado_actual){
         case 1 : case 3:
            AgregarCaracter(caracter);
         break;
         case 2 :
            ungetc(c,in);
            estado_actual=0;
            //Verificar si es Palabra reservada o un identificador
            //Si es un identificador, verificar que no excedio los 32 caracteres
            //Si es un identificador correcto almacenarlo en la Tabla de Simbolos
            return EsReservada();
         break;
         case 4:
            ungetc(c,in);
            estado_actual=0;
            //Almacenar Secuencia de Digitos en Tabla de Simbolos
            LimpiarBuffer();
            return 5;
         break;
         case 5:
            estado_actual=0;
            return 11;
         break;
         case 6:
            estado_actual=0;
            return 12;
         break;
         case 7:
            estado_actual=0;
            return 6;
         break;
         case 8:
            estado_actual=0;
            return 7;
         break;
         case 9:
            estado_actual=0;
            return 9;
         break;
         case 10:
            estado_actual=0;
            return 8;
         break;
         case 12:
            estado_actual=0;
            return 10;
         break;
         case 14:
            return -4;
         break;
         default:
         break;
      }
   }
   if (estado_actual==13 || feof(in)!=0) return 13;

}

int columna(int car)
{

   if ((isalpha(car))!=0) car=0;
   else if((isdigit(car))!=0) car=1;
   else if ((isspace(car))!=0) car=11;
   else if (feof(in)!=0) car=10;

   switch (car)
   {
      case 0 :
      return 0;
      break;
      case 1 :
      return 1;
      break;
      case '+':
      return 2;
      break;
      case '-':
      return 3;
      break;
      case '(':
      return 4;
      break;
      case ')':
      return 5;
      break;
      case ',':
      return 6;
      break;
      case ';':
      return 7;
      break;
      case ':':
      return 8;
      break;
      case '=':
      return 9;
      break;
      case 10:
      return 10;
      break;
      case 11 :
      return 11;
      break;
      default:
      return 12;
      break;
   }
}

int automata (int car, int estado_act)
{
   static int tabla[16][13]={{ 1, 3, 5, 6, 7, 8, 9,10,11,14,13, 0,14},
                             { 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             { 4, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {14,14,14,14,14,14,14,14,14,12,14,14,14},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15},
                             {15,15,15,15,15,15,15,15,15,15,15,15,15}};

   int estado;

   estado=tabla[estado_act][car];
   return estado;
}

void AgregarCaracter(int car)
{
   buffer[i]=car;
   i++;
   buffer[i]='\0';
}

void LimpiarBuffer(void)
{
   i=0;
   buffer[i]='\0';
}

TOKEN EsReservada(void)
{
   if (strcmp(buffer,"INICIO")==0)
   {
      LimpiarBuffer();
      return 0;
   }
   else if (strcmp(buffer,"FIN")==0)
   {
      LimpiarBuffer();
      return 1;
   }
   else if (strcmp(buffer,"LEER")==0)
   {
      LimpiarBuffer();
      return 2;
   }
   else if (strcmp(buffer,"ESCRIBIR")==0)
   {
      LimpiarBuffer();
      return 3;
   }
   else
   {
      LimpiarBuffer();
      return 4;
   }
}

void ImprimeToken(TOKEN a)
{
   switch(a)
   {
      case 0:
         printf("INICIO\n");
      break;
      case 1:
         printf("FIN\n");
      break;
      case 2:
         printf("LEER\n");
      break;
      case 3:
         printf("ESCRIBIR\n");
      break;
      case 4:
         printf("ID\n");
      break;
      case 5:
         printf("CONSTANTE\n");
      break;
      case 6:
         printf("PARENTEIZQUIERDO\n");
      break;
      case 7:
         printf("PARENTEDERECHO\n");
      break;
      case 8:
         printf("PUNTOYCOMA\n");
      break;
      case 9:
         printf("COMA\n");
      break;
      case 10:
         printf("ASIGNACION\n");
      break;
      case 11:
         printf("SUMA\n");
      break;
      case 12:
         printf("RESTA\n");
      break;
      default:
         printf("FDT\n");
      break;
   }
}
