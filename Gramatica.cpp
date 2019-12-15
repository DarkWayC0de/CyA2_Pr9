/**
*@file Gramatica.cpp
*@version 1.0
*@date 19/11/2019
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
*@brief Este archivo contiene la implementacion de la clase
*@brief gramatica
*/

#include <fstream>
#include <iostream>
#include "Gramatica.h"

Gramatica::Gramatica(std::string const &archivo_grama): Simbolo_arranque_(),
                                                 Simbolos_noterminales_(),
                                                 Alfabeto_(),
                                                 Reglas_produccion_(){

  cargar_gramatica(archivo_grama);
}
Gramatica::Gramatica(): Simbolo_arranque_(),
                        Simbolos_noterminales_(),
                        Alfabeto_(),
                        Reglas_produccion_(){

}
void Gramatica::  cargar_gramatica(std::string const &archivo_gramatica) {
  std::string  const  IMPLICA="->";
  std::ifstream readfile(archivo_gramatica);
  if (readfile.is_open()){
    int n_terminales = std::stoi(this -> getlinea(readfile));
    for (int i = 0; i <n_terminales ; ++i) {
      Alfabeto_.insert( this -> getlinea(readfile));            ///Se lee el alfabeto
    }
    int n_noterminales = std::stoi( this -> getlinea( readfile));
    for (int j = 0; j <n_noterminales ; ++j) {
      Simbolos_noterminales_.insert( this -> getlinea( readfile));        /// Se lee Simbolos no terminales
    }
    Simbolo_arranque_ = this -> getlinea(readfile);
    if ( !pertenece(Simbolo_arranque_,Simbolos_noterminales_) ){
      std:: cout <<"\nERROR, Simbolo de arranque no pertenese a los Simbolos"
                     " No Terminales anteriormente definidos\n";
      exit(2);
    }
    int n_Producciones = std::stoi( this -> getlinea( readfile));
    Reglas_produccion_.resize(Simbolos_noterminales_.size());
    for (int k = 0; k < n_Producciones; ++k) {
      std::string  lineaProducion = getlinea(readfile);
      Simbolo_noterminal simbolo_de_producion;
      this -> extraerPalabra(lineaProducion,simbolo_de_producion);     /// Se extrae el simbolo de la producion
      if ( !pertenece( simbolo_de_producion, Simbolos_noterminales_)){
        std::cout<<"\nERROR, Simbolo de producion no pertenes a los Simbolos"
                   " No Terminales anterirmente definidos\n";
        exit(3);
      }
      std::string implica;
      this -> extraerPalabra(lineaProducion,implica); ///Se extra el simbolo de implicacion
      if( implica != IMPLICA){
        std::cout<<"\nERROR, No se ha encontrado el signo de implicacion\n";
        exit(4);
      }
      std::string producion;
      this -> extraerPalabra(lineaProducion,producion);
      if(comprobar_producion(producion)) {
        auto i = Simbolos_noterminales_.begin();
        int j;
        for (i = Simbolos_noterminales_.begin(), j = 0;
             i != Simbolos_noterminales_.end();
             ++i, ++j) {
          if ((*i) == simbolo_de_producion) {
            Reglas_produccion_[j].insert(producion);   /// Se guarda la producion
          }
        }
      } else {
        std::cout<<"\nERROR, Producion mal definada\n";
        exit(5);
      }
    }
    readfile.close();
  } else{
    std::cout<<"\nERROR, en la apertura de archivo .gra\n";
    exit(1);
  }
}
void Gramatica::escribir_archivo(std::string const &archivo_gramatica) {
  std::string  const  IMPLICA="->";
  std::ofstream writefile(archivo_gramatica);
  if(writefile.is_open()){
    writefile << Alfabeto_.size()<<"\n";
    for (auto k = Alfabeto_.begin();  k != Alfabeto_.end() ; ++k) {
      writefile<< *k<<"\n";                                           /// Se escribe el alfabeto
    }
    writefile << Simbolos_noterminales_.size()<<"\n";
    for (auto k = Simbolos_noterminales_.begin();
         k != Simbolos_noterminales_.end() ;
         ++k) {
      writefile<< *k<<"\n";                                      /// Se escribe los sibolos no terminales
    }
    writefile << Simbolo_arranque_ <<"\n";                      /// Se escribe el simbolo de arranque
    int n_produciones =0 ;
    for (int i = 0; i <Reglas_produccion_.size() ; ++i) {
      n_produciones +=  Reglas_produccion_[i].size();
    }
    writefile << n_produciones<<"\n";
    int j =0;
    for (auto i = Simbolos_noterminales_.begin();
         i != Simbolos_noterminales_.end(); ++i,
         ++j) {
      for (auto k = Reglas_produccion_[j].begin();
           k != Reglas_produccion_[j].end();
           ++ k) {
        writefile << (*i) << " "<< IMPLICA <<" "<< (*k) <<"\n";     ///Se escriben las pproduciones
      }
    }
    writefile.close();
  } else {
    std::cout<<"\nERROR, apertura de archivo de escritura de formato gra\n";
    exit(6);
  }

}
void Gramatica::tranformar_forma_normal_Chomsky() {
  std::vector<Simbolo_terminal> produciones_alfabeticas;       ///guardara los simbolos terminales que requieren una producion propias
  produciones_alfabeticas = buscar_produciones_alfabeticas();  /// Se buscan los simbolos terminales de producion propia
  generar_produciones_alfabeticas(produciones_alfabeticas);    /// Se generan las produciones propias de los simbolos terminales
  generar_produciones_noterminales();                          /// Desarrolla las produciones para quedarse son produciones de 2 miembros no terminales
}
std::string Gramatica::getlinea(std::ifstream &readfile){
  std::string const comentario = "//";
  std::string linea;
  do {
    std::getline(readfile, linea);
  }while (linea.substr(0,2) == comentario && !readfile.eof());
  return linea;
}
bool Gramatica::pertenece(const std::string& elemento,
                          const std::set<std::string>& conjunto){
  auto i = conjunto.begin();
  for ( ; i != conjunto.end(); i++){
    if( *i == elemento){
      break;
    }
  }
  return (i != conjunto.end());
}
void Gramatica::extraerPalabra(std::string &linea, std::string &palabra){
  char const ESPACIO =' ';
  int i = 0;
  for ( ; i < linea.length() ; ++i) {
    if (linea[i] == ESPACIO){
      break;
    }
  }
  palabra = linea.substr(0,i);
  if (i != linea.length()) {
    linea = linea.substr(i + 1, linea.length());
  }
}
bool Gramatica::comprobar_producion(const Producion& producion) {
  bool comprobar =false ;
  for (int i = 0; i <producion.length() ; ) {
    int j;
    for ( j = 1; j <=producion.length() ; ++j) {
      std::string cadena = producion.substr(i, j);
      if (pertenece(cadena, Simbolos_noterminales_)) {
        comprobar = true;
        break;
      } else {
        if (j == 1) {
          if (pertenece(cadena, Alfabeto_)) {
            comprobar = true;
            break;
          }
        }
        comprobar = false;
      }
    }
    i +=j;
  }
  return comprobar;
}
std::vector<Simbolo_terminal> Gramatica::buscar_produciones_alfabeticas() {
  std::vector<Simbolo_terminal> produciones_alfabeticas;
  for (int i = 0; i < Reglas_produccion_.size() ; ++i) {
    for (auto j = Reglas_produccion_[i].begin();
         j != Reglas_produccion_[i].end() ; ++j) {
      if ((*j).size() >= 2 ) {
        Producion producion =(*j);
        for (int m  = 0; m <producion.length() ; ) {
          int n;
          for (n = 1; n <=producion.length() ; ++n) {
            std::string cadena = producion.substr(m, n);
            if (pertenece(cadena, Simbolos_noterminales_)) {
              break;
            } else {
              if (n == 1) {
                if (pertenece(cadena, Alfabeto_)) {
                  produciones_alfabeticas.push_back(cadena);
                  break;
                }
              }
            }
          }
          m +=n;
        }
      }
    }
  }
  return produciones_alfabeticas;
}
void Gramatica::generar_produciones_alfabeticas(
                std::vector<Simbolo_terminal> produciones_alfabeticas) {
  std::set<Simbolo_noterminal> copia_simbolos_noterminles;
  copia_simbolos_noterminles= Simbolos_noterminales_;
  for (int i = 0; i <produciones_alfabeticas.size() ; ++i) {
    Simbolos_noterminales_.insert("C"+produciones_alfabeticas[i]);
  }
  std::vector<std::set<Producion>>nuevas_reglas_deproducion;
  nuevas_reglas_deproducion.resize(Simbolos_noterminales_.size());
  int j = 0;
  for (auto i = Simbolos_noterminales_.begin();
       i !=Simbolos_noterminales_.end();
       ++i, ++j) {
    int l = 0;
    for (auto k = copia_simbolos_noterminles.begin();
         k !=copia_simbolos_noterminles.end();
         ++k, ++l) {
      if((*i)==(*k)){
        nuevas_reglas_deproducion[j] = Reglas_produccion_[l];
      }
    }
    for (int m = 0; m <produciones_alfabeticas.size() ; ++m) {
      if((*i)== "C"+produciones_alfabeticas[m]){
        nuevas_reglas_deproducion[j].insert(produciones_alfabeticas[m]);
      }
    }
  }
  for (int i = 0; i < nuevas_reglas_deproducion.size() ; ++i) {
    std::set<Producion> nueva_regla_deproducion;
    for (auto j = nuevas_reglas_deproducion[i].begin();
         j != nuevas_reglas_deproducion[i].end() ; ++j) {
      if ((*j).size() >= 2 ) {
        Producion antigua_producion =(*j);
        Producion nueva_producion;
        for (int i = 0; i < antigua_producion.length(); ) {
          int j;
          for ( j = 1; j <= antigua_producion.length(); ++j) {
            std::string cadena = antigua_producion.substr(i, j);
            if (pertenece(cadena, Simbolos_noterminales_)) {
              nueva_producion +=cadena;
              break;
            } else {
              if (j == 1) {
                if (pertenece(cadena, Alfabeto_)) {
                  nueva_producion += "C"+cadena;
                  break;
                }
              }
            }
          }
          i +=j;
        }
        nueva_regla_deproducion.insert(nueva_producion);
      } else{
        nueva_regla_deproducion.insert(*j);
      }
    }
    nuevas_reglas_deproducion[i] = nueva_regla_deproducion;
  }
  Reglas_produccion_= nuevas_reglas_deproducion;
}
void Gramatica::generar_produciones_noterminales(char nombre_nueva_producion) {
  std::set<Simbolo_noterminal> nuevos_simbolos_noterminles;
  std::vector<std::pair<Simbolo_noterminal,std::set<Producion>>>nuevas_reglas_deproducion;
  auto k = this -> Simbolos_noterminales_.begin();
  for (int i = 0; i < Reglas_produccion_.size(); ++i, ++k) {
    for (auto j = Reglas_produccion_[i].begin();
         j != Reglas_produccion_[i].end(); ++j){
      if( (*j).length() == 1){
        int l;
        for ( l = 0; l <nuevas_reglas_deproducion.size() ; ++l) {
          if ( (*k)== nuevas_reglas_deproducion[l].first){
            break;
          }
        }
        if (l <nuevas_reglas_deproducion.size()){
          nuevas_reglas_deproducion[l].second.insert(*j);
        }else{
          std::pair<Simbolo_noterminal,std::set<Producion>> producion;
          producion.first=(*k);
          producion.second.insert(*j);
          nuevas_reglas_deproducion.push_back(producion);
        }
      } else {
        int contador_simbolos_noterminales = 0;
        for (int n = 0; n <=(*j).length() ; ) {
          int m =1;
          for ( ;n+m <=(*j).length(); ++m) {
            if (pertenece((*j).substr(n, m), Simbolos_noterminales_)) {
              contador_simbolos_noterminales++;
              break;
            }
          }
          n += m;
        }
        if (contador_simbolos_noterminales > 2){

          for (int m = 1 ;m <=(*j).length(); ++m) {
            if (pertenece((*j).substr(0, m), Simbolos_noterminales_)) {
              std::pair<Simbolo_noterminal, std::set<Producion>> producion;
              producion.first = (nombre_nueva_producion);

              producion.second.insert((*j).substr(m, (*j).length()));
              nuevas_reglas_deproducion.push_back(producion);
              int l;
              for ( l = 0; l <nuevas_reglas_deproducion.size() ; ++l) {
                if ( (*k)== nuevas_reglas_deproducion[l].first){
                  break;
                }
              }
              if (l <nuevas_reglas_deproducion.size()){
                nuevas_reglas_deproducion[l].second.insert(((*j).substr(0, m)+(nombre_nueva_producion)));
              }else{
                std::pair<Simbolo_noterminal,std::set<Producion>> producion;
                producion.first=(*k);
                producion.second.insert(((*j).substr(0, m)+(nombre_nueva_producion)));
                nuevas_reglas_deproducion.push_back(producion);
              }
              nombre_nueva_producion+=1;
              break;
            }
          }
        } else {
          int l;
          for ( l = 0; l <nuevas_reglas_deproducion.size() ; ++l) {
            if ( (*k)== nuevas_reglas_deproducion[l].first){
              break;
            }
          }
          if (l <nuevas_reglas_deproducion.size()){
            nuevas_reglas_deproducion[l].second.insert((*j));
          }else{
            std::pair<Simbolo_noterminal,std::set<Producion>> producion;
            producion.first=(*k);
            producion.second.insert((*j));
            nuevas_reglas_deproducion.push_back(producion);
          }
        }
      }
    }
  }
  for (int i = 0; i <nuevas_reglas_deproducion.size(); ++i) {
    nuevos_simbolos_noterminles.insert(nuevas_reglas_deproducion[i].first);
  }
  if(Simbolos_noterminales_ != nuevos_simbolos_noterminles) {
    this->Simbolos_noterminales_ = nuevos_simbolos_noterminles;
    this->Reglas_produccion_.clear();
    for (auto i = nuevos_simbolos_noterminles.begin();
         i != nuevos_simbolos_noterminles.end();
         i++) {
      for (int j = 0; j < nuevas_reglas_deproducion.size(); ++j) {
        if ((*i) == nuevas_reglas_deproducion[j].first) {
          this->Reglas_produccion_.push_back(nuevas_reglas_deproducion[j].second);
          break;
        }
      }
    }
    generar_produciones_noterminales(nombre_nueva_producion);
  }
}

