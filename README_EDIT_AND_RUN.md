README - Editar y ejecutar el proyecto Unreal

Objetivo
- Tener comandos/batch simples para compilar y abrir el proyecto desde la carpeta del workspace.

Archivos añadidos
- `build.bat` : Compila la solución `SurvivalShooter.sln` usando `msbuild` (requiere Build Tools o Visual Studio).
- `run-editor.bat` : Abre `SurvivalShooter.uproject` con Unreal Editor (intenta rutas comunes o una variable de entorno `UE4_EDITOR`).

Cómo usar
1) Abrir una consola (cmd.exe) en:
   `C:\Users\Ivansito\RiderProjects\SurvivalShooterIvan\SurvivalShooter`

2) Para abrir el editor:
```bat
run-editor.bat
```
Si tienes instalado el editor en una ruta diferente, define la variable de entorno `UE4_EDITOR` con la ruta completa a `UnrealEditor.exe`:
```bat
setx UE4_EDITOR "C:\\Program Files\\Epic Games\\UE_5.2\\Engine\\Binaries\\Win64\\UnrealEditor.exe"
```

3) Para compilar (C++):
```bat
build.bat
```
Asegúrate de tener `msbuild` en PATH (instala "Build Tools for Visual Studio" o abre la solución en Visual Studio y compila desde ahí).

Notas y recomendaciones
- El repositorio no incluye `Binaries/`, `Intermediate/`, `Saved/`, ni `DerivedDataCache/`.
- Es recomendable usar Git LFS para archivos `.uasset` y `.umap` (ya añadí `.gitattributes`).
- Si editas código C++ desde Rider/Visual Studio, abre `SurvivalShooter.sln`.

Si quieres que inicialice git + git-lfs y haga un commit inicial desde aquí, dime y lo ejecuto.

