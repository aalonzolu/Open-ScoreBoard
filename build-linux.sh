#!/bin/bash

# =============================================================================
# Open-ScoreBoard Linux Build Script
# =============================================================================
# Este script compila el proyecto Open-ScoreBoard en sistemas Linux
# Autor: Generated for aalonzolu/Open-ScoreBoard
# =============================================================================

set -e  # Salir inmediatamente si un comando falla

# Colores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Función para imprimir mensajes con colores
print_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

print_header() {
    echo -e "${BLUE}============================================${NC}"
    echo -e "${BLUE} Open-ScoreBoard - Linux Build Script${NC}"
    echo -e "${BLUE}============================================${NC}"
}

# Función para instalar herramientas AppImage
install_appimage_tools() {
    print_info "Instalando herramientas para AppImage..."
    
    # Verificar si linuxdeploy está instalado
    if ! command -v linuxdeploy &> /dev/null; then
        print_info "Descargando linuxdeploy..."
        wget -q https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage -O linuxdeploy
        chmod +x linuxdeploy
        sudo mv linuxdeploy /usr/local/bin/
    fi
    
    # Verificar si linuxdeploy-plugin-qt está instalado
    if ! command -v linuxdeploy-plugin-qt &> /dev/null; then
        print_info "Descargando linuxdeploy-plugin-qt..."
        wget -q https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage -O linuxdeploy-plugin-qt
        chmod +x linuxdeploy-plugin-qt
        sudo mv linuxdeploy-plugin-qt /usr/local/bin/
    fi
    
    # Instalar dependencias adicionales para AppImage
    sudo apt-get install -y file wget fuse libfuse2 squashfs-tools imagemagick || true
}

# Función para verificar dependencias
check_dependencies() {
    print_info "Verificando dependencias..."
    
    # Verificar CMake
    if ! command -v cmake &> /dev/null; then
        print_error "CMake no está instalado"
        print_info "Instalando CMake..."
        sudo apt-get update -o Acquire::AllowInsecureRepositories=true || true
        sudo apt-get install -y cmake
    else
        print_success "CMake encontrado: $(cmake --version | head -n1)"
    fi
    
    # Verificar compilador C++
    if ! command -v g++ &> /dev/null; then
        print_error "g++ no está instalado"
        print_info "Instalando build-essential..."
        sudo apt-get update -o Acquire::AllowInsecureRepositories=true || true
        sudo apt-get install -y build-essential
    else
        print_success "g++ encontrado: $(g++ --version | head -n1)"
    fi
    
    # Verificar Qt6 (agregamos más formas de encontrarlo)
    qt6_found=false
    qt_version=""
    
    if pkg-config --exists Qt6Core; then
        qt6_found=true
        qt_version=$(pkg-config --modversion Qt6Core)
        print_success "Qt6 encontrado via pkg-config: $qt_version"
    elif [ -d "/usr/lib/x86_64-linux-gnu/cmake/Qt6" ]; then
        qt6_found=true
        print_success "Qt6 encontrado en sistema"
    elif command -v qmake6 &> /dev/null; then
        qt6_found=true
        qt_version=$(qmake6 -version | grep "Qt version" | cut -d' ' -f3)
        print_success "Qt6 encontrado via qmake6: $qt_version"
    elif command -v qmake &> /dev/null; then
        qt_version=$(qmake -version | grep "Qt version" | cut -d' ' -f3)
        if [[ $qt_version == 6.* ]]; then
            qt6_found=true
            print_success "Qt6 encontrado via qmake: $qt_version"
        elif [[ $qt_version == 5.* ]]; then
            print_warning "Solo Qt5 disponible: $qt_version"
            print_info "El proyecto necesita Qt6, pero Qt5 podría funcionar con modificaciones"
        fi
    fi
    
    if [ "$qt6_found" = false ]; then
        print_warning "Qt6 no encontrado, intentando instalación..."
        
        # Intentar actualizar repositorios ignorando errores de firmas
        print_info "Actualizando repositorios..."
        sudo apt-get update -o Acquire::AllowInsecureRepositories=true || {
            print_warning "Actualización de repositorios tuvo algunos problemas, continuando..."
        }
        
        # Lista de paquetes Qt6 para probar
        qt6_packages=(
            "qt6-base-dev qt6-multimedia-dev"
            "libqt6core6-dev libqt6widgets6-dev libqt6multimedia6-dev"
            "qt6-tools-dev qt6-base-dev-tools"
            "qt6-qmake qt6-base-dev"
        )
        
        print_info "Intentando instalación de Qt6..."
        qt6_installed=false
        
        for packages in "${qt6_packages[@]}"; do
            print_info "Probando: $packages"
            if sudo apt-get install -y $packages 2>/dev/null; then
                print_success "Qt6 instalado: $packages"
                qt6_installed=true
                break
            fi
        done
        
        if [ "$qt6_installed" = false ]; then
            print_warning "Qt6 no disponible, intentando Qt5 como alternativa..."
            
            # Intentar instalar Qt5
            qt5_packages=(
                "qt5-default qtbase5-dev qtmultimedia5-dev"
                "qtbase5-dev libqt5multimedia5-dev"
                "qt5-qmake qtbase5-dev-tools"
            )
            
            for packages in "${qt5_packages[@]}"; do
                print_info "Probando Qt5: $packages"
                if sudo apt-get install -y $packages 2>/dev/null; then
                    print_success "Qt5 instalado: $packages"
                    print_warning "NOTA: Proyecto diseñado para Qt6, puede necesitar ajustes para Qt5"
                    break
                fi
            done
        fi
    fi
}

