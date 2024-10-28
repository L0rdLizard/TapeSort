# Tape Device Sorter

## About code

Данный проект эмулирует работу устройства хранения данных типа лента (Tape) по средством обычного файла и реализует алгоритм сортировки данных с входной ленты на выходную.



## Getting started
### 1. Configure
```
$ cmake -S . -B build
```

### 2. Build 
```
$ cmake --build build
```

### 3. Run
При запуске приложения указываются имена файлов без указания расширения
```
$ ./build/TapeSort <inputFilename> <outputFilename>
```

