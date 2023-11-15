#include <stdlib.h>
#include <aiio_log.h>
#include <aiio_err.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

#include "aiio_http_server.h"
#include "aiio_httpd_priv.h"


#ifndef F_GETFD
#define	F_GETFD		1	/* Get fildes flags (close on exec) */
#endif


typedef enum {
    HTTPD_TASK_NONE = 0,
    HTTPD_TASK_INIT,            // Init session
    HTTPD_TASK_GET_ACTIVE,      // Get active session (fd!=-1)
    HTTPD_TASK_GET_FREE,        // Get free session slot (fd<0)
    HTTPD_TASK_FIND_FD,         // Find session with specific fd
    HTTPD_TASK_SET_DESCRIPTOR,  // Set descriptor
    HTTPD_TASK_DELETE_INVALID,  // Delete invalid session
    HTTPD_TASK_FIND_LOWEST_LRU, // Find session with lowest lru
    HTTPD_TASK_CLOSE            // Close session
} task_t;

typedef struct {
    task_t task;
    int fd;
    fd_set *fdset;
    int max_fd;
    struct aiio_httpd_data *hd;
    uint64_t lru_counter;
    struct aiio_sock_db    *session;
} enum_context_t;

void aiio_httpd_sess_enum(struct aiio_httpd_data *hd, aiio_httpd_session_enum_function enum_function, void *context)
{
    if ((!hd) || (!hd->hd_sd) || (!hd->config.max_open_sockets)) {
        return;
    }

    struct aiio_sock_db *current = hd->hd_sd;
    struct aiio_sock_db *end = hd->hd_sd + hd->config.max_open_sockets - 1;

    while (current <= end) {
        if (enum_function && (!enum_function(current, context))) {
            break;
        }
        current++;
    }
}

// Check if a FD is valid
static int fd_is_valid(int fd)
{
    return fcntl(fd, F_GETFD,0) != -1 || errno != EBADF;
}

static int enum_function(struct aiio_sock_db *session, void *context)
{
    if ((!session) || (!context)) {
        return 0;
    }
    enum_context_t *ctx = (enum_context_t *) context;
    int found = 0;
    switch (ctx->task) {
    // Initialize session
    case HTTPD_TASK_INIT:
        session->fd = -1;
        session->ctx = NULL;
        break;
    // Get active session
    case HTTPD_TASK_GET_ACTIVE:
        found = (session->fd != -1);
        break;
    // Get free slot
    case HTTPD_TASK_GET_FREE:
        found = (session->fd < 0);
        break;
    // Find fd
    case HTTPD_TASK_FIND_FD:
        found = (session->fd == ctx->fd);
        break;
    // Set descriptor
    case HTTPD_TASK_SET_DESCRIPTOR:
        if (session->fd != -1) {
            FD_SET(session->fd, ctx->fdset);
            if (session->fd > ctx->max_fd) {
                ctx->max_fd = session->fd;
            }
        }
        break;
    // Delete invalid session
    case HTTPD_TASK_DELETE_INVALID:
        if (!fd_is_valid(session->fd)) {
            aiio_log_w("Closing invalid socket %d", session->fd);
            aiio_httpd_sess_delete(ctx->hd, session);
        }
        break;
    // Find lowest lru
    case HTTPD_TASK_FIND_LOWEST_LRU:
        // Found free slot - no need to check other sessions
        if (session->fd == -1) {
            return 0;
        }
        // Check/update lowest lru
        if (session->lru_counter < ctx->lru_counter) {
            ctx->lru_counter = session->lru_counter;
            ctx->session = session;
        }
        break;
    case HTTPD_TASK_CLOSE:
        if (session->fd != -1) {
            aiio_log_d("cleaning up socket %d", session->fd);
            aiio_httpd_sess_delete(ctx->hd, session);
        }
        break;
    default:
        return 0;
    }
    if (found) {
        ctx->session = session;
        return 0;
    }
    return 1;
}

