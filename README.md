# Procesador PAES con OpenMP

Este programa procesa los datos de la Prueba de Acceso a la Educación Superior (PAES) utilizando procesamiento paralelo con OpenMP para obtener el máximo rendimiento.

## Descripción

El programa calcula los puntajes de los estudiantes en las seis pruebas de la PAES:
- Matemáticas
- Lenguaje y Literatura  
- Ciencias Sociales
- Biología
- Física
- Química

Cada prueba incluye 100 preguntas, donde los errores se penalizan con -0.25 puntos.

### Fórmulas de Cálculo

**Puntaje de Prueba:**
```
Puntaje = 100 + (aciertos total) * 9
donde: aciertos = max(0, (correctas - malas * 0.25))
```

**Puntaje de Educación Secundaria (PES):**
```
PES = (promedio_notas / 7.0) * 1000
```

## Compilación

### Requisitos
- Compilador C++ con soporte para C++11
- OpenMP habilitado
- Sistema operativo Windows/Linux/macOS

### Compilar con g++
```bash
g++ -std=c++11 -O3 -fopenmp -Wall -Wextra -o paes_processor paes_processor_v2.cpp
```

### Compilar con Makefile
```bash
make
```

## Uso

El programa se ejecuta desde la línea de comandos con los siguientes parámetros:

```bash
./paes_processor -e estudiantes.csv -p paes.csv -c correctas.csv -r resultados.csv
```

### Parámetros

- `-e`: Archivo de entrada con los datos de los estudiantes
- `-p`: Archivo de entrada con las respuestas PAES de los estudiantes
- `-c`: Archivo de entrada con las respuestas correctas
- `-r`: Archivo de salida donde se almacenan los resultados

### Ejemplo de Ejecución

```bash
./paes_processor -e Data/estudiantes.csv -p Data/paes.csv -c Data/correctas.csv -r resultados.csv
```

## Formato de Archivos

### Archivo de Estudiantes (`-e`)
Archivo CSV con separador `;` y campos delimitados por comillas dobles:
- CÓDIGO: Código único del estudiante
- GÉNERO: Género declarado
- FECHA DE NACIMIENTO: Fecha en formato ISO 8601
- NOMBRES: Nombres del estudiante
- APELLIDOS: Apellidos del estudiante
- REGIÓN GEOGRÁFICA: Zona geográfica
- PROMEDIO DE NOTAS: Promedio con coma decimal

### Archivo de Respuestas PAES (`-p`)
Archivo CSV con separador `;` y campos delimitados por comillas dobles:
- ESTUDIANTE: Código único del estudiante
- PRUEBA: Nombre de la prueba
- PREGUNTA 001 a PREGUNTA 100: Respuestas (A, B, C, D, E o "" para omitir)

### Archivo de Respuestas Correctas (`-c`)
Archivo CSV con separador `;` y campos delimitados por comillas dobles:
- PRUEBA: Nombre de la prueba
- RESPUESTA 001 a RESPUESTA 100: Respuestas correctas

### Archivo de Resultados (`-r`)
Archivo CSV de salida con separador `;` y campos delimitados por comillas dobles:
- CÓDIGO ESTUDIANTE: Código único del estudiante
- PES: Puntaje de Educación Secundaria (7 decimales)
- PRUEBA: Nombre de la prueba
- BUENAS: Cantidad de preguntas correctas
- OMITIDAS: Cantidad de preguntas omitidas
- MALAS: Cantidad de preguntas erróneas
- PUNTAJE: Puntaje obtenido (2 decimales)

## Características Técnicas

- **Procesamiento Paralelo**: Utiliza OpenMP para procesar múltiples respuestas simultáneamente
- **Optimización**: Compilado con optimización O3 para máximo rendimiento
- **Manejo de Errores**: Reporta errores de procesamiento sin interrumpir la ejecución
- **Progreso**: Muestra el progreso de carga de datos cada 10,000 líneas
- **Estadísticas**: Reporta el número de respuestas procesadas y errores encontrados

## Rendimiento

El programa está optimizado para procesar grandes volúmenes de datos:
- Procesa más de 1.7 millones de respuestas de estudiantes
- Utiliza múltiples núcleos de CPU para acelerar el procesamiento
- Maneja eficientemente archivos CSV grandes

## Información del Grupo

```
=== INFORMACIÓN DEL GRUPO ===
Programa: Procesador PAES con OpenMP
Desarrollado por: Ignacio Molina y Matías Estay
Fecha: 2025
=============================
```

## Solución de Problemas

### Errores Comunes

1. **"No se pudo abrir el archivo"**: Verificar que las rutas de los archivos sean correctas
2. **"Faltan argumentos requeridos"**: Asegurarse de proporcionar todos los parámetros necesarios
3. **"Estudiante no encontrado"**: Normal cuando hay inconsistencias en los datos entre archivos

### Verificación de Resultados

El programa reporta estadísticas al final:
- Número de estudiantes cargados
- Número de respuestas correctas cargadas
- Número de respuestas de estudiantes procesadas
- Número de respuestas procesadas exitosamente
- Número de errores encontrados

## Licencia

Este programa fue desarrollado como parte de un proyecto académico para el procesamiento de datos de la PAES.


