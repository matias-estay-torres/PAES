@echo off
REM Ejecuta el procesador PAES con los archivos de ejemplo en la carpeta Data
if not exist paes_processor.exe (
    echo No se encontró paes_processor.exe, compilando...
    call build.bat
    if %ERRORLEVEL% neq 0 (
        echo Error durante la compilación. Abortando.
        exit /b %ERRORLEVEL%
    )
)

.
:: Ejecutar con rutas relativas
paes_processor.exe -e Data/estudiantes.csv -p Data/paes.csv -c Data/correctas.csv -r resultados.csv

echo Ejecución finalizada. Resultados en resultados.csv
