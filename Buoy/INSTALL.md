# WWXS Buoy Project

This project uses the ESP-IDF v4.2.1 Development Framework

## Development environment setup

The guide for setting up the environment needed to compile the code and flash it to the board is here:

**v4.2.1** : [https://docs.espressif.com/projects/esp-idf/en/v4.2.1/esp32/get-started/index.html](https://docs.espressif.com/projects/esp-idf/en/v4.2.1/esp32/get-started/index.html)

Basically, install Toolchain, download the ESP-IDF libraries, setup the IDF_PATH environment variable, and make sure you have the required Python packages.

## Windows (Windows 10, 20H2)

#### Prerequisite - Visual Studio, Microsoft C++ Build Tools

1. Download the installer available at: https://visualstudio.microsoft.com/visual-cpp-build-tools/

2. Run the installer, which will download the Visual Studio installer.

3. Once that installation completes, you will be presented with the Visual Studio Installer window with the necessary packages already selected (MSBuild Tools). Click "Install" to start the installation. Once the installation is done, you may close the Visual Studio Installer window.

#### Setting up the Toolchain:

1. Download the ESP-IDF Tools installer: [https://dl.espressif.com/dl/esp-idf-tools-setup-2.3.exe](https://dl.espressif.com/dl/esp-idf-tools-setup-2.3.exe)

2. Run the ESP-IDF Tools installer, which will install the required development tools.

3. Select to install Python 3.7 when presented. (Does not work with Python 3.9)

4. When presented to download ESP-IDF, select the version v4.2.1 (release version) to download and install. Install the `esp-idf` folder in your Home directory, instead of the Desktop.
`C:\Users\$USER\esp-idf` where `$USER` is your PC account name.

5. Select the checkbox to add the ESP-IDF directory to Windows Defender exclusions, so compilation performance will be better.

6. Downloading and installing may take up to 10-15 minutes, depending on your internet connection. Once installation is complete, you should have the ESP-IDF tools installed successfully.

#### Using ESP-IDF tools in a project

- Execute the script export.bat for Command Prompt or export.ps1 for Powershell each time you run a Command Propmt or Powershell session for the project. It will load all the necessary directories into the `PATH` environment variable. 

- Command Prompt: 
> `%userprofile%\esp-idf\export.bat`

- Powershell (May require setting Powershell [Execution Policies](https:/go.microsoft.com/fwlink/?LinkID=135170) to get the script to run):
> `.$HOME/esp-idf/export.ps1` 

## Linux (Ubuntu 20.04)

#### Setting up the Toolchain:
1. Install these prerequisites:
>`sudo apt-get install git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0`

2. Fix Permission issues **Failed to open port /dev/ttyUSB0**:
>`sudo usermod -a -G dialout $USER`
3. Re-login to enable read and write permission for the serial port.

#### Install ESP-IDF
1. Download and install ESP-IDF v4.3 from GitHub in your home `~/` directory:
> `mkdir -p ~/esp`
>
> `cd ~/esp`
>
> `git clone -b v4.2.1 --recursive https://github.com/espressif/esp-idf.git`
>
> `cd ~/esp/esp-idf`
>
> `./install.sh`

## Using ESP-IDF in your project

#### Linux

Run the `export.sh` script in your Linux terminal when working on the project.
    
> `. $HOME/esp/esp-idf/export.sh`

... or you can create an alias in your shell's profile (.bashrc, .profile, etc.):

> `alias get_idf='. $HOME/esp/esp-idf/export.sh'`

Then refresh your configuration by restarting the terminal session, or by running:

> `source ~/.bashrc` <your shell profile>

Run the alias `get_idf` to activate the ESP-IDF tools in your terminal session.

- Command Prompt: 
> `%userprofile%\esp-idf\export.bat`

- Powershell (May require setting Powershell [Execution Policies](https:/go.microsoft.com/fwlink/?LinkID=135170) to get the script to run):
> `.$HOME/esp-idf/export.ps1`

Now, you can use `idf.py` to build, flash, and monitor your esp32 project.

- **Build** the project:
> idf.py build

- **Flash** the project to the ESP32 (ESP32 is connected with USB):
> idf.py flash

- **Monitor** the serial output of the ESP32 (ESP32 is connected with USB):
> idf.py monitor

- **Clean** and remove the build files of the project:
> idf.py clean

3. Now you can use `idf.py` to build, flash, and monitor your esp32 project.

    - Build the project:
    > idf.py build
    - Flash the project to the ESP32 (ESP32 is connected with USB):
    > idf.py flash
    - Monitor the serial output of the ESP32 (ESP32 is connected with USB):
    > idf.py monitor
    - Clean and remove the build files of the project:
    > idf.py clean

## Changelog

05-06-21: Revised steps for ESP-IDF v4.2.1. v3.2.2 was previous and initial version. (DS)
