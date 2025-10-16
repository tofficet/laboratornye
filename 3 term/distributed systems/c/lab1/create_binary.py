#!/usr/bin/env python3
# create_binary.py - скрипт для создания тестового бинарного файла

# Данные для записи в бинарный файл
test_data = [
    0x02, 0x04, 0x06, 0x08,    # Первое 4-байтовое слово
    0x09, 0x0B, 0x0D, 0x10,    # Второе слово
    0x11, 0x13, 0x15, 0x17,    # Третье слово
    0x01, 0x03, 0x05, 0x07     # Четвертое слово
]

# Создаем бинарный файл
with open('test_data.bin', 'wb') as f:
    f.write(bytes(test_data))

print("Бинарный файл 'test_data.bin' создан успешно!")
print("Размер файла:", len(test_data), "байт")
print("HEX содержимое:", bytes(test_data).hex())
print("Слова файла (little-endian):")
for i in range(0, len(test_data), 4):
    word_bytes = test_data[i:i+4]
    word_value = word_bytes[0] | (word_bytes[1] << 8) | (word_bytes[2] << 16) | (word_bytes[3] << 24)
    print(f"Word {i//4 + 1}: {word_bytes} = 0x{word_value:08X}")