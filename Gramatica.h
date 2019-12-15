/**
 *@file Gramatica.h
 *@version 1.0
 *@date 18/11/2019
 *@author Diego Cruz Rodríguez
 *@title Clase Gramatica
 *@brief Universidad de La Laguna
 *@brief Escuela Superior de Ingeniería y Tecnología
 *@brief Grado en Ingeniería Informática
 *@brief Asignatura: Computabilidad y Algoritmia (CyA)
 *@brief Curso: 2º
 *@brief Práctica 9 CyA - Forma Normal de Chomsky
 *@brief Correo: alu0101105802@ull.edu.es
 *@brief
 *@brief Este archivo contiene la definicion de la clase
 *@brief gramatica y la defincion de Simbolo_terminal ,
 *@brief Simbolo_noterminal y Producion
 */

#ifndef GRAMATICA_H_
#define GRAMATICA_H_

#include <string>
#include <set>
#include <vector>

typedef std::string Simbolo_terminal;  /// Definicion de Simbolo_terminal
typedef std::string Simbolo_noterminal;/// Definicion de Simbolo_noterminal
typedef std::string Producion;         /// Definicion de Producion
class Gramatica {
 public:
  /**
   * @brief Constructor por defecto de la clase Gramatica
   */
  Gramatica();
  /**
   * @brief Destructor por defecto de la clase Gramatica
   */
  ~Gramatica() = default;
  /**
   * @brief Constructor de gramatica a partir de un archivo
   * @param archivo_gramatica  nombre del archivo que contiene la gramatica
   */
  Gramatica(std::string const &archivo_gramatica);
  /**
   * @brief Carga una gramatica a memoria a partir de un archivo
   * @param archivo_gramatica nombre del archivo que contiene la grmatica
   */
  void cargar_gramatica(std::string const &archivo_gramatica);
  /**
   * @brief Escribe en un archivo la gramatica que esta en memoria
   * @param archivo_gramatica  nombre del archivo en el que guarda la gramatica
   */
  void escribir_archivo(std::string const &archivo_gramatica);
  /**
   * @brief Tranforma la gramatica de memoria a forma normal de chomsky
   */
  void tranformar_forma_normal_Chomsky();
 private:
  std::string const CADENA_VACIA ="~";                ///Constante  de la cadena vacia
  std::set<Simbolo_noterminal> Simbolos_noterminales_;///Simbolos no terminales de la tupla Gramatica
  std::set<Simbolo_terminal> Alfabeto_;               ///Alfabeto de la tupla Gramatica
  Simbolo_noterminal Simbolo_arranque_;               ///Simbolo de Arranque de la tupla Gramatica
  std::vector<std::set<Producion>> Reglas_produccion_;///Reglas de Producion de la tupla Gramatica
  /**
   * @brief Extrae una linea apartir de un archivo de entrada, ignora los comentarios //
   * @param readfile  archivo del cual extrae la linea
   * @return devuelve la cadena extraida
   */
  std::string getlinea(std::ifstream &readfile);
  /**
   * @brief Comprueba si un elmento pertenece a un conjunto
   * @param elemento  elemento a comprobar su pertenecia
   * @param conjunto  conjunto en el que buscar el elemento
   * @return  devuelve un boleano que es verdadero si el elemento esta en el conjunto
   */
  bool pertenece(const std::string &elemento, const std::set<std::string> &conjunto);
  /**
   * @brief Extrae la primera palabra separada por espacio de una linea
   * @param linea  linea de la cual se extrae la palabra
   * @param palabra palabra extraida
   */
  void extraerPalabra(std::string &linea, std::string &palabra);
  /**
   * @brief Comprueba que una producion este compuesta por Simbolos no terminales y terminales de nuestra gramatica
   * @param producion  producion a comprobar
   * @return  devuelve verdadero si la producion es correcta
   */
  bool comprobar_producion(const Producion& producion);
  /**
   * @brief Identifica los simbolos terminales que pertenezcan a una producion compuesta
   * @return devuelve un vector de simbolos terminales para crear las produciones alfabeticas
   */
  std::vector<Simbolo_terminal> buscar_produciones_alfabeticas();
  /**
   * @brief apartir de una lista de terminales, genera las produciones de esos terminales y las sustitulle en las
   * @brief produciones compuestas
   * @param produciones_alfabeticas  lista terminales a generar
   */
  void generar_produciones_alfabeticas(std::vector<Simbolo_terminal> produciones_alfabeticas);
  /**
   * @brief Crea nuevas produciones para permitir que la gramatica solo se quede con produciones de 2 no terminles
   * @param nombre_nueva_producion
   */
  void generar_produciones_noterminales(char nombre_nueva_producion = 'D');
};

#endif //GRAMATICA_H_
