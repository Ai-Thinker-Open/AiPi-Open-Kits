#if 0
#include <stdlib.h>
#include <stdbool.h>
#include <aiio_system.h>
#include <aiio_http_client.h>

#include "unity.h"
#include "test_utils.h"

#define HOST  "httpbin.org"
#define USERNAME  "user"
#define PASSWORD  "challenge"

TEST_CASE("Test in common case: Only URL and hostname are specified.", "[AIIO HTTP CLIENT]")
{
    aiio_http_client_config_t config_incorrect = {0};

    test_case_uses_tcpip();

    aiio_http_client_handle_t client = aiio_http_client_init(&config_incorrect);
    TEST_ASSERT(client == NULL);

    aiio_http_client_config_t config_with_url = {
        .url = "http://httpbin.org/get",
    };
    client = aiio_http_client_init(&config_with_url);
    TEST_ASSERT(client != NULL);
    TEST_ASSERT(aiio_http_client_cleanup(client) == AIIO_OK);


    aiio_http_client_config_t config_with_hostname_path = {
        .host = HOST,
        .path = "/get",
    };
    client = aiio_http_client_init(&config_with_hostname_path);
    TEST_ASSERT(client != NULL);
    TEST_ASSERT(aiio_http_client_cleanup(client) == AIIO_OK);
}

TEST_CASE("Get username and password after initialization.", "[AIIO HTTP CLIENT]")
{
    aiio_http_client_config_t config_with_auth = {
        .host = HOST,
        .path = "/",
        .username = USERNAME,
        .password = PASSWORD
    };
    char *value = NULL;
    aiio_http_client_handle_t client = aiio_http_client_init(&config_with_auth);
    TEST_ASSERT_NOT_NULL(client);
    // Test with username
    aiio_err_t r = aiio_http_client_get_username(client, &value);
    TEST_ASSERT_EQUAL(AIIO_OK, r);
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING(USERNAME, value);
    // Test with password
    value = NULL;
    r = aiio_http_client_get_password(client, &value);
    TEST_ASSERT_EQUAL(AIIO_OK, r);
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING(PASSWORD, value);
    aiio_http_client_cleanup(client);
}

/**
 * Test case to test that, the aiio_http_client_set_url won't drop username and password
 * when pass a path "/abc" for url.
 **/
TEST_CASE("Username is unmodified when we change to new path", "[AIIO HTTP CLIENT]")
{
    aiio_http_client_config_t config_with_auth = {
        .host = HOST,
        .path = "/",
        .username = USERNAME,
        .password = PASSWORD
    };
    char *value = NULL;
    aiio_http_client_handle_t client = aiio_http_client_init(&config_with_auth);
    TEST_ASSERT_NOT_NULL(client);
    aiio_err_t r = aiio_http_client_get_username(client, &value);
    TEST_ASSERT_EQUAL(AIIO_OK, r);
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING(USERNAME, value);
    aiio_http_client_set_url(client, "/something-else/");
    r = aiio_http_client_get_username(client, &value);
    TEST_ASSERT_EQUAL(AIIO_OK, r);
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING(USERNAME, value);
    aiio_http_client_cleanup(client);
}

/**
 * Test case to test that, the aiio_http_client_set_url do not reset the auth credentials
 * Explicit APIs aiio_http_client_set_username and aiio_http_client_set_password are used to change
 * the auth credentials
 **/
TEST_CASE("Username and password will not reset if new absolute URL doesnot specify auth credentials.", "[AIIO HTTP CLIENT]")
{
    aiio_http_client_config_t config_with_auth = {
        .host = HOST,
        .path = "/",
        .username = USERNAME,
        .password = PASSWORD
    };
    char *value = NULL;
    aiio_http_client_handle_t client = aiio_http_client_init(&config_with_auth);
    TEST_ASSERT_NOT_NULL(client);
    aiio_err_t r = aiio_http_client_get_username(client, &value);
    TEST_ASSERT_EQUAL(AIIO_OK, r);
    TEST_ASSERT_NOT_NULL(value);
    TEST_ASSERT_EQUAL_STRING(USERNAME, value);
    aiio_http_client_set_url(client, "http://" HOST "/get");
    aiio_http_client_set_username(client, value);
    // aiio_http_client_set_username sets new username and thus invalidates the original one
    // which we still reference in the local variable `value` (better forget it)
    value = NULL;
    aiio_http_client_set_password(client, USERNAME);
    //checks if username is set or not
    r = aiio_http_client_get_username(client, &value);
    TEST_ASSERT_EQUAL(AIIO_OK, r);
    //If username is set then value should not be NULL
    TEST_ASSERT_NOT_NULL(value);
    //checks if password is set or not
    r = aiio_http_client_get_password(client, &value);
    TEST_ASSERT_EQUAL(AIIO_OK, r);
    //If password is set then value should not be NULL
    TEST_ASSERT_NOT_NULL(value);
    aiio_http_client_cleanup(client);
}

/**
 * Test case to verify that, aiio_http_client_init() should return NULL if configuration has url with empty hostname.
 **/
TEST_CASE("aiio_http_client_init() should return NULL if configured with wrong url", "[AIIO HTTP CLIENT]")
{
    aiio_http_client_config_t config = {
        .url = "//httpbin.org/post",
    };
    aiio_http_client_handle_t client = aiio_http_client_init(&config);
    TEST_ASSERT_NULL(client);
    aiio_http_client_cleanup(client);
}
#endif
