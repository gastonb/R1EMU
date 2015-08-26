/**
 *
 *   ██████╗   ██╗ ███████╗ ███╗   ███╗ ██╗   ██╗
 *   ██╔══██╗ ███║ ██╔════╝ ████╗ ████║ ██║   ██║
 *   ██████╔╝ ╚██║ █████╗   ██╔████╔██║ ██║   ██║
 *   ██╔══██╗  ██║ ██╔══╝   ██║╚██╔╝██║ ██║   ██║
 *   ██║  ██║  ██║ ███████╗ ██║ ╚═╝ ██║ ╚██████╔╝
 *   ╚═╝  ╚═╝  ╚═╝ ╚══════╝ ╚═╝     ╚═╝  ╚═════╝
 *
 * @file router.h
 *
 * @brief Router is the routing component of the network.
 * It accepts packets on a specific port, and routes them to the workers.
 *
 * @license GNU GENERAL PUBLIC LICENSE - Version 2, June 1991
 *          See LICENSE file for further information
 */

#pragma once

#include "R1EMU.h"
#include "common/redis/redis.h"
#include "common/mysql/mysql.h"

#define ROUTER_FRONTEND_ENDPOINT           "tcp://%s:%d"
#define ROUTER_BACKEND_ENDPOINT            "inproc://routerWorkersBackend-%d"
#define ROUTER_SUBSCRIBER_ENDPOINT         "inproc://routerWorkersSubscriber-%d"

#define ROUTER_GLOBAL_ENDPOINT             "tcp://%s:%d"

/** Enumeration of all the packets headers that the Router handles */
// we want to differentiate the headers being received from the the ones being send, but we also want to keep a list
// with uniques header IDs. So, let's declare all the IDs here, and distribute them afterward
typedef enum RouterHeader {
    _ROUTER_WORKER_READY,     // Ready message
    _ROUTER_WORKER_NORMAL,    // Normal message
    _ROUTER_WORKER_MULTICAST, // Multicast message
    _ROUTER_WORKER_ERROR,     // Error message received from a worker
    _ROUTER_PING,             // Ping message
    _ROUTER_PONG,             // Pong message
} RouterHeader;

// macro helper for the distribution
#define DECL_ROUTER_HEADER(x) \
    x = _##x

/** Enumeration of all the packets header that the Router accepts */
typedef enum RouterRecvHeader {
    DECL_ROUTER_HEADER(ROUTER_WORKER_READY),
    DECL_ROUTER_HEADER(ROUTER_WORKER_NORMAL),
    DECL_ROUTER_HEADER(ROUTER_WORKER_MULTICAST),
    DECL_ROUTER_HEADER(ROUTER_WORKER_ERROR),
    DECL_ROUTER_HEADER(ROUTER_PING),
} RouterRecvHeader;

/** Enumeration of all the packets header that the Router sends */
typedef enum RouterSendHeader {
    DECL_ROUTER_HEADER (ROUTER_PONG),
} RouterSendHeader;

#undef DECL_ROUTER_HEADER

typedef struct Router Router;

typedef struct {
    uint16_t routerId;
    char *ip;
    int *ports;
    int portsCount;
    int workersCount;
    RedisStartupInfo redisInfo;
    MySQLStartupInfo sqlInfo;
} RouterStartupInfo;

/**
 * @brief Allocate a new Router structure.
 * @param info An allocated RouterStartupInfo already initialized.
 * @return A pointer to an allocated Router.
 */
Router *routerNew(RouterStartupInfo *info);

/**
 * @brief Initialize an allocated Router structure.
 * @param self An allocated Router to initialize.
 * @return true on success, false otherwise.
 */
bool routerInit(Router *self, RouterStartupInfo *info);

/**
 * @brief Initialize an allocated RouterStartupInfo structure.
 * @param self An allocated RouterStartupInfo to initialize.
 * @param routerId The Server ID
 * @param ip The IP of the router
 * @param ports The ports binded by the Router
 * @param portsCount Ports count
 * @param workersCount Number of workers linked to the Router
 * @return true on success, false otherwise
 */
bool routerStartupInfoInit(
    RouterStartupInfo *self,
    uint16_t routerId,
    char *ip,
    int *ports,
    int portsCount,
    int workersCount,
    RedisStartupInfo *redisInfo,
    MySQLStartupInfo *sqlInfo);

/**
 * @brief Start a new Router
 * @param self An allocated Router to start
 * @return true on success, false otherwise.
 */
bool routerStart(Router *self);

/**
 * @brief Return the ID of a Router
 * @param self An allocated Router
 * @return The Router ID
 */
int routerGetId(Router *self);

/**
 * @brief Free the members of the RouterStartupInfo structure
 * @param self A pointer to an allocated RouterStartupInfo.
 */
void routerStartupInfoFree(RouterStartupInfo *self);

/**
 * @brief Free an allocated Router structure and nullify the content of the pointer.
 * @param self A pointer to an allocated Router.
 */
void routerDestroy(Router **self);
