Конвертирует data.bin в XML и обратно.	

Утилита преобразовывает файл data.bin в соответствующий по содержанию xml-файл и выполняет обратное преобразование.
Использование:
usage: databin2xml file.bin file.xml
   or: databin2xml [--compress] file.xml file.bin
В первом случае выполняется преобразование .bin -> .xml, во втором - обратное. Ключ --compress предназначен для пропуска списков названий столбцов таблиц при генерации .bin.

by exc!ton