static void httpd_sess_close(void *arg)
{
    struct aiio_sock_db *aiio_sock_db = (struct aiio_sock_db *) arg;
    if (!aiio_sock_db) {
        return;
    }

    if (!aiio_sock_db->lru_counter && !aiio_sock_db->lru_socket) {
        aiio_log_d("Skipping session close for %d as it seems to be a race condition", aiio_sock_db->fd);
        return;
    }
    aiio_sock_db->lru_socket = false;
    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) aiio_sock_db->handle;
    aiio_httpd_sess_delete(hd, aiio_sock_db);
}

struct aiio_sock_db *aiio_httpd_sess_get_free(struct aiio_httpd_data *hd)
{
    if ((!hd) || (hd->hd_sd_active_count == hd->config.max_open_sockets)) {
        return NULL;
    }
    enum_context_t context = {
        .task = HTTPD_TASK_GET_FREE
    };
    aiio_httpd_sess_enum(hd, enum_function, &context);
    return context.session;
}

bool aiio_httpd_is_sess_available(struct aiio_httpd_data *hd)
{
    return aiio_httpd_sess_get_free(hd) ? true : false;
}

struct aiio_sock_db *aiio_httpd_sess_get(struct aiio_httpd_data *hd, int sockfd)
{
    if ((!hd) || (!hd->hd_sd) || (!hd->config.max_open_sockets)) {
        return NULL;
    }

    // Check if called inside a request handler, and the session sockfd in use is same as the parameter
    // => Just return the pointer to the aiio_sock_db corresponding to the request
    if ((hd->hd_req_aux.sd) && (hd->hd_req_aux.sd->fd == sockfd)) {
        return hd->hd_req_aux.sd;
    }

    enum_context_t context = {
        .task = HTTPD_TASK_FIND_FD,
        .fd = sockfd
    };
    aiio_httpd_sess_enum(hd, enum_function, &context);
    return context.session;
}

aiio_err_t aiio_httpd_sess_new(struct aiio_httpd_data *hd, int newfd)
{
    aiio_log_d("fd = %d", newfd);

    if (aiio_httpd_sess_get(hd, newfd)) {
        aiio_log_e("session already exists with fd = %d", newfd);
        return AIIO_FAIL;
    }

    struct aiio_sock_db *session = aiio_httpd_sess_get_free(hd);
    if (!session) {
        aiio_log_d("unable to launch session for fd = %d", newfd);
        return AIIO_FAIL;
    }

    // Clear session data
    memset(session, 0, sizeof (struct aiio_sock_db));
    session->fd = newfd;
    session->handle = (aiio_httpd_handle_t) hd;
    session->send_fn = aiio_httpd_default_send;
    session->recv_fn = aiio_httpd_default_recv;

    // Call user-defined session opening function
    if (hd->config.open_fn) {
        aiio_err_t ret = hd->config.open_fn(hd, session->fd);
        if (ret != AIIO_OK) {
            aiio_httpd_sess_delete(hd, session);
            aiio_log_d("open_fn failed for fd = %d", newfd);
            return ret;
        }
    }

    // increment number of sessions
    hd->hd_sd_active_count++;
    aiio_log_d("active sockets: %d", hd->hd_sd_active_count);

    return AIIO_OK;
}

void aiio_httpd_sess_free_ctx(void **ctx, aiio_httpd_free_ctx_fn_t free_fn)
{
    if ((!ctx) || (!*ctx)) {
        return;
    }
    if (free_fn) {
        free_fn(*ctx);
    } else {
        free(*ctx);
    }
    *ctx = NULL;
}

void httpd_sess_clear_ctx(struct aiio_sock_db *session)
{
    if ((!session) || ((!session->ctx) && (!session->transport_ctx))) {
        return;
    }

    // free user ctx
    if (session->ctx) {
        aiio_httpd_sess_free_ctx(&session->ctx, session->free_ctx);
        session->free_ctx = NULL;
    }

    // Free 'transport' context
    if (session->transport_ctx) {
        aiio_httpd_sess_free_ctx(&session->transport_ctx, session->free_transport_ctx);
        session->free_transport_ctx = NULL;
    }
}