# Función para limpiar builds anteriores
clean_build() {
    if [ "$1" = "--clean" ] || [ "$1" = "-c" ]; then
        print_info "Limpiando build anterior..."
        rm -rf build/
        print_success "Build limpiado"
    fi
}

# Función principal de build
build_project() {
    print_info "Iniciando build del proyecto..."
    
    # Crear directorio build
    mkdir -p build
    cd build
    
    print_info "Configurando proyecto con CMake..."
    
    # Intentar diferentes configuraciones de CMake
    cmake_success=false
    
    # Detectar versión de Qt disponible
    qt_version_major=""
    if pkg-config --exists Qt6Core; then
        qt_version_major="6"
        print_info "Usando Qt6 para el build"
    elif pkg-config --exists Qt5Core; then
        qt_version_major="5"
        print_warning "Usando Qt5 - puede requerir modificaciones al código"
    fi
    
    # Intentar configuración estándar
    if cmake .. -DCMAKE_BUILD_TYPE=Release; then
        cmake_success=true
    elif cmake .. -DCMAKE_BUILD_TYPE=Release -DQt6_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6; then
        cmake_success=true
    elif cmake .. -DCMAKE_BUILD_TYPE=Release -DQt5_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt5; then
        cmake_success=true
    elif cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/usr/lib/x86_64-linux-gnu/cmake; then
        cmake_success=true
    fi
    
    if [ "$cmake_success" = false ]; then
        print_error "CMake configuración falló"
        print_info "Intentando build alternativo con qmake..."
        cd ..
        
        # Probar diferentes versiones de qmake
        qmake_cmd=""
        if command -v qmake6 &> /dev/null; then
            qmake_cmd="qmake6"
        elif command -v qmake &> /dev/null; then
            qmake_cmd="qmake"
        fi
        
        if [ -n "$qmake_cmd" ]; then
            print_info "Usando $qmake_cmd para build..."
            if $qmake_cmd Scoreboard.pro; then
                print_success "qmake configuración exitosa"
                if make -j$(nproc); then
                    print_success "Build con qmake exitoso"
                else
                    print_error "Build con qmake falló"
                    exit 1
                fi
            else
                print_error "qmake configuración falló"
                exit 1
            fi
        else
            print_error "No se encontró qmake"
            exit 1
        fi
    else
        print_success "CMake configuración exitosa"
        
        print_info "Compilando proyecto..."
        if cmake --build . --config Release -j$(nproc); then
            print_success "Compilación exitosa"
        else
            print_error "Compilación falló"
            exit 1
        fi
    fi
}

