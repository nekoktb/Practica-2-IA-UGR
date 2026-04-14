#include "ingeniero.hpp"
#include "motorlib/util.h"
#include <iostream>
#include <queue>
#include <set>

using namespace std;

// =========================================================================
// ÁREA DE IMPLEMENTACIÓN DEL ESTUDIANTE
// =========================================================================

Action ComportamientoIngeniero::think(Sensores sensores)
{
  Action accion = IDLE;

  // Decisión del agente según el nivel
  switch (sensores.nivel)
  {
  case 0:
    accion = ComportamientoIngenieroNivel_0(sensores);
    break;
  case 1:
    accion = ComportamientoIngenieroNivel_1(sensores);
    break;
  case 2:
    accion = ComportamientoIngenieroNivel_2(sensores);
    break;
  case 3:
    accion = ComportamientoIngenieroNivel_3(sensores);
    break;
  case 4:
    accion = ComportamientoIngenieroNivel_4(sensores);
    break;
  case 5:
    accion = ComportamientoIngenieroNivel_5(sensores);
    break;
  case 6:
    accion = ComportamientoIngenieroNivel_6(sensores);
    break;
  }

  return accion;
}

/**
 * @brief Determina si casilla viable por altura.
 * @param casilla tipo de terreno
 * @param dif diferencia de altura entre casillas
 * @param zap indica si estoy en posesión de las zapatillas
 * @return 'P' si no es accesible por altura y casilla en otro caso
 */
char ViablePorAlturaI(char casilla, int dif, bool zap){
  if (abs(dif) <= 1 or (zap and abs(dif) <= 2))
    return casilla;
  else
    return 'P';
}

/**
 * @brief Determina la mejor opcion entre las 3 casillas que tiene delante.
 * @param i terreno que hay en la posición 1 de superficie (45 izq)
 * @param c terreno que hay en la posición 2 de superficie (justo delante)
 * @param d terreno que hay en la posición 3 de superficie (45 dch)
 * @param vis_i número de visitas a la casilla izquierda
 * @param vis_c número de visitas a la casilla central
 * @param vis_d número de visitas a la casilla derecha
 * @param zap indica si el agente tiene zapatillas
 * @return 2 si es mejor WALK, 1 para TURN_SL y 3 para TURN_SR. 0 no hay nada interesante.
 */
int VeoCasillaInteresanteI_Nivel0(char i, char c, char d, int vis_i, int vis_c, int vis_d, bool zap){
<<<<<<< HEAD
  // Prioridad absoluta a la meta ('U')
  if (c == 'U') return 2; // Centro
  if (i == 'U') return 1; // Izquierda
  if (d == 'U') return 3; // Derecha

=======
>>>>>>> b5a4dd27023c0eea04d51eaccd7a43b2360f0664
  int mejor_opcion = 0;     
  int min_visitas = 999999; // Récord de visitas altísimo para que cualquier casilla lo mejore

  // 1. Primero, calculamos si cada casilla es "transitable" (si podemos pisarla o nos interesa)
<<<<<<< HEAD
  // Para el ingeniero, nos interesa si es 'U' (tratamiento de residuos), 'C' (camino) o 'D' (zapatillas)
  bool transitable_i = (i == 'U' || i == 'C' || (i == 'D'));
  bool transitable_c = (c == 'U' || c == 'C' || (c == 'D'));
  bool transitable_d = (d == 'U' || d == 'C' || (d == 'D'));
=======
  // Para el ingeniero, nos interesa si es 'U' (tratamiento de residuos), 'C' (camino) 
  // o 'D' (zapatillas) SOLO si aún no tenemos zapatillas (!zap).
  bool transitable_i = (i == 'U' || i == 'C' || (i == 'D' && !zap));
  bool transitable_c = (c == 'U' || c == 'C' || (c == 'D' && !zap));
  bool transitable_d = (d == 'U' || d == 'C' || (d == 'D' && !zap));
>>>>>>> b5a4dd27023c0eea04d51eaccd7a43b2360f0664

  // 2. Evaluamos la casilla del CENTRO
  if (transitable_c == true && vis_c < min_visitas) { 
      mejor_opcion = 2;       // 2 significa avanzar de frente
      min_visitas = vis_c;    
  }

  // 3. Evaluamos la casilla de la IZQUIERDA
  if (transitable_i == true && vis_i < min_visitas) { 
      mejor_opcion = 1;       // 1 significa girar a la izquierda
      min_visitas = vis_i;
  }

  // 4. Evaluamos la casilla de la DERECHA
  if (transitable_d == true && vis_d < min_visitas) { 
      mejor_opcion = 3;       // 3 significa girar a la derecha
      min_visitas = vis_d;
  }

  // Devolvemos la opción ganadora (la que menos visitas tenía)
  return mejor_opcion;

}


