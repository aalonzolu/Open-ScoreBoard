# Open-ScoreBoard

[![Build Status](https://github.com/aalonzolu/Open-ScoreBoard/workflows/Build%20Open-ScoreBoard/badge.svg)](https://github.com/aalonzolu/Open-ScoreBoard/actions)
[![Qt6](https://img.shields.io/badge/Qt-6.5+-brightgreen.svg)](https://www.qt.io/)
[![License](https://img.shields.io/badge/license-GPL--3.0-blue.svg)](LICENSE)

Open ScoreBoard es una aplicación moderna para llevar el conteo de puntos en juegos de Basketball. Completamente reescrita y modernizada para Qt 6.5+ con soporte multiplataforma.

## ✨ Características

- **Interfaz moderna**: UI responsive adaptada para monitores modernos  
- **Soporte multi-monitor**: Scoreboard en pantalla secundaria automáticamente
- **Control por teclado**: Shortcuts para operar sin mouse durante el juego
- **Sonidos**: Efectos de audio para eventos del juego  
- **Cross-platform**: Funciona en Windows, Linux y macOS
- **Temas**: Soporte para modo claro y oscuro
- **Portable**: Ejecutables autocontenidos

## 🏀 Funcionalidad de Basketball

- Temporizador de juego con controles play/pause/stop/reset
- Marcador para ambos equipos (incrementos de 1, 2, 3 puntos)  
- Contador de faltas por equipo con bonificaciones automáticas
- Control de períodos del juego
- Logos personalizables para cada equipo
- Indicador de posesión del balón
- Títulos de torneo configurables

## 📦 Descargas

### Releases Automáticos

Los builds se generan automáticamente para cada commit:

- **Windows**: `open-scoreboard-windows.zip` (executable + DLLs)
- **Linux**: `open-scoreboard-linux.AppImage` (portable)  
- **macOS**: `open-scoreboard-macos.dmg` (app bundle)

Descarga desde [Releases](https://github.com/aalonzolu/Open-ScoreBoard/releases) o las [GitHub Actions](https://github.com/aalonzolu/Open-ScoreBoard/actions).

## 🔧 Build desde Código

### Requisitos

- **Qt 6.5+** (LTS recomendado)
- **CMake 3.16+** 
- **C++17** compatible compiler
- Módulos Qt: Core, Widgets, Multimedia

### Compilación rápida

```bash
# Clonar repositorio
git clone https://github.com/aalonzolu/Open-ScoreBoard.git
cd Open-ScoreBoard

# Build con CMake (recomendado)
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel

# O build con qmake (alternativo)  
qmake Scoreboard.pro
make release
```

### Builds específicos por plataforma

#### Windows (con Qt CreatorWindows/Visual Studio)
```cmd
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022" -DCMAKE_BUILD_TYPE=Release  
cmake --build . --config Release
```

#### Linux (Ubuntu/Debian)
```bash
# Instalar dependencias
sudo apt install qt6-base-dev qt6-multimedia-dev cmake build-essential

# Build y crear AppImage
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
cpack  # Opcional: crear paquetes
```

#### macOS (con Homebrew)
```bash
# Instalar dependencias  
brew install qt6 cmake

# Build
mkdir build && cd build  
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --parallel
```

## 🎮 Controles de Teclado

### Puntos
- **W/S**: Equipo 1 +1/-1 punto
- **I/K**: Equipo 2 +1/-1 punto  

### Faltas 
- **D/A**: Equipo 1 +1/-1 falta
- **L/J**: Equipo 2 +1/-1 falta

### Posesión
- **1**: Posesión Equipo 1
- **2**: Posesión Equipo 2  

### Timer
- **P**: Play/Pausa del reloj
- **0/9**: Período siguiente/anterior

## 🏗️ Arquitectura Técnica

- **Qt 6.5 LTS**: Framework principal con APIs modernas
- **CMake**: Sistema de build multiplataforma principal  
- **GitHub Actions**: CI/CD automática para 3 plataformas
- **QSoundEffect**: Audio moderno (reemplaza QSound deprecado)
- **QScreen API**: Soporte multi-monitor moderno (reemplaza QDesktopWidget)
- **Modern C++17**: Connect syntax, smart pointers, etc.

## 📁 Estructura del Proyecto

```
Open-ScoreBoard/
├── CMakeLists.txt              # Build system principal
├── Scoreboard.pro              # Build system alternativo (qmake)  
├── main.cpp                    # Punto de entrada
├── mainwindow.{cpp,h,ui}       # Ventana principal de control
├── boardw.{cpp,h,ui}          # Ventana del scoreboard
├── resources.qrc               # Recursos embebidos
├── images/                     # Iconos y gráficos
├── sounds/                     # Efectos de sonido  
├── assets/                     # Archivos de packaging
├── .github/workflows/          # CI/CD automática
└── tests/                      # Testing framework (futuro)
```

## 🧪 Testing

```bash
# Ejecutar tests (cuando estén implementados)
cd build
ctest --output-on-failure
```

## 🤝 Contribuir

1. Fork el repositorio
2. Crear branch para feature: `git checkout -b feature/nueva-funcionalidad`
3. Commit cambios: `git commit -am 'Agregar nueva funcionalidad'` 
4. Push al branch: `git push origin feature/nueva-funcionalidad`
5. Crear Pull Request

## 📋 TODO / Roadmap

- [x] Modernizar código base para Qt 6  
- [x] CMake build system
- [x] CI/CD multiplataforma  
- [ ] Testing framework completo
- [ ] Temas dark/light mode
- [ ] UI responsive mejorada
- [ ] Persisten configuration

## 📄 Licencia  

Este proyecto está bajo la licencia GPL-3.0. Ver [LICENSE](LICENSE) para más detalles.

## 🏆 Créditos

- Proyecto original por [aalonzolu](https://github.com/aalonzolu) 
- Modernización 2026: Qt 6.5 + arquitectura moderna
- Iconos de basketball y UI assets incluidos
