dnl $Id$
dnl config.m4 for extension mydebug

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(mydebug, for mydebug support,
dnl Make sure that the comment is aligned:
dnl [  --with-mydebug             Include mydebug support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(mydebug, whether to enable mydebug support,
Make sure that the comment is aligned:
[  --enable-mydebug           Enable mydebug support])

if test "$PHP_MYDEBUG" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-mydebug -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/mydebug.h"  # you most likely want to change this
  dnl if test -r $PHP_MYDEBUG/$SEARCH_FOR; then # path given as parameter
  dnl   MYDEBUG_DIR=$PHP_MYDEBUG
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for mydebug files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MYDEBUG_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MYDEBUG_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the mydebug distribution])
  dnl fi

  dnl # --with-mydebug -> add include path
  dnl PHP_ADD_INCLUDE($MYDEBUG_DIR/include)

  dnl # --with-mydebug -> check for lib and symbol presence
  dnl LIBNAME=mydebug # you may want to change this
  dnl LIBSYMBOL=mydebug # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MYDEBUG_DIR/$PHP_LIBDIR, MYDEBUG_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MYDEBUGLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong mydebug lib version or lib not found])
  dnl ],[
  dnl   -L$MYDEBUG_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MYDEBUG_SHARED_LIBADD)

  PHP_NEW_EXTENSION(mydebug, mydebug.c, $ext_shared)
fi
if test -z "$PHP_DEBUG"; then   
    AC_ARG_ENABLE(debug,  
    [   --enable-debug          compile with debugging symbols],[  
        PHP_DEBUG=$enableval  
    ],[ PHP_DEBUG=no  
    ])  
fi 
