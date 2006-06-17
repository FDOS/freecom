echo #define GETOPT_JUST_VERSION_NUMBER >getoptx.c
type getopt.src >>getoptx.c
echo #define GETOPT_LONG_NO_FLAG_MEMBER >getoptx1.c
type getoptl.c >>getoptx1.c
echo #define GETOPT_LONG_NO_FLAG_MEMBER >getoptx2.c
type getoptlo.c >>getoptx2.c
echo #define SUPPORT_UNC_PATH >dfnumerg.c
type dfnmerge.c >>dfnumerg.c
echo #define SUPPORT_UNC_PATH >dfnuspli.c
type dfnsplit.c >>dfnuspli.c
echo #define SUPPORT_UNC_PATH >dfnuexpa.c
type dfnexpan.c >>dfnuexpa.c
echo #define SUPPORT_UNC_PATH >dfnufull.c
type dfnfullp.c >>dfnufull.c
echo #define SUPPORT_UNC_PATH >dfnuful2.c
type dfnfull2.c >>dfnuful2.c
echo #define SUPPORT_UNC_PATH >dfnusque.c
type dfnsquee.c >>dfnusque.c
echo #define SUPPORT_UNC_PATH >dfnumatc.c
type dfnmatch.c >>dfnumatc.c
echo #define SUPPORT_UNC_PATH >dfnumat2.c
type dfnmatc2.c >>dfnumat2.c
