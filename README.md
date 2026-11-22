
# cfetch

A minimal neofetch-style system information tool written in C.
A learning project: no third-party libraries, no dependencies, one binary.

## Features

- OS name, read from `/etc/os-release` (`PRETTY_NAME`)
- Kernel name and release, via `uname`
- Hostname, via `gethostname`
- Two-column output: ASCII logo on the left, fields on the right
- Falls back to a plain single-column list when stdout is not a TTY
  (pipes, redirects) or when the terminal is too narrow for both columns

## Build

```
make
./cfetch
```

Requires Linux, a C11 compiler and make. Nothing else.

## How it works

Collecting and printing are separate stages.

Every collector has the same signature:

```c
int get_thing(char *buf, size_t len);
```

It writes the value into a caller-provided buffer and returns `0` on
success or `-1` when there is no data. On failure the buffer is left
empty, so the renderer has a single rule: an empty field is not shown.
Collectors never print and never add labels.

`collect_all()` fills one `struct system_info` and reports success if at
least one field was collected, so a missing field degrades the output
instead of breaking it.

Rendering lives in `main.c`: it pads the logo column to the width of the
widest art line and prints the fields beside it.

## Layout

```
include/info.h    struct + collector prototypes
include/ascii.h   ASCII logo
src/info.c        collectors and collect_all()
src/main.c        field table, layout, rendering
Makefile
```

## License

MIT. See [LICENSE](LICENSE).

---

# cfetch

Минимальный аналог neofetch, написанный на C.
Учебный проект: без сторонних библиотек, без зависимостей, один бинарник.

## Что показывает

- Имя дистрибутива из `/etc/os-release` (`PRETTY_NAME`)
- Имя и релиз ядра через `uname`
- Имя хоста через `gethostname`
- Вывод в две колонки: ASCII-логотип слева, поля справа
- Автоматический переход к обычному списку в одну колонку, если stdout
  не терминал (пайп, редирект) или терминал слишком узкий для двух колонок

## Сборка

```
make
./cfetch
```

Нужны Linux, компилятор с поддержкой C11 и make. Больше ничего.

## Как устроено

Сбор данных и печать разделены.

У всех сборщиков одинаковая сигнатура:

```c
int get_thing(char *buf, size_t len);
```

Функция пишет значение в переданный буфер и возвращает `0` при успехе или
`-1`, если данных нет. При ошибке буфер остаётся пустым, поэтому у рендера
одно правило: пустое поле не выводится. Сборщики ничего не печатают и не
добавляют метки вида «OS:».

`collect_all()` заполняет одну структуру `struct system_info` и сообщает
успех, если удалось собрать хотя бы одно поле: отсутствие данных ухудшает
вывод, но не ломает его.

Рендер находится в `main.c`: он добивает колонку логотипа до ширины самой
длинной строки арта и печатает поля рядом.

## Структура

```
include/info.h    структура и прототипы сборщиков
include/ascii.h   ASCII-логотип
src/info.c        сборщики и collect_all()
src/main.c        таблица полей, раскладка, вывод
Makefile
```

## Лицензия

MIT. Подробности в [LICENSE](LICENSE).
