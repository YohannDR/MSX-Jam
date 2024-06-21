// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄      
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  by Guillaume 'Aoineko' Blanchard under CC BY-SA license
//─────────────────────────────────────────────────────────────────────────────

//*******************************************************************************
// BUILD STEPS
//*******************************************************************************

// DoClean   = false;	//-- Clear all intermediate files and exit (boolean)
// DoCompile = true;	//-- Compile all the project and engine source code (boolean). Generate all REL files
// DoMake    = true;	//-- Link all the project and engine source code (boolean). Merge all REL into one IHX file
// DoPackage = true;	//-- Generate final binary file (boolean). Binarize the IHX file
// DoDeploy  = true;	//-- Gathering of all files necessary for the program to work (boolean). Depends on the type of target
// DoRun     = false;	//-- Start the program automatically at the end of the build (boolean)

//*****************************************************************************
// PROJECT SETTINGS
//*****************************************************************************

//-- Project name (string). Will be use for output filename
ProjName = "template";

//-- List of project modules to build (array). If empty, ProjName will be added
ProjModules = [ ProjName ];

//-- Project segments base name (string). ProjName will be used if not defined
// ProjSegments = "";

//-- List of library modules to build (array)
LibModules = [ "system", "bios", "vdp", "print", "input", "memory", "game", "game_pawn", "math", "string", "debug", "scroll" ];

//-- Additional sources to be compiled and linked with the project (array)
AddSources = [ "entity.c", "player.c", "ball.c" ];

//-- Target MSX machine version (string)
//   - 1        MSX1
//   - 2        MSX2
//   - 12       MSX1 and 2 (multi support)
//   - 2K       Korean MSX2 (SC9 support)
//   - 2P       MSX2+
//   - 22P      MSX2 and 2+ (multi support)
//   - 122P     MSX1, 2 and 2+ (multi support)
//   - 0        MSX0
//   - TR       MSX turbo R
//   - 3        MSX3 (reserved)
Machine = "2";

//-- Target program format (string)
//   - BIN              .bin    BASIC binary program (starting at 8000h)
//   - BIN_DISK         .bin    BASIC binary program (starting at 8000h) on disk
//   - BIN_TAPE         .bin    BASIC binary program (starting at 8000h) on tape
//   - BIN_USR          .bin    BASIC USR binary driver (starting at C000h)
//   - DOS1             .com    MSX-DOS 1 program (starting at 0100h)
//   - DOS2             .com    MSX-DOS 2 program (starting at 0100h)
//   - DOS2_MAPPER      .com    MSX-DOS 2 launcher to RAM mapper (launcher starting at 0100h, program at 4000h)
//   - DOS0             .com    Direct program boot from disk (starting at 0100h)
//   - ROM_8K           .rom    8 KB ROM in page 1 (4000h ~ 5FFFh)
//   - ROM_8K_P2        .rom    8 KB ROM in page 2 (8000h ~ 9FFFh)
//   - ROM_16K          .rom    16 KB ROM in page 1 (4000h ~ 7FFFh)
//   - ROM_16K_P2       .rom    16 KB ROM in page 2 (8000h ~ BFFFh)
//   - ROM_32K          .rom    32 KB ROM in page 1&2 (4000h ~ BFFFh)
//   - ROM_48K          .rom    48 KB ROM in page 0-2 (0000h ~ BFFFh)
//   - ROM_48K_ISR      .rom    48 KB ROM in page 0-2 (0000h ~ BFFFh) with ISR replacement
//   - ROM_64K          .rom    64 KB ROM in page 0-3 (0000h ~ FFFFh)
//   - ROM_64K_ISR      .rom    64 KB ROM in page 0-3 (0000h ~ FFFFh) with ISR replacement
//   - ROM_ASCII8       .rom    ASCII-8: 8 KB segments for a total of 64 KB to 2 MB
//   - ROM_ASCII16      .rom    ASCII-16: 16 KB segments for a total of 64 KB to 4 MB
//   - ROM_KONAMI       .rom    Konami MegaROM (aka Konami4): 8 KB segments for a total of 64 KB to 2 MB
//   - ROM_KONAMI_SCC   .rom    Konami MegaROM SCC (aka Konami5): 8 KB segments for a total of 64 KB to 2 MB
//   - ROM_NEO8         .rom    NEO-8: 8 KB segments for a total of 1 MB to 32 MB
//   - ROM_NEO16        .rom    NEO-16: 16 KB segments for a total of 1 MB to 64 MB
Target = "ROM_32K";

//-- ROM mapper total size in KB (number). Must be a multiple of 8 or 16 depending on the mapper type (from 64 to 4096)
// ROMSize = 128;

//-- Postpone the ROM startup to let the other ROMs initialize like Disk controller or Network cartridge (boolean)
// ROMDelayBoot = false;

//-- Add a ROM signature to help flasher and emulator to detect the ROM type properly (boolean)
AddROMSignature = true;

