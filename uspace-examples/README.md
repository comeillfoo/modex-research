# Формирование примеров реализаций сетевых протоколов пользовательского пространства

## Заметки

Возможные кандидаты:

1. [OpenSSL](https://github.com/openssl/openssl)
1. [wget](https://github.com/mirror/wget)
1. [curl / libcurl](https://github.com/curl/curl)
1. [libuv](https://github.com/libuv/libuv)
1. [netcat (ncat / nmap)](https://github.com/nmap/nmap)
1. [tcpdump](https://github.com/the-tcpdump-group/tcpdump) / [libpcap](https://github.com/the-tcpdump-group/libpcap)
1. [OpenSSH](https://github.com/openssh/openssh-portable)
1. [Mosquitto?](https://github.com/eclipse-mosquitto/mosquitto)
1. [9p](https://blog.aqwari.net/9p/)

###  netcat

Основная директория с утилитой `nmap/ncat`. `nmap/ncat/ncat_main.c` содержит
функцию `main`, которая парсит входные параметры и дергает другие функции,
осуществляющие непосредственную работу: в режиме `listen` &mdash;
`ncat_listen_mode`, иначе &mdash; `ncat_connect_mode`. Далее стек вызовов в
режиме `select` и для протокола `TCP` выглядит так:

#### Listen. Серверная

```C
ncat_main.c:ncat_listen_mode(...) {
    // some checks on arguments
    ncat_listen.c:ncat_listen(...) {
        // setup fd list and listening sockets
        // also adds stdin as client fd
        while (client_fdlist.nfds > 1 || get_conn_count() > 0) /* suppose:
while active connections exist, including stdin? */ {
            fds_ready = fselect(client_fdlist.fdmax + 1, &readfds, &writefds,
                                NULL, tvp); // get ready file descriptors
            for (i = 0; i < client_fdlist.nfds && fds_ready > 0; i++) /* for
each ready fd */ {
                if (checked_fd_isset(cfd, &listen_fds)) /* is listening socket
ready? */       {
                    /* we have a new connection request */
                    handle_connection(cfd, type, &listen_fds);
                } else if (cfd == STDIN_FILENO) {
                    /* Read from stdin and write to all clients. */
                    rc = read_stdin(...) {
                        ncat_broadcast(...);
                    }
                } else if (!o.sendonly) /* --send-only disabled? */ {
                    /* Read from a client and write to stdout. */
                    rc = read_socket(cfd) {
                        do {
                            /* ncat.prx:ncat_recv(...) 42 */
                            n = ncat_recv(fdn, buf, sizeof(buf), &pending);
                        } while (pending); /* fd in pending? */
                    }
                }
            }
        }
    }
}
```

#### Connect. Клиентская

Образец для теста: `nsock/tests/connect.c:connect_tcp`.

```
ncat_main.c:ncat_listen_mode() {
    // TODO: заполнить
}
```

## Отчет о проделанной работе

Главным критерием при отборе реализаций сетевых протоколов было число строк в
исходном коде, так как большие проекты труднее было бы осознать как мне самому,
так и читающему.

Под данный критерий в основном подпадают разные утилиты (`netcat`, `curl`,
`wget`, `tcpdump`, `tc`, `iptables`) и библиотеки к ним (`libcurl`, `libpcap`,
`libuv`) или относительно простые протоколы типа _запрос-ответ_ (`9p` и `HTTP`).

Решено было остановиться на утилите `netcat`, а именно на ее реализации от
проекта `nmap` (`ncat`), как на одном из самых простых проектов, реализующих
передачу по протоколам `TCP` и `UDP`.

`TODO: выбрать, обосновать выбор, почему только один или несколько, выводы`.
`TODO: описать какие манипуляции были произведены с примером`.

## Источники

1. `TODO: источник на разъясняющие материалы по выбранному проекту`
