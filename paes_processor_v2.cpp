#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <omp.h>
#include <cstring>

// Estructuras de datos
struct Estudiante {
    std::string codigo;
    std::string genero;
    std::string fecha_nacimiento;
    std::string nombres;
    std::string apellidos;
    std::string region;
    double promedio_notas;
};

struct RespuestaCorrecta {
    std::string prueba;
    std::vector<std::string> respuestas; // 100 respuestas
};

struct RespuestaEstudiante {
    std::string estudiante;
    std::string prueba;
    std::vector<std::string> respuestas; // 100 respuestas
};

struct Resultado {
    std::string codigo_estudiante;
    double pes;
    std::string prueba;
    int buenas;
    int omitidas;
    int malas;
    double puntaje;
};

// Función para parsear CSV de forma robusta
std::vector<std::string> parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;
    
    for (size_t i = 0; i < line.length(); i++) {
        char c = line[i];
        
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ';' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
        }
    }
    fields.push_back(field);
    
    return fields;
}

// Función para cargar estudiantes
std::map<std::string, Estudiante> cargarEstudiantes(const std::string& filename) {
    std::map<std::string, Estudiante> estudiantes;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return estudiantes;
    }
    
    // Saltar la primera línea (encabezados)
    std::getline(file, line);
    
    int line_count = 0;
    while (std::getline(file, line)) {
        line_count++;
        if (line_count % 10000 == 0) {
            std::cout << "Procesando línea " << line_count << " de estudiantes..." << std::endl;
        }
        
        std::vector<std::string> fields = parseCSVLine(line);
        
        if (fields.size() >= 7) {
            Estudiante est;
            est.codigo = fields[0];
            est.genero = fields[1];
            est.fecha_nacimiento = fields[2];
            est.nombres = fields[3];
            est.apellidos = fields[4];
            est.region = fields[5];
            
            // Convertir promedio de notas (formato con coma decimal)
            std::string promedio_str = fields[6];
            std::replace(promedio_str.begin(), promedio_str.end(), ',', '.');
            est.promedio_notas = std::stod(promedio_str);
            
            estudiantes[est.codigo] = est;
        }
    }
    
    file.close();
    return estudiantes;
}

// Función para cargar respuestas correctas
std::map<std::string, RespuestaCorrecta> cargarRespuestasCorrectas(const std::string& filename) {
    std::map<std::string, RespuestaCorrecta> respuestas_correctas;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return respuestas_correctas;
    }
    
    // Saltar la primera línea (encabezados)
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        std::vector<std::string> fields = parseCSVLine(line);
        
        if (fields.size() >= 101) { // 1 prueba + 100 respuestas
            RespuestaCorrecta rc;
            rc.prueba = fields[0];
            
            for (int i = 1; i <= 100; i++) {
                rc.respuestas.push_back(fields[i]);
            }
            
            respuestas_correctas[rc.prueba] = rc;
        }
    }
    
    file.close();
    return respuestas_correctas;
}

// Función para cargar respuestas de estudiantes
std::vector<RespuestaEstudiante> cargarRespuestasEstudiantes(const std::string& filename) {
    std::vector<RespuestaEstudiante> respuestas_estudiantes;
    std::ifstream file(filename);
    std::string line;
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return respuestas_estudiantes;
    }
    
    // Saltar la primera línea (encabezados)
    std::getline(file, line);
    
    int line_count = 0;
    while (std::getline(file, line)) {
        line_count++;
        if (line_count % 10000 == 0) {
            std::cout << "Procesando línea " << line_count << " de respuestas PAES..." << std::endl;
        }
        
        std::vector<std::string> fields = parseCSVLine(line);
        
        if (fields.size() >= 102) { // 1 estudiante + 1 prueba + 100 respuestas
            RespuestaEstudiante re;
            re.estudiante = fields[0];
            re.prueba = fields[1];
            
            for (int i = 2; i <= 101; i++) {
                re.respuestas.push_back(fields[i]);
            }
            
            respuestas_estudiantes.push_back(re);
        }
    }
    
    file.close();
    return respuestas_estudiantes;
}

// Función para calcular PES (Puntaje de Educación Secundaria)
// PES = promedio_notas (escala 1-7 convertida a 0-1000)
double calcularPES(double promedio_notas) {
    return (promedio_notas / 7.0) * 1000.0;
}

// Función para calcular puntaje de prueba
// Puntaje = 100 + (aciertos total) * 9
// donde aciertos = max(0, (correctas - malas * 0.25))
double calcularPuntaje(int buenas, int malas) {
    double aciertos = std::max(0.0, buenas - (malas * 0.25));
    return 100.0 + aciertos * 9.0;
}

// Función para procesar respuestas de un estudiante
Resultado procesarRespuesta(const RespuestaEstudiante& respuesta, 
                           const RespuestaCorrecta& correcta,
                           const Estudiante& estudiante) {
    Resultado resultado;
    resultado.codigo_estudiante = respuesta.estudiante;
    resultado.prueba = respuesta.prueba;
    resultado.pes = calcularPES(estudiante.promedio_notas);
    
    int buenas = 0, omitidas = 0, malas = 0;
    
    for (int i = 0; i < 100; i++) {
        std::string respuesta_estudiante = respuesta.respuestas[i];
        std::string respuesta_correcta = correcta.respuestas[i];
        
        if (respuesta_estudiante.empty()) {
            omitidas++;
        } else if (respuesta_estudiante == respuesta_correcta) {
            buenas++;
        } else {
            malas++;
        }
    }
    
    resultado.buenas = buenas;
    resultado.omitidas = omitidas;
    resultado.malas = malas;
    resultado.puntaje = calcularPuntaje(buenas, malas);
    
    return resultado;
}

