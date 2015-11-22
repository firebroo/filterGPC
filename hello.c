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

#include<sys/types.h>
#include<regex.h>
#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_hello.h"

/* If you declare any globals in php_hello.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(hello)
*/

/* True global resources - no need for thread safety here */
static int le_hello;

/* {{{ hello_functions[]
 *
 * Every user visible function must have an entry in hello_functions[].
 */
const zend_function_entry hello_functions[] = {
	PHP_FE(confirm_hello_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in hello_functions[] */
};
/* }}} */

/* {{{ hello_module_entry
 */
zend_module_entry hello_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"hello",
	hello_functions,
	PHP_MINIT(hello),
	PHP_MSHUTDOWN(hello),
	PHP_RINIT(hello),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(hello),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(hello),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_HELLO_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_HELLO
ZEND_GET_MODULE(hello)
#endif

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("hello.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_hello_globals, hello_globals)
    STD_PHP_INI_ENTRY("hello.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_hello_globals, hello_globals)
PHP_INI_END()
*/
/* }}} */

/* {{{ php_hello_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_hello_init_globals(zend_hello_globals *hello_globals)
{
	hello_globals->global_value = 0;
	hello_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(hello)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(hello)
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

PHP_RINIT_FUNCTION(hello)
{
    HashTable *ht = &EG(symbol_table);
    zval **val;
    if(zend_hash_find(ht,"_GET",sizeof("_GET"),(void**)&val) == SUCCESS) {
        recurse_filter(((*val)->value).ht);
        //const char * value = (*((zval**)result->pListHead->pData))->value.str.val;
        //ZEND_SET_SYMBOL( &EG(symbol_table),  "_GET" , *val);
    }
    if(zend_hash_find(ht,"_POST",sizeof("_POST"),(void**)&val) == SUCCESS) {
        recurse_filter(((*val)->value).ht);
    }
    if(zend_hash_find(ht,"_COOKIE",sizeof("_COOKIE"),(void**)&val) == SUCCESS) {
        recurse_filter(((*val)->value).ht);
    }
    return SUCCESS;
}

int recurse_filter(HashTable *ht) {
    Bucket *head = ht->pListHead;
    zval **val;
    while(head) {
        val = (zval**)head->pData;
        HashTable *isArray = ((*val)->value).ht;
        unsigned int nTableSize = isArray->nTableSize;
        unsigned int nTableMask = isArray->nTableMask;
        if (nTableSize - nTableMask == 1){
            recurse_filter(isArray);
        }else {
            char *value = Z_STRVAL_PP(val);
	        regex_filter(&value);
            (*(zval**)head->pData)->value.str.val = value;
            (*(zval**)head->pData)->value.str.len = strlen(value);
        }
        head = head->pListNext;
    }
	return SUCCESS;
}


int regex_filter(char **value){
	int status, i;
    int cflags = REG_EXTENDED | REG_ICASE;
	regmatch_t pmatch[1];
	const size_t nmatch = 1;
    regex_t reg;
    //it's a sample.
    const char *pattern = "shell_exec|passthru|system|exec|((\\.\\.[/\\])+)|select|sleep|benchmark|and|or|between";

    regcomp(&reg, pattern, cflags);
    status = regexec(&reg, *value, nmatch, pmatch, 0);
    if (status == 0) {
        strcpy(*value,"attack");
    }
    regfree(&reg);
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(hello)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(hello)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "hello support", "enabled");
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
/* {{{ proto string confirm_hello_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_hello_compiled)
{
	char *arg = NULL;
	int arg_len, len;
	char *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	len = spprintf(&strg, 0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "hello", arg);
	RETURN_STRINGL(strg, len, 0);
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
