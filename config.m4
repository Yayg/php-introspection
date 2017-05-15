dnl $Id$
dnl config.m4 for extension sqreen

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(sqreen, for sqreen support,
dnl Make sure that the comment is aligned:
dnl [  --with-sqreen             Include sqreen support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(sqreen, whether to enable sqreen support,
Make sure that the comment is aligned:
[  --enable-sqreen           Enable sqreen support])

if test "$PHP_SQREEN" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-sqreen -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/sqreen.h"  # you most likely want to change this
  dnl if test -r $PHP_SQREEN/$SEARCH_FOR; then # path given as parameter
  dnl   SQREEN_DIR=$PHP_SQREEN
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for sqreen files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       SQREEN_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$SQREEN_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the sqreen distribution])
  dnl fi

  dnl # --with-sqreen -> add include path
  dnl PHP_ADD_INCLUDE($SQREEN_DIR/include)

  dnl # --with-sqreen -> check for lib and symbol presence
  dnl LIBNAME=sqreen # you may want to change this
  dnl LIBSYMBOL=sqreen # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $SQREEN_DIR/$PHP_LIBDIR, SQREEN_SHARED_LIBADD)
  AC_DEFINE(HAVE_SQREENLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong sqreen lib version or lib not found])
  dnl ],[
  dnl   -L$SQREEN_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(SQREEN_SHARED_LIBADD)

  PHP_NEW_EXTENSION(sqreen, sqreen.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
