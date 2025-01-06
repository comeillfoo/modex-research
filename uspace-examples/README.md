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

### [netcat (nmap)](https://github.com/nmap/nmap/tree/master/ncat)

Основная директория с утилитой `nmap/ncat`. `nmap/ncat/ncat_main.c` содержит
функцию `main`, которая парсит входные параметры и дергает другие функции,
осуществляющие непосредственную работу: в режиме `listen` &mdash;
`ncat_listen_mode`, иначе &mdash; `ncat_connect_mode`. Далее стек вызовов в
режиме `select` и для протокола `TCP` выглядит так:

#### Listen. Серверная

```C
ncat_main.c:ncat_listen_mode(...) {
    /*% mdx: some checks on arguments %*/
    ncat_listen.c:ncat_listen(...) {
        /*% mdx: setup fd list and listening sockets also adds stdin as client fd %*/
        while (client_fdlist.nfds > 1 || ncat_listen.c:get_conn_count() > 0) /*%
mdx: suppose: while active connections exist, including stdin? %*/ {
            fds_ready = ../nbase/nbase_misc.c:fselect(client_fdlist.fdmax + 1,
                        &readfds, &writefds, NULL, tvp); /*% mdx: get ready file
descriptors %*/
            for (i = 0; i < client_fdlist.nfds && fds_ready > 0; i++) /*% mdx:
            for each ready fd %*/ {
                if (../nbase/nbase.h:checked_fd_isset(cfd, &listen_fds)) /*% mdx:
                is listening socket ready? %*/ {
                    /* we have a new connection request */
                    ncat_listen.c:handle_connection(cfd, type, &listen_fds);
                } else if (cfd == STDIN_FILENO) {
                    /* Read from stdin and write to all clients. */
                    rc = ncat_listen.c:read_stdin(...) {
                        nbytes = READ_STDIN(buf, sizeof(buf));
                        ncat_core.c:ncat_broadcast(...);
                    };
                } else if (!o.sendonly) /*% mdx: --send-only disabled? %*/ {
                    /* Read from a client and write to stdout. */
                    rc = ncat_listen.c:read_socket(cfd) {
                        do {
                            n = ncat_core.c:ncat_recv(fdn, buf, sizeof(buf),
                                                      &pending);
                        } while (pending); /*% mdx: fd in pending? %*/
                    };
                }
            }
        }
    };
};
```

#### Connect. Клиентская

Образец для теста: `nsock/tests/connect.c:connect_tcp`.

```C
ncat/ncat_main.c:ncat_connect_mode() {
    /*% mdx: some checks on arguments %*/
    ncat/ncat_connect.c:ncat_connect() {
        if ((mypool = nsock/src/nsock_pool.c:nsock_pool_new(NULL)) == NULL)
        /*% mdx: init nsock pool and sets callbacks on io_operations according
         *% to set engine, default "select" %*/
            bye("Failed to create nsock_pool.");

        if (!o.proxytype) {
            ncat/ncat_connect.c:try_nsock_connect(...) {
                nsock/src/nsock_connect.c:nsock_connect_tcp(...) {
                    nse = event_new(ms, NSE_TYPE_CONNECT, nsi, ...);
                    /*% mdx: ... %*/
                    /* Do the actual connect() */
                    nsock/src/nsock_connect.c:nsock_connect_internal(...) {
                        /* Now it is time to actually attempt the connection */
                        if (nsock_make_socket(...) == -1) /*% mdx: creates
                        socket via socket() syscall %*/ {
                            /*% mdx: sets error values in event struct %*/
                        } else {
                            if (ms->engine->io_operations->iod_connect(...) /*%
mdx: nsock/src/nsock_engines.c:posix_iod_connect(...) %*/ {
                                return connect(sockfd, addr, addrlen);
                            } == -1) {
                                /*% mdx: sets error values in event struct %*/
                            }
                        }
                    };
                    /*% mdx: this will make nsp->events_pending > 0 %*/
                    nsock/src/nsock_core.c:nsock_pool_add_event(ms, nse) {
                        nsp->events_pending++;
                    };
                };
            };
        } else {
            /* A proxy connection. */
            /*% mdx: some setups that are not important for us %*/
        }
        /* connect */
        rc = nsock/src/nsock_core.c:nsock_loop(mypool, -1) {
            while (1) {
                if (nsock/src/nsock_internal.h:nsock_engine_loop(ms,
                msecs_left) {
                    return nsp->engine->loop(nsp, msec_timeout)
                    /*% mdx: nsock/src/engine_select.c:select_loop(...) %*/ {
                        /*% mdx: because of try_connect, there is always single
                         *% pending event of nse->type == NSE_TYPE_CONNECT %*/
                        if (nsp->events_pending == 0)
                            return 0; /* No need to wait on 0 events ... */

                        do {
                            nse = nsock/src/nsock_internal.h:next_expirable_event(nsp)
                                  /*% mdx: obtains event from min-heap priority queue %*/;

                            if (iod_count > 0) {
                                /* Set up the descriptors for select */
                                /*% mdx: read/write/except descriptors setup %*/
                                results_left = ../nbase/nbase_misc.c:fselect(...);
                                if (results_left == -1)
                                /*% mdx: I bet that ends do-while loop %*/
                                    sock_err = socket_errno();
                            } else if (combined_msecs > 0) {
                                // No compatible IODs; sleep the remainder of the wait time.
                                usleep(combined_msecs * 1000);
                            }
                        } while (results_left == -1 && sock_err == EINTR); /*
                        repeat only if signal occurred */

                        /* Iterate through all the event lists (such as connect_events, read_events,
                         * timer_events, etc) and take action for those that have completed (due to
                         * timeout, i/o, etc) */
                        nsock/src/engine_select.c:iterate_through_event_lists(nsp) {
                            for (current = ...; ...; current = next) {
                                struct niod *nsi = container_of(current,...);
                                if (nsi->state != NSIOD_STATE_DELETED && nsi->events_pending)
                                /*% mdx: this function invokes posix_io_read
                                 *% and posix_io_write %*/
                                    process_iod_events(nsp, nsi, get_evmask(nsp, nsi));

                                next = gh_lnode_next(current);
                            }

                            /* iterate through timers and expired events */
                            process_expired_events(nsp);
                        };
                        return 1;
                    };
                } == -1) {
                    quitstatus = NSOCK_LOOP_ERROR;
                    break;
                }
            }
            return quitstatus;
        };
    };
};
```
> я дропаю эту тяжелую махину, тут надо столько резать, чтобы modex на него натравить

### [netcat (OpenBSD)](https://github.com/openbsd/src/blob/master/usr.bin/nc/netcat.c)

`TODO: заполнить`
> для modex достаточно дважды `readwrite` как `passive proctype` излечь

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
