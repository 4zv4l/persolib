#include "net.h"

bool
parse_ip(char host[static 1], string *ip, u32 *port)
{
    string sport;

    *ip = strtok(host, ":");
    sport = strtok(0, ":");
    if (!sport)
        return false;

    *port = atoi(sport);
    return true;
}

i32
tcp_listen(char host[static 1])
{
    i32 connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connfd == -1)
    {
        close(connfd);
        return log_warn("socket(): %s", strerror(errno)), 0;
    }

    string dup_host = strdup(host);
    if (!dup_host)
    {
        close(connfd);
        return log_warn("strdup(%s): couldnt clone host addr", host), 0;
    }

    string ip; u32 port;
    if (!parse_ip(dup_host, &ip, &port))
    {
        free(dup_host);
        close(connfd);
        return log_warn("parse_ip(host): wrong ip format", host), 0;
    }

    struct sockaddr_in addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(ip)
    };
    free(dup_host);

    if (setsockopt(connfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1)
    {
        close(connfd);
        return log_warn("setsockopt(): %s", strerror(errno)), 0;
    }
    log_info("setsockopt(): success !");

    if (bind(connfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
    {
        close(connfd);
        return log_warn("bind(): %s", strerror(errno)), 0;
    }
    log_info("bind(): success !");

    if (listen(connfd, 0) == -1)
    {
        close(connfd);
        return log_warn("listen(): %s", strerror(errno)), 0;
    }

    return connfd;
}

i32
tcp_connect(char host[static 1])
{
    i32 connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connfd == -1)
        return log_warn("socket(): %s", strerror(errno)), 0;

    string dup_host = strdup(host);
    if (!dup_host)
    {
        close(connfd);
        return log_warn("strdup(%s): couldnt clone host addr", host), 0;
    }

    string ip; u32 port;
    if (!parse_ip(dup_host, &ip, &port))
    {
        free(dup_host);
        close(connfd);
        return log_warn("parse_ip(%s): wrong ip format", host), 0;
    }

    struct sockaddr_in addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(ip)
    };

    free(dup_host);

    if (connect(connfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
        close(connfd);
        return log_warn("connect(%s): %s", host, strerror(errno)), 0;
    }
    log_info("connect(%s): success !", host);

    return connfd;
}

i32
tcp_accept(i32 serverfd)
{
    struct sockaddr caddr = {0};
    socklen_t caddr_size  = {0};

    i32 clientfd = accept(serverfd, &caddr, &caddr_size);

    if (clientfd == -1)
        return log_warn("accept(): %s", strerror(errno)), 0;
    return clientfd;
}

usize
send_line(i32 connfd, string data)
{
    usize idx = 0;
    usize tmp = 0;
    usize len = index_of('\n', data);

    if (len)
        log_info("found '\\n' at index %llu", len);
    else
        len = strlen(data);

    idx += tmp = write(connfd, data, len);
    if (!tmp)
        return log_warn("write(): couldnt send data"), 0;

    idx += tmp = write(connfd, "\n", 1);
    if (!tmp)
        return log_warn("write(): couldnt send newline"), 0;

    log_info("send_line(): \"%.*s\\n\" (%llu)", len, data, idx);
    return len;
}

string
recv_line(i32 connfd)
{
    usize idx = 0;
    usize size = 128;
    string line = malloc(size * sizeof(*line)), tmp;
    if (!line)
        return log_warn("malloc(): couldnt allocate memory for tmp"), (string)0;

    while(read(connfd, line+idx, 1))
    {
        if (line[idx] == '\n') {
            // resize the string to be as small as possible
            tmp = realloc(line, idx+2);
            if (!tmp)
            {
                free(line);
                return log_warn("realloc(): couldnt realloc tmp"), (string)0;
            }
            line = tmp;
            log_info("recv_line(): \"%.*s\\n\" (%llu)", idx, line, idx+1);
            line[idx+1] = 0;
            return line;
        }

        idx += 1;
        // resize if the incoming data is too big
        if (idx > size)
        {
            size += 128;
            tmp = realloc(line, size);
            if (!tmp)
            {
                free(line);
                return log_warn("realloc(): couldnt realloc tmp"), (string)0;
            }
            line = tmp;
        }
    }

    free(line);
    return 0;
}
