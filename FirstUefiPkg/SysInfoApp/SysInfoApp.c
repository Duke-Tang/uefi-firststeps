/** @file
  SysInfoApp — a minimal UEFI application built with EDK II.

  What it does (small on purpose):
    1. Prints "Hello, UEFI!" to the active console.
    2. Reads three fields straight out of the EFI System Table (gST):
       firmware vendor, firmware revision, and the UEFI spec revision
       that this table conforms to.
    3. Blocks on a single key press using Boot Services (gBS) and the
       ConIn "WaitForKey" event, then returns control to whatever launched
       it (UEFI Shell or the Boot Manager).

  Why these three steps: each one lets me defend a different layer in an
  interview — the global tables (gST/gBS), the protocol model (Simple Text
  In/Out), and the event/wait model — without the file becoming bloated.

  Copyright notice: this is original training code, MIT-style, do as you like.
**/

#include <Uefi.h>                                  // Core UEFI types: EFI_STATUS, EFI_HANDLE, etc.
#include <Library/UefiLib.h>                        // Print()
#include <Library/UefiApplicationEntryPoint.h>      // Wires UefiMain() as the entry point
#include <Library/UefiBootServicesTableLib.h>       // Exposes the gST, gBS, gImageHandle globals

/**
  Application entry point.

  The UefiApplicationEntryPoint library provides the real _ModuleEntryPoint,
  sets up the global tables, and then calls this function. That is why the
  name "UefiMain" must match ENTRY_POINT in the .inf file.

  @param[in] ImageHandle  The firmware-allocated handle for this image.
  @param[in] SystemTable  Pointer to the EFI System Table.

  @retval EFI_SUCCESS     The application ran and exited normally.
**/
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS     Status;
  EFI_INPUT_KEY  Key;
  UINTN          EventIndex;

  //
  // 1. Say hello. Print() is a UefiLib helper; under the hood it formats a
  //    UTF-16 (CHAR16, the L"..." prefix) string and pushes it to
  //    gST->ConOut->OutputString — the Simple Text Output protocol.
  //
  Print (L"Hello, UEFI!  --  SysInfoApp\n\n");

  //
  // 2. Pull data directly out of the System Table. gST is the same pointer
  //    as the SystemTable parameter; the library just makes it global so any
  //    file can reach it.
  //
  Print (L"Firmware Vendor    : %s\n", gST->FirmwareVendor);
  Print (L"Firmware Revision  : 0x%08x\n", gST->FirmwareRevision);
  Print (
    L"UEFI Spec Revision : %d.%d\n",
    (gST->Hdr.Revision >> 16),     // major version in the high 16 bits
    (gST->Hdr.Revision & 0xFFFF)   // minor version in the low 16 bits
    );

  Print (L"\nPress any key to exit...\n");

  //
  // 3. Wait for one keystroke. gBS is the Boot Services table. WaitForEvent
  //    blocks until the ConIn keypress event fires, then we drain one key.
  //
  Status = gBS->WaitForEvent (1, &gST->ConIn->WaitForKey, &EventIndex);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  gST->ConIn->ReadKeyStroke (gST->ConIn, &Key);

  return EFI_SUCCESS;
}
