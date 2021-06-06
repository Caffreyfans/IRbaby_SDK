/*
 * @Author: Caffreyfans
 * @Date: 2021-06-04 22:03:39
 * @LastEditTime: 2021-06-05 21:47:47
 * @Description: coreHTTP interface porting
 */
#ifndef INTERFACE_H_
#define INTERFACE_H_
#include "transport_interface.h"
#include "core_http_client.h"

struct NetworkContext {
    int fd; // socket discriptor
};

typedef struct {
    char *url;
    char *method;
    char *payload;
    size_t payload_len;
    char *schema;
    char *host;
    char *port;
    char *path;
    NetworkContext_t context;
    HTTPRequestHeaders_t header;
    TransportInterface_t interface;
    HTTPResponse_t response;
} HTTPClient;


uint8_t http_client_init(HTTPClient *client);

uint8_t http_client_close(HTTPClient *client);

uint8_t http_client_send(HTTPClient *client);

uint8_t http_url_parse(HTTPClient *client);
/**
 * @description: open socket transport
 * @param {NetworkContext_t} *context, the pointer of NetworkContext_t
 * @param {const char} *url, http url
 * @return
 *          - OK, open socket transport success
 *          - FAIL, open socket transport failed
 */
uint8_t open_transport(HTTPClient *client);

/**
 * @description: close transport souces
 * @param {NetworkContext_t} *context, the pointer of NetworkContext_t
 * @return
 *          - OK, close socket transport sources success
 *          - FAIL, close socket transport failed
 */
uint8_t close_transport(NetworkContext_t *context);

int32_t core_http_send(NetworkContext_t *pNetworkContext, const void *pBuffer, size_t bytesToSend);

int32_t core_http_recv(NetworkContext_t *pNetworkContext, void *pBuffer, size_t bytesToRecv);
#endif // INTERFACE_H_