// Función para escribir resultados
void escribirResultados(const std::vector<Resultado>& resultados, const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: No se pudo crear el archivo " << filename << std::endl;
        return;
    }
    
    // Escribir encabezados
    file << "\"CÓDIGO ESTUDIANTE\";\"PES\";\"PRUEBA\";\"BUENAS\";\"OMITIDAS\";\"MALAS\";\"PUNTAJE\"" << std::endl;
    
    // Escribir resultados
    for (const auto& resultado : resultados) {
        file << "\"" << resultado.codigo_estudiante << "\";";
        file << std::fixed << std::setprecision(7) << resultado.pes << ";";
        file << "\"" << resultado.prueba << "\";";
        file << resultado.buenas << ";";
        file << resultado.omitidas << ";";
        file << resultado.malas << ";";
        file << std::fixed << std::setprecision(2) << resultado.puntaje << std::endl;
    }
    
    file.close();
}

// Función para mostrar información del grupo
void mostrarInformacionGrupo() {
    std::cout << "=== INFORMACIÓN DEL GRUPO ===" << std::endl;
    std::cout << "Programa: Procesador PAES con OpenMP" << std::endl;
    std::cout << "Desarrollado por: Ignacio Molina, Matías Estay" << std::endl;
    std::cout << "Fecha: 2025" << std::endl;
    std::cout << "=============================" << std::endl;
}

int main(int argc, char* argv[]) {
    // Mostrar información del grupo
    mostrarInformacionGrupo();
    
    // Verificar argumentos de línea de comandos
    std::string archivo_estudiantes, archivo_paes, archivo_correctas, archivo_resultados;
    
    for (int i = 1; i < argc; i += 2) {
        if (i + 1 >= argc) {
            std::cerr << "Error: Argumento incompleto para " << argv[i] << std::endl;
            return 1;
        }
        
        if (strcmp(argv[i], "-e") == 0) {
            archivo_estudiantes = argv[i + 1];
        } else if (strcmp(argv[i], "-p") == 0) {
            archivo_paes = argv[i + 1];
        } else if (strcmp(argv[i], "-c") == 0) {
            archivo_correctas = argv[i + 1];
        } else if (strcmp(argv[i], "-r") == 0) {
            archivo_resultados = argv[i + 1];
        } else {
            std::cerr << "Error: Argumento desconocido " << argv[i] << std::endl;
            return 1;
        }
    }
    
    // Verificar que todos los archivos estén especificados
    if (archivo_estudiantes.empty() || archivo_paes.empty() || 
        archivo_correctas.empty() || archivo_resultados.empty()) {
        std::cerr << "Error: Faltan argumentos requeridos" << std::endl;
        std::cerr << "Uso: " << argv[0] << " -e estudiantes.csv -p paes.csv -c correctas.csv -r resultados.csv" << std::endl;
        return 1;
    }
    
    // Cargar datos
    std::cout << "Cargando datos..." << std::endl;
    
    auto estudiantes = cargarEstudiantes(archivo_estudiantes);
    if (estudiantes.empty()) {
        std::cerr << "Error: No se pudieron cargar los estudiantes" << std::endl;
        return 1;
    }
    
    auto respuestas_correctas = cargarRespuestasCorrectas(archivo_correctas);
    if (respuestas_correctas.empty()) {
        std::cerr << "Error: No se pudieron cargar las respuestas correctas" << std::endl;
        return 1;
    }
    
    auto respuestas_estudiantes = cargarRespuestasEstudiantes(archivo_paes);
    if (respuestas_estudiantes.empty()) {
        std::cerr << "Error: No se pudieron cargar las respuestas de estudiantes" << std::endl;
        return 1;
    }
    
    std::cout << "Datos cargados exitosamente:" << std::endl;
    std::cout << "- Estudiantes: " << estudiantes.size() << std::endl;
    std::cout << "- Respuestas correctas: " << respuestas_correctas.size() << std::endl;
    std::cout << "- Respuestas de estudiantes: " << respuestas_estudiantes.size() << std::endl;
    
    // Procesar respuestas en paralelo
    std::cout << "Procesando respuestas..." << std::endl;
    
    std::vector<Resultado> resultados;
    resultados.resize(respuestas_estudiantes.size());
    
    int procesados = 0;
    int errores = 0;
    
    #pragma omp parallel for reduction(+:procesados,errores)
    for (size_t i = 0; i < respuestas_estudiantes.size(); i++) {
        const auto& respuesta = respuestas_estudiantes[i];
        
        // Buscar estudiante
        auto it_estudiante = estudiantes.find(respuesta.estudiante);
        if (it_estudiante == estudiantes.end()) {
            errores++;
            continue;
        }
        
        // Buscar respuestas correctas para la prueba
        auto it_correcta = respuestas_correctas.find(respuesta.prueba);
        if (it_correcta == respuestas_correctas.end()) {
            errores++;
            continue;
        }
        
        // Procesar respuesta
        resultados[i] = procesarRespuesta(respuesta, it_correcta->second, it_estudiante->second);
        procesados++;
    }
    
    std::cout << "Procesamiento completado:" << std::endl;
    std::cout << "- Respuestas procesadas: " << procesados << std::endl;
    std::cout << "- Errores encontrados: " << errores << std::endl;
    
    // Escribir resultados
    std::cout << "Escribiendo resultados..." << std::endl;
    escribirResultados(resultados, archivo_resultados);
    
    std::cout << "Procesamiento completado exitosamente." << std::endl;
    std::cout << "Resultados guardados en: " << archivo_resultados << std::endl;
    
    return 0;
}
