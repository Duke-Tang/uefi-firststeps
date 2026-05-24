# SysInfoApp — First Steps with UEFI / EDK II

A deliberately small UEFI application, built with **EDK II**, meant as a bridge
project from microcontroller firmware (STM32 / bare-metal) into the **x86 + UEFI**
world that BIOS/firmware roles ask for.

It does three small things, each chosen so I can explain a different layer of UEFI:

1. Prints `Hello, UEFI!` to the console.
2. Reads firmware vendor / firmware revision / UEFI spec revision from the **EFI System Table**.
3. Waits for a key press using **Boot Services** + the ConIn event, then exits.

```
FirstUefiPkg/
├── FirstUefiPkg.dsc          # optional standalone platform description
└── SysInfoApp/
    ├── SysInfoApp.c          # the application (entry point: UefiMain)
    └── SysInfoApp.inf        # the build "contract" for this module
```

---

## 1. Install dependencies (GitHub Codespaces / Ubuntu)

```bash
sudo apt-get update
sudo apt-get install -y build-essential uuid-dev iasl git nasm \
                        python-is-python3 qemu-system-x86 ovmf
```

## 2. Get EDK II and set up the build environment

Clone edk2 **next to** this repo (so both folders share one parent):

```bash
git clone https://github.com/tianocore/edk2.git
cd edk2
git submodule update --init        # pulls in BaseTools deps, openssl, etc.
make -C BaseTools                  # builds the C helper tools once
source edksetup.sh                 # sets WORKSPACE / EDK_TOOLS_PATH for THIS shell
cd ..
```

> `source` matters: `edksetup.sh` exports environment variables that must
> persist in your current shell. Running it as `./edksetup.sh` would lose them.

## 3. Build the application (recommended path)

We build the **single module** against the maintained `MdeModulePkg.dsc`. That
DSC already resolves every library class a UEFI application needs, so this path
keeps working across edk2 versions.

```bash
# Make the build see BOTH trees: edk2 itself, and this repo (for FirstUefiPkg)
export PACKAGES_PATH=$PWD/edk2:$PWD
cd edk2

build -a X64 -t GCC5 \
      -p MdeModulePkg/MdeModulePkg.dsc \
      -m FirstUefiPkg/SysInfoApp/SysInfoApp.inf
```

> **Toolchain gotcha (this is a real one to know):** on a **stable tag**
> (`edk2-stable202xxx`) the toolchain tag is `GCC5`. On the **latest master**
> the `GCC5` tag was deprecated — use `-t GCC` instead. If the build complains
> about an unknown tool chain, that's the first thing to switch.

Output (DEBUG build):

```
edk2/Build/MdeModule/DEBUG_GCC5/X64/SysInfoApp.efi
```

*Optional self-contained build* (study the library resolutions yourself):

```bash
build -a X64 -t GCC5 -p FirstUefiPkg/FirstUefiPkg.dsc
# output: edk2/Build/FirstUefiPkg/DEBUG_GCC5/X64/SysInfoApp.efi
```

## 4. Run it in QEMU with OVMF

UEFI auto-launches the default boot file `EFI/BOOT/BOOTX64.EFI`, so we copy our
`.efi` there and hand QEMU a FAT directory as a virtual disk:

```bash
mkdir -p /tmp/esp/EFI/BOOT
cp Build/MdeModule/DEBUG_GCC5/X64/SysInfoApp.efi /tmp/esp/EFI/BOOT/BOOTX64.EFI

# OVMF firmware path differs by distro — find yours:
dpkg -L ovmf | grep -i 'OVMF.*\.fd'    # e.g. /usr/share/ovmf/OVMF.fd

qemu-system-x86_64 \
  -bios /usr/share/ovmf/OVMF.fd \
  -drive format=raw,file=fat:rw:/tmp/esp \
  -net none
```

> **Headless gotcha (Codespaces):** OVMF's console is graphical, so a remote
> Codespace won't pop a window. Easiest fix for a screenshot: run this step on
> local Ubuntu / WSL2 / macOS where a QEMU window opens. (Building in Codespaces
> is fine — only *seeing* the screen needs a display.)

You should see the three System Table fields print, then "Press any key to exit".

---

## How the pieces connect (one paragraph)

`SysInfoApp.inf` declares a `UEFI_APPLICATION` whose entry point is `UefiMain`.
The `UefiApplicationEntryPoint` library supplies the real `_ModuleEntryPoint`,
initialises the global tables, and calls `UefiMain`. Inside, `gST` (System Table)
and `gBS` (Boot Services) come from `UefiBootServicesTableLib`, and `Print()`
comes from `UefiLib`. The build system picks a concrete library *instance* for
each library *class* using the active `.dsc` — that indirection is the whole
point of EDK II's design.

## License

Original code. Use freely (MIT-style, no warranty).
