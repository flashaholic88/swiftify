# Swiftify

a spotify player for desktop, built with Qt Quick on top of a Rust core that
wraps [librespot](https://github.com/librespot-org/librespot). it follows
Material 3 Expressive (heavy inspo from end4) and has an immersive player,
synced lyrics, visualizers, an equalizer, and local files with local playlists
where u can edit the tracks metadata.
![Alt text](/assets/home.png)
this apps purpose is to replace spotifys heavy webview client, but its also mostly eyecandy lol
it runs significantly lighter than the spotify client.

**EVEN WITH THE EYECANDY TURNED ON....** it is lighter than spotify + spotifys renderers both on cpu AND ram!!!!!

id recommend turning the visualizers and background off if the app runs in the background tho lol

***anyways runs on linux (made for wayland and hyprland) macos, and windows.***

## previews
first image shows equalizer, tempo and pitch settings. second picture shows how the immersive player and the visualizer/lyrics look
<table border="0">
  <tr>
    <td><img src="/assets/eq.png" alt="equalizer and tempo" width="400"/></td>
    <td><img src="/assets/immersive.png" alt="immersive player with visualizers" width="400"/></td>
  </tr>
</table>

## download

you can download swiftify for your operating system at [https://shredos.dev](https://shredos.dev/#download)

<br/>

## Contents

- [Before you start](#before-you-start)
- [Prebuilt builds](#prebuilt-builds)
- [Building from source](#building-from-source)
  - [Build requirements](#build-requirements)
  - [macOS](#macos)
  - [Linux](#linux)
  - [Windows](#windows)
- [Stats](#stats)
- [Command line](#command-line)
- [License](#license)

## Before you start

- **spotify premium.** playback goes through librespot, and that only works with premium.
- **a spotify client id.** your library, playlists and search come from the spotify
  web api through your own developer app. the first launch walks u through it:
  make an app in the [spotify developer dashboard](https://developer.spotify.com/dashboard),
  add `http://127.0.0.1:8898/login` as a redirect uri, and paste the apps client id
  into swiftify. you dont need the client secret.

## Tutorials

### set up soloist

this is the solution of 80% of problems, and often also for the error message "Spotify will not play this track"

spotify refuses librespot the keys to decrypt songs mostly on newer accounts (librespot's
logs say `error audio key 0 1`), so every song fails. swiftify can play
spotify's songs through [spotify soloist](https://developer.spotify.com/documentation/soloist) instead, spotify's own connect client, which isnt affected.

set up:

1. [download the soloist binary from spotify](https://developer.spotify.com/documentation/soloist/tutorials/getting-started#download-spotify-soloist)
2. extract the downloaded .zip file and 
   1. WINDOWS: move `soloist.exe` to the exact same folder your `swiftify.exe` is in
   2. MACOS or LINUX: move the soloist binary to `~/.local/bin/`, `/opt/homebrew/bin/` or anywhere on your `PATH` 
3. go to the [spotify developer soloist dashboard](https://developer.spotify.com/dashboard/soloist) and generate and copy your API key from there.
4. create a text file in `%LOCALAPPDATA%\soloist` on windows or `~/.config/soloist/` called `api-key`. you can also put your api key inside of `SOLOIST_API_KEY` (this path and filename must match exactly this. please really check this twice as windows loves to append ".txt" to text files and doesnt show it)
5. open that file with a text editor and paste your previously copied api key into this file.
6. save the file.
7. open spotify on your phone or on your pc and go to the devices popup
8. open swiftify and try to play something. you should get an error saying you need to connect soloist.
9. a "Swiftify" device should show up in the devices in the native spotify app. Click that. if it doesnt show up fully restart swiftify and try again.
10. once it connects try to play anything. Congratulations you just set up Spotify soloist


you can disable soloist manually with `SWIFTIFY_PLAYBACK=librespot`

if spotify doesnt list "Swiftify" for step 9, please retry or a firewall (ufw) is probably
dropping the pairing connection; pair from the spotify desktop app on the same computer.

### Discord RPC

enable "Discord Rick Presence" inside the swiftify account settings to display your current track, artist, public album cover
and playback status on your Discord profile. the Discord app needs to be running for this to work. I recommend disabling
"Display spotify as your status" if you have connected your Spotify account so it doesnt show twice.

set up discord rpc:
1. create an application on [Discord's Developer Portal](https://discord.com/developers/applications)
2. copy its public AppID (this is just an identifier for your app. this is not confidential dont worry about that)
3. paste your AppID into the Application ID field in the swiftify account settings.

this integration works with the native app, Canary builds, Beta builds, Vesktop and app stores like Flatpak or Snap

## Prebuilt builds

[releases](https://github.com/shredos67/swiftify/releases) have windows and linux x86_64 builds,
and a macos dmg for apple silicon.

- **windows.** unzip it anywhere and run `bin\swiftify.exe`.
- **linux.** unpack it anywhere and run `bin/swiftify`. it brings its own Qt, but needs
  alsa and libsecret, which pretty much every desktop already has.
- **macos.** needs macos 14.4 or newer. open the dmg and drag swiftify into applications.
  its signed ad hoc, so the first launch gets blocked, open it anyway from system settings ›
  privacy & security, or run `xattr -dr com.apple.quarantine /Applications/Swiftify.app`.

## Building from source

### Build requirements

- Qt 6.10 or newer: Qt Base, Qt Declarative, Qt Shader Tools and Qt WebSockets
- QtKeychain and QCoro 0.10 or newer, both built for Qt 6
- CMake 3.24 or newer, Ninja, and a C++20 compiler
- Rust (stable, from [rustup](https://rustup.rs))

the first cmake run needs internet: it pulls [Corrosion](https://github.com/corrosion-rs/corrosion)
(which builds the rust core as part of the cmake build) and two fonts, Material Symbols
Rounded and Google Sans Flex, each checked against a pinned hash.

the builds below skip the tests with `-DBUILD_TESTING=OFF`. for a manual CMake
build, drop that and run `ctest --test-dir build` if u want them.

### macOS

u need [homebrew](https://brew.sh) and apple's command line tools (`xcode-select --install`, or full xcode). then:

```sh
brew install qtbase qtdeclarative qtshadertools qtwebsockets qtkeychain qcoro6 cmake ninja
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
source "$HOME/.cargo/env"

git clone https://github.com/shredos67/swiftify.git
cd swiftify
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF
cmake --build build
ditto build/swiftify.app /Applications/Swiftify.app
```

it's signed ad hoc, so no developer account needed.
with full xcode installed the icon gets compiled with `actool` and picks up macos 26's glass,
dark and tinted looks; with just the command line tools it's built from the PNGs.

### Linux

on arch install from the aur with paru or yay:

```sh
# precompiled release binary (recommended)
paru -S swiftify-bin

# or build from source
paru -S swiftify

# or development branch (git master)
paru -S swiftify-git
```

or build a package manually:

```sh
git clone https://github.com/shredos67/swiftify.git
cd swiftify/packaging/arch
makepkg -si
```
or use cmake:

```sh
# replace with your package manager if not on arch
sudo pacman -S --needed git qt6-base qt6-declarative qt6-shadertools qt6-wayland qt6-websockets qtkeychain-qt6 qcoro libpulse alsa-lib openssl cmake ninja rust pkgconf

git clone https://github.com/shredos67/swiftify.git
cd swiftify
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF -DCMAKE_INSTALL_PREFIX="$HOME/.local"
cmake --build build
cmake --install build
```

on debian/ubuntu (or derivatives with Qt 6.8 or 6.9), use the build script that handles
compatibility patches automatically:

```sh
sudo apt-get install git qt6-base-dev qt6-declarative-dev qt6-shader-baker \
    qt6-shadertools-dev qt6-wayland qt6-websockets-dev qtkeychain-qt6-dev qcoro-qt6-dev \
    libpulse-dev libasound2-dev libssl-dev cmake ninja-build pkg-config
# install rust from https://rustup.rs if you dont have it

git clone https://github.com/shredos67/swiftify.git
cd swiftify
./tools/build-debian.sh
```

or with `--install-deps` to install the dependencies automatically:

```sh
git clone https://github.com/shredos67/swiftify.git
cd swiftify
./tools/build-debian.sh --install-deps
```

anywhere else, install the same reqs (Qt 6.10+ with D-Bus, OpenGL and Wayland, QtKeychain
and QCoro for Qt 6, libpulse, plus the ALSA, OpenSSL and pkg-config dev packages), then
run the cmake commands above. if you have Qt 6.8 or 6.9, use `tools/build-debian.sh` which
applies the necessary compatibility patches.

`cmake --install` drops `swiftify` in `~/.local/bin` (make sure that's on your `PATH`)
and the desktop entry, icons and fonts in `~/.local/share`. 

your login gets saved in the secret service through QtKeychain, so u need something
providing one: gnome-keyring, kwallet or keepassxc.

on linux it also hooks into MPRIS (media keys, `playerctl`, shell media widgets),
logind (pauses before the machine sleeps), pipewire/pulseaudio device changes, and
the tray. `-DSWIFTIFY_DESKTOP_INTEGRATION=OFF` turns those off.

### Windows

**because most Windows users cant even use their own OS, i made a script that handles building so it's as simple as possible :)**

install:
- **Qt 6.10** with *Qt Shader Tools* and *Qt WebSockets*, **CMake** and **Ninja** from the **Qt online Installer**
- **Rust** from [rustup](https://rustup.rs)
- **git** from [git-scm.com](https://git-scm.com/install/)

choose either **Qt for MSVC 2022 64-bit** with Visual Studio 2022 17.1+ (or Build Tools) and its *Desktop development with C++*
workload, or **Qt for MinGW 64-bit** with its matching MinGW toolchain.

then run this in Windows PowerShell:

```powershell
git clone https://github.com/shredos67/swiftify.git
Set-Location swiftify
.\tools\build-windows.ps1
```

for a different Qt kit, toolchain location or install directory, pass overrides:

```powershell
.\tools\build-windows.ps1 -QtPath 'C:\Qt\6.11.2\msvc2022_64'
.\tools\build-windows.ps1 -QtPath 'C:\Qt\6.10.3\mingw_64' -MingwPath 'C:\Qt\Tools\mingw1310_64\bin' -InstallPrefix "$env:LOCALAPPDATA\Programs\Swiftify"
```

adjust the example paths to your installed versions, MinGW must match your Qt
kit, not just the example folder name. 

**the toolchain decides one feature:** the Windows media controls (the media overlay,
lock screen and media keys showing what's playing) need `SystemMediaTransportControls`
in the compiler's Windows headers.

| toolchain                                               | media controls            |
|---------------------------------------------------------|---------------------------|
| MSVC 2022                                               | yes                       |
| Qt's MinGW 13.1 (`mingw1310_64`, mingw-w64 runtime v11) | no, everything else works |
| a newer mingw-w64 (tested: GCC 16.1, runtime v14)       | yes                       |

when they're left out, cmake warns `building without the Windows media controls`.
for every feature, build with MSVC. a newer MinGW works too, but it isn't the one
your Qt kit was built with, so the script warns about the mismatch.

**quit a running installed copy through its tray menu before updating it.**

if PowerShell blocks the script, review it first, then run
`powershell -NoProfile -ExecutionPolicy Bypass -File .\tools\build-windows.ps1`.
this applies only to that process, not your machine's execution policy.

start the installed app with:

```powershell
& "$env:LOCALAPPDATA\Programs\Swiftify\bin\swiftify.exe"
```

font downloads are hash-checked on every configure; on Windows, a failed CMake
download is retried with `curl.exe` using the system certificate store.

## Command line

only one swiftify runs at a time. launching it again just passes the command to the one
that's already running, so these work great as keybinds:

| flag           |                           |
|----------------|---------------------------|
| `--immersive`  | open the immersive player |
| `--play-pause` | play or pause             |
| `--next`       | next song                 |
| `--previous`   | previous song             |
| `--stats`      | open the stats page       |

on hyprland for example:

```ini
bind = SUPER SHIFT, M, exec, swiftify --immersive
bind = , XF86AudioPlay, exec, swiftify --play-pause
```

## Authors

Swiftify was developed by [shred0s](https://github.com/shred0s) at [github.com/swiftify-org/swiftify](https://github.com/swiftify-org/swiftify)

## License

swiftify is licensed under the gnu gpl v3 or later, see [LICENSE](LICENSE)
