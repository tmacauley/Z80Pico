# Example Program to emulate Grant Searle's 7 chip Z80 computer

Link
[http://searle.x10host.com/z80/SimpleZ80.html]


Modifications:
*   RAM shadows the ROM - writing to the ROM addresses will write to the shadow RAM.
*   Added ram_page variable to bank switch ROM to RAM.

Files:
*   gs56kz80rom_original.hex - Grant Searle's original hex file
*   gs56kz80rom_PhilG.hex - PhilG's modified ROM with monitor. Link: [http://www.mccrash-racing.co.uk/philg/retro/retro.htm]
