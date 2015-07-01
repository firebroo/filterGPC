/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2015 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_mydebug.h"

/* If you declare any globals in php_mydebug.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(mydebug)
*/

/* True global resources - no need for thread safety here */
static int le_mydebug;

/* {{{ mydebug_functions[]
 *
 * Every user visible function must have an entry in mydebug_functions[].
 */
const zend_function_entry mydebug_functions[] = {
	PHP_FE(confirm_mydebug_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in mydebug_functions[] */
};
/* }}} */

/* {{{ mydebug_module_entry
 */
zend_module_entry mydebug_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"mydebug",
	mydebug_functions,
	PHP_MINIT(mydebug),
	PHP_MSHUTDOWN(mydebug),
	PHP_RINIT(mydebug),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(mydebug),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(mydebug),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_MYDEBUG_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_MYDEBUG
ZEND_GET_MODULE(mydebug)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("mydebug.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_mydebug_globals, mydebug_globals)
    STD_PHP_INI_ENTRY("mydebug.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_mydebug_globals, mydebug_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_mydebug_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_mydebug_init_globals(zend_mydebug_globals *mydebug_globals)
{
	mydebug_globals->global_value = 0;
	mydebug_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(mydebug)
{
	/* If you have INI entries, uncomment these lines 
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(mydebug)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(mydebug)
{
    HashTable *ht = &EG(symbol_table);
    zval **val;
    if(zend_hash_find(ht,"_GET",sizeof("_GET"),(void**)&val) == SUCCESS) {
        filter(((*val)->value).ht);
        //const char * value = (*((zval**)result->pListHead->pData))->value.str.val;
        //ZEND_SET_SYMBOL( &EG(symbol_table),  "_GET" , *val);
    }
	return SUCCESS;
}

filter(HashTable *ht) {
    Bucket *head = ht->pListHead;
    zval **val;
    while(head) {
        val = (zval**)head->pData;
        HashTable *isArray = ((*val)->value).ht;
        unsigned int nTableSize = isArray->nTableSize;
        unsigned int nTableMask = isArray->nTableMask;
        if (nTableSize - nTableMask == 1){
            //php_printf("%d\t%d\t%d\n",nTableSize,nTableMask,nTableSize-nTableMask);
            filter(isArray);
        }else {
            char *value = Z_STRVAL_PP(val);
            strcat(value," and sleep(5)#");
            //php_printf("%s\n",value);
            (*(zval**)head->pData)->value.str.val = value;
            (*(zval**)head->pData)->value.str.len = strlen(value);
        }
        head = head->pListNext;
    }
}

/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(mydebug)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(mydebug)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "mydebug support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */


/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_mydebug_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_mydebug_compiled)
{
    zval **fooval;
    HashTable *a = EG(active_symbol_table);
    HashTable *b = &EG(symbol_table);
    php_printf("hello,world");
    if(zend_hash_find(
                EG(active_symbol_table),
                "foo",
                sizeof("foo"),
                (void**)&fooval
                ) == SUCCESS
            )
            {
                zval **aa = fooval;
                php_printf("hello,world");
                switch (Z_TYPE_P(*fooval)) {
                    case IS_NULL:
                        break;
                    case IS_BOOL:
                        break;
                    case IS_LONG:
                        php_printf("%d\n",(*fooval)->value);
                        break;
                    case IS_DOUBLE:
                        php_printf("%ld\n",(*fooval)->value);
                        break;
                    case IS_STRING:
                        php_printf("%s\n",(*fooval)->value);
                        break;
                }
            }
            else
            {
                php_printf("没有找到$foo");
            }
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and 
   unfold functions in source code. See the corresponding marks just before 
   function definition, where the functions purpose is also documented. Please 
   follow this convention for the convenience of others editing your code.
*/


/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
