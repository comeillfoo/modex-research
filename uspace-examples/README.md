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
