/* value for text segment (like _TEXT for OMF), same calculation
   as in main linker script
 */
_TEXTSEG = LOADADDR (.text) / 16 - __msdos_mz_hdr_paras -
           ADDR (.text) / 16 + 0x10000;
