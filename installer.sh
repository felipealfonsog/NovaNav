#!/bin/bash

welcome() {
    echo "
    ╔═══════════════════════════════════════╗
    ║                                       ║
    ║   ~ NovaNav Browser ~                 ║
    ║   Developed with ❤️ by                 ║
    ║   Felipe Alfonso González L.          ║
    ║   Computer Science Engineer           ║
    ║   Chile                               ║
    ║                                       ║
    ║   Contact: f.alfonso@res-ear.ch       ║
    ║   Licensed under BSD 3-clause         ║
    ║   GitHub: github.com/felipealfonsog   ║
    ║                                       ║
    ╚═══════════════════════════════════════╝
    "
    echo "Welcome to the NovaNav Browser (C++ version) - Bash installer!"
    echo "---------------------------------------------------------------------"
}

check_execute_permission() {
    if [[ ! -x "$0" ]]; then
        echo "Error: The installer script does not have execute permission."
        echo "Do you want to grant it?"
        select yn in "Yes" "No"; do
            case $yn in
                Yes)
                    chmod +x "$0"
                    exec "$0" "$@"
                    ;;
                No)
                    echo "Exiting program."
                    exit 1
                    ;;
                *)
                    echo "Invalid option. Please choose a valid option."
                    ;;
            esac
        done
    fi
}

check_homebrew_installation_macOS() {
    if ! command -v brew &> /dev/null; then
        echo "Homebrew is not installed on macOS. Do you want to install it?"
        select yn in "Yes" "No"; do
            case $yn in
                Yes)
                    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
                    if ! command -v brew &> /dev/null; then
                        echo "Error: Homebrew installation failed."
                        exit 1
                    fi
                    break
                    ;;
                No)
                    echo "Exiting program."
                    exit 1
                    ;;
                *)
                    echo "Invalid option. Please choose a valid option."
                    ;;
            esac
        done
    fi
}

check_homebrew_installation_linux() {
    if ! command -v brew &> /dev/null; then
        echo "Homebrew/Linuxbrew is not installed on Linux. Do you want to install it?"
        select yn in "Yes" "No"; do
            case $yn in
                Yes)
                    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
                    if ! command -v brew &> /dev/null; then
                        echo "Error: Homebrew/Linuxbrew installation failed."
                        exit 1
                    fi
                    break
                    ;;
                No)
                    echo "Exiting program."
                    exit 1
                    ;;
                *)
                    echo "Invalid option. Please choose a valid option."
                    ;;
            esac
        done
    fi
}

install_dependencies_linux() {
    # Check if gcc, clang, qt5-base, and qt5-webengine are installed
    local packages=("gcc" "clang" "qt5-base" "qt5-webengine")
    local missing_packages=()
    for pkg in "${packages[@]}"; do
        if ! pacman -Qs "$pkg" &> /dev/null && ! dpkg -s "$pkg" &> /dev/null; then
            missing_packages+=("$pkg")
        fi
    done

    if [[ ${#missing_packages[@]} -gt 0 ]]; then
        echo "The following packages are missing: ${missing_packages[*]}"
        echo "Do you want to install them?"
        select yn in "Yes, using Homebrew" "Yes, using package manager" "No"; do
            case $yn in
                "Yes, using Homebrew")
                    brew install "${missing_packages[@]}"
                    break
                    ;;
                "Yes, using package manager")
                    if [[ -f /etc/arch-release ]]; then
                        sudo pacman -S "${missing_packages[@]}"
                    elif [[ -f /etc/debian_version ]]; then
                        sudo apt-get update && sudo apt-get install "${missing_packages[@]}"
                    else
                        echo "Error: Unsupported Linux distribution. Please install the required packages manually."
                        exit 1
                    fi
                    break
                    ;;
                "No")
                    echo "Exiting program."
                    exit 1
                    ;;
                *)
                    echo "Invalid option. Please choose a valid option."
                    ;;
            esac
        done
    fi
}

download_compile_install() {
    local pkgver="0.0.9"
    local pkgname="novanav-cpp"
    local url="https://github.com/felipealfonsog/NovaNav/archive/refs/tags/v.${pkgver}.tar.gz"
    local filename="${pkgname}-${pkgver}.tar.gz"
    local src_dir=""
    local bin_dir="/usr/local/bin"
    local icon_dir="/usr/share/pixmaps"
    local desktop_dir="/usr/share/applications"

    if [[ "$(uname)" == "Darwin" ]]; then
        src_dir="src/cpp_macos"
    else
        src_dir="src/cpp"
    fi

    # Download source
    wget -O "$filename" "$url"

    # Extract source
    tar xf "$filename"

    # Enter source directory
    cd "NovaNav-v.${pkgver}/${src_dir}"

    # Prepare build
    qmake PREFIX=/usr

    # Build
    make

    # Install binary
    sudo install -Dm755 novanav "$bin_dir/novanav"

    # Install icon
    sudo install -Dm644 "../nnav-iconlogo.png" "$icon_dir/novanav.png"

    # Install .desktop file
    sudo install -Dm644 "../novanav.desktop" "$desktop_dir/novanav.desktop"
}

configure_path() {
    if [[ "$(uname)" == "Darwin" ]]; then
        echo 'export PATH="/usr/local/bin:$PATH"' >> ~/.bash_profile
        source ~/.bash_profile
    else
        if [[ -f ~/.bashrc ]]; then
            echo 'export PATH="/usr/local/bin:$PATH"' >> ~/.bashrc
            source ~/.bashrc
        elif [[ -f ~/.bash_profile ]]; then
            echo 'export PATH="/usr/local/bin:$PATH"' >> ~/.bash_profile
            source ~/.bash_profile
        else
            echo 'export PATH="/usr/local/bin:$PATH"' >> ~/.profile
            source ~/.profile
        fi
    fi
}

reload_shell() {
    if [[ "$(uname)" == "Darwin" ]]; then
        source ~/.bash_profile
    elif [[ "$(uname)" == "Linux" ]]; then
        if [[ -f /etc/arch-release || -f /etc/debian_version ]]; then
            source ~/.bashrc
        else
            source ~/.bashrc
        fi
    fi
}

cleanup() {
    # Clean up
    cd ../..
    rm -rf "NovaNav-v.${pkgver}" "$filename"
}

main() {
    welcome
    check_execute_permission

    if [[ "$(uname)" == "Darwin" ]]; then
        check_homebrew_installation_macOS
    elif [[ "$(uname)" == "Linux" ]]; then
        check_homebrew_installation_linux
        install_dependencies_linux
    fi

    download_compile_install
    configure_path
    reload_shell
    cleanup

    echo "--------------------------------------------------------------------------------"
    echo "You can now run the program by typing 'novanav' in the terminal on macOS."
    echo "If you're using Arch Linux, Debian or any other distro, you can find NovaNav Browser in your program menu."
    echo "--------------------------------------------------------------------------------"
}

main
