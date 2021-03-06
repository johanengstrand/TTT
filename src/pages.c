#include "pages.h"

static page_t empty_page = {
    .id = -1,
    .prev_id = -1,
    .next_id = -1,
    .unix_date = -1,
    .title = NULL,
    .tokens = NULL,
    .last_token = NULL
};

page_t *page_create_empty() {
    page_t *page = calloc(1, sizeof(page_t));

    if (!page) {
        return NULL;
    }

    memcpy(page, &empty_page, sizeof(empty_page));
    return page;
}

page_token_t *page_token_create_empty() {
    page_token_t *token = calloc(1, sizeof(page_token_t));
    token->type = PAGE_TOKEN_TEXT;
    token->style.fg = PAGE_TOKEN_ATTR_WHITE,
           token->style.bg = PAGE_TOKEN_ATTR_BG_BLACK;
    token->style.extra = PAGE_TOKEN_ATTR_NONE;
    return token;
}

page_collection_t *page_collection_create(size_t size) {
    page_collection_t *collection = calloc(1, sizeof(page_collection_t));

    if (size == 0) {
        collection->pages = NULL;
    } else {
        collection->pages = calloc(size, sizeof(page_t *));
    }

    collection->size = size;
    return collection;
}

void page_token_append(page_t *page, page_token_t *token, bool inherit_style) {
    if (!page || !token) {
        return;
    }

    if (inherit_style) {
        token->style.bg = page->last_token->style.bg;
        token->style.fg = page->last_token->style.fg;
        token->style.extra = page->last_token->style.extra;
    }

    if (page->last_token) {
        page->last_token->next = token;
    } else {
        page->tokens = token;
    }

    page->last_token = token;
}

void page_collection_resize(page_collection_t *collection, size_t new_size) {
    if (!collection || collection->size == new_size) {
        return;
    }

    page_t **pages = realloc(collection->pages, sizeof(page_t *) * new_size);

    if (!pages) {
        return;
    }

    collection->pages = pages;
    collection->size = new_size;
}

/// @brief Checks if a page is empty
/// @param page the page to check if empty (must be fully initialized, e.g. using 'calloc()')
/// @return true if page contains only the default values
bool page_is_empty(page_t *page) {
    if (!page) {
        return true;
    }

    return memcmp(page, &empty_page, sizeof(empty_page)) == 0;
}

void page_tokens_print(page_t *page) {
    if (!page->tokens) {
        printf("Page contains no tokens\n\n");
        return;
    }

    page_token_t *cursor = page->tokens;

    while (cursor != page->last_token) {
        printf("| - type: %d\n", cursor->type);
        printf("| - text: %s\n", cursor->text);
        printf("| - length: %d\n", cursor->length);
        printf("| - href: %d\n", cursor->href);
        printf("| - fg: %d\n", cursor->style.fg);
        printf("| - bg: %d\n", cursor->style.bg);
        printf("| - extra: %d\n", cursor->style.extra);
        printf("| \n");
        cursor = cursor->next;
    }
}

void page_print(page_t *page) {
    printf("\nPAGE %d\n", page->id);
    printf("* id: %d\n", page->id);
    printf("* prev_id: %d\n", page->prev_id);
    printf("* next_id: %d\n", page->next_id);
    printf("* unix_date: %lu\n", page->unix_date);
    printf("* title: %s\n", page->title);
    printf("* tokens: \n");
    page_tokens_print(page);
}

void page_collection_print(page_collection_t *collection) {
    for (size_t i = 0; i < collection->size; i++) {
        page_print(collection->pages[i]);
    }
}

void page_token_destroy(page_token_t *token) {
    if (token->text != NULL) {
        free(token->text);
    }

    free(token);
}

void page_tokens_destroy(page_t *page) {
    if (page->tokens) {
        page_token_t *tmp;
        page_token_t *cursor = page->tokens;

        while (cursor != NULL) {
            tmp = cursor->next;
            page_token_destroy(cursor);
            cursor = tmp;
        }
    }
}

void page_destroy(page_t *page) {
    if (!page) {
        return;
    }

    page_tokens_destroy(page);
    free(page->title);
    free(page);
}

void page_collection_destroy(page_collection_t *collection) {
    for (size_t i = 0; i < collection->size; i++) {
        page_destroy(collection->pages[i]);
    }

    free(collection->pages);
    free(collection);
}
