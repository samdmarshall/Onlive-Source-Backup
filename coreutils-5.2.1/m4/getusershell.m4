# getusershell.m4 serial 2
dnl Copyright (C) 2002, 2003 Free Software Foundation, Inc.
dnl This file is free software, distributed under the terms of the GNU
dnl General Public License.  As a special exception to the GNU General
dnl Public License, this file may be distributed as part of a program
dnl that contains a configuration script generated by Autoconf, under
dnl the same distribution terms as the rest of that program.

AC_DEFUN([gl_FUNC_GETUSERSHELL],
[
  AC_REPLACE_FUNCS(getusershell)
  if test $ac_cv_func_getusershell = no; then
    gl_PREREQ_GETUSERSHELL
  fi
])

# Prerequisites of lib/getusershell.c.
AC_DEFUN([gl_PREREQ_GETUSERSHELL], [
  AC_REQUIRE([AC_HEADER_STDC])
  AC_CHECK_FUNCS_ONCE(isascii)
])