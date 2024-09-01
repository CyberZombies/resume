# Этап 1: Установка зависимостей, MSYS2, MinGW-w64 и GTK+
FROM mcr.microsoft.com/windows/servercore:ltsc2022 AS dependencies

# Установка PowerShell Core
SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]
RUN Install-PackageProvider -Name NuGet -GuardClause Bypass -Force

# Установка вспомогательных утилит (wget, unzip)
RUN apt-get update && apt-get install -y wget unzip

# Загрузка и установка MSYS2
RUN  Invoke-WebRequest -Uri "https://github.com/msys2/msys2-installer/releases/download/2023-05-21/msys2-x86_64-20230521.exe" -OutFile "msys2-installer.exe" ; \
     Start-Process -FilePath ".\msys2-installer.exe" -ArgumentList "--accept-messages", "--confirm-command", "--no-start" -Wait ; \
     Remove-Item ".\msys2-installer.exe" -Force

# Обновление базы данных пакетов и установка MinGW-w64
RUN C:\msys64\usr\bin\bash.exe -c 'pacman -Syu --noconfirm && pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-gtk3 pkg-config --noconfirm'

# Настройка переменных окружения для MinGW-w64
ENV PATH="/c/msys64/mingw64/bin:${PATH}"
ENV PKG_CONFIG_PATH="/c/msys64/mingw64/lib/pkgconfig:/c/msys64/mingw64/share/pkgconfig"

# Этап 2: Сборка приложения
FROM mcr.microsoft.com/windows/servercore:ltsc2022 

# Копирование необходимых файлов GTK+ из этапа dependencies (может потребоваться корректировка путей)
COPY --from=dependencies /c/msys64/mingw64/ /c/msys64/mingw64/

# Создание рабочей директории
WORKDIR /app

# Копирование исходного кода приложения
COPY . /app

# Компиляция приложения
SHELL ["cmd", "/c"] 
RUN gcc -o interface interface.c bruteForceCombinations.c conclusion.c pressing_setting.c buffer_library2.h `pkg-config --cflags --libs gtk+-3.0` 

# Запуск приложения
CMD ["interface"]