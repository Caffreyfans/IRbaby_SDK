/*
 * @Author: Caffreyfans
 * @Date: 2021-06-04 22:12:44
 * @LastEditTime: 2021-08-24 23:13:36
 * @Description:
 */
#include "http_client.h"
#ifdef __linux__
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#else  // for mbedded device
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#endif  // __linux__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http_parser.h"

/* implemente this print debug message */
#define LOG(fmt, ...)
#ifndef LOG
#define LOG(fmt, ...) printf(fmt, ##__VA_ARGS__)
#endif  // LOG

#define CLIENT_OK 1
#define CLIENT_FAIL 0  // open transport failed

#define HTTP_DEFAULT_PORT "80"
#define HTTPS_DEFAULT_PORT "443"
#define HEADER_MAX_LEN 1024
#define RESPONSE_MAX_LEN 2048

static char *malloc_str(const char *str, size_t len) {
  if (len > 0) {
    char *dest = (char *)malloc(len + 1);
    if (dest == NULL) {
      return NULL;
    }
    memcpy(dest, str, len);
    dest[len] = '\0';
    return dest;
  }
  return NULL;
}

uint8_t http_url_parse(HTTPClient *client) {
  /* 1. parser url */
  struct http_parser_url purl;

  LOG("enter parser url\n");
  http_parser_url_init(&purl);
  int parser_status =
      http_parser_parse_url(client->url, strlen(client->url), 0, &purl);
  if (parser_status != 0) {
    return CLIENT_FAIL;
  }

  LOG("parse ok\n");
  if (purl.field_data[UF_SCHEMA].len) {
    client->schema = malloc_str(client->url + purl.field_data[UF_SCHEMA].off,
                                purl.field_data[UF_SCHEMA].len);
    if (strcmp(client->schema, "https") == 0) {
      client->port = malloc_str(HTTPS_DEFAULT_PORT, strlen(HTTPS_DEFAULT_PORT));
    } else {
      client->port = malloc_str(HTTP_DEFAULT_PORT, strlen(HTTP_DEFAULT_PORT));
    }
  }

  if (purl.field_data[UF_HOST].len) {
    client->host = malloc_str(client->url + purl.field_data[UF_HOST].off,
                              purl.field_data[UF_HOST].len);
  }

  if (purl.field_data[UF_PORT].len) {
    client->port = malloc_str(client->url + purl.field_data[UF_PORT].off,
                              purl.field_data[UF_PORT].len);
  }

  if (purl.field_data[UF_PATH].len) {
    client->path = malloc_str(client->url + purl.field_data[UF_PATH].off,
                              purl.field_data[UF_PATH].len);
  }
  LOG("client->schema = %s\n", client->schema);
  LOG("client->host = %s\n", client->host);
  LOG("client->port = %s\n", client->port);
  LOG("client->path = %s\n", client->path);

  return CLIENT_OK;
}

uint8_t http_client_send(HTTPClient *client) {
  HTTPStatus_t status =
      HTTPClient_Send(&client->interface, &client->header, client->payload,
                      client->payload_len, &client->response, 0);

  if (status == HTTPSuccess && client->response.statusCode == HTTP_STATUS_OK) {
    ((char *)(client->response.pBody))[client->response.bodyLen] = 0x00;
    return CLIENT_OK;
  }
  return CLIENT_FAIL;
}

uint8_t http_client_init(HTTPClient *client) {
  if (client == NULL) goto err;

  if (http_url_parse(client) != CLIENT_OK) goto err;
  LOG("http_url_parse ok\n");
  if (open_transport(client) != CLIENT_OK) goto err;
  LOG("open transport CLIENT_OK\n");
  HTTPRequestInfo_t requestInfo = {0};
  uint8_t *header_buffer;
  uint8_t *response_buffer;

  int success =
      (((header_buffer = (uint8_t *)malloc(HEADER_MAX_LEN)) != NULL) &&
       ((response_buffer = (uint8_t *)malloc(RESPONSE_MAX_LEN)) != NULL));

  if (!success) goto err;

  client->response.pBuffer = response_buffer;
  client->response.bufferLen = RESPONSE_MAX_LEN;

  // Set a buffer to serialize request headers to.
  client->header.pBuffer = header_buffer;
  client->header.bufferLen = HEADER_MAX_LEN;

  // Set the Method, Path, and Host in the HTTPRequestInfo_t.
  requestInfo.pMethod = client->method;
  requestInfo.methodLen = strlen(client->method);
  requestInfo.pPath = client->path;
  requestInfo.pathLen = strlen(client->path);
  requestInfo.pHost = client->host;
  requestInfo.hostLen = strlen(client->host);
  //    requestInfo.reqFlags |= HTTP_REQUEST_KEEP_ALIVE_FLAG;

  HTTPStatus_t status =
      HTTPClient_InitializeRequestHeaders(&client->header, &requestInfo);

  if (status != HTTPSuccess) {
    LOG("header init failed\n");
    goto err;
  }

  client->interface.send = core_http_send;
  client->interface.recv = core_http_recv;
  client->interface.pNetworkContext = &client->context;

  LOG("finish client init\n");
  return CLIENT_OK;

err:
  http_client_close(client);
  return CLIENT_FAIL;
}

uint8_t open_transport(HTTPClient *client) {
  const struct addrinfo hints = {.ai_family = AF_INET,
                                 .ai_socktype = SOCK_STREAM};
  struct addrinfo *result, *rp;

  int s = getaddrinfo(client->host, client->port, &hints, &result);
  if (s != 0 || result == NULL) {
    LOG("dns lookup failed\n");
    goto err;
  }

  /* getaddrinfo() returns a list of address structures.
     Try each address until we successfully connect.
     If socket fails, we close the socket and try the
     next address. */
  int fd = -1;
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (fd == -1) continue;

    if (connect(fd, rp->ai_addr, rp->ai_addrlen) != -1) {
      LOG("socket connected\n");
      break;  // success
    }

    close(fd);
  }
  freeaddrinfo(result);
  client->context.fd = fd;
  return (fd >= 0 ? CLIENT_OK : CLIENT_FAIL);

err:
  http_client_close(client);
  return CLIENT_FAIL;
}

uint8_t http_client_close(HTTPClient *client) {
  if (client->context.fd >= 0) {
    close(client->context.fd);
  }
  if (client->schema != NULL) {
    free(client->schema);
    client->schema = NULL;
  }
  if (client->host != NULL) {
    free(client->host);
    client->host = NULL;
  }
  if (client->port != NULL) {
    free(client->port);
    client->port = NULL;
  }
  if (client->path != NULL) {
    free(client->path);
    client->path = NULL;
  }
  if (client->header.pBuffer != NULL) {
    free(client->header.pBuffer);
    client->header.pBuffer = NULL;
  }
  if (client->response.pBuffer != NULL) {
    free(client->response.pBuffer);
    client->response.pBuffer = NULL;
  }
  return CLIENT_OK;
}

int32_t core_http_send(NetworkContext_t *pNetworkContext, const void *pBuffer,
                       size_t bytesToSend) {
  return write(pNetworkContext->fd, pBuffer, bytesToSend);
}

int32_t core_http_recv(NetworkContext_t *pNetworkContext, void *pBuffer,
                       size_t bytesToRecv) {
  return read(pNetworkContext->fd, pBuffer, bytesToRecv);
}