## Поиск подстроки

Найдите все вхождения шаблона в строку. Длина шаблона – `p`, длина строки ­– `n`. *Время* `O(n + p)`, доп. *память* – `O(p)`.

С помощью префикс-функции;


*Формат входного файла.*

Шаблон, символ перевода строки, строка.

*Формат выходного файла.*

Позиции вхождения шаблона в строке.

In:
```
aba
abacababa
```
Out:
```
0 4 6
```
