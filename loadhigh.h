/* LOADHIGH.H - contains macros, structures, prototypes and
 * global variables for LOADHIGH.C */

/* Macro to convert bytes to paragraphs */
#define topara(x) (((x) + 0xf) >> 4)

/* Make sure the FP_SEG macro is a valid lvalue */
#ifdef FP_SEG
#undef FP_SEG
#endif

#define FP_SEG(x)  (*(((unsigned short *)&(x))+1))

/* typedefs for common DOS data types */
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;

/* memory control block */
struct MCB
{
  BYTE sig;
  WORD owner;
  WORD size;
  BYTE res[3];
  BYTE name[8];
};

/* Prototypes for the assembler functions */
WORD DosAlloc(WORD paragraphs); /* DOS func. 48h */
WORD DosSetStrategy(WORD strat);  /* DOS func. 5801h */
int DosSetUMBLink(int value);   /* DOS func. 5803h */
int DosResize(WORD block, WORD newSize);  /* DOS func. 4ah */
void DosFree(WORD block);       /* DOS func. 49h */
WORD GetFirstMCB(void);

/* far memory version of memcmp() */
int farmemcmp(void far * s1, void far * s2, size_t nbytes);

/* These variables are used to save the DOS malloc strategy
 * and UMB link state, to restore them when the program terminates */
WORD old_link,
  old_strat;
int umbLink;                    /* This variable holds the current state of the UMB link */

/* This array will contain the memory blocks that the new program can't use */
int allocatedBlocks = 0;
WORD *block;

int loadfix_flag;               /* Flag: are we processing LOADFIX or LOADHIGH? */
int upper_flag;                 /* Flag: should the program be loaded high? */

/* UMB region info */
int umbRegions = 0;             /* How many UMB regions are there? */

struct UMBREGION
{
  WORD start;                   /* start of the region */
  WORD end;                     /* end of the region */
  WORD minSize;                 /* minimum free size, given by the L switch */
  int access;                   /* does the program have access to this region? */
}
 *umbRegion;

/* functions... */
//int loadhigh(char *first, char *rest);
//int cmd_loadfix(char *first, char *rest);
int findUMBRegions(void);
int parseArgs(char *cmdline, char **fnam, char **rest);
void lh_error(int errcode);
int lh_lf(char *c);
int loadhigh_prepare(void);
int loadfix_prepare(void);
int initialise(void);
void cleanup(void);

enum error_codes
{
  err_help = -1, OK,
  err_silent = -2,

  /* These error codes are more or less DOS-compatible */
  err_file_not_found = 2, err_mcb_chain = 7,
  err_out_of_memory = 8, err_invalid_parms = 87
};

/* error message strings */
//#define REGION_WARNING  "LOADHIGH: Illegal memory region %d - ignored\n"
//#define BADUSAGE        "invalid command line"
//#define BADMCBCHAIN     "MCB chain corrupt, or MS-DOS incompatible system"

