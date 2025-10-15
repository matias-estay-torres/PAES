# Makefile para el procesador PAES con OpenMP

# Compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -std=c++11 -O3 -fopenmp -Wall -Wextra

# Nombre del ejecutable
TARGET = paes_processor

# Archivo fuente
SOURCE = paes_processor_v2.cpp

# Regla por defecto
all: $(TARGET)

# Regla para compilar el programa
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

# Regla para limpiar archivos generados
clean:
	rm -f $(TARGET) *.o

# Regla para ejecutar el programa con datos de ejemplo
test: $(TARGET)
	./$(TARGET) -e Data/estudiantes.csv -p Data/paes.csv -c Data/correctas.csv -r resultados.csv

# Regla para mostrar ayuda
help:
	@echo "Makefile para el procesador PAES con OpenMP"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make          - Compila el programa"
	@echo "  make clean    - Limpia archivos generados"
	@echo "  make test     - Compila y ejecuta con datos de ejemplo"
	@echo "  make help     - Muestra esta ayuda"
	@echo ""
	@echo "Uso del programa:"
	@echo "  ./$(TARGET) -e estudiantes.csv -p paes.csv -c correctas.csv -r resultados.csv"

# Marcar reglas como phony
.PHONY: all clean test help
