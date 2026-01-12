@echo off
REM run-editor.bat - Abre el .uproject con el editor de Unreal (si la ruta del editor está en UE4_EDITOR env var intenta usarla).
SETLOCAL
set UPROJECT="%~dp0SurvivalShooter.uproject"
if not exist %UPROJECT% (
  echo No se encontro %UPROJECT%
  exit /b 1
)

if defined UE4_EDITOR (
  echo Usando UE4_EDITOR: %UE4_EDITOR%
  start "" "%UE4_EDITOR%" %UPROJECT%
  exit /b 0
)

REM Buscar UnrealEditor.exe en ubicaciones comunes (Engine instalado en Program Files)
setlocal enabledelayedexpansion
for %%p in ("C:\Program Files\Epic Games\UE_5.0\Engine\Binaries\Win64\UnrealEditor.exe" "C:\Program Files\Epic Games\UE_5.1\Engine\Binaries\Win64\UnrealEditor.exe" "C:\Program Files\Epic Games\UE_5.2\Engine\Binaries\Win64\UnrealEditor.exe" "C:\Program Files\Epic Games\UE_5.3\Engine\Binaries\Win64\UnrealEditor.exe") do (
  if exist %%~p (
    echo Abriendo editor: %%~p
    start "" "%%~p" %UPROJECT%
    endlocal
    exit /b 0
  )
)

echo No se encontro UnrealEditor.exe en rutas comunes. Abre el proyecto manualmente desde el Epic Launcher o define la variable de entorno UE4_EDITOR con la ruta completa al ejecutable.
endlocal
exit /b 1