void *aiio_httpd_sess_get_ctx(aiio_httpd_handle_t handle, int sockfd)
{
    struct aiio_sock_db *session = aiio_httpd_sess_get(handle, sockfd);
    if (!session) {
        return NULL;
    }

    // Check if the function has been called from inside a
    // request handler, in which case fetch the context from
    // the aiio_httpd_req_t structure
    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) handle;
    if (hd->hd_req_aux.sd == session) {
        return hd->hd_req.sess_ctx;
    }
    return session->ctx;
}

void aiio_httpd_sess_set_ctx(aiio_httpd_handle_t handle, int sockfd, void *ctx, aiio_httpd_free_ctx_fn_t free_fn)
{
    struct aiio_sock_db *session = aiio_httpd_sess_get(handle, sockfd);
    if (!session) {
        return;
    }

    // Check if the function has been called from inside a
    // request handler, in which case set the context inside
    // the aiio_httpd_req_t structure
    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) handle;
    if (hd->hd_req_aux.sd == session) {
        if (hd->hd_req.sess_ctx != ctx) {
            // Don't free previous context if it is in sockdb
            // as it will be freed inside httpd_req_cleanup()
            if (session->ctx != hd->hd_req.sess_ctx) {
                aiio_httpd_sess_free_ctx(&hd->hd_req.sess_ctx, hd->hd_req.free_ctx); // Free previous context
            }
            hd->hd_req.sess_ctx = ctx;
        }
        hd->hd_req.free_ctx = free_fn;
        return;
    }

    // Else set the context inside the aiio_sock_db structure
    if (session->ctx != ctx) {
        // Free previous context
        aiio_httpd_sess_free_ctx(&session->ctx, session->free_ctx);
        session->ctx = ctx;
    }
    session->free_ctx = free_fn;
}

void *aiio_httpd_sess_get_transport_ctx(aiio_httpd_handle_t handle, int sockfd)
{
    struct aiio_sock_db *session = aiio_httpd_sess_get(handle, sockfd);
    return session ? session->transport_ctx : NULL;
}

void aiio_httpd_sess_set_transport_ctx(aiio_httpd_handle_t handle, int sockfd, void *ctx, aiio_httpd_free_ctx_fn_t free_fn)
{
    struct aiio_sock_db *session = aiio_httpd_sess_get(handle, sockfd);
    if (!session) {
        return;
    }

    if (session->transport_ctx != ctx) {
        // Free previous transport context
        aiio_httpd_sess_free_ctx(&session->transport_ctx, session->free_transport_ctx);
        session->transport_ctx = ctx;
    }
    session->free_transport_ctx = free_fn;
}

void aiio_httpd_sess_set_descriptors(struct aiio_httpd_data *hd, fd_set *fdset, int *maxfd)
{
    enum_context_t context = {
        .task = HTTPD_TASK_SET_DESCRIPTOR,
        .max_fd = -1,
        .fdset = fdset
    };
    aiio_httpd_sess_enum(hd, enum_function, &context);
    if (maxfd) {
        *maxfd = context.max_fd;
    }
}

void aiio_httpd_sess_delete_invalid(struct aiio_httpd_data *hd)
{
    enum_context_t context = {
        .task = HTTPD_TASK_DELETE_INVALID,
        .hd = hd
    };
    aiio_httpd_sess_enum(hd, enum_function, &context);
}

void aiio_httpd_sess_delete(struct aiio_httpd_data *hd, struct aiio_sock_db *session)
{
    if ((!hd) || (!session) || (session->fd < 0)) {
        return;
    }

    aiio_log_d("fd = %d", session->fd);

    // Call close function if defined
    if (hd->config.close_fn) {
        hd->config.close_fn(hd, session->fd);
    } else {
        close(session->fd);
    }

    // clear all contexts
    httpd_sess_clear_ctx(session);

    // mark session slot as available
    session->fd = -1;

    // decrement number of sessions
    hd->hd_sd_active_count--;
    aiio_log_d("active sockets: %d", hd->hd_sd_active_count);
    if (!hd->hd_sd_active_count) {
        hd->lru_counter = 0;
    }
}

