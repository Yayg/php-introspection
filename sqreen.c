/*
   +----------------------------------------------------------------------+
   | PHP Version 7                                                        |
   +----------------------------------------------------------------------+
   | Copyright (c) 1997-2017 The PHP Group                                |
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

#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <link.h>

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_sqreen.h"

/* If you declare any globals in php_sqreen.h uncomment this:
   ZEND_DECLARE_MODULE_GLOBALS(sqreen)
   */

/* True global resources - no need for thread safety here */
static int le_sqreen;

/* {{{ PHP_INI
*/
/* Remove comments and fill if you need to have entries in php.ini
   PHP_INI_BEGIN()
   STD_PHP_INI_ENTRY("sqreen.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_sqreen_globals, sqreen_globals)
   STD_PHP_INI_ENTRY("sqreen.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_sqreen_globals, sqreen_globals)
   PHP_INI_END()
   */
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_sqreen_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(confirm_sqreen_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "sqreen", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
   */

PHP_FUNCTION(fopen_sqreen)
{
}


PHP_FUNCTION(sqreenOn)
{
	zend_function *func, *dummy_func;
////char *old_fname, *new_fname, *old_fname, *new_fname;
	char *old_fname = "fopen";
	char *new_fname = "fopen_sqreen";
	int old_fname_len = strlen(old_fname);
	int new_fname_len = strlen(new_fname);
	int i;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ss",
								&old_fname, &old_fname_len, &new_fname, &new_fname_len) == FAILURE) {
		return;
	}

  if (zend_hash_find(EG(function_table), old_fname, old_fname_len + 1, (void **) &func) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s(%s, %s) failed: %s does not exist!",
						get_active_function_name(TSRMLS_C),
						old_fname,  new_fname, old_fname);
		RETVAL_FALSE;
		goto sqreenOn_end;
	}

	if (func->type != ZEND_USER_FUNCTION) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s(%s, %s) failed: \"%s\" is an internal function!",
							get_active_function_name(TSRMLS_C),
							old_fname,  new_fname, old_fname);
		RETVAL_FALSE;
		goto sqreenOn_end;
	}

	if (zend_hash_find(EG(function_table), new_fname, new_fname_len + 1, (void **) &dummy_func) == SUCCESS) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s(%s, %s) failed: %s already exists!",
							get_active_function_name(TSRMLS_C),
							old_fname,  new_fname, new_fname);
		RETVAL_FALSE;
		goto sqreenOn_end;
	}

	if (zend_hash_add(EG(function_table), new_fname, new_fname_len + 1, func, sizeof(zend_function), NULL)
			== FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s() failed to insert %s into EG(function_table)",
							get_active_function_name(TSRMLS_C), new_fname);
		RETVAL_FALSE;
		goto sqreenOn_end;
	}

	/* NOTE(cjiang): This line of code is our fix for APD's rename_function. */
	function_add_ref(func);

	if (zend_hash_del(EG(function_table), old_fname, old_fname_len + 1) == FAILURE) {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "%s() failed to remove %s from function table",\
							get_active_function_name(TSRMLS_C), old_fname);

		zend_hash_del(EG(function_table), new_fname, new_fname_len + 1);
		RETVAL_FALSE;
		goto sqreenOn_end;
	}
	RETVAL_TRUE;

sqreenOn_end:
	free(old_fname);
	free(new_fname);

}

PHP_FUNCTION(sqreenOff)
{
	RETURN_STRING("Hello world");
}



/* {{{ PHP_MINIT_FUNCTION
*/
PHP_MINIT_FUNCTION(sqreen)
{
	/* If you have INI entries, uncomment these lines
	   REGISTER_INI_ENTRIES();
	   */
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
*/
PHP_MSHUTDOWN_FUNCTION(sqreen)
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
PHP_RINIT_FUNCTION(sqreen)
{
#if defined(COMPILE_DL_SQREEN) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
*/
PHP_RSHUTDOWN_FUNCTION(sqreen)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
*/
PHP_MINFO_FUNCTION(sqreen)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "sqreen support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	   DISPLAY_INI_ENTRIES();
	   */
}
/* }}} */

/* {{{ sqreen_functions[]
 *
 * Every user visible function must have an entry in sqreen_functions[].
 */
const zend_function_entry sqreen_functions[] = {
	PHP_FE(confirm_sqreen_compiled,	NULL)		/* For testing, remove later. */
		PHP_FE(sqreenOn, NULL)
		PHP_FE(fopen_sqreen, NULL)
		PHP_FE(sqreenOff, NULL)
		PHP_FE_END	/* Must be the last line in sqreen_functions[] */
};
/* }}} */

/* {{{ sqreen_module_entry
*/
zend_module_entry sqreen_module_entry = {
	STANDARD_MODULE_HEADER,
	"sqreen",
	sqreen_functions,
	PHP_MINIT(sqreen),
	PHP_MSHUTDOWN(sqreen),
	PHP_RINIT(sqreen),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(sqreen),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(sqreen),
	PHP_SQREEN_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_SQREEN
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(sqreen)
#endif

	/*
	 * Local variables:
	 * tab-width: 4
	 * c-basic-offset: 4
	 * End:
	 * vim600: noet sw=4 ts=4 fdm=marker
	 * vim<600: noet sw=4 ts=4
	 */