// Niveles iniciales (Comportamientos reactivos simples)
Action ComportamientoIngeniero::ComportamientoIngenieroNivel_0(Sensores sensores)
{
  Action accion = IDLE;

  // Si está evadiendo al técnico, continuar girando
  if (giro45Izq > 0) {
    accion = TURN_SL;
    giro45Izq--;
    last_action = accion;
    return accion;
  }

  // Actualiza el mapa con la visión actual
  ActualizarMapa(sensores);

  // registrar visita a la casilla actual
  mapa_visitas[{sensores.posF, sensores.posC}]++;

  if (sensores.superficie[0] == 'D') tiene_zapatillas = true;
  if (sensores.superficie[0] == 'U') return IDLE;

  ubicacion estado_actual;
  estado_actual.f = sensores.posF;
  estado_actual.c = sensores.posC;
  estado_actual.brujula = sensores.rumbo;

  // Visitas Centro (frente directo)
  ubicacion pos_c = Delante(estado_actual);
  int vis_c = mapa_visitas[{pos_c.f, pos_c.c}];

  // Visitas Izquierda (rumbo - 1)
  ubicacion estado_izq = estado_actual;
  estado_izq.brujula = (Orientacion)(((int)estado_actual.brujula + 7) % 8);
  ubicacion pos_i = Delante(estado_izq);
  int vis_i = mapa_visitas[{pos_i.f, pos_i.c}];

  // Visitas Derecha (rumbo + 1)
  ubicacion estado_dch = estado_actual;
  estado_dch.brujula = (Orientacion)(((int)estado_actual.brujula + 1) % 8);
  ubicacion pos_d = Delante(estado_dch);
  int vis_d = mapa_visitas[{pos_d.f, pos_d.c}];

 
  char i = ViablePorAlturaI(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
  char c = ViablePorAlturaI(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
  char d = ViablePorAlturaI(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);
  
  int pos = VeoCasillaInteresanteI_Nivel0(i, c, d, vis_i, vis_c, vis_d, tiene_zapatillas);

  switch (pos){
  case 2:
    if (!sensores.choque) accion = WALK;
    else {
      accion = TURN_SL;
      giro45Izq = 3; // para dar la vuelta completa
    }
    break;
  case 1:
    accion = TURN_SL;
    break;
  case 3:
    accion = TURN_SR;
    break;  
  default:
    accion = TURN_SL;
    break;
  }
  
  //devolver la siguiente accion a hacer
  last_action = accion;
  return accion;

}

/**
 * @brief Comprueba si una celda es de tipo camino transitable.
 * @param c Carácter que representa el tipo de superficie.
 * @return true si es camino ('C'), zapatillas ('D') o meta ('U').
 */
bool ComportamientoIngeniero::es_camino(unsigned char c) const
{
  return (c == 'C' || c == 'D' || c == 'U');
}

/**
 * @brief Función de evaluación para decidir la mejor casilla a elegir en el nivel 1, considerando el tipo de terreno
 * para darle prioridad a los caminos y senderos (C y S).
 * @param terreno Carácter que representa el tipo de terreno 
 * @param visitas Número de veces que se ha visitado esa casilla
 * @return Un valor que combina el número de visitas con una penalización si el terreno no es un camino o sendero.
 */
int CondicionaPesoI_Nivel1(char terreno, int visitas){
  if (terreno == 'C' || terreno == 'S' || terreno == 'D') {
    return visitas; // Sin penalización para caminos, senderos y zapatillas (para evitar que no pueda continuar)
  } else return visitas + 1000; // Penalización alta para zonas sin tratar
}

/**
 * @brief Determina la mejor opcion entre las 3 casillas que tiene delante.
 * @param i terreno que hay en la posición 1 de superficie (45 izq)
 * @param c terreno que hay en la posición 2 de superficie (justo delante)
 * @param d terreno que hay en la posición 3 de superficie (45 dch)
 * @param vis_i número de visitas a la casilla izquierda
 * @param vis_c número de visitas a la casilla central
 * @param vis_d número de visitas a la casilla derecha
 * @param zap indica si el agente tiene zapatillas
 * @param can_jump indica si se puede saltar sobre la casilla central
 * @return 2 si es mejor WALK, 1 para TURN_SL y 3 para TURN_SR. 0 no hay nada interesante.
 */
int VeoCasillaInteresanteI_Nivel1(char i, char c, char d, int vis_i, int vis_c, int vis_d, bool zap, bool can_jump){
  int mejor_opcion = 0;     
  int min_visitas = 999999; // Récord de visitas altísimo para que cualquier casilla lo mejore

  // Ahora los senderos tambien son transitables (S)
  bool transitable_i = (i == 'U' || i == 'C' || i == 'S' || (i == 'D' && !zap));
  bool transitable_c = (c == 'U' || c == 'C' || c == 'S' || (c == 'D' && !zap) || can_jump);
  bool transitable_d = (d == 'U' || d == 'C' || d == 'S' || (d == 'D' && !zap));


  if (transitable_c == true && CondicionaPesoI_Nivel1(c, vis_c) < min_visitas) { 
      mejor_opcion = 2;       // avanzar de frente
      min_visitas = CondicionaPesoI_Nivel1(c, vis_c);
  }
  if (transitable_i == true && CondicionaPesoI_Nivel1(i, vis_i) < min_visitas) { 
      mejor_opcion = 1;       // girar a la izquierda
      min_visitas = CondicionaPesoI_Nivel1(i, vis_i);
  }
  if (transitable_d == true && CondicionaPesoI_Nivel1(d, vis_d) < min_visitas) { 
      mejor_opcion = 3;       // girar a la derecha
      min_visitas = CondicionaPesoI_Nivel1(d, vis_d);
  }

  // Devolvemos la opción ganadora (la que menos visitas tenía)
  return mejor_opcion;

}


/**
 * @brief Determina si una casilla es transitable para el ingeniero.
 * @param f Fila de la casilla.
 * @param c Columna de la casilla.
 * @param tieneZapatillas Indica si el agente posee las zapatillas.
 * @return true si la casilla es transitable
 */
bool ComportamientoIngeniero::EsCasillaTransitableLevel1(int f, int c, bool tieneZapatillas)
{
  if (f < 0 || f >= mapaResultado.size() || c < 0 || c >= mapaResultado[0].size())
    return false;
  return (mapaResultado[f][c]=='C' || mapaResultado[f][c]=='S' || mapaResultado[f][c]=='D' || mapaResultado[f][c]=='U' );
}
/**
 * @brief Comportamiento reactivo del ingeniero para el Nivel 1.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoIngeniero::ComportamientoIngenieroNivel_1(Sensores sensores)
{
  
  Action accion = IDLE;

  // Si está evadiendo al técnico, continuar girando
  if (giro45Izq > 0) {
    accion = TURN_SL;
    giro45Izq--;
    last_action = accion;
    return accion;
  }

  // Actualiza el mapa con la visión actual
  ActualizarMapa(sensores);

  // registrar visita a la casilla actual
  mapa_visitas[{sensores.posF, sensores.posC}]++;

  if (sensores.superficie[0] == 'D') tiene_zapatillas = true;
  if (sensores.superficie[0] == 'U') return IDLE;

  ubicacion estado_actual;
  estado_actual.f = sensores.posF;
  estado_actual.c = sensores.posC;
  estado_actual.brujula = sensores.rumbo;

  // Verificar si la casilla detrás es transitable para evitar caerse al saltar
  ubicacion dos_pasos = Delante(Delante(estado_actual));
<<<<<<< HEAD
  bool can_jump = EsCasillaTransitableLevel1(dos_pasos.f, dos_pasos.c, tiene_zapatillas);
=======
  bool terreno_detras_transitable = EsCasillaTransitableLevel1(dos_pasos.f, dos_pasos.c, tiene_zapatillas);
>>>>>>> b5a4dd27023c0eea04d51eaccd7a43b2360f0664

  // Visitas Centro (frente directo)
  ubicacion pos_c = Delante(estado_actual);
  int vis_c = mapa_visitas[{pos_c.f, pos_c.c}];

  // Visitas Izquierda (rumbo - 1)
  ubicacion estado_izq = estado_actual;
  estado_izq.brujula = (Orientacion)(((int)estado_actual.brujula + 7) % 8);
  ubicacion pos_i = Delante(estado_izq);
  int vis_i = mapa_visitas[{pos_i.f, pos_i.c}];

  // Visitas Derecha (rumbo + 1)
  ubicacion estado_dch = estado_actual;
  estado_dch.brujula = (Orientacion)(((int)estado_actual.brujula + 1) % 8);
  ubicacion pos_d = Delante(estado_dch);
  int vis_d = mapa_visitas[{pos_d.f, pos_d.c}];

  char i = ViablePorAlturaI(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
  //char c = ViablePorAlturaI(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
  char c = sensores.superficie[2]; // Para el centro, no considerar la altura para permitir saltar aunque haya un desnivel
<<<<<<< HEAD
  if (c == 'P' || c == 'B' || c == 'M' ) 
    can_jump = false; // Si el terreno intermedio es Precipicio/Bosque/Muro eliminamos la opcion de saltar.
=======
  if (c == 'P') terreno_detras_transitable = false; // Si el terreno central es precipicio eliminamos la opcion de saltar.
>>>>>>> b5a4dd27023c0eea04d51eaccd7a43b2360f0664
  char d = ViablePorAlturaI(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);

  // Para 'U', considerarlo viable aunque no lo sea por altura, para poder saltar
  if (sensores.superficie[2] == 'U') c = 'U';
  
<<<<<<< HEAD
  int pos = VeoCasillaInteresanteI_Nivel1(i, c, d, vis_i, vis_c, vis_d, tiene_zapatillas, can_jump);
=======
  int pos = VeoCasillaInteresanteI_Nivel1(i, c, d, vis_i, vis_c, vis_d, tiene_zapatillas, terreno_detras_transitable);
>>>>>>> b5a4dd27023c0eea04d51eaccd7a43b2360f0664

  switch (pos){
  case 2:
    if (!sensores.choque) {
      if (sensores.superficie[2] == 'U') {
<<<<<<< HEAD
        if (can_jump) {
=======
        if (terreno_detras_transitable) {
>>>>>>> b5a4dd27023c0eea04d51eaccd7a43b2360f0664
          accion = JUMP;
        } else {
          accion = TURN_SL;
          giro45Izq = 3; // para dar la vuelta completa
        }
<<<<<<< HEAD
      } else if ( !ViablePorAlturaI(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas) && can_jump) {
        accion = JUMP;
      } else if (ViablePorAlturaI(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas)) {
=======
      } else if ( !ViablePorAlturaI(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas) && terreno_detras_transitable) {
        accion = JUMP;
      } else {
>>>>>>> b5a4dd27023c0eea04d51eaccd7a43b2360f0664
        accion = WALK;
      }
    } else {
      accion = TURN_SL;
      giro45Izq = 3; // para dar la vuelta completa
    }
    break;
  case 1:
    accion = TURN_SL;
    break;
  case 3:
    accion = TURN_SR;
    break;  
  default:
    accion = TURN_SL;
    break;
  }
  
  //devolver la siguiente accion a hacer
  last_action = accion;
  return accion;

}

// Niveles avanzados (Uso de búsqueda)
/**
 * @brief Comportamiento del ingeniero para el Nivel 2 (búsqueda).
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoIngeniero::ComportamientoIngenieroNivel_2(Sensores sensores)
{
  // TODO: Implementar búsqueda para el Nivel 2.
  return IDLE;
}

/**
 * @brief Comportamiento del ingeniero para el Nivel 3.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoIngeniero::ComportamientoIngenieroNivel_3(Sensores sensores)
{
  return IDLE;
}

/**
 * @brief Comportamiento del ingeniero para el Nivel 4.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoIngeniero::ComportamientoIngenieroNivel_4(Sensores sensores)
{
  return IDLE;
}

/**
 * @brief Comportamiento del ingeniero para el Nivel 5.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoIngeniero::ComportamientoIngenieroNivel_5(Sensores sensores)
{
  return IDLE;
}

/**
 * @brief Comportamiento del ingeniero para el Nivel 6.
 * @param sensores Datos actuales de los sensores.
 * @return Acción a realizar.
 */
Action ComportamientoIngeniero::ComportamientoIngenieroNivel_6(Sensores sensores)
{
  return IDLE;
}

// =========================================================================
// FUNCIONES PROPORCIONADAS
// =========================================================================

/**
 * @brief Actualiza el mapaResultado y mapaCotas con la información de los sensores.
 * @param sensores Datos actuales de los sensores.
 */
void ComportamientoIngeniero::ActualizarMapa(Sensores sensores)
{
  mapaResultado[sensores.posF][sensores.posC] = sensores.superficie[0];
  mapaCotas[sensores.posF][sensores.posC] = sensores.cota[0];

  int pos = 1;
  switch (sensores.rumbo)
  {
  case norte:
    for (int j = 1; j < 4; j++)
      for (int i = -j; i <= j; i++)
      {
        mapaResultado[sensores.posF - j][sensores.posC + i] = sensores.superficie[pos];
        mapaCotas[sensores.posF - j][sensores.posC + i] = sensores.cota[pos++];
      }
    break;
  case noreste:
    mapaResultado[sensores.posF - 1][sensores.posC] = sensores.superficie[1];
    mapaCotas[sensores.posF - 1][sensores.posC] = sensores.cota[1];
    mapaResultado[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[2];
    mapaCotas[sensores.posF - 1][sensores.posC + 1] = sensores.cota[2];
    mapaResultado[sensores.posF][sensores.posC + 1] = sensores.superficie[3];
    mapaCotas[sensores.posF][sensores.posC + 1] = sensores.cota[3];
    mapaResultado[sensores.posF - 2][sensores.posC] = sensores.superficie[4];
    mapaCotas[sensores.posF - 2][sensores.posC] = sensores.cota[4];
    mapaResultado[sensores.posF - 2][sensores.posC + 1] = sensores.superficie[5];
    mapaCotas[sensores.posF - 2][sensores.posC + 1] = sensores.cota[5];
    mapaResultado[sensores.posF - 2][sensores.posC + 2] = sensores.superficie[6];
    mapaCotas[sensores.posF - 2][sensores.posC + 2] = sensores.cota[6];
    mapaResultado[sensores.posF - 1][sensores.posC + 2] = sensores.superficie[7];
    mapaCotas[sensores.posF - 1][sensores.posC + 2] = sensores.cota[7];
    mapaResultado[sensores.posF][sensores.posC + 2] = sensores.superficie[8];
    mapaCotas[sensores.posF][sensores.posC + 2] = sensores.cota[8];
    mapaResultado[sensores.posF - 3][sensores.posC] = sensores.superficie[9];
    mapaCotas[sensores.posF - 3][sensores.posC] = sensores.cota[9];
    mapaResultado[sensores.posF - 3][sensores.posC + 1] = sensores.superficie[10];
    mapaCotas[sensores.posF - 3][sensores.posC + 1] = sensores.cota[10];
    mapaResultado[sensores.posF - 3][sensores.posC + 2] = sensores.superficie[11];
    mapaCotas[sensores.posF - 3][sensores.posC + 2] = sensores.cota[11];
    mapaResultado[sensores.posF - 3][sensores.posC + 3] = sensores.superficie[12];
    mapaCotas[sensores.posF - 3][sensores.posC + 3] = sensores.cota[12];
    mapaResultado[sensores.posF - 2][sensores.posC + 3] = sensores.superficie[13];
    mapaCotas[sensores.posF - 2][sensores.posC + 3] = sensores.cota[13];
    mapaResultado[sensores.posF - 1][sensores.posC + 3] = sensores.superficie[14];
    mapaCotas[sensores.posF - 1][sensores.posC + 3] = sensores.cota[14];
    mapaResultado[sensores.posF][sensores.posC + 3] = sensores.superficie[15];
    mapaCotas[sensores.posF][sensores.posC + 3] = sensores.cota[15];
    break;
  case este:
    for (int j = 1; j < 4; j++)
      for (int i = -j; i <= j; i++)
      {
        mapaResultado[sensores.posF + i][sensores.posC + j] = sensores.superficie[pos];
        mapaCotas[sensores.posF + i][sensores.posC + j] = sensores.cota[pos++];
      }
    break;
  case sureste:
    mapaResultado[sensores.posF][sensores.posC + 1] = sensores.superficie[1];
    mapaCotas[sensores.posF][sensores.posC + 1] = sensores.cota[1];
    mapaResultado[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[2];
    mapaCotas[sensores.posF + 1][sensores.posC + 1] = sensores.cota[2];
    mapaResultado[sensores.posF + 1][sensores.posC] = sensores.superficie[3];
    mapaCotas[sensores.posF + 1][sensores.posC] = sensores.cota[3];
    mapaResultado[sensores.posF][sensores.posC + 2] = sensores.superficie[4];
    mapaCotas[sensores.posF][sensores.posC + 2] = sensores.cota[4];
    mapaResultado[sensores.posF + 1][sensores.posC + 2] = sensores.superficie[5];
    mapaCotas[sensores.posF + 1][sensores.posC + 2] = sensores.cota[5];
    mapaResultado[sensores.posF + 2][sensores.posC + 2] = sensores.superficie[6];
    mapaCotas[sensores.posF + 2][sensores.posC + 2] = sensores.cota[6];
    mapaResultado[sensores.posF + 2][sensores.posC + 1] = sensores.superficie[7];
    mapaCotas[sensores.posF + 2][sensores.posC + 1] = sensores.cota[7];
    mapaResultado[sensores.posF + 2][sensores.posC] = sensores.superficie[8];
    mapaCotas[sensores.posF + 2][sensores.posC] = sensores.cota[8];
    mapaResultado[sensores.posF][sensores.posC + 3] = sensores.superficie[9];
    mapaCotas[sensores.posF][sensores.posC + 3] = sensores.cota[9];
    mapaResultado[sensores.posF + 1][sensores.posC + 3] = sensores.superficie[10];
    mapaCotas[sensores.posF + 1][sensores.posC + 3] = sensores.cota[10];
    mapaResultado[sensores.posF + 2][sensores.posC + 3] = sensores.superficie[11];
    mapaCotas[sensores.posF + 2][sensores.posC + 3] = sensores.cota[11];
    mapaResultado[sensores.posF + 3][sensores.posC + 3] = sensores.superficie[12];
    mapaCotas[sensores.posF + 3][sensores.posC + 3] = sensores.cota[12];
    mapaResultado[sensores.posF + 3][sensores.posC + 2] = sensores.superficie[13];
    mapaCotas[sensores.posF + 3][sensores.posC + 2] = sensores.cota[13];
    mapaResultado[sensores.posF + 3][sensores.posC + 1] = sensores.superficie[14];
    mapaCotas[sensores.posF + 3][sensores.posC + 1] = sensores.cota[14];
    mapaResultado[sensores.posF + 3][sensores.posC] = sensores.superficie[15];
    mapaCotas[sensores.posF + 3][sensores.posC] = sensores.cota[15];
    break;
  case sur:
    for (int j = 1; j < 4; j++)
      for (int i = -j; i <= j; i++)
      {
        mapaResultado[sensores.posF + j][sensores.posC - i] = sensores.superficie[pos];
        mapaCotas[sensores.posF + j][sensores.posC - i] = sensores.cota[pos++];
      }
    break;
  case suroeste:
    mapaResultado[sensores.posF + 1][sensores.posC] = sensores.superficie[1];
    mapaCotas[sensores.posF + 1][sensores.posC] = sensores.cota[1];
    mapaResultado[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[2];
    mapaCotas[sensores.posF + 1][sensores.posC - 1] = sensores.cota[2];
    mapaResultado[sensores.posF][sensores.posC - 1] = sensores.superficie[3];
    mapaCotas[sensores.posF][sensores.posC - 1] = sensores.cota[3];
    mapaResultado[sensores.posF + 2][sensores.posC] = sensores.superficie[4];
    mapaCotas[sensores.posF + 2][sensores.posC] = sensores.cota[4];
    mapaResultado[sensores.posF + 2][sensores.posC - 1] = sensores.superficie[5];
    mapaCotas[sensores.posF + 2][sensores.posC - 1] = sensores.cota[5];
    mapaResultado[sensores.posF + 2][sensores.posC - 2] = sensores.superficie[6];
    mapaCotas[sensores.posF + 2][sensores.posC - 2] = sensores.cota[6];
    mapaResultado[sensores.posF + 1][sensores.posC - 2] = sensores.superficie[7];
    mapaCotas[sensores.posF + 1][sensores.posC - 2] = sensores.cota[7];
    mapaResultado[sensores.posF][sensores.posC - 2] = sensores.superficie[8];
    mapaCotas[sensores.posF][sensores.posC - 2] = sensores.cota[8];
    mapaResultado[sensores.posF + 3][sensores.posC] = sensores.superficie[9];
    mapaCotas[sensores.posF + 3][sensores.posC] = sensores.cota[9];
    mapaResultado[sensores.posF + 3][sensores.posC - 1] = sensores.superficie[10];
    mapaCotas[sensores.posF + 3][sensores.posC - 1] = sensores.cota[10];
    mapaResultado[sensores.posF + 3][sensores.posC - 2] = sensores.superficie[11];
    mapaCotas[sensores.posF + 3][sensores.posC - 2] = sensores.cota[11];
    mapaResultado[sensores.posF + 3][sensores.posC - 3] = sensores.superficie[12];
    mapaCotas[sensores.posF + 3][sensores.posC - 3] = sensores.cota[12];
    mapaResultado[sensores.posF + 2][sensores.posC - 3] = sensores.superficie[13];
    mapaCotas[sensores.posF + 2][sensores.posC - 3] = sensores.cota[13];
    mapaResultado[sensores.posF + 1][sensores.posC - 3] = sensores.superficie[14];
    mapaCotas[sensores.posF + 1][sensores.posC - 3] = sensores.cota[14];
    mapaResultado[sensores.posF][sensores.posC - 3] = sensores.superficie[15];
    mapaCotas[sensores.posF][sensores.posC - 3] = sensores.cota[15];
    break;
  case oeste:
    for (int j = 1; j < 4; j++)
      for (int i = -j; i <= j; i++)
      {
        mapaResultado[sensores.posF - i][sensores.posC - j] = sensores.superficie[pos];
        mapaCotas[sensores.posF - i][sensores.posC - j] = sensores.cota[pos++];
      }
    break;
  case noroeste:
    mapaResultado[sensores.posF][sensores.posC - 1] = sensores.superficie[1];
    mapaCotas[sensores.posF][sensores.posC - 1] = sensores.cota[1];
    mapaResultado[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[2];
    mapaCotas[sensores.posF - 1][sensores.posC - 1] = sensores.cota[2];
    mapaResultado[sensores.posF - 1][sensores.posC] = sensores.superficie[3];
    mapaCotas[sensores.posF - 1][sensores.posC] = sensores.cota[3];
    mapaResultado[sensores.posF][sensores.posC - 2] = sensores.superficie[4];
    mapaCotas[sensores.posF][sensores.posC - 2] = sensores.cota[4];
    mapaResultado[sensores.posF - 1][sensores.posC - 2] = sensores.superficie[5];
    mapaCotas[sensores.posF - 1][sensores.posC - 2] = sensores.cota[5];
    mapaResultado[sensores.posF - 2][sensores.posC - 2] = sensores.superficie[6];
    mapaCotas[sensores.posF - 2][sensores.posC - 2] = sensores.cota[6];
    mapaResultado[sensores.posF - 2][sensores.posC - 1] = sensores.superficie[7];
    mapaCotas[sensores.posF - 2][sensores.posC - 1] = sensores.cota[7];
    mapaResultado[sensores.posF - 2][sensores.posC] = sensores.superficie[8];
    mapaCotas[sensores.posF - 2][sensores.posC] = sensores.cota[8];
    mapaResultado[sensores.posF][sensores.posC - 3] = sensores.superficie[9];
    mapaCotas[sensores.posF][sensores.posC - 3] = sensores.cota[9];
    mapaResultado[sensores.posF - 1][sensores.posC - 3] = sensores.superficie[10];
    mapaCotas[sensores.posF - 1][sensores.posC - 3] = sensores.cota[10];
    mapaResultado[sensores.posF - 2][sensores.posC - 3] = sensores.superficie[11];
    mapaCotas[sensores.posF - 2][sensores.posC - 3] = sensores.cota[11];
    mapaResultado[sensores.posF - 3][sensores.posC - 3] = sensores.superficie[12];
    mapaCotas[sensores.posF - 3][sensores.posC - 3] = sensores.cota[12];
    mapaResultado[sensores.posF - 3][sensores.posC - 2] = sensores.superficie[13];
    mapaCotas[sensores.posF - 3][sensores.posC - 2] = sensores.cota[13];
    mapaResultado[sensores.posF - 3][sensores.posC - 1] = sensores.superficie[14];
    mapaCotas[sensores.posF - 3][sensores.posC - 1] = sensores.cota[14];
    mapaResultado[sensores.posF - 3][sensores.posC] = sensores.superficie[15];
    mapaCotas[sensores.posF - 3][sensores.posC] = sensores.cota[15];
    break;
  }
}

/**
 * @brief Determina si una casilla es transitable para el ingeniero.
 * @param f Fila de la casilla.
 * @param c Columna de la casilla.
 * @param tieneZapatillas Indica si el agente posee las zapatillas.
 * @return true si la casilla es transitable (no es muro ni precipicio).
 */
bool ComportamientoIngeniero::EsCasillaTransitableLevel0(int f, int c, bool tieneZapatillas)
{
  if (f < 0 || f >= mapaResultado.size() || c < 0 || c >= mapaResultado[0].size())
    return false;
  return es_camino(mapaResultado[f][c]); // Solo 'C', 'D', 'U' son transitables en Nivel 0
}

/**
 * @brief Comprueba si la casilla de delante es accesible por diferencia de altura.
 * Para el ingeniero: desnivel máximo 1 sin zapatillas, 2 con zapatillas.
 * @param actual Estado actual del agente (fila, columna, orientacion, zap).
 * @return true si el desnivel con la casilla de delante es admisible.
 */
bool ComportamientoIngeniero::EsAccesiblePorAltura(const ubicacion &actual, bool zap)
{
  ubicacion del = Delante(actual);
  if (del.f < 0 || del.f >= mapaCotas.size() || del.c < 0 || del.c >= mapaCotas[0].size())
    return false;
  int desnivel = abs(mapaCotas[del.f][del.c] - mapaCotas[actual.f][actual.c]);
  if (zap && desnivel > 2)
    return false;
  if (!zap && desnivel > 1)
    return false;
  return true;
}

/**
 * @brief Devuelve la posición (fila, columna) de la casilla que hay delante del agente.
 * Calcula la casilla frontal según la orientación actual (8 direcciones).
 * @param actual Estado actual del agente (fila, columna, orientacion).
 * @return Estado con la fila y columna de la casilla de enfrente.
 */
ubicacion ComportamientoIngeniero::Delante(const ubicacion &actual) const
{
  ubicacion delante = actual;
  switch (actual.brujula)
  {
  case 0:
    delante.f--;
    break; // norte
  case 1:
    delante.f--;
    delante.c++;
    break; // noreste
  case 2:
    delante.c++;
    break; // este
  case 3:
    delante.f++;
    delante.c++;
    break; // sureste
  case 4:
    delante.f++;
    break; // sur
  case 5:
    delante.f++;
    delante.c--;
    break; // suroeste
  case 6:
    delante.c--;
    break; // oeste
  case 7:
    delante.f--;
    delante.c--;
    break; // noroeste
  }
  return delante;
}

/**
 * @brief Imprime por consola la secuencia de acciones de un plan.
 *
 * @param plan  Lista de acciones del plan.
 */
void ComportamientoIngeniero::PintaPlan(const list<Action> &plan)
{
  auto it = plan.begin();
  while (it != plan.end())
  {
    if (*it == WALK)
    {
      cout << "W ";
    }
    else if (*it == JUMP)
    {
      cout << "J ";
    }
    else if (*it == TURN_SR)
    {
      cout << "r ";
    }
    else if (*it == TURN_SL)
    {
      cout << "l ";
    }
    else if (*it == COME)
    {
      cout << "C ";
    }
    else if (*it == IDLE)
    {
      cout << "I ";
    }
    else
    {
      cout << "-_ ";
    }
    it++;
  }
  cout << "( longitud " << plan.size() << ")" << endl;
}

/**
 * @brief Imprime las coordenadas y operaciones de un plan de tubería.
 *
 * @param plan  Lista de pasos (fila, columna, operación),
 *              donde operacion = -1 (DIG), operación = 1 (RAISE).
 */
void ComportamientoIngeniero::PintaPlan(const list<Paso> &plan)
{
  auto it = plan.begin();
  while (it != plan.end())
  {
    cout << it->fil << ", " << it->col << " (" << it->op << ")\n";
    it++;
  }
  cout << "( longitud " << plan.size() << ")" << endl;
}

/**
 * @brief Convierte un plan de acciones en una lista de casillas para
 *        su visualización en el mapa 2D.
 *
 * @param st    Estado de partida.
 * @param plan  Lista de acciones del plan.
 */
void ComportamientoIngeniero::VisualizaPlan(const ubicacion &st,
                                            const list<Action> &plan)
{
  listaPlanCasillas.clear();
  ubicacion cst = st;

  listaPlanCasillas.push_back({cst.f, cst.c, WALK});
  auto it = plan.begin();
  while (it != plan.end())
  {

    switch (*it)
    {
    case JUMP:
      switch (cst.brujula)
      {
      case 0:
        cst.f--;
        break;
      case 1:
        cst.f--;
        cst.c++;
        break;
      case 2:
        cst.c++;
        break;
      case 3:
        cst.f++;
        cst.c++;
        break;
      case 4:
        cst.f++;
        break;
      case 5:
        cst.f++;
        cst.c--;
        break;
      case 6:
        cst.c--;
        break;
      case 7:
        cst.f--;
        cst.c--;
        break;
      }
      if (cst.f >= 0 && cst.f < mapaResultado.size() &&
          cst.c >= 0 && cst.c < mapaResultado[0].size())
        listaPlanCasillas.push_back({cst.f, cst.c, JUMP});
    case WALK:
      switch (cst.brujula)
      {
      case 0:
        cst.f--;
        break;
      case 1:
        cst.f--;
        cst.c++;
        break;
      case 2:
        cst.c++;
        break;
      case 3:
        cst.f++;
        cst.c++;
        break;
      case 4:
        cst.f++;
        break;
      case 5:
        cst.f++;
        cst.c--;
        break;
      case 6:
        cst.c--;
        break;
      case 7:
        cst.f--;
        cst.c--;
        break;
      }
      if (cst.f >= 0 && cst.f < mapaResultado.size() &&
          cst.c >= 0 && cst.c < mapaResultado[0].size())
        listaPlanCasillas.push_back({cst.f, cst.c, WALK});
      break;
    case TURN_SR:
      cst.brujula = (Orientacion) (( (int) cst.brujula + 1) % 8);
      break;
    case TURN_SL:
      cst.brujula = (Orientacion) (( (int) cst.brujula + 7) % 8);
      break;
    }
    it++;
  }
}

/**
 * @brief Convierte un plan de tubería en la lista de casillas usada
 *        por el sistema de visualización.
 *
 * @param st    Estado de partida (no utilizado directamente).
 * @param plan  Lista de pasos del plan de tubería.
 */
void ComportamientoIngeniero::VisualizaRedTuberias(const list<Paso> &plan)
{
  listaCanalizacionTuberias.clear();
  auto it = plan.begin();
  while (it != plan.end())
  {
    listaCanalizacionTuberias.push_back({it->fil, it->col, it->op});
    it++;
  }
}
