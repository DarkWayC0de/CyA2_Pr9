/**
 *@file G2CNF.cpp
 *@version 1.0
 *@date 16/11/2019
 *@author Diego Cruz Rodríguez
 *@title Archivo Principal
 *@brief Universidad de La Laguna
 *@brief Escuela Superior de Ingeniería y Tecnología
 *@brief Grado en Ingeniería Informática
 *@brief Asignatura: Computabilidad y Algoritmia (CyA)
 *@brief Curso: 2º
 *@brief Práctica 9 CyA - Forma Normal de Chomsky
 *@brief Correo: alu0101105802@ull.edu.es
 *@brief
 *@brief Este Archivo contiene la funcion principal del programa,
 *@brief en ella se realiza la gestiosn y comprobacion de los
 *@brief argumentos, y ejecucion de la clase gramatica  y el metodo
 *@brief tranformación de Chomsky
 */

#include <iostream>
#include <string>
#include "Gramatica.h"
int main(int argc, char *argv[]){
  std::string const AYUDA = "--help";
  std::string const ARCHIVO_GRAMA = ".gra";
  switch (argc) {
    case 3 : {
      std::string nombreArchivoLectura = argv[1];
      std::string nombreArchivoEscritura = argv[2];
      std::string extencion_grama = nombreArchivoLectura.substr(nombreArchivoLectura.length() - 4, 4);
      if (extencion_grama == ARCHIVO_GRAMA) {
        extencion_grama = nombreArchivoEscritura.substr(nombreArchivoEscritura.length() - 4, 4);
        if (extencion_grama == ARCHIVO_GRAMA) {
          Gramatica gramatica(nombreArchivoLectura);
          gramatica.tranformar_forma_normal_Chomsky();
          gramatica.escribir_archivo(nombreArchivoEscritura);
          break;
        }
      }
    }
    case 2 : {
      std::string comando = argv[1];
      if (comando == AYUDA) {
        std::cout<<"\nEste programa tranforma una grámatica independiente del contexto\n"
                 <<"con formato .gra a una grámatica equivalente a la de entrada, escrita\n"
                 <<"en Forma Normal de Chonsky con formato .gra para su correcto\n"
                 <<"funcionamiento se debe introducir un fichero de entrada input.gra\n"
                 <<"y un fichero de salida output.gra\n\n"
                 <<"---------------------------------------------------------\n\n"
                 <<"Ejemplo de uso:\n"
                 <<"$ ./G2CNF input.gra output.gra\n\n";
        break;
      }
    }
    case 1 :
    default :
      std::cout<<"Modo de empleo: ./G2CNF input.gra output.gra\n"
               <<"Pruebe './G2CNF --help' para más información.\n";
  }
  return 0;
}