void aiio_httpd_sess_init(struct aiio_httpd_data *hd)
{
    enum_context_t context = {
        .task = HTTPD_TASK_INIT
    };
    aiio_httpd_sess_enum(hd, enum_function, &context);
}

bool aiio_httpd_sess_pending(struct aiio_httpd_data *hd, struct aiio_sock_db *session)
{
    if (!session) {
        return false;
    }
    if (session->pending_fn) {
        // test if there's any data to be read (besides read() function, which is handled by select() in the main httpd loop)
        // this should check e.g. for the SSL data buffer
        if (session->pending_fn(hd, session->fd) > 0) {
            return true;
        }
    }
    return (session->pending_len != 0);
}

/* This MUST return AIIO_OK on successful execution. If any other
 * value is returned, everything related to this socket will be
 * cleaned up and the socket will be closed.
 */
aiio_err_t aiio_httpd_sess_process(struct aiio_httpd_data *hd, struct aiio_sock_db *session)
{
    if ((!hd) || (!session)) {
        return AIIO_FAIL;
    }

    aiio_log_d("aiio_httpd_req_new");
    if (aiio_httpd_req_new(hd, session) != AIIO_OK) {
        return AIIO_FAIL;
    }
    aiio_log_d("aiio_httpd_req_delete");
    if (aiio_httpd_req_delete(hd) != AIIO_OK) {
        return AIIO_FAIL;
    }
    aiio_log_d("success");
    session->lru_counter = ++hd->lru_counter;
    return AIIO_OK;
}

aiio_err_t aiio_httpd_sess_update_lru_counter(aiio_httpd_handle_t handle, int sockfd)
{
    if (handle == NULL) {
        return AIIO_ERR_INVALID_ARG;
    }

    struct aiio_httpd_data *hd = (struct aiio_httpd_data *) handle;

    enum_context_t context = {
        .task = HTTPD_TASK_FIND_FD,
        .fd = sockfd
    };
    aiio_httpd_sess_enum(hd, enum_function, &context);
    if (context.session) {
        context.session->lru_counter = ++hd->lru_counter;
        return AIIO_OK;
    }
    return AIIO_ERR_NOT_FOUND;
}

aiio_err_t aiio_httpd_sess_close_lru(struct aiio_httpd_data *hd)
{
    enum_context_t context = {
        .task = HTTPD_TASK_FIND_LOWEST_LRU,
        .lru_counter = UINT64_MAX,
        .fd = -1
    };
    aiio_httpd_sess_enum(hd, enum_function, &context);
    if (!context.session) {
        return AIIO_OK;
    }
    aiio_log_d("Closing session with fd %d", context.session->fd);
    context.session->lru_socket = true;
    return aiio_httpd_sess_trigger_close_(hd, context.session);
}

aiio_err_t aiio_httpd_sess_trigger_close_(aiio_httpd_handle_t handle, struct aiio_sock_db *session)
{
    if (!session) {
        return AIIO_ERR_NOT_FOUND;
    }
    return aiio_httpd_queue_work(handle, httpd_sess_close, session);
}

aiio_err_t aiio_httpd_sess_trigger_close(aiio_httpd_handle_t handle, int sockfd)
{
    struct aiio_sock_db *session = aiio_httpd_sess_get(handle, sockfd);
    if (!session) {
        return AIIO_ERR_NOT_FOUND;
    }
    return aiio_httpd_sess_trigger_close_(handle, session);
}

void aiio_httpd_sess_close_all(struct aiio_httpd_data *hd)
{
    enum_context_t context = {
        .task = HTTPD_TASK_CLOSE,
        .hd = hd
    };
    aiio_httpd_sess_enum(hd, enum_function, &context);
}
