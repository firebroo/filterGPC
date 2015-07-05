dnl $Id$
dnl config.m4 for extension hello

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(hello, for hello support,
dnl Make sure that the comment is aligned:
dnl [  --with-hello             Include hello support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(hello, whether to enable hello support,
Make sure that the comment is aligned:
[  --enable-hello           Enable hello support])

if test "$PHP_HELLO" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-hello -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/hello.h"  # you most likely want to change this
  dnl if test -r $PHP_HELLO/$SEARCH_FOR; then # path given as parameter
  dnl   HELLO_DIR=$PHP_HELLO
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for hello files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       HELLO_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$HELLO_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the hello distribution])
  dnl fi

  dnl # --with-hello -> add include path
  dnl PHP_ADD_INCLUDE($HELLO_DIR/include)

  dnl # --with-hello -> check for lib and symbol presence
  dnl LIBNAME=hello # you may want to change this
  dnl LIBSYMBOL=hello # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $HELLO_DIR/$PHP_LIBDIR, HELLO_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_HELLOLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong hello lib version or lib not found])
  dnl ],[
  dnl   -L$HELLO_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(HELLO_SHARED_LIBADD)

  PHP_NEW_EXTENSION(hello, hello.c, $ext_shared)
fi

if test -z "$PHP_DEBUG"; then   
    AC_ARG_ENABLE(debug,  
    [   --enable-debug          compile with debugging symbols],[  
        PHP_DEBUG=$enableval  
    ],[ PHP_DEBUG=no  
    ])  
fi  
