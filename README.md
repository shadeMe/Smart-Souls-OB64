# 📑 Smart Souls

Knocking some sense into those petty souls!

### 🔨 Building

Refer to instructions [here](https://github.com/shadeMe/CommonLibOB64-Template).

## 📖 License

[GPL-3.0-or-later](COPYING) WITH [Modding Exception AND GPL-3.0 Linking Exception (with Corresponding Source)](EXCEPTIONS).

Specifically, the **Modded Code** is Oblivion Remastered (and its variants) and **Modding Libraries** include [OBSE64](https://github.com/ianpatt/obse64).

For additional information on GPL licenses, see the [FSF FAQ](https://www.gnu.org/licenses/gpl-faq.en.html)

# CommonLibOB64 Plugin Template

This is a basic plugin template using CommonLibOB64.

### Requirements

- [XMake](https://xmake.io) [2.8.2+]
- C++23 Compiler (MSVC, Clang-CL)

## Getting Started

```bat
git clone --recurse-submodules https://github.com/libxse/commonlibob64-template
cd commonlibob64-template
```

### Build

To build the project, run the following command:

```bat
xmake build
```

> **_Note:_** _This will generate a `build/windows/` directory in the **project's root directory** with the build output._

### Build Output (Optional)

If you want to redirect the build output, set one of or both of the following environment variables:

- Path to a Oblivion install folder: `XSE_TES4_GAME_PATH`

- Path to a Mod Manager mods folder: `XSE_TES4_MODS_PATH`

### Project Generation (Optional)

If you want to generate a Visual Studio project, run the following command:

```bat
xmake project -k vsxmake
```

> **_Note:_** _This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system._

### Upgrading Packages (Optional)

If you want to upgrade the project's dependencies, run the following commands:

```bat
xmake repo --update
xmake require --upgrade
```

## Documentation

Please refer to the [Wiki](../../wiki/Home) for more advanced topics.