//-- Select RAM in slot 0 and install ISR there (boolean). For MSX with at least 64 KB of RAM
// InstallRAMISR = false;

//-- Type of custom ISR to install (string). ISR is install in RAM or ROM depending on Target and InstallRAMISR parameters
//   - NONE       No ISR
//   - VBLANK     V-blank handler
//   - VHBLANK    V-blank and h-blank handler (V9938 or V9958)
//   - V9990      V-blank, h-blank and command end handler (V9990)
// CustomISR = "VBLANK";

//-- Use automatic banked call and trampoline functions (boolean). For mapped ROM
// BankedCall = 0;

//-- Overwrite RAM starting address (number). For example. 0xE0000 for 8K RAM machine
// ForceRamAddr = 0;

//-- List of data files to copy to disk (array)
// DiskFiles = [];

// --List of data files to copy to disk (array)
// DiskFiles = [];

//-- BASIC USR driver default address (number)
// USRAddr = 0xC000;

//-- Parse MSX-DOS command-line arguments
// DOSParseArg = false;

//*******************************************************************************
// SIGNATURE SETTINGS
//*******************************************************************************

//-- Add application signature to binary data (boolean)
AppSignature = true;

//-- Application company (*). Can be 2 character string or 16-bits integer (0~65535)
AppCompany = "GL";

//-- Application ID. Can be 2 character string or 16-bits integer (0~65535)
AppID = "EX";

//-- Application extra data (array). Comma-separated bytes starting with data size
// AppExtra = [];

//*******************************************************************************
// MAKE SETTINGS
//*******************************************************************************

//-- Force to generate MSXgl static library even if 'msxgl.lib' already exist (boolean)
// BuildLibrary = true;

//-- Prepare program for debug (boolean)
// Debug = false;

//-- Move debug symbols to deployement folder (boolean)
// DebugSymbols = false;

//-- Allow compiler to generate undocumented Z80 instructions (boolean)
// AllowUndocumented = false;

//-- Assembler code optimizer (string)
//   - None
//   - Peep       SDCC peep hole otpimizer
//   - MDL        MDL z80 otpimizer
// AsmOptim = "None";

//-- Code optimization priority (string)
//   - Default
//   - Speed
//   - Size
// Optim = "Speed";

//-- Code optimization priority (string/integer)
//   - Fast			    2000
//   - Default		    3000
//   - Optimized	   50000
//   - Ultra		  200000
//   - Insane		10000000
CompileComplexity = "Default";

//-- Additionnal compilation options (string)
// CompileOpt = "";

//-- Skip file if compile data (REL) is newer than the source code (boolean)
// CompileSkipOld = false;

//-- Additionnal link options (string)
// LinkOpt = "";

//-- Automatic increment of build version in a header file (boolean)
// BuildVersion = false;

//-- List files to be localized (array)
// LocFiles = [];

//-- Localization output filename (string)
// LocOutput = "localization.h";

//-- Localization structure name (string)
// LocStruct = "g_LocData";

//-- Package all segments into a lib file to reduce the number of files to link (boolean)
// PackSegments = false;

//*****************************************************************************
// BUILD TOOL OPTION
//*****************************************************************************

//-- Activate verbose mode and get more build information (boolean)
Verbose = true;

//-- Output build information to the standard console (boolean)
// LogStdout = true;

//-- Output build information to a log file (boolean)
// LogFile = false;

//-- Name of the log file (string)
// LogFileName = "";

//*******************************************************************************
// EMULATOR SETINGS
//*******************************************************************************

//-------------------------------------------------------------------------------
// General options

// EmulMachine    = true;				//-- Force the MSX version of the emulated machine (boolean)
// Emul60Hz       = false;				//-- Force the emulated machine to be at 60 Hz (boolean)
// EmulFullScreen = false;				//-- Force the emulator to start in fullscreen mode (boolean)
// EmulMute       = false;				//-- Disable emulator sound (boolean)
// EmulDebug      = false;				//-- Start emulator debugger with program launch (boolean)
// EmulTurbo      = false;				//-- Start emulator in turbo mode (boolean)

//-- Emulator extra parameters to be add to command-line (string). Emulator sotfware specific
// EmulExtraParam = "";

//-------------------------------------------------------------------------------
// Extension options

// EmulSCC      = false;				//-- Add SCC extension (boolean)
// EmulMSXMusic = false;				//-- Add MSX-Music extension (boolean)
// EmulMSXAudio = false;				//-- Add MSX-Audio extension (boolean)
// EmulOPL4     = false;				//-- Add OPL4 extension (boolean)
// EmulPSG2     = false;				//-- Add second PSG extension (boolean)
// EmulV9990    = false;				//-- Add V9990 video-chip extension (boolean)

//-------------------------------------------------------------------------------
// Input options

//-- Plug a virtual device into the joystick port A (string)
//   - Joystick
//   - Keyboard         Fake joystick
//   - Mouse
//   - NinjaTap
// EmulPortA = "";

//-- Plug a virtual device into the joystick port B (string)
// EmulPortB = "";