#ifndef MEDIDOR_H_INCLUIDO
#define MEDIDOR_H_INCLUIDO

// ------------------------------------------------------
// ------------------------------------------------------

// Creación de una clase llamada "Medidor" para encapsular todas las funcionalidades relacionadas con la medición.
class Medidor {
  // .....................................................
  // .....................................................

  // Definición de pines utilizados para conexiones de sensores.
private:
  int pinVGas = 2;    // Pin para el sensor de ozono
  int pinVRef = 29;   // Pin para la referencia
  int pinVTemp = 28;  // Pin para el sensor de temperatura
  double ppmOzono;

  // Variables públicas para almacenar los valores medidos.
public:
  float vgas;    // Tensión del sensor de ozono
  float vref;    // Tensión de referencia
  float VeTemp;  // Tensión del sensor de temperatura

  float aGas;   // Valor analógico del sensor de ozono
  float aRef;   // Valor analógico de referencia
  float aTemp;  // Valor analógico del sensor de temperatura

  float Temp;     // Temperatura medida
  float Vmedida;  // Tensión medida
  float Cx;       // Concentración de ozono
  float Itemp;    // Variación de temperatura
  float fCx;      // Concentración ajustada de ozono

  // Constructor que inicializa los pines
  Medidor() {
    pinMode(pinVGas, INPUT);
    pinMode(pinVRef, INPUT);
    pinMode(pinVTemp, INPUT);
  }  // ()

  float digToVolt(int Vin) {
    return ((Vin * 3.3) / 1024);
  }

  // Función para realizar una calibración lineal
  double calibrarLectura(double valorMedido, double &m) {
    m = 0.3;                                      // Pendiente de la recta, ajustar según sea necesario
    const double b = -1.5;                        // Intersección de la recta, ajustar según sea necesario
    double valorCalibrado = m * valorMedido + b;  // Calcula el valor calibrado

    return valorCalibrado > 0 ? valorCalibrado : 0;
  }

  // .....................................................
  // Método para realizar inicializaciones necesarias.
  void iniciarMedidor() {
    vgas = 0;
    vref = 0;
    ppmOzono = 0;
    pinMode(pinVRef, INPUT);  // Uso correcto de pinVRef
    pinMode(pinVGas, INPUT);  // Uso correcto de pinVGas
  }  // ()

  // .....................................................
  // Método para medir la concentración de ozono.
  int medirCO2() {
    // Lee el valor de los pines del sensor
    int Agas = analogRead(pinVGas);  // Uso correcto de pinVGas
    int Aref = analogRead(pinVRef);  // Uso correcto de pinVRef

    // Convierte el valor digital a voltios
    vgas = digToVolt(Agas);
    vref = digToVolt(Aref);

    // Constante de conversión basada en la especificación del sensor
    const double M = -41.96 * 499 * (0.000001);

    // Calcula las ppm de ozono
    double res = ((1 / M) * (vgas - vref));
    ppmOzono = res > 0 ? res : 0;

    // Almacenar el valor de la pendiente
    double m;

    // Aplicamos la calibración lineal al valor de ppmOzono
    double ppmCalibrado = calibrarLectura(ppmOzono, m);

    // Calcular el valor de ppmOzono * 10
    int ppm10 = (int)(ppmOzono * 10);

    // Imprimir valores de referencia, gas leídos, m, ppmOzono * 10 y el valor calibrado
    Serial.print("VGAS: ");
    Serial.println(vgas);
    Serial.print("VREF: ");
    Serial.println(vref);
    Serial.print("M: ");
    Serial.println(M);  // Mostrar el valor de M
    Serial.print("PPM Ozono * 10: ");
    Serial.println(ppm10);  // Mostrar el valor de ppmOzono * 10
    Serial.print("PPM Ozono (calibrado): ");
    Serial.println(ppmCalibrado);  // Mostrar solo el valor calibrado

    return ppmCalibrado;  // Devuelve el valor calibrado
  }

  // .....................................................
  // Método para medir la temperatura.
  int medirTemperatura() {
    return Temp * 100;
  }  // ()

};  // class

// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
// ------------------------------------------------------
#endif
