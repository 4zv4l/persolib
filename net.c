#include "net.h"

void
parse_ip(char host[static 1], string *ip, u32 *port)
{
    string bak;

    *ip = strtok_r(host, ":", &bak);
    *port = atoi(strtok_r(0, ":", &bak));
}

i32
tcp_listen(char host[static 1])
{
    i32 connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connfd == -1)
        return log_warn("socket(): %s", strerror(errno)), 0;

    string dup_host = strdup(host);
    if (!dup_host)
        return log_warn("strdup(): couldnt clone host addr"), 0;

    string ip; u32 port;
    parse_ip(dup_host, &ip, &port);

    struct sockaddr_in addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(ip)
    };
    free(dup_host);

    if (setsockopt(connfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) == -1)
        return log_warn("setsockopt(): %s", strerror(errno)), 0;
    log_info("setsockopt(): success !");

    if (bind(connfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        return log_warn("bind(): %s", strerror(errno)), 0;
    log_info("bind(): success !");

    if (listen(connfd, 0) == -1)
        return log_warn("listen(): %s", strerror(errno)), 0;

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
        return log_warn("strdup(): couldnt clone host addr"), 0;

    string ip; u32 port;
    parse_ip(dup_host, &ip, &port);

    struct sockaddr_in addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = inet_addr(ip)
    };

    free(dup_host);

    if (connect(connfd, (struct sockaddr*)&addr, sizeof(addr)) == -1)
        return log_warn("connect(): %s", strerror(errno)), 0;
    log_info("connect(): success !");

    return connfd;
}

usize
send_line(i32 connfd, string data)
{
    usize idx = 0;
    usize tmp = 0;
    usize len = index_of('\n', data);

    if (len) log_info("found '\\n' at index %llu", len);
    else len = strlen(data);

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
    string tmp = malloc(size);
    if (!tmp)
        return log_warn("malloc(): couldnt allocate memory for tmp"), (string)0;

    while(read(connfd, tmp+idx, 1))
    {
        if (tmp[idx] == '\n') {
            tmp = realloc(tmp, idx+2);
            if (!tmp)
                return log_warn("realloc(): couldnt realloc tmp"), (string)0;
            log_info("recv_line(): \"%.*s\\n\" (%llu)", idx, tmp, idx+1);
            tmp[idx+1] = 0;
            return tmp;
        }

        idx += 1;
        if (idx > size)
        {
            size += 128;
            tmp = realloc(tmp, size);
            if (!tmp)
                return log_warn("realloc(): couldnt realloc tmp"), (string)0;
        }
    }
    return 0;
}
