## @file
#  FirstUefiPkg — OPTIONAL standalone platform description.
#
#  You do NOT need this file for the recommended build path in the README
#  (which builds the single module against MdeModulePkg.dsc using -m). It is
#  included so the package can build self-contained, and so you can SEE every
#  library-class -> library-instance decision in one place — useful to study.
#
#  If a library class below errors out because your edk2 checkout differs,
#  fall back to the -m build path in README.md; that path borrows a
#  maintained, always-current set of resolutions from MdeModulePkg.dsc.
##

[Defines]
  PLATFORM_NAME                  = FirstUefiPkg
  PLATFORM_GUID                  = a1b2c3d4-e5f6-4a5b-8c9d-0e1f2a3b4c5d
  PLATFORM_VERSION               = 0.1
  DSC_SPECIFICATION              = 0x00010006
  OUTPUT_DIRECTORY               = Build/FirstUefiPkg
  SUPPORTED_ARCHITECTURES        = IA32|X64
  BUILD_TARGETS                  = DEBUG|RELEASE
  SKUID_IDENTIFIER               = DEFAULT

[LibraryClasses]
  #
  # Entry-point and global-table libraries — the heart of a UEFI application.
  #
  UefiApplicationEntryPoint|MdePkg/Library/UefiApplicationEntryPoint/UefiApplicationEntryPoint.inf
  UefiBootServicesTableLib|MdePkg/Library/UefiBootServicesTableLib/UefiBootServicesTableLib.inf
  UefiRuntimeServicesTableLib|MdePkg/Library/UefiRuntimeServicesTableLib/UefiRuntimeServicesTableLib.inf
  UefiLib|MdePkg/Library/UefiLib/UefiLib.inf
  DevicePathLib|MdePkg/Library/UefiDevicePathLib/UefiDevicePathLib.inf

  #
  # Base "plumbing" libraries that the ones above depend on in turn.
  #
  BaseLib|MdePkg/Library/BaseLib/BaseLib.inf
  BaseMemoryLib|MdePkg/Library/BaseMemoryLib/BaseMemoryLib.inf
  MemoryAllocationLib|MdePkg/Library/UefiMemoryAllocationLib/UefiMemoryAllocationLib.inf
  PrintLib|MdePkg/Library/BasePrintLib/BasePrintLib.inf
  PcdLib|MdePkg/Library/BasePcdLibNull/BasePcdLibNull.inf
  DebugLib|MdePkg/Library/UefiDebugLibConOut/UefiDebugLibConOut.inf
  DebugPrintErrorLevelLib|MdePkg/Library/BaseDebugPrintErrorLevelLib/BaseDebugPrintErrorLevelLib.inf

[Components]
  FirstUefiPkg/SysInfoApp/SysInfoApp.inf
