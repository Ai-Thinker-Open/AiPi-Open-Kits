#if 0
// #include "unity.h"

#include "aiio_transport.h"
#include "aiio_transport_tcp.h"
#include "aiio_transport_ssl.h"
#include "aiio_transport_ws.h"
#include "aiio_log.h"


TEST_CASE("tcp_transport: init and deinit transport list", "[tcp_transport][leaks=0]")
{
    aiio_transport_list_handle_t transport_list = aiio_transport_list_init();
    aiio_transport_handle_t tcp = aiio_transport_tcp_init();
    aiio_transport_list_add(transport_list, tcp, "tcp");
    TEST_ASSERT_EQUAL(AIIO_OK, aiio_transport_list_destroy(transport_list));
}

TEST_CASE("tcp_transport: using ssl transport separately", "[tcp_transport][leaks=0]")
{
    aiio_transport_handle_t h = aiio_transport_ssl_init();
    TEST_ASSERT_EQUAL(AIIO_OK, aiio_transport_destroy(h));
}

TEST_CASE("tcp_transport: using ws transport separately", "[tcp_transport][leaks=0]")
{
    aiio_transport_handle_t tcp = aiio_transport_tcp_init();
    aiio_transport_handle_t ws = aiio_transport_ws_init(tcp);
    TEST_ASSERT_EQUAL(AIIO_OK, aiio_transport_destroy(ws));
    TEST_ASSERT_EQUAL(AIIO_OK, aiio_transport_destroy(tcp));
}

TEST_CASE("transport: init and deinit multiple transport items", "[tcp_transport][leaks=0]")
{
    aiio_transport_list_handle_t transport_list = aiio_transport_list_init();
    aiio_transport_handle_t tcp = aiio_transport_tcp_init();
    aiio_transport_list_add(transport_list, tcp, "tcp");
    aiio_transport_handle_t ssl = aiio_transport_ssl_init();
    aiio_transport_list_add(transport_list, ssl, "ssl");
    aiio_transport_handle_t ws = aiio_transport_ws_init(tcp);
    aiio_transport_list_add(transport_list, ws, "ws");
    aiio_transport_handle_t wss = aiio_transport_ws_init(ssl);
    aiio_transport_list_add(transport_list, wss, "wss");
    TEST_ASSERT_EQUAL(AIIO_OK, aiio_transport_list_destroy(transport_list));
}
#endif
