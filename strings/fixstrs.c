/*
This program will read in the strings.txt file and create two files:

strings.dat - contains all the strings. will be appended on the end
              of the exe file.

strings.h - include file that contains all the defines.

*/

#include <stdio.h>


#define LOOKING_FOR_START  1
#define GETTING_STRING     2

#define MAXSTRINGS       256

#define STRINGS_ID "FreeDOS"


void main(void)
{
  FILE *in, *dat, *inc;
  char temp[128];

  struct
  {
    unsigned index;
    unsigned size;
  } ptr[MAXSTRINGS];
  unsigned offset;
  unsigned datsize;
  unsigned state = LOOKING_FOR_START;
  unsigned define_count = 0,x;
  int firstline;
  char *id = STRINGS_ID;


  if ((in = fopen("strings.txt","r")) == NULL)
  {
    perror("opening strings.txt");
    return;
  }

  if ((dat = fopen("strings.dat","wb")) == NULL)
  {
    fclose(in);
    perror("creating strings.dat");
    return;
  }

  if ((inc = fopen("strings.h","w")) == NULL)
  {
    fclose(in);
    fclose(dat);
    perror("creating strings.h");
    return;
  }

  while (fgets(temp,sizeof(temp),in))
  {
    switch (state)
    {
      case LOOKING_FOR_START:
        if (*temp == ':')
        {
          while (*temp && temp[strlen(temp)-1] < ' ') temp[strlen(temp)-1] = '\0';
          state = GETTING_STRING;
          ptr[define_count].index = ftell(dat);
          if (define_count)
          {
            ptr[define_count-1].size = ptr[define_count].index - ptr[define_count-1].index;
          }
//          printf("%-40s  0x%02X   @ %ld\n",temp+1,define_count+1,index[define_count]);
//          fprintf(inc,"#define  %-40s  0x%02X\n",temp+1,define_count+1);
          define_count++;
          firstline = 1;
        }
        break;

      case GETTING_STRING:
        while (*temp && temp[strlen(temp)-1] < ' ') temp[strlen(temp)-1] = '\0';
        if ((*temp == '.' || *temp == ',') && (temp[1] == '\0'))
        {
          if (*temp == '.') fputc('\n',dat);
          fputc('\0',dat);
          state = LOOKING_FOR_START;
        }
        else
        {
          if (!firstline) fputc('\n',dat);
          firstline = 0;
          fputs(temp,dat);
        }
    }
  }

  ptr[define_count].size = ftell(dat) - ptr[define_count].index;
//  datsize = ftell(dat);

  fprintf(inc,"#define  STRINGS_ID         \"%s\"\n\n",STRINGS_ID);
  fprintf(inc,"#define  NUMBER_OF_STRINGS  0x%02X\n\n",define_count);
  fprintf(inc,"#define  SIZE_OF_STRINGS    0x%04X\n\n",ftell(dat));

  rewind(in);
  rewind(dat);

  fputs(id,dat);

//  fwrite(&define_count,sizeof(define_count),1,dat);

//  offset = define_count * sizeof(unsigned)
//           + sizeof(define_count)
//           + sizeof(datsize);

//  datsize += offset;

//  for (x=0; x < define_count; x++)
//    index[x] += offset;

  fwrite(ptr,sizeof(ptr[0]),define_count,dat);

//  fwrite(&datsize,sizeof(datsize),1,dat);

  define_count = 0;

  while (fgets(temp,sizeof(temp),in))
  {
    switch (state)
    {
      case LOOKING_FOR_START:
        if (*temp == ':')
        {
          while (*temp && temp[strlen(temp)-1] < ' ') temp[strlen(temp)-1] = '\0';
          state = GETTING_STRING;
//          index[define_count] = ftell(dat);
          fprintf(inc,"#define  %-34s 0x%02X /* @ 0x%04X */\n",temp+1,define_count,ptr[define_count].index);
          define_count++;
          firstline = 1;
        }
        break;

      case GETTING_STRING:
        while (*temp && temp[strlen(temp)-1] < ' ') temp[strlen(temp)-1] = '\0';
        if ((*temp == '.' || *temp == ',') && (temp[1] == '\0'))
        {
          if (*temp == '.') fputc('\n',dat);
          fputc('\0',dat);
          state = LOOKING_FOR_START;
        }
        else
        {
          if (!firstline) fputc('\n',dat);
          firstline = 0;
          fputs(temp,dat);
        }
    }
  }

  fclose(in);
  fclose(dat);
  fclose(inc);
}
