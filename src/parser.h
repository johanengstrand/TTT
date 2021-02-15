#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#include "pages.h"
#include "shared.h"
#include "errors.h"
#include "html_parser.h"

page_collection_t *parser_get_page_collection(const char *data, size_t size);