/*
 * ERROR.C -- Error reporting functions
 *
 * 07/12/98 (Rob Lake)
 *      started
 *
 * 27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 * - added config.h include
 *
 *  1998/08/10 ska
 *  - added "long line in batchfile", "batchfile vanished"
 *
 * 2000/01/15 ska
 * add: error functions for CTTY implementation
 */

#include "config.h"

#include <stdio.h>
#include "command.h"
#include "strings.h"

void error_invalid_switch(char c)
{
  displayString(TEXT_ERROR_INVALID_SWITCH, c);
}

void error_invalid_lswitch(const char * const str)
{
  displayString(TEXT_ERROR_INVALID_LSWITCH, str);
}

void error_too_many_parameters(const char * const s)
{
  displayString(TEXT_ERROR_TOO_MANY_PARAMETERS_STR, s);
}

void error_path_not_found(void)
{
  displayString(TEXT_ERROR_PATH_NOT_FOUND);
}

void error_file_not_found(void)
{
  displayString(TEXT_ERROR_FILE_NOT_FOUND);
}

void error_sfile_not_found(const char * const f)
{
  displayString(TEXT_ERROR_FILE_NOT_FOUND, f);
}

void error_req_param_missing(void)
{
  displayString(TEXT_ERROR_REQ_PARAM_MISSING);
}

void error_invalid_drive(void)
{
  displayString(TEXT_ERROR_INVALID_DRIVE);
}

void error_bad_command(void)
{
  displayString(TEXT_ERROR_BADCOMMAND);
}

void error_no_pipe(void)
{
  displayString(TEXT_ERROR_CANNOTPIPE);
}

void error_out_of_memory(void)
{
  displayString(TEXT_ERROR_OUT_OF_MEMORY);
}

void error_syntax(const char * const s)
{
  if (s)
    displayString(TEXT_ERROR_SYNTAX_STR, s);
  else
    displayString(TEXT_ERROR_SYNTAX);
}

void error_nothin_to_do(void)
{ displayString(TEXT_ERROR_NOTHING_TO_DO);
}

void error_selfcopy(const char * const s)
{ displayString(TEXT_ERROR_SELFCOPY, s);
}

void error_long_batchline(const char * const fnam, long linenr)
{
  displayString(TEXT_ERROR_LONG_LINE_BATCHFILE, linenr, fnam);
}

void error_bfile_vanished(const char * const fnam)
{
  displayString(TEXT_ERROR_BFILE_VANISHED, fnam);
}

void error_bfile_no_such_label(const char * const fnam, const char * const label)
{
  displayString(TEXT_ERROR_BFILE_LABEL, fnam, label);
}

void error_env(int err_no, const char * const var)
{
  switch(err_no) {
  case -1:
    displayString(TEXT_ERROR_SET_ENV_VAR, var);
    break;
    case -2:
    displayString(TEXT_ERROR_ENV_VAR_NOT_FOUND, var);
    break;
    case -4:
      displayString(TEXT_ERROR_NO_ENVIRONMENT);
    case 0:
      break;
    default:
      displayString(TEXT_ERROR_SYNTAX);
      break;
    }
}

void error_env_var(const char * const var)
{
  displayString(TEXT_ERROR_SET_ENV_VAR, var);
}

void error_env_var_not_found(const char * const var)
{
  displayString(TEXT_ERROR_ENV_VAR_NOT_FOUND, var);
}

void error_filename_too_long(const char * const fname)
{
  displayString(TEXT_ERROR_FILENAME_TOO_LONG, fname);
}

void error_command_too_long(void)
{
  displayString(TEXT_ERROR_COMMAND_TOO_LONG);
}

void error_line_too_long(void)
{
  displayString(TEXT_ERROR_LINE_TOO_LONG);
}

void error_tempfile(void)
{
  displayString(TEXT_ERROR_TEMPFILE);
}

void error_close_quote(int quote)
{
  displayString(TEXT_ERROR_CLOSE_QUOTE, quote);
}

void error_illformed_option(const char * const s)
{
  displayString(TEXT_ERROR_ILLFORMED_OPTION, s);
}
void error_opt_arg(const char * const  str)
{
  displayString(TEXT_ERROR_OPT_ARG, str);
}

void error_opt_noarg(const char * const  str)
{
  displayString(TEXT_ERROR_OPT_NOARG, str);
}

void error_leading_plus(void)
{ displayString(TEXT_ERROR_LEADING_PLUS);
}

void error_trailing_plus(void)
{ displayString(TEXT_ERROR_TRAILING_PLUS);
}

void error_open_file(const char * const fnam)
{ displayString(TEXT_ERROR_OPEN_FILE, fnam);
}

void error_read_file(const char * const fnam)
{ displayString(TEXT_ERROR_READ_FILE, fnam);
}

void error_write_file(const char * const fnam)
{ displayString(TEXT_ERROR_WRITE_FILE, fnam);
}

void error_copy(void)
{ displayString(TEXT_ERROR_COPY);
}

void error_nothing_to_do(void)
{ displayString(TEXT_ERROR_NOTHING_TO_DO);
}

void dispCopy(const char src[], const char dst[], int append)
{ displayString(TEXT_MSG_COPYING, src, append? "=>>": "=>", dst);
}

void error_invalid_number(const char * const s)
{ displayString(TEXT_INVALID_NUMBER, s);
}

void error_init_fully_qualified(const char * const s)
{ displayString(TEXT_INIT_FULLY_QUALIFIED, s);
}

void error_corrupt_command_line(void)
{ displayString(TEXT_CORRUPT_COMMAND_LINE);
}

void error_quoted_c_k(void)
{ displayString(TEXT_QUOTED_C_OR_K);
}

void error_ctty_excluded(void)
{ displayString(TEXT_CTTY_NOTIMPLEMENTED);
}

void error_l_notimplemented(void)
{ displayString(TEXT_ERROR_L_NOTIMPLEMENTED);
}

void error_u_notimplemented(void)
{ displayString(TEXT_ERROR_U_NOTIMPLEMENTED);
}

void error_restore_session(void)
{ displayString(TEXT_ERROR_RESTORE_SESSION);
}

void error_save_session(void)
{ displayString(TEXT_ERROR_SAVE_SESSION);
}

void error_no_rw_device(const char * const devname)
{ displayString(TEXT_ERROR_NORW_DEVICE, devname);
}

void error_ctty_dup(const char * const devname)
{ displayString(TEXT_ERROR_CTTY_DUP, devname);
}

//void newperror(unsigned error_id)
//{
//
//}
