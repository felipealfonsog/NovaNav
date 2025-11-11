<!--
#### NovaNav Browser written in other languages (Experimental part of the project)*

[![NovaNav under development](https://img.shields.io/badge/NovaNav-In%20Other%20Languages-blue)](#)



-->
<!-- 
<details>

<summary><strong>NovaNav written in C++ 🚀 - (Manual instructions)*</strong></summary>

<br>


[![Install NovaNav](https://img.shields.io/badge/Install-NovaNav%20Browser-purple)](https://github.com/felipealfonsog/NovaNav#installation)
#### Via AUR using YAY (Version written in C++)

[![NovaNav in Python](https://img.shields.io/badge/NovaNav-Python%20Version-yellow)](#)

[![AUR](https://img.shields.io/aur/version/novanav-cpp)](https://aur.archlinux.org/packages/novanav-cpp)

<!-- 
[![AUR](https://img.shields.io/aur/version/novanav-cpp.svg)](https://aur.archlinux.org/packages/novanav-cpp)
-->

https://aur.archlinux.org/packages/novanav-cpp

NovaNav Browser is available on AUR (Arch User Repository), and it can be installed using the `yay` package manager. Follow the steps below to install NovaNav:

1. Make sure you have `yay` installed. If not, you can install it with the following command:
   
   ```
   sudo pacman -S yay
   ```

   Once yay is installed, you can install by running the following command:
   
   ```
   yay -S novanav-cpp
   ```

   Or just:
  
   ```
   yay -S novanav
   ```

This command will automatically fetch the package from AUR and handle the installation process for you.
You can find NovaNav Browser in your program menu!
-->
<!-- 
#

##### Instructions for Compiling from Source

Prerequisites

Make sure you have the following dependencies installed on your system:

- Qt 5 Base
- Qt 5 WebEngine

Compilation Steps

1. Clone the NovaNav repository from GitHub:

```
git clone https://github.com/felipealfonsog/NovaNav.git
```

Navigate to the directory containing the source code:

```
cd NovaNav/src/cpp
```

Configure the build using qmake. You can adjust parameters as needed:

```
qmake PREFIX=/usr/local
```

Compile the source code:

```
make
```
-->
<!--
Once compilation is complete, the novanav binary will be generated in the current directory.

Installation
To install NovaNav system-wide after compilation, you can use the provided PKGBUILD or manually copy the binary to /usr/local/bin, the icon to /usr/share/pixmaps, and the .desktop file to /usr/share/applications, located in the 'src' folder.

<sub>* It might contain lots of errors, bugs, etc. Meanwhile, Keep it simple now with C++ stable version, running smomothly!</sub>

</details>


<details>

<summary><strong>NovaNav written in Python 🚀 - beta/testing (experimental alternative in progress)*</strong></summary>

<br>
-->
<!--
##### Python version: 

  - python
  - python-pip
  - python-pyqt5
  - python-pyqt5-webengine

-->
<!--
[![Install NovaNav](https://img.shields.io/badge/Install-NovaNav%20Browser-purple)](https://github.com/felipealfonsog/NovaNav#installation)
#### Via AUR using YAY (Version written in Python)

[![NovaNav in Python](https://img.shields.io/badge/NovaNav-Python%20Version-yellow)](#)

[![AUR](https://img.shields.io/aur/version/novanav-py)](https://aur.archlinux.org/packages/novanav-py)

<!-- 
[![AUR](https://img.shields.io/aur/version/novanav-py.svg)](https://aur.archlinux.org/packages/novanav-py)
-->

https://aur.archlinux.org/packages/novanav-py

NovaNav Browser is available on AUR (Arch User Repository), and it can be installed using the `yay` package manager. Follow the steps below to install NovaNav:

1. Make sure you have `yay` installed. If not, you can install it with the following command:
   
   ```
   sudo pacman -S yay
   ```

   Once yay is installed, you can install by running the following command:
   
   ```
   yay -S novanav-py
   ```

This command will automatically fetch the package from AUR and handle the installation process for you.
You can find NovaNav Browser in your program menu!

-->
<!--

#### 🚀 Requirements & Instructions for compiling and executing manually.

Install Python3, then:

You might need perhaps to create an enviroment for pip: 
```
python3 -m venv novanav-env
source novanav-env/bin/activate
```

If not, just go with this:

```
pip3 install -r requirements.txt
```

Runt it with: 

```
python3 ./src/novanav.py
```


<sub>* It might contain lots of errors, bugs, etc. Meanwhile, Keep it simple now with C++ stable version, running smomothly!</sub>

</details>


<details>

<summary><strong>NovaNav written in Rust 🚀 - beta/testing (experimental alternative in progress)*</strong></summary>

<br>

Important library to install:

```
sudo pacman -S libsoup
```

In Arch, add this line to .bashrc:

```
export PKG_CONFIG_PATH=/usr/lib/pkgconfig:$PKG_CONFIG_PATH
```

 Instructions for installation/compiling the program written in Rust*: 



Add that to the [dependencies] section of your `Cargo.toml` file:
```
vim Cargo.toml
```

To test it , run this command on terminal:

```
chmod +x run_novanav_rust.sh
./run_novanav_rust.sh
```
Compiling: 

```
rustc src/rust/novanav.rs
./novanav
```

<sub>* It might contain lots of errors, bugs, etc. Meanwhile, Keep it simple now with C++ stable version, running smomothly!</sub>

</details>


#

[![Errors Reported](https://img.shields.io/badge/Errors-Reported-blue)](#)

<details>

<summary><strong>Errors, Fixes and tips</strong></summary>
<br>


To force the link and overwrite all conflicting files (Errors).

If you get this error while you are trying to run the software in the terminal or the software doesn't show up, follow the next steps on macOS or Linux:

E.g.:

```
user@host src % python3 novanav_linux.py
Traceback (most recent call last):
  File "/Users/user/NovaNav/src/novanav_linux.py", line 2, in <module>
    from PyQt5.QtCore import QUrl, Qt
ModuleNotFoundError: No module named 'PyQt5'
```

Install or reinstall pyqt5 with the following command in brew:
E.g.:

```
user@host src % brew install pyqt5
==> Downloading https://formulae.brew.sh/api/formula.jws.json
############################################################################################################ 100.0%
Warning: Formula pyqt5 was renamed to pyqt@5.
==> Downloading https://formulae.brew.sh/api/cask.jws.json
############################################################################################################ 100.0%
Warning: pyqt@5 5.15.10 is already installed, it's just not linked.
To link this version, run:
  brew link pyqt@5
user@host src % brew link pyqt@5
Linking /usr/local/Cellar/pyqt@5/5.15.10... 
Error: Could not symlink bin/pylupdate5
Target /usr/local/bin/pylupdate5
already exists. You may want to remove it:
  rm '/usr/local/bin/pylupdate5'

To force the link and overwrite all conflicting files:
  brew link --overwrite pyqt@5

To list all files that would be deleted:
  brew link --overwrite pyqt@5 --dry-run

```

**So, if you want to delete all files that will be deleted by `brew link --overwrite`, use the `--dry-run` option
Then run,**

```
rm '/usr/local/bin/pylupdate5'
```

```
brew link --overwrite pyqt@5
```

```
brew link --overwrite pyqt@5 --dry-run
```
The last one will show you what would delete if you proceed. If everything looks good, hit return. Then you should have PyQt5 properly installed.

And then try running your program again.

E.g.:

```
user@host src % rm '/usr/local/bin/pylupdate5'
user@host src % brew link --overwrite pyqt@5

Linking /usr/local/Cellar/pyqt@5/5.15.10... 5279 symlinks created.
```

```
user@host src % python3 novanav.py  
```

Then should be fine with no errors. 

<sub>If you still have problems please let me know.
Also if you want to install `pyrcc5` (which is not necessary but can help) just do a normal installation using pip: I am using Python 3.x.</sub>

</details>

-->

#
