@echo off
REM build.bat - Compila la solución Unreal (Windows) usando MSBuild si está disponible.

SETLOCAL
if not exist "%~dp0SurvivalShooter.sln" (
  echo No se encontró SurvivalShooter.sln en %~dp0
  exit /b 1
)

where msbuild >nul 2>&1
if errorlevel 1 (
  echo msbuild no encontrado en PATH.
  echo Abre la solución en Visual Studio o instala "Build Tools for Visual Studio".
  exit /b 1
)

echo Compilando SurvivalShooter.sln...
msbuild "%~dp0SurvivalShooter.sln" /m /p:Configuration=Development /p:Platform=Win64
if errorlevel 1 (
  echo La compilacion falló.
  exit /b 1
)
echo Compilacion completada.
ENDLOCAL
exit /b 0

