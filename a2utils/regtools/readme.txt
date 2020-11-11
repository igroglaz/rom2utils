Author: Vladimir Chebotarev aka ex-lend
Categories: RoM 2, For developers
Version: 1.0

Две программы для работы с файлами формата .reg. regdump преобразовывает один или несколько файлов в текстовый формат (выводит на экран), regset позволяет изменить какую-либо ветку.

Использование:
regset <regfile> </path/to/entry> <dword|dword[]|byte[]|string> <value|value1,value2,..,valueN>
regdump <regfile> [<regfile2> ...]