# Función para crear AppImage
create_appimage() {
    print_info "Creando AppImage..."
    
    # Buscar el ejecutable
    executable=""
    possible_locations=(
        "build/release/Open-ScoreBoard"
        "build/OpenScoreBoard"
        "build/Scoreboard"
        "OpenScoreBoard"
        "Scoreboard"
    )
    
    for location in "${possible_locations[@]}"; do
        if [ -f "$location" ] && [ -x "$location" ]; then
            executable="$location"
            break
        fi
    done
    
    if [ -z "$executable" ]; then
        print_error "No se encontró el ejecutable para crear AppImage"
        return 1
    fi
    
    print_success "Ejecutable encontrado: $executable"
    
    # Crear directorio temporal para AppDir
    APPDIR="Open-ScoreBoard.AppDir"
    rm -rf "$APPDIR"
    mkdir -p "$APPDIR/usr/bin"
    mkdir -p "$APPDIR/usr/share/applications"
    mkdir -p "$APPDIR/usr/share/icons/hicolor/256x256/apps"
    
    # Copiar ejecutable
    cp "$executable" "$APPDIR/usr/bin/Open-ScoreBoard"
    
    # Crear archivo .desktop
    cat > "$APPDIR/Open-ScoreBoard.desktop" << EOF
[Desktop Entry]
Type=Application
Name=Open ScoreBoard
Comment=Basketball Scoreboard Application
Exec=Open-ScoreBoard
Icon=Open-ScoreBoard
Categories=Game;Sports;
Terminal=false
EOF
    
    # Copiar archivo .desktop a la ubicación estándar
    cp "$APPDIR/Open-ScoreBoard.desktop" "$APPDIR/usr/share/applications/"
    
    # Crear un ícono con resolución válida para AppImage
    print_info "Preparando icono para AppImage..."
    
    # Instalar ImageMagick si no está disponible
    if ! command -v convert &> /dev/null; then
        print_info "Instalando ImageMagick para manejo de iconos..."
        sudo apt-get install -y imagemagick > /dev/null 2>&1 || {
            print_warning "No se pudo instalar ImageMagick"
        }
    fi
    
    if [ -f "src/images/balon.png" ]; then
        print_info "Redimensionando icono existente a 256x256..."
        if command -v convert &> /dev/null; then
            # Redimensionar el icono existente a 256x256 (resolución válida)
            convert "src/images/balon.png" -resize 256x256! "$APPDIR/Open-ScoreBoard.png" 2>/dev/null || {
                print_warning "No se pudo redimensionar, usando icono original..."
                cp "src/images/balon.png" "$APPDIR/Open-ScoreBoard.png"
            }
        else
            print_warning "ImageMagick no disponible, usando icono original..."
            cp "src/images/balon.png" "$APPDIR/Open-ScoreBoard.png"
        fi
        cp "$APPDIR/Open-ScoreBoard.png" "$APPDIR/usr/share/icons/hicolor/256x256/apps/Open-ScoreBoard.png"
    else
        print_info "Creando icono básico 256x256..."
        # Crear un icono básico con ImageMagick en resolución válida
        if command -v convert &> /dev/null; then
            convert -size 256x256 xc:orange -pointsize 60 -fill white -gravity center -annotate +0+0 "OSB" "$APPDIR/Open-ScoreBoard.png" 2>/dev/null || {
                print_error "Error creando icono con ImageMagick"
                # Fallback: crear archivo PNG mínimo válido
                echo "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNkYPhfDwAChwGA60e6kgAAAABJRU5ErkJggg==" | base64 -d > "$APPDIR/Open-ScoreBoard.png"
            }
        else
            # Usar un ícono genérico del sistema si está disponible
            if [ -f "/usr/share/pixmaps/applications-games.png" ]; then
                print_info "Usando icono genérico del sistema..."
                if command -v convert &> /dev/null; then
                    convert "/usr/share/pixmaps/applications-games.png" -resize 256x256! "$APPDIR/Open-ScoreBoard.png" 2>/dev/null || {
                        cp "/usr/share/pixmaps/applications-games.png" "$APPDIR/Open-ScoreBoard.png"
                    }
                else
                    cp "/usr/share/pixmaps/applications-games.png" "$APPDIR/Open-ScoreBoard.png"
                fi
            else
                print_warning "Creando icono mínimo..."
                # Crear archivo PNG mínimo válido como último recurso
                echo "iVBORw0KGgoAAAANSUhEUgAAAAEAAAABCAYAAAAfFcSJAAAADUlEQVR42mNkYPhfDwAChwGA60e6kgAAAABJRU5ErkJggg==" | base64 -d > "$APPDIR/Open-ScoreBoard.png"
            fi
        fi
        cp "$APPDIR/Open-ScoreBoard.png" "$APPDIR/usr/share/icons/hicolor/256x256/apps/" 2>/dev/null || true
    fi
    
    # Verificar que el icono tiene resolución válida para AppImage  
    if command -v identify &> /dev/null && [ -f "$APPDIR/Open-ScoreBoard.png" ]; then
        icon_size=$(identify -format "%wx%h" "$APPDIR/Open-ScoreBoard.png" 2>/dev/null || echo "256x256")
        print_info "Icono preparado con resolución: $icon_size"
        
        # Lista de resoluciones válidas para AppImage
        valid_sizes="8x8|16x16|20x20|22x22|24x24|28x28|32x32|36x36|42x42|48x48|64x64|72x72|96x96|128x128|160x160|192x192|256x256|384x384|480x480|512x512"
        
        if [[ ! "$icon_size" =~ ^($valid_sizes)$ ]]; then
            print_warning "Resolución inválida ($icon_size), corrigiendo a 256x256..."
            if command -v convert &> /dev/null; then
                convert "$APPDIR/Open-ScoreBoard.png" -resize 256x256! "$APPDIR/Open-ScoreBoard.png" 2>/dev/null || {
                    print_error "No se pudo corregir resolución del icono"
                }
            fi
        else
            print_success "Icono con resolución válida: $icon_size"
        fi
    else
        print_info "Usando icono sin verificación de resolución"
    fi
    
    # Usar linuxdeploy para empaquetar dependencias
    print_info "Empaquetando dependencias con linuxdeploy..."
    
    export OUTPUT="Open-ScoreBoard-x86_64.AppImage"
    
    if command -v linuxdeploy &> /dev/null && command -v linuxdeploy-plugin-qt &> /dev/null; then
        # Intentar linuxdeploy con plugin Qt y icono
        print_info "Intentando linuxdeploy con plugin Qt e icono..."
        linuxdeploy --appdir "$APPDIR" \
                   --executable "$APPDIR/usr/bin/Open-ScoreBoard" \
                   --desktop-file "$APPDIR/Open-ScoreBoard.desktop" \
                   --icon-file "$APPDIR/Open-ScoreBoard.png" \
                   --plugin qt \
                   --output appimage 2>/dev/null || {
            
            print_warning "linuxdeploy con icono falló, intentando sin icono..."
            # Intentar sin icono si el icono causa problemas
            linuxdeploy --appdir "$APPDIR" \
                       --executable "$APPDIR/usr/bin/Open-ScoreBoard" \
                       --desktop-file "$APPDIR/Open-ScoreBoard.desktop" \
                       --plugin qt \
                       --output appimage 2>/dev/null || {
                
                print_warning "linuxdeploy con plugin Qt falló, intentando método básico..."
                # Método alternativo sin plugin Qt
                linuxdeploy --appdir "$APPDIR" \
                           --executable "$APPDIR/usr/bin/Open-ScoreBoard" \
                           --desktop-file "$APPDIR/Open-ScoreBoard.desktop" \
                           --output appimage 2>/dev/null || {
                    
                    print_warning "linuxdeploy básico falló, creando AppImage manualmente..."
                    
                    # Último recurso: método manual
                    # Instalar squashfs-tools si no está disponible
                    if ! command -v mksquashfs &> /dev/null; then
                        print_info "Instalando squashfs-tools..."
                        sudo apt-get install -y squashfs-tools > /dev/null 2>&1 || {
                            print_error "No se pudo instalar squashfs-tools"
                            return 1
                        }
                    fi
                    
                    # Crear AppRun manualmente
                    print_info "Creando AppRun manual..."
                    cat > "$APPDIR/AppRun" << 'EOF'
#!/bin/bash
SELF=$(readlink -f "$0")
HERE=${SELF%/*}
export PATH="${HERE}/usr/bin/:${PATH}"
export LD_LIBRARY_PATH="${HERE}/usr/lib/:${LD_LIBRARY_PATH}"
cd "${HERE}"
exec "${HERE}/usr/bin/Open-ScoreBoard" "$@"
EOF
                    chmod +x "$APPDIR/AppRun"
                    
                    # Crear AppImage con mksquashfs
                    print_info "Empaquetando AppImage manualmente..."
                    mksquashfs "$APPDIR" "$OUTPUT" -root-owned -noappend > /dev/null 2>&1 || {
                        print_error "No se pudo crear AppImage manualmente"
                        return 1
                    }
                }
            }
        }
    else
        print_error "linuxdeploy no está disponible"
        return 1
    fi
    
    # Verificar si se creó el AppImage
    if [ -f "$OUTPUT" ]; then
        print_success "AppImage creado exitosamente: $OUTPUT"
        
        # Mostrar información del AppImage
        size=$(stat -c%s "$OUTPUT" 2>/dev/null || echo "N/A")
        if [ "$size" != "N/A" ]; then
            size_mb=$((size / 1024 / 1024))
            print_info "Tamaño del AppImage: ${size_mb} MB"
        fi
        
        # Hacer ejecutable
        chmod +x "$OUTPUT"
        print_success "AppImage listo para distribución!"
        
        return 0
    else
        print_error "No se pudo crear el AppImage"
        return 1
    fi
}

# Función para verificar el resultado
verify_build() {
    print_info "Verificando resultado del build..."
    
    # Buscar el ejecutable en varias ubicaciones posibles
    executable=""
    possible_locations=(
        "build/OpenScoreBoard"
        "build/Scoreboard" 
        "build/release/Open-ScoreBoard"
        "build/release/OpenScoreBoard"
        "build/release/Scoreboard"
        "OpenScoreBoard"
        "Scoreboard"
        "Open-ScoreBoard"
    )
    
    for location in "${possible_locations[@]}"; do
        if [ -f "$location" ] && [ -x "$location" ]; then
            executable="$location"
            break
        fi
    done
    
    if [ -n "$executable" ]; then
        print_success "Ejecutable creado: $executable"
        file_info=$(file "$executable")
        print_info "Información del archivo: $file_info"
        
        # Verificar tamaño del archivo
        size=$(stat -f%z "$executable" 2>/dev/null || stat -c%s "$executable" 2>/dev/null || echo "N/A")
        if [ "$size" != "N/A" ]; then
            size_mb=$((size / 1024 / 1024))
            size_kb=$((size / 1024))
            if [ $size_mb -gt 0 ]; then
                print_info "Tamaño: ${size_mb} MB"
            else
                print_info "Tamaño: ${size_kb} KB"
            fi
        fi
        
        # Verificar dependencias
        if command -v ldd &> /dev/null; then
            print_info "Dependencias dinámicas principales:"
            ldd "$executable" | grep -E "(Qt|libstdc|libgcc)" || true
        fi
    else
        print_warning "No se encontró el ejecutable principal"
        print_info "Archivos en directorio build:"
        ls -la build/ 2>/dev/null || ls -la .
    fi
}

# Función de ayuda
show_help() {
    echo "Uso: $0 [opciones]"
    echo ""
    echo "Opciones:"
    echo "  -c, --clean     Limpiar build anterior antes de compilar"
    echo "  -h, --help      Mostrar esta ayuda"
    echo "  --install-deps  Solo instalar dependencias"
    echo "  --verify        Solo verificar dependencias sin compilar"
    echo "  --appimage      Crear AppImage después del build"
    echo "  --setup-appimage Solo instalar herramientas AppImage"
    echo ""
    echo "Ejemplos:"
    echo "  $0              Build normal (genera AppImage automáticamente)"
    echo "  $0 --clean      Build limpio con AppImage"
    echo "  $0 --verify     Solo verificar dependencias"
    echo "  $0 --appimage   Build normal y crear AppImage"
}

# =============================================================================
# SCRIPT PRINCIPAL
# =============================================================================

# Verificar que estamos en el directorio correcto
if [ ! -f "CMakeLists.txt" ] && [ ! -f "Scoreboard.pro" ]; then
    print_error "No se encontró CMakeLists.txt o Scoreboard.pro"
    print_error "Por favor ejecuta este script desde el directorio raíz del proyecto"
    exit 1
fi

print_header

# Variable para controlar la creación de AppImage
CREATE_APPIMAGE=true

# Procesar argumentos
case "$1" in
    -h|--help)
        show_help
        exit 0
        ;;
    --verify)
        check_dependencies
        print_success "Verificación completada"
        exit 0
        ;;
    --install-deps)
        check_dependencies
        print_success "Dependencias instaladas"
        exit 0
        ;;
    --setup-appimage)
        install_appimage_tools
        print_success "Herramientas AppImage instaladas"
        exit 0
        ;;
    --appimage)
        CREATE_APPIMAGE=true
        clean_build
        check_dependencies
        install_appimage_tools
        build_project
        verify_build
        create_appimage
        ;;
    -c|--clean)
        clean_build --clean
        check_dependencies
        install_appimage_tools
        build_project
        verify_build
        if [ "$CREATE_APPIMAGE" = true ]; then
            create_appimage
        fi
        ;;
    "")
        clean_build
        check_dependencies
        install_appimage_tools  
        build_project
        verify_build
        if [ "$CREATE_APPIMAGE" = true ]; then
            create_appimage
        fi
        ;;
    *)
        print_error "Opción desconocida: $1"
        show_help
        exit 1
        ;;
esac

print_success "Build completado exitosamente! 🎉"
print_info "Para ejecutar el programa, usa:"

# Verificar si hay AppImage disponible
if [ -f "Open-ScoreBoard-x86_64.AppImage" ]; then
    echo "  ./Open-ScoreBoard-x86_64.AppImage  (AppImage - recomendado para distribución)"
fi

# Mostrar ejecutable local
if [ -f "build/release/Open-ScoreBoard" ]; then
    echo "  ./build/release/Open-ScoreBoard  (ejecutable local)"
elif [ -f "build/OpenScoreBoard" ]; then
    echo "  ./build/OpenScoreBoard  (ejecutable local)"
elif [ -f "build/Scoreboard" ]; then
    echo "  ./build/Scoreboard  (ejecutable local)"
else
    echo "  (Busca el ejecutable en el directorio build/)"
fi

if [ -f "Open-ScoreBoard-x86_64.AppImage" ]; then
    print_info "AppImage creado para fácil distribución en cualquier distribución Linux!"